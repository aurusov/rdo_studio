#include "stdafx.h"
#include "chatchildview.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatChildView
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatChildView, CWnd )
	//{{AFX_MSG_MAP(CChatChildView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatChildView::CChatChildView()
{
}

CChatChildView::~CChatChildView()
{
}

BOOL CChatChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CWnd::PreCreateWindow( cs ) )
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}

int CChatChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;

	viewer.Create( NULL, "", 0, CRect( 0, 0, 0, 0 ), this, 0x1010 );

	edit.CreateEx( WS_EX_CLIENTEDGE, _T("EDIT"), NULL, WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL | ES_LEFT, CRect( 0, 0, 0, 0 ), this, 0x1011 );
	edit.SetFont( &chatApp.getFont() );

	return 0;
}

void CChatChildView::OnSize( UINT nType, int cx, int cy ) 
{
	CWnd::OnSize( nType, cx, cy );
	CRect r;
	GetClientRect( r );

	CRect rEdit;
	rEdit.left   = r.left;
	rEdit.right  = r.right;
	rEdit.top    = r.bottom - 21;
	rEdit.bottom = r.bottom;
	edit.MoveWindow( rEdit );

	CRect rViewer;
	rViewer.left   = r.left;
	rViewer.right  = r.right;
	rViewer.top    = r.top;
	rViewer.bottom = rEdit.top;
	viewer.MoveWindow( rViewer );
}

void CChatChildView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus( pOldWnd );
	edit.SetFocus();
}

BOOL CChatChildView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( viewer.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( edit.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
