#include "stdafx.h"
#include "rdoeditorchildview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorChildView
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorChildView, CWnd )
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

RDOEditorChildView::RDOEditorChildView(): CWnd()
{
}

RDOEditorChildView::~RDOEditorChildView()
{
}

BOOL RDOEditorChildView::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CWnd::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

int RDOEditorChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, -1 );

	return 0;
}

BOOL RDOEditorChildView::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOEditorChildView::OnSetFocus( CWnd* )
{
	tab.SetFocus();
}

void RDOEditorChildView::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	CRect r;
	GetClientRect( r );
	tab.MoveWindow( r );
}
