#include "stdafx.h"
#include "rdotreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTree
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOTree, CTreeCtrl )

	ON_WM_KEYDOWN()

END_MESSAGE_MAP()

RDOTree::RDOTree(): CTreeCtrl()
{
}

RDOTree::~RDOTree()
{
}

BOOL RDOTree::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	//cs.style = TCS_TABS | TCS_HOTTRACK | TCS_SINGLELINE | WS_CHILD | WS_VISIBLE | WS_GROUP;
	cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	return TRUE;
}

void RDOTree::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CTreeCtrl::OnKeyDown( nChar, nRepCnt, nFlags );
	if ( GetParent() ) {
		GetParent()->SendMessage( WM_KEYDOWN, nChar, nFlags );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOTreeCtrl
// ----------------------------------------------------------------------------
#define RDOTree_ID 5001

IMPLEMENT_DYNAMIC( RDOTreeCtrl, CWnd )

BEGIN_MESSAGE_MAP( RDOTreeCtrl, CWnd )

	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()

END_MESSAGE_MAP()

RDOTreeCtrl::RDOTreeCtrl(): CWnd()
{
}

RDOTreeCtrl::~RDOTreeCtrl()
{
}

BOOL RDOTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}

int RDOTreeCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	tree.Create( 0, CRect(0, 0, 400, 100), this, RDOTree_ID );
	//tree.ModifyStyle( 0, WS_CLIPCHILDREN );
	return 0;
}

BOOL RDOTreeCtrl::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( tree.m_hWnd ) {
		if ( tree.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	}
	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOTreeCtrl::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CWnd::OnShowWindow( bShow, nStatus );
}

void RDOTreeCtrl::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	RECT r;
	GetClientRect( &r );
	tree.MoveWindow( &r );
}

const RDOTree& RDOTreeCtrl::getTree() const
{
	return tree;
}

void RDOTreeCtrl::setFont( CFont* font )
{
	tree.SetFont( font );
}
