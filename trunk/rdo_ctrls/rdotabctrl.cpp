#include "rdo_ctrls_pch.h"
#include "rdotabctrl.h"

// ----------------------------------------------------------------------------
// ---------- RDOTab
// ----------------------------------------------------------------------------
class RDOTab: public CTabCtrl
{
friend class RDOTabCtrl;

protected:
	RDOTabCtrl* tabCtrl;

	RDOTab( RDOTabCtrl* _tabCtrl );
	virtual ~RDOTab();

	void showCurrentPage();
	void changeCurrentItem();

	int   getItemCount() const              { return GetItemCount();               };
	CWnd* getItem( const int index ) const;
	CWnd* getItemNext( const CWnd* const currentItem = NULL, const bool direction = true, const bool loop = true ) const;
	CWnd* getItemCurrent() const            { return getItem( getCurrentIndex() ); };
	int   getCurrentIndex() const           { return GetCurSel();                  };
	int   findItem( const CWnd* const item ) const;
	int   findItem( const HWND item ) const;
	void  insertItem( CWnd* pWnd, LPCTSTR lpName, const int image_index = -1 );
	void  setCurrentItem( const int index );

	//{{AFX_MSG(RDOTab)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOTab)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
};

BEGIN_MESSAGE_MAP( RDOTab, CTabCtrl )
	//{{AFX_MSG_MAP(RDOTab)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT( TCN_SELCHANGE, OnSelChanged )
END_MESSAGE_MAP()

RDOTab::RDOTab( RDOTabCtrl* _tabCtrl ):
	CTabCtrl(),
	tabCtrl( _tabCtrl )
{
}

RDOTab::~RDOTab()
{
}

BOOL RDOTab::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = TCS_FOCUSNEVER | TCS_TABS | TCS_HOTTRACK | TCS_SINGLELINE | WS_CHILD | WS_VISIBLE | WS_GROUP;
	if ( !CTabCtrl::PreCreateWindow(cs) ) return FALSE;
	return TRUE;
}

BOOL RDOTab::OnEraseBkgnd(CDC* pDC) 
{
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	GetItem( GetCurSel(), &tci );
	CWnd* wnd = (CWnd*)tci.lParam;
	if ( wnd ) {
		COLORREF color = ::GetSysColor( COLOR_BTNFACE );
		CRect clientRect;
		GetClientRect( clientRect );
		CRect wndRect( clientRect );
		AdjustRect( false, wndRect );
		CRect rect;
		rect.left   = 0;
		rect.top    = 0;
		rect.right  = wndRect.left - clientRect.left;
		rect.bottom = clientRect.Height();
		pDC->FillSolidRect( rect, color );
		rect.left   = 0;
		rect.top    = 0;
		rect.right  = clientRect.Width();
		rect.bottom = wndRect.top - clientRect.top;
		pDC->FillSolidRect( rect, color );
		rect.left   = 0;
		rect.top    = wndRect.Height() + wndRect.top - clientRect.top;
		rect.right  = clientRect.Width();
		rect.bottom = clientRect.Height();
		pDC->FillSolidRect( rect, color );
		rect.left   = wndRect.Width() + wndRect.left - clientRect.left;
		rect.top    = 0;
		rect.right  = clientRect.Width();
		rect.bottom = clientRect.Height();
		pDC->FillSolidRect( rect, color );
		return TRUE;
	} else {
		return CTabCtrl::OnEraseBkgnd(pDC);
	}
}

BOOL RDOTab::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ( HIWORD( wParam ) == WM_SETFOCUS ) {
		HWND hwnd = reinterpret_cast<HWND>(lParam);
		while ( hwnd ) {
			int pos = findItem( hwnd );
			if ( pos != -1 ) {
				setCurrentItem( pos );
				break;
			} else {
				hwnd = ::GetParent( hwnd );
			}
		}
	}
	return CTabCtrl::OnCommand( wParam, lParam );
}

void RDOTab::OnSelChanged( NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/ )
{
	showCurrentPage();
	changeCurrentItem();
}

void RDOTab::showCurrentPage()
{
	CWnd* pWnd = getItemCurrent();
	if ( pWnd ) {
		CRect r;
		GetClientRect( r );
		AdjustRect( false, r );
		pWnd->MoveWindow( r );
		if ( !pWnd->IsWindowVisible() ) {
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

void RDOTab::changeCurrentItem()
{
	tabCtrl->changeCurrentItem();
}

CWnd* RDOTab::getItem( const int index ) const
{
	if ( index < 0 && index > getItemCount() ) return NULL;
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	GetItem( index, &tci );
	return (CWnd*)tci.lParam;
}

CWnd* RDOTab::getItemNext( const CWnd* const currentItem, const bool direction, const bool loop ) const
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
	if ( loop ) {
		if ( index < 0 ) index = getItemCount()-1;
		if ( index == getItemCount() ) index = 0;
		return getItem( index );
	} else {
		if ( index >= 0 && index < getItemCount() ) return getItem( index );
	}
	return NULL;
}

int RDOTab::findItem( const CWnd* const item ) const
{
	for ( int i = 0; i < getItemCount(); i++ ) {
		if ( getItem( i ) == item ) return i;
	}
	return -1;
}

int RDOTab::findItem( const HWND item ) const
{
	for ( int i = 0; i < getItemCount(); i++ ) {
		if ( getItem( i )->m_hWnd == item ) return i;
	}
	return -1;
}

void RDOTab::insertItem( CWnd* pWnd, LPCTSTR lpName, const int image_index )
{
	int item_id = getItemCount();
	InsertItem( item_id, lpName );
	if ( pWnd ) {
		CRect r( 0, 0, 0, 0 );
		GetItemRect( 0, r );
		pWnd->SetWindowPos( NULL, r.left, r.top + r.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		pWnd->ShowWindow( SW_HIDE );
		TC_ITEM TabItem;
		TabItem.mask   = TCIF_PARAM;
		TabItem.lParam = (LPARAM)pWnd;
		if ( image_index != -1 ) {
			TabItem.mask |= TCIF_IMAGE;
			TabItem.iImage = image_index;
		}
		SetItem( item_id, &TabItem );
	}
}

void RDOTab::setCurrentItem( const int index )
{
	if ( index < 0 && index > getItemCount() ) return;
	if ( index != getCurrentIndex() ) {
		SetCurSel( index );
		showCurrentPage();
		changeCurrentItem();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOTabCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOTabCtrl, CWnd )
	//{{AFX_MSG_MAP(RDOTabCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTabCtrl::RDOTabCtrl():
	CWnd(),
	tab( NULL ),
	autoDelete( true )
{
	tab = new RDOTab( this );
}

RDOTabCtrl::~RDOTabCtrl()
{
	if ( tab ) { delete tab; tab = NULL; }
	DestroyWindow();
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
	tab->Create( 0, CRect(0, 0, 400, 100), this, 0 );
	tab->ModifyStyle( 0, WS_CLIPCHILDREN );

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		tab->SetFont( CFont::FromHandle( hf ) );
	}

	return 0;
}

void RDOTabCtrl::OnDestroy()
{
	if ( autoDelete ) {
		int i = getItemCount();
		for ( ; i >= 0; i-- ) {
			CWnd* pWnd = getItem( i );
			if ( pWnd ) {
				bool mustbedelete = true;
				CRuntimeClass* runtimeinfo = pWnd->GetRuntimeClass();
				if ( runtimeinfo && (runtimeinfo->IsDerivedFrom(RUNTIME_CLASS(CFrameWnd)) || runtimeinfo->IsDerivedFrom(RUNTIME_CLASS(CView))) ) {
					mustbedelete = false;
				}
				pWnd->DestroyWindow();
				if ( mustbedelete ) {
					delete pWnd;
				}
			}
		}
	}
}

BOOL RDOTabCtrl::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	CWnd* tabWnd   = getItemCurrent();
	CWnd* focusWnd = GetFocus();
	if ( tabWnd && focusWnd ) {
		bool flag = false;
		if ( tabWnd != focusWnd ) {
			CWnd* wnd = focusWnd->GetParent();
			while ( wnd ) {
				if ( wnd == tabWnd ) {
					flag = true;
					break;
				} else {
					wnd = wnd->GetParent();
				}
			}
		} else {
			flag = true;
		}
		if ( flag ) {
			if ( !focusWnd->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) {
				if ( tabWnd->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) {
					return TRUE;
				}
			} else {
				return TRUE;
			}
		}
	}
	if ( tab->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOTabCtrl::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CWnd::OnShowWindow( bShow, nStatus );
	tab->showCurrentPage();
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
	tab->MoveWindow( r );
	tab->showCurrentPage();
}

void RDOTabCtrl::OnEnable( BOOL bEnable )
{
	CWnd::OnEnable( bEnable );

	tab->EnableWindow( bEnable );

	for ( int i = 0; i < getItemCount(); i++ ) {
		getItem( i )->EnableWindow( bEnable );
	}
}

int RDOTabCtrl::getItemCount() const
{
	return tab->getItemCount();
}

CWnd* RDOTabCtrl::getItem( const int index ) const
{
	return tab->getItem( index );
}

CWnd* RDOTabCtrl::getItemNext( const CWnd* const currentItem, const bool direction, const bool loop ) const
{
	return tab->getItemNext( currentItem, direction, loop );
}

CWnd* RDOTabCtrl::getItemCurrent() const
{
	return tab->getItemCurrent();
}

int RDOTabCtrl::getCurrentIndex() const
{
	return tab->getCurrentIndex();
}

int RDOTabCtrl::findItem( const CWnd* const item ) const
{
	return tab->findItem( item );
}

void RDOTabCtrl::insertItem( CWnd* pWnd, LPCTSTR lpName, const int image_index )
{
	tab->insertItem( pWnd, lpName, image_index );
}

void RDOTabCtrl::setCurrentItem( const int index )
{
	tab->setCurrentItem( index );
}

void RDOTabCtrl::changeCurrentItem()
{
}

void RDOTabCtrl::modifyTabStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags )
{
	tab->ModifyStyle( dwRemove, dwAdd, nFlags );
}
