#include "stdafx.h"
#include "bkemulmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKMainFrame
// --------------------------------------------------------------
IMPLEMENT_DYNAMIC(BKMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(BKMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(BKMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

BKMainFrame::BKMainFrame()
{
}

BKMainFrame::~BKMainFrame()
{
}

BOOL BKMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0 );

	return TRUE;
}

int BKMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;

	if ( !childView.Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL ) ) {
		TRACE0("Failed to create view window\n");
		return -1;
	}
/*
	if ( !toolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) || !toolBar.LoadToolBar( IDR_MAINFRAME ) ) {
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if ( !statusBar.Create( this ) || !statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) ) ) {
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	toolBar.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar( &toolBar );
*/
	return 0;
}

#ifdef _DEBUG
void BKMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void BKMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

void BKMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	childView.SetFocus();
}

BOOL BKMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( childView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ) return TRUE;
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void BKMainFrame::OnMove( int x, int y )
{
	CFrameWnd::OnMove( x, y );

	if ( childView.GetSafeHwnd() ) {
		childView.updateBounds();
	}
}

void BKMainFrame::RecalcLayout(BOOL bNotify)
{
	CFrameWnd::RecalcLayout(bNotify);

	if ( childView.GetSafeHwnd() ) {
		childView.updateBounds();
	}
}
