#include "stdafx.h"
#include "chatedit.h"
#include "chatapp.h"
#include "chatmainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatEdit
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatEdit, CEdit )
	//{{AFX_MSG_MAP(CChatEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatEdit::CChatEdit():
	CEdit(),
	index( -1 )
{
}

CChatEdit::~CChatEdit()
{
	list.clear();
}

void CChatEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CEdit::OnKeyDown( nChar, nRepCnt, nFlags );
	if ( nChar == VK_RETURN ) {
		CString s;
		GetWindowText( s );
		std::string str = s;
		std::string str_trim = str;
		trim( str_trim );
		if ( !str_trim.empty() ) {
			list.push_back( str );
			index = list.size() - 1;

			chatApp.udp.send( "<msg:" + str + ">" );

			SetWindowText( "" );

			chatApp.sounds.play( CST_ChatRet );
		}
	} else if ( nChar == VK_BACK ) {
		chatApp.sounds.play( CST_ChatBack );

	} else if ( ( nChar == VK_SPACE || nChar == 187 || ::IsCharAlphaNumeric( nChar ) ) && nChar != VK_NUMLOCK ) {
		chatApp.sounds.play( CST_ChatType );
	}
	chatApp.mainFrame->restoreStatusMode();
}

LRESULT CChatEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message == WM_KEYDOWN ) {
		if ( wParam == VK_UP ) {
			if ( index != -1 && index < list.size() ) {
				SetWindowText( list[index].c_str() );
				SetSel( list[index].length(), list[index].length(), FALSE );
				if ( index ) index--;
			}
			return 0;
		}
		if ( wParam == VK_DOWN ) {
			if ( index != -1 && index + 1 < list.size() ) {
				index++;
				SetWindowText( list[index].c_str() );
				SetSel( list[index].length(), list[index].length(), FALSE );
			}
			return 0;
		}
	}
	return CEdit::WindowProc(message, wParam, lParam);
}

bool CChatEdit::isSelected() const
{
	DWORD sel = GetSel();
	return HIWORD(sel) != LOWORD(sel);
}

void CChatEdit::paste()
{
	if ( OpenClipboard() ) {
		HGLOBAL global_str = ::GetClipboardData( CF_OEMTEXT );
		if ( global_str ) {
			LPTSTR lpstr = static_cast<LPTSTR>(::GlobalLock( global_str ));
			if ( lpstr ) {
				CString str( lpstr );
				str.OemToAnsi();
				ReplaceSel( str );
				::GlobalUnlock( global_str );
			}
		}
		CloseClipboard();
	}
}
