#include "stdafx.h"
#include "rdotabctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTab
// ----------------------------------------------------------------------------
RDOTab::RDOTab(): CTabCtrl()
{
}

BOOL RDOTab::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = TCS_FOCUSNEVER | TCS_TABS | TCS_HOTTRACK | TCS_SINGLELINE | WS_CHILD | WS_VISIBLE | WS_GROUP;
	if ( !CTabCtrl::PreCreateWindow(cs) ) return FALSE;
	return TRUE;
}

// ----------------------------------------------------------------------------
// ---------- RDOTabCtrl
// ----------------------------------------------------------------------------
#define RDOTab_ID 1

BEGIN_MESSAGE_MAP( RDOTabCtrl, CWnd )

	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_ENABLE()
	ON_NOTIFY( TCN_SELCHANGE, RDOTab_ID, OnSelChangeTab )

END_MESSAGE_MAP()

RDOTabCtrl::RDOTabCtrl():
	CWnd(),
	autoDelete( true )
{
}

RDOTabCtrl::~RDOTabCtrl()
{
}

BOOL RDOTabCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}

int RDOTabCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	tab.Create( 0, CRect(0, 0, 400, 100), this, RDOTab_ID );
	tab.ModifyStyle( 0, WS_CLIPCHILDREN );
	return 0;
}

void RDOTabCtrl::OnDestroy()
{
	if ( autoDelete ) {
		int i = getItemCount();
		for ( ; i >= 0; i-- ) {
			CWnd* pWnd = getItem( i );
			if ( pWnd ) {
				pWnd->DestroyWindow();
				delete pWnd;
			}
		}
	}
}

BOOL RDOTabCtrl::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	CWnd* pWnd = getItemCurrent();
	if ( pWnd ) if ( pWnd->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;

	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOTabCtrl::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CWnd::OnShowWindow( bShow, nStatus );
	showCurrentPage();
}

void RDOTabCtrl::OnSetFocus( CWnd* )
{
	CWnd* pWnd = getItemCurrent();
	if ( pWnd ) pWnd->SetFocus();
}

void RDOTabCtrl::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	CRect r;
	GetClientRect( r );
	tab.MoveWindow( r );
	showCurrentPage();
}

void RDOTabCtrl::OnEnable( BOOL bEnable )
{
	CWnd::OnEnable( bEnable );

	tab.EnableWindow( bEnable );

	for ( int i = 0; i < getItemCount(); i++ ) {
		getItem( i )->EnableWindow( bEnable );
	}
}

void RDOTabCtrl::OnSelChangeTab( NMHDR* /* pNMHDR */, LRESULT* pResult )
{
	showCurrentPage();
	changeCurrentItem();
	*pResult = 0;
}

void RDOTabCtrl::showCurrentPage()
{
	CWnd* pWnd = getItemCurrent();
	if ( pWnd ) {
		CRect r;
		tab.GetClientRect( r );
		tab.AdjustRect( false, r );
		pWnd->MoveWindow( r );
		if ( !(pWnd->GetStyle() & WS_VISIBLE) ) {
			pWnd->ShowWindow( SW_SHOW );
			pWnd->SetFocus();
		}
	}
	for ( int i = 0; i < getItemCount(); i++ ) {
		CWnd* pWnd_item = getItem( i );
		if ( pWnd_item && pWnd != pWnd_item && (pWnd_item->GetStyle() & WS_VISIBLE) ) {
			pWnd_item->ShowWindow( SW_HIDE );
		}
	}
}

void RDOTabCtrl::changeCurrentItem()
{
}

CWnd* RDOTabCtrl::getItem( const int index ) const
{
	if ( index < 0 && index > getItemCount() ) return NULL;
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	tab.GetItem( index, &tci );
	return (CWnd*)tci.lParam;
}

CWnd* RDOTabCtrl::getItemNext( const CWnd* const currentItem, const bool direction, const bool loop ) const
{
	if ( !getItemCount() ) return NULL;
	int index;
	if ( currentItem ) {
		index = findItem( currentItem );
		if ( index == - 1 ) return NULL;
	} else {
		index = getCurrentIndex();
	}
	if ( direction ) {
		index++;
	} else {
		index--;
	}
	if ( index < 0 ) index = getItemCount()-1;
	if ( index == getItemCount() ) index = 0;
	return getItem( index );
}

int RDOTabCtrl::findItem( const CWnd* const item ) const
{
	for ( int i = 0; i < getItemCount(); i++ ) {
		if ( getItem( i ) == item ) return i;
	}
	return -1;
}

void RDOTabCtrl::insertItem( CWnd* pWnd, LPCTSTR lpName )
{
	int item_id = getItemCount();
	tab.InsertItem( item_id, lpName );
	if ( pWnd ) {
		CRect r( 0, 0, 0, 0 );
		tab.GetItemRect( 0, r );
		pWnd->SetWindowPos( NULL, r.left, r.top + r.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		pWnd->ShowWindow( SW_HIDE );
		TC_ITEM TabItem;
		TabItem.mask   = TCIF_PARAM;
		TabItem.lParam = (LPARAM)pWnd;
		tab.SetItem( item_id, &TabItem );
	}
}

void RDOTabCtrl::setCurrentItem( const int index )
{
	if ( index < 0 && index > getItemCount() ) return;
	tab.SetCurSel( index );
	showCurrentPage();
	changeCurrentItem();
}
