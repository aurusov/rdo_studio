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

// ----------------------------------------------------------------------------
// ---------- CChatNetworkCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatNetworkCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(CChatNetworkCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
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

void CChatNetworkCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	RDOTreeCtrl::OnLButtonDblClk( nFlags, point );

	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net->getType() == CChatNet::shared ) {
			::ShellExecute( chatApp.mainFrame->m_hWnd, "open", static_cast<CChatNetShared*>(net)->getName().c_str(), 0, 0, SW_SHOWNORMAL );
		}
	}
}

int CChatNetworkCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( uFlags & TVHT_ONITEM ) ) {
		CChatNet* net = reinterpret_cast<CChatNet*>(GetItemData( hitem ));
		if ( net->getType() == CChatNet::server ) {
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
		if ( net->getType() == CChatNet::server ) {
			strTipText = static_cast<CChatNetServer*>(net)->getToolTipInfo().c_str();
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
