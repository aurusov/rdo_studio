#include "stdafx.h"
#include "chatnetworkctrl.h"
#include "chatnetwork.h"
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
// ---------- CChatNetworkCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatNetworkCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(CChatNetworkCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_NETWORK_OPEN, OnNetworkOpen)
	ON_COMMAND(ID_NETWORK_INFO, OnNetworkInfo)
	ON_WM_CONTEXTMENU()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

CChatNetworkCtrl::CChatNetworkCtrl():
	RDOTreeCtrl(),
	m_pchTip( NULL ),
	m_pwchTip( NULL )
{
}

CChatNetworkCtrl::~CChatNetworkCtrl()
{
	if ( m_pwchTip ) delete m_pwchTip;
	if ( m_pchTip  ) delete m_pchTip;
}

BOOL CChatNetworkCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int CChatNetworkCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTreeCtrl::OnCreate( lpCreateStruct ) == -1 ) return -1;

	ModifyStyle( 0, TVS_HASBUTTONS | TVS_LINESATROOT );
//	ModifyStyle( 0, WS_TABSTOP | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP );

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 3, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_NET );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );

	EnableToolTips();

	return 0;
}

UINT CChatNetworkCtrl::shellExecute( LPVOID pParam )
{
	CChatNet* net = reinterpret_cast<CChatNet*>(pParam);
	::ShellExecute( CWnd::GetDesktopWindow()->m_hWnd, "open", net->getFullPath().c_str(), 0, 0, SW_SHOWNORMAL );
	net->openingThread = NULL;
	return 0;
}

void CChatNetworkCtrl::OnNetworkOpen()
{
	HTREEITEM hitem = GetSelectedItem();
	if ( hitem ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::shared || net->getType() == CChatNet::server ) {
			net->openingThread = AfxBeginThread( shellExecute, net );
		}
	}
}

void CChatNetworkCtrl::OnNetworkInfo()
{
	HTREEITEM hitem = GetSelectedItem();
	if ( hitem ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::server ) {
		}
	}
}

// Using code from Megavarnan Selvaraj
int CChatNetworkCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::server ) {
			RECT rect;
			GetItemRect( hitem, &rect, TRUE );
			pTI->hwnd     = m_hWnd;
			pTI->uId      = reinterpret_cast<UINT>(hitem);
			pTI->lpszText = LPSTR_TEXTCALLBACK;
			pTI->rect     = rect;
			return pTI->uId;
		}
	}

	return -1;
}

// Using code from Megavarnan Selvaraj
BOOL CChatNetworkCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
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
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::server ) {
			strTipText = net->getToolTipInfo().c_str();
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

void CChatNetworkCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	RDOTreeCtrl::OnLButtonDblClk( nFlags, point );

	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::shared ) {
			net->openingThread = AfxBeginThread( shellExecute, net );
		}
	}
}

void CChatNetworkCtrl::OnRButtonDown( UINT nFlags, CPoint point )
{
//	RDOTreeCtrl::OnRButtonDown( nFlags, point );
}

void CChatNetworkCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UINT uFlags;
	ScreenToClient( &point );
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		SetFocus();
		SelectItem( hitem );
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net && net->getType() == CChatNet::shared || net->getType() == CChatNet::server ) {
			CMenu menu;
			menu.LoadMenu( IDR_NETWORK_MENU );
			CMenu* subMenu = menu.GetSubMenu( 0 );
			if ( net->openingThread ) {
				subMenu->EnableMenuItem( 0, MF_BYPOSITION | MF_GRAYED );
			}
//			if ( net->getType() == CChatNet::shared ) {
				subMenu->RemoveMenu( 1, MF_BYPOSITION );
//			}
			ClientToScreen( &point );
			subMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
			menu.DestroyMenu();
		}
	}
}

UINT CChatNetworkCtrl::OnGetDlgCode()
{
	return RDOTreeCtrl::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CChatNetworkCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	RDOTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	chatApp.mainFrame->restoreStatusMode();

	if ( nChar == VK_RETURN ) {
		HTREEITEM hitem = GetSelectedItem();
		if ( hitem ) {
			CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
			if ( net && net->getType() == CChatNet::shared || net->getType() == CChatNet::server ) {
				net->openingThread = AfxBeginThread( shellExecute, net );
			}
		}
	}
}
