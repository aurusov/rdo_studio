#include "stdafx.h"
#include "chatedit.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatEdit
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatEdit, CEdit )
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CChatEdit::CChatEdit(): CEdit()
{
}

CChatEdit::~CChatEdit()
{
}

void CChatEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CEdit::OnKeyDown( nChar, nRepCnt, nFlags );
	if ( nChar == VK_RETURN ) {
		CString s;
		GetWindowText( s );

		chatApp.udp.Send( "<msg:" + s + ">" );

		SetWindowText( "" );

		chatApp.sounds.play( CST_ChatRet );

	} else if ( nChar == VK_BACK ) {
		chatApp.sounds.play( CST_ChatBack );

	} else if ( ( nChar == VK_SPACE || nChar == 187 || ::IsCharAlphaNumeric( nChar ) ) && nChar != VK_NUMLOCK ) {
		chatApp.sounds.play( CST_ChatType );
	}
	chatApp.pFrame->restoreStatusMode();
}
