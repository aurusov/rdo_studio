#include "stdafx.h"
#include "chatuserlistctrl.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Using code from Megavarnan Selvaraj for tooltip

// ----------------------------------------------------------------------------
// ---------- CChatUserListCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatUserListCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(CChatUserListCtrl)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

CChatUserListCtrl::CChatUserListCtrl():
	RDOTreeCtrl(),
	m_pchTip( NULL ),
	m_pwchTip( NULL )
{
}

CChatUserListCtrl::~CChatUserListCtrl()
{
	eraseMenu( &popupMenu );
	if ( m_pwchTip ) delete m_pwchTip;
	if ( m_pchTip  ) delete m_pchTip;
}

BOOL CChatUserListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int CChatUserListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTreeCtrl::OnCreate( lpCreateStruct ) == -1 ) return -1;

//	ModifyStyle( 0, WS_TABSTOP | TVS_HASBUTTONS /*| TVS_LINESATROOT*/ | TVS_HASLINES | TVS_DISABLEDRAGDROP );

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 3, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_STATUSMODE );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );

	popupMenu.CreatePopupMenu();
	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	appendMenu( mainMenu->GetSubMenu( 4 ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 4 ), 1, &popupMenu );

	EnableToolTips();

	return 0;
}

HTREEITEM CChatUserListCtrl::findUser( const CChatUser* const user )
{
	HTREEITEM item = GetRootItem();

	while ( item ) {
		CChatUser* data = reinterpret_cast<CChatUser*>(GetItemData( item ));
		if ( data && data == user ) return item;
		item = GetNextItem( item, TVGN_NEXT );
	}

	return NULL;
}

void CChatUserListCtrl::addUser( const CChatUser* const user )
{
	if ( !findUser( user ) ) {
		HTREEITEM item;
		item = InsertItem( user->getUserName().c_str(), user->getStatusMode(), user->getStatusMode() );
		SetItemData( item, reinterpret_cast<DWORD>(user) );
		SortChildren( TVI_ROOT );
	}
}

void CChatUserListCtrl::deleteUser( const CChatUser* const user )
{
	HTREEITEM item;
	item = findUser( user );
	if ( item ) {
		DeleteItem( item );
	}
}

void CChatUserListCtrl::updateUserName( const CChatUser* const user )
{
	HTREEITEM item = findUser( user );
	if ( item ) {
		SetItemText( item, user->getUserName().c_str() );
	}
}

void CChatUserListCtrl::updateUserStatus( const CChatUser* const user )
{
	HTREEITEM item = findUser( user );
	if ( item ) {
		if ( !user->isIgnored() ) {
			SetItemImage( item, user->getStatusMode(), user->getStatusMode() );
		} else {
			SetItemImage( item, 3, 3 );
		}
	}
}

void CChatUserListCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	RDOTreeCtrl::OnKeyDown( nChar, nRepCnt, nFlags );

	chatApp.mainFrame->restoreStatusMode();

	if ( nChar == VK_RETURN ) {
		chatApp.mainFrame->sendMessageToUser();
	}
}

// Using code from Megavarnan Selvaraj
int CChatUserListCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		RECT rect;
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd     = m_hWnd;
		pTI->uId      = reinterpret_cast<UINT>(hitem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect     = rect;
		return pTI->uId;
	}

	return -1;
}

// Using code from Megavarnan Selvaraj
BOOL CChatUserListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText = "";
	UINT nID = pNMHDR->idFrom;

	// Do not process the message from built in tooltip 
	if( nID == (UINT)m_hWnd &&
		(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
		( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
		return FALSE;

	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	pt = pMessage->pt;

	ScreenToClient( &pt );

	UINT nFlags;
	HTREEITEM hitem = HitTest( pt, &nFlags );
	if( nFlags & TVHT_ONITEM)
	{
		CChatUser* user = reinterpret_cast<CChatUser*>(GetItemData( hitem ));
		if ( user ) {
			strTipText = user->getToolTipInfo().c_str();
		}
	}

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0;
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0; // end of text
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip != NULL)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()+1];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		m_pchTip[strTipText.GetLength()] = 0; // end of text
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	else
	{
		if(m_pwchTip != NULL)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()+1];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		m_pwchTip[strTipText.GetLength()] = 0;
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif

	*pResult = 0;

	return TRUE;    // message was handled
}

void CChatUserListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
//	RDOTreeCtrl::OnRButtonDown( nFlags, point );
}

void CChatUserListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UINT uFlags;
	ScreenToClient( &point );
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		SetFocus();
		SelectItem( hitem );
		ClientToScreen( &point );
		popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, chatApp.mainFrame );
	}
}

UINT CChatUserListCtrl::OnGetDlgCode()
{
	return RDOTreeCtrl::OnGetDlgCode() | DLGC_WANTALLKEYS;
}
