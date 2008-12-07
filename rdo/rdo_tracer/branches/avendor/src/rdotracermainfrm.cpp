#include "stdafx.h"
#include "rdotracerapp.h"
#include "rdotracermainfrm.h"
#include "./tracer_ctrls/rdosplitterwnd.h"
#include "./tracer_ctrls/rdotracertreectrl.h"
//#include "rdotraceroptions.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerMainFrame
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOTracerMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(RDOTracerMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	ON_COMMAND( ID_START_TRACE, onStartTrace )
	ON_COMMAND( ID_STOP_TRACE, onStopTrace )
	ON_COMMAND( ID_VIEW_LOG, onViewLog )
	ON_COMMAND( ID_VIEW_TREE, onViewTree )
	ON_COMMAND( ID_VIEW_TRACE_TOOLBAR, onViewTraceToolBar )
	ON_COMMAND( ID_VIEW_LOG_TOOLBAR, onViewLogToolBar )
	ON_COMMAND( ID_VIEW_ZOOM_TOOLBAR, onViewZoomToolBar )
	ON_COMMAND( ID_VIEW_STATUSBAR, onViewStatusBar )
	ON_COMMAND( ID_VIEW_OPTIONS, onViewOptions )
	ON_UPDATE_COMMAND_UI( ID_START_TRACE, onUpdateStartTrace )
	ON_UPDATE_COMMAND_UI( ID_STOP_TRACE, onUpdateStopTrace )
	ON_UPDATE_COMMAND_UI( ID_VIEW_LOG, onUpdateLog )
	ON_UPDATE_COMMAND_UI( ID_VIEW_TREE, onUpdateTree )
	ON_UPDATE_COMMAND_UI( ID_VIEW_TRACE_TOOLBAR, onUpdateTraceToolBar )
	ON_UPDATE_COMMAND_UI( ID_VIEW_LOG_TOOLBAR, onUpdateLogToolBar )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ZOOM_TOOLBAR, onUpdateZoomToolBar )
	ON_UPDATE_COMMAND_UI( ID_VIEW_STATUSBAR, onUpdateStatusBar )
	ON_COMMAND(ID_SM_NOSHOW, OnSmNoshow)
	ON_UPDATE_COMMAND_UI(ID_SM_NOSHOW, OnUpdateSmNoshow)
	ON_COMMAND(ID_SM_ANIMATION, OnSmAnimation)
	ON_UPDATE_COMMAND_UI(ID_SM_ANIMATION, OnUpdateSmAnimation)
	ON_COMMAND(ID_SM_MONITOR, OnSmMonitor)
	ON_UPDATE_COMMAND_UI(ID_SM_MONITOR, OnUpdateSmMonitor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_SEPARATOR,
	ID_COORD_STATUSBAR,
	ID_MODIFY_STATUSBAR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

RDOTracerMainFrame::RDOTracerMainFrame():
	CFrameWnd(),
	log( NULL ),
	tab( NULL ),
	tree( NULL ),
	log_pane( NULL ),
	tab_pane( NULL ),
	tree_pane( NULL ),
	showMode( SM_Animation )
{
}

RDOTracerMainFrame::~RDOTracerMainFrame()
{
	trace_reader.stopTrace();
	
	if ( log_pane ) log_pane->setControl( NULL );
	if ( log ) delete log;
	if ( log_pane ) delete log_pane;

	if ( tree_pane ) tree_pane->setControl( NULL );
	if ( tree ) delete tree;
	if ( tree_pane ) delete tree_pane;

	if ( tab_pane ) tab_pane->setControl( NULL );
	if ( tab ) delete tab;
	if ( tab_pane ) delete tab_pane;
}

BOOL RDOTracerMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

BOOL RDOTracerMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	tracer->registerClipboardFormat();
	
	style_trace.init( "trace" );
	style_trace.load();
	style_chart.init( "chart" );
	style_chart.load();

	if ( !logSplitter.CreateStatic( this, 2, 1, WS_CHILD | WS_VISIBLE ) ) return FALSE;

	if ( !treeSplitter.CreateStatic( &logSplitter, 1, 2, WS_CHILD | WS_VISIBLE | WS_BORDER, logSplitter.IdFromRowCol( 0, 0 ) ) ) return FALSE;
	
	if ( !logSplitter.CreateView( 1, 0, RUNTIME_CLASS( RDOTracerPane ), CSize(200, 50), pContext ) ) return FALSE;

	if ( !treeSplitter.CreateView( 0, 0, RUNTIME_CLASS( RDOTracerPane ), CSize(200, 50), pContext ) ||
		 !treeSplitter.CreateView( 0, 1, RUNTIME_CLASS( RDOTracerPane ), CSize( 0,0 ), pContext ))
		return FALSE;
	
	log_pane = static_cast<RDOTracerPane*>(logSplitter.GetPane( 1, 0 ));
	log = tracer->createLog();
	log->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), log_pane, 0 );
	log_pane->setControl( log );

	tree_pane = static_cast<RDOTracerPane*>(treeSplitter.GetPane( 0, 0 ));
	tree = tracer->createTree();
	tree->Create( 0, CRect(0, 0, 0, 0), tree_pane, 0 );
	tree_pane->setControl( tree );

	tab_pane = static_cast<RDOTracerPane*>(treeSplitter.GetPane( 0, 1 ));
	tab = new RDOTabCtrl();
	tab->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab_pane, 0 );
	tab_pane->setControl( tab );

	CRect rect;
	RecalcLayout();
	logSplitter.GetClientRect( &rect );

#pragma warning( disable : 4244 )
	logSplitter.SetRowInfo( 0, rect.Height() / 2, 10 );
	logSplitter.SetRowInfo( 1, rect.Height() / 2, 10 );
	logSplitter.RecalcLayout();
	treeSplitter.GetClientRect( &rect );
	int width = rect.Width() / 3.2;
	treeSplitter.SetColumnInfo( 0, width, 10 );
	treeSplitter.SetColumnInfo( 1, rect.Width() - width, 10 );
	treeSplitter.RecalcLayout();
#pragma warning( default : 4244 )

	return TRUE;
}

int RDOTracerMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	string s;
	s = format( IDS_TRACE_TOOLBAR );
	if ( !traceToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC ) ||
		 !traceToolBar.LoadToolBar( IDR_TRACE_TOOLBAR ) ) {
		TRACE0( "Failed to create trace toolbar\n" );
		return -1;
	}
	traceToolBar.GetToolBarCtrl().SetWindowText( s.c_str() );
	if ( traceToolBarImageList.Create( IDB_TRACE_TOOLBAR_D, 16, 0, 0xFF00FF ) )
		traceToolBar.GetToolBarCtrl().SetDisabledImageList( &traceToolBarImageList );
	
	s = format( IDS_LOG_TOOLBAR );
	if ( !logToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC ) ||
		 !logToolBar.LoadToolBar( IDR_LOG_TOOLBAR ) ) {
		TRACE0( "Failed to create log toolbar\n" );
		return -1;
	}
	logToolBar.GetToolBarCtrl().SetWindowText( s.c_str() );
	if ( logToolBarImageList.Create( IDB_LOG_TOOLBAR_D, 16, 0, 0xFF00FF ) )
		logToolBar.GetToolBarCtrl().SetDisabledImageList( &logToolBarImageList );

	s = format( IDS_ZOOM_TOOLBAR );
	if ( !zoomToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC ) ||
		 !zoomToolBar.LoadToolBar( IDR_ZOOM_TOOLBAR ) ) {
		TRACE0( "Failed to create zoom toolbar\n" );
		return -1;
	}
	zoomToolBar.GetToolBarCtrl().SetWindowText( s.c_str() );
	if ( zoomToolBarImageList.Create( IDB_ZOOM_TOOLBAR_D, 16, 0, 0xFF00FF ) )
		zoomToolBar.GetToolBarCtrl().SetDisabledImageList( &zoomToolBarImageList );

	if ( !statusBar.Create( this ) ||
		 !statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) )) {
		TRACE0( "Failed to create status bar\n" );
		return -1;
	}
	statusBar.SetPaneInfo( 1, ID_COORD_STATUSBAR           , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_MODIFY_STATUSBAR          , SBPS_NORMAL , 70 );

	traceToolBar.EnableDocking( CBRS_ALIGN_ANY );
	logToolBar.EnableDocking( CBRS_ALIGN_ANY );
	zoomToolBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &traceToolBar );
	dockControlBarBesideOf( logToolBar, traceToolBar );
	dockControlBarBesideOf( zoomToolBar, logToolBar );

	return 0;
}

BOOL RDOTracerMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( log && log->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( tree && tree->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( tab && tab->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOTracerMainFrame::OnSetFocus( CWnd *pOldWnd )
{
	CFrameWnd::OnSetFocus ( pOldWnd );
}

void RDOTracerMainFrame::OnMove( int x, int y )
{
	CFrameWnd::OnMove( x, y );
}

void RDOTracerMainFrame::onViewOptions()
{
/*	RDOTracerOptions dlg( *((RDOTracerLogStyle*)&(log->getStyle())) );
	dlg.DoModal();*/
}

void RDOTracerMainFrame::onStartTrace()
{
	OnSmAnimation();
	trace_reader.startTrace();
}

void RDOTracerMainFrame::onStopTrace()
{
	trace_reader.stopTrace();
}

void RDOTracerMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
{
	RecalcLayout( TRUE );
	
	CRect rect;
	baseBar.GetWindowRect( rect );

	DWORD dw = baseBar.GetBarStyle();

	UINT n = 0;
	n = ( dw & CBRS_ALIGN_TOP          ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = ( dw & CBRS_ALIGN_BOTTOM && !n ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = ( dw & CBRS_ALIGN_LEFT   && !n ) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = ( dw & CBRS_ALIGN_RIGHT  && !n ) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	int dx = 0;
	int dy = 0;
	if ( n == AFX_IDW_DOCKBAR_TOP || n == AFX_IDW_DOCKBAR_BOTTOM ) dx = 1;
	if ( n == AFX_IDW_DOCKBAR_LEFT || n == AFX_IDW_DOCKBAR_RIGHT ) dy = 1;

	rect.OffsetRect( dx, dy );

	DockControlBar( &bar, n, rect );
}

RDOTracerLogCtrl* RDOTracerMainFrame::getLog() const
{
	return log;
}

RDOTabCtrl* RDOTracerMainFrame::getTab() const
{
	return tab;
}

RDOTracerTreeCtrl* RDOTracerMainFrame::getTree() const
{
	return tree;
}

void RDOTracerMainFrame::setModelName( const string modelName )
{
	SetWindowText( format( IDS_CAPTION, modelName.c_str() ).c_str() );
}

#ifdef _DEBUG
void RDOTracerMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void RDOTracerMainFrame::Dump( CDumpContext& dc ) const
{
	CFrameWnd::Dump(dc);
}
#endif

void RDOTracerMainFrame::OnSmNoshow() 
{
	if ( showMode != SM_NoShow ) {
		bool need_unpause = showMode == SM_Monitor;
		showMode = SM_NoShow;
		tracer->setShowMode( showMode );
		if ( need_unpause )
			trace_reader.unpauseTrace();
	}
}

void RDOTracerMainFrame::OnUpdateSmNoshow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( trace_reader.isTracing() );
	pCmdUI->SetCheck( trace_reader.isTracing() ? showMode == SM_NoShow : 0 );
}

void RDOTracerMainFrame::OnSmAnimation() 
{
	if ( showMode != SM_Animation ) {
		bool need_unpause = showMode == SM_Monitor;
		showMode = SM_Animation;
		tracer->setShowMode( showMode );
		if ( need_unpause )
			trace_reader.unpauseTrace();
	}
}

void RDOTracerMainFrame::OnUpdateSmAnimation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( trace_reader.isTracing() );
	pCmdUI->SetCheck( trace_reader.isTracing() ? showMode == SM_Animation : 0 );
}

void RDOTracerMainFrame::OnSmMonitor() 
{
	if ( showMode != SM_Monitor ) {
		showMode = SM_Monitor;
		tracer->setShowMode( showMode );
		trace_reader.pauseTrace();
	}
}

void RDOTracerMainFrame::OnUpdateSmMonitor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( trace_reader.isTracing() );
	pCmdUI->SetCheck( trace_reader.isTracing() ? showMode == SM_Monitor : 0 );
}
