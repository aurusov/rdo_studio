#include "stdafx.h"
#include "rdostudiomainfrm.h"
#include "rdostudioapp.h"
#include "rdostudiomodel.h"
#include "rdostudiooptions.h"
#include "rdostudioplugins.h"
#include "rdo_tracer/rdotracer.h"
#include "htmlhelp.h"
#include "resource.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
const int WORKSPACE_SHOW_MESSAGE = ::RegisterWindowMessage( "WORKSPACE_SHOW_MESSAGE" );
const int OUTPUT_SHOW_MESSAGE    = ::RegisterWindowMessage( "OUTPUT_SHOW_MESSAGE" );

IMPLEMENT_DYNAMIC(RDOStudioMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RDOStudioMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RDOStudioMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TOOLBAR_FILE_TOOLBAR, OnViewFileToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDIT_TOOLBAR, OnViewEditToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_ZOOM_TOOLBAR, OnViewZoomToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_MODEL_TOOLBAR, OnViewModelToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FILE_TOOLBAR, OnUpdateViewFileToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDIT_TOOLBAR, OnUpdateViewEditToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_ZOOM_TOOLBAR, OnUpdateViewZoomToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_MODEL_TOOLBAR, OnUpdateViewModelToolbar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_MODEL_RUN_NOSHOW, OnModelRunNoShow)
	ON_COMMAND(ID_MODEL_RUN_ANIMATION, OnModelRunAnimation)
	ON_COMMAND(ID_MODEL_RUN_MONITOR, OnModelRunMonitor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_NOSHOW, OnUpdateModelRunNoShow)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_ANIMATION, OnUpdateModelRunAnimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_MONITOR, OnUpdateModelRunMonitor)
	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowRateInc)
	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowRateIncFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowRateDecFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowRateDec)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowRateInc)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowRateIncFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowRateDecFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowRateDec)
	ON_COMMAND(ID_MODEL_FRAME_NEXT, OnModelFrameNext)
	ON_COMMAND(ID_MODEL_FRAME_PREV, OnModelFramePrev)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_NEXT, OnUpdateModelFrameNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_PREV, OnUpdateModelFramePrev)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMRESET, OnUpdateViewZoomReset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMAUTO, OnUpdateViewZoomAuto)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR           , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR          , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR , OnUpdateInsertOverwriteStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_TIME_STATUSBAR      , OnUpdateModelTimeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_RUNTYPE_STATUSBAR   , OnUpdateModelRunTypeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_SHOWRATE_STATUSBAR  , OnUpdateModelShowRateStatusBar )
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORD_STATUSBAR,
	ID_MODIFY_STATUSBAR,
	ID_INSERTOVERWRITE_STATUSBAR,
	ID_MODEL_TIME_STATUSBAR,
	ID_MODEL_RUNTYPE_STATUSBAR,
	ID_MODEL_SHOWRATE_STATUSBAR,
	ID_PROGRESSSTATUSBAR
};

RDOStudioMainFrame::RDOStudioMainFrame():
	CMDIFrameWnd(),
	syncObject( NULL )
{
}

RDOStudioMainFrame::~RDOStudioMainFrame()
{
}

int RDOStudioMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CMDIFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	syncObject = new RDOMainFrmSyncClient( *this, ::GetCurrentThreadId() );
	kernel.insertSyncClient( syncObject );

	model = new RDOStudioModel;

	style_editor.init( "editor" );
	style_editor.load();

	style_build.init( "build" );
	style_build.window->wordWrap          = true;
	style_build.window->showHorzScrollBar = false;
	style_build.load();

	style_debug.init( "debug" );
	style_debug.window->wordWrap          = true;
	style_debug.window->showHorzScrollBar = false;
	style_debug.load();

	style_trace.init( "trace" );
	style_trace.load();

	style_results.init( "results" );
	style_results.load();

	style_find.init( "find" );
	style_find.load();

	style_frame.init( "frame" );
	style_frame.load();

	style_chart.init( "chart" );
	style_chart.load();

	fileToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	fileToolBar.LoadToolBar( IDR_FILE_TOOLBAR );
	fileToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	fileToolBar.SetWindowText( rdo::format( IDR_FILE_TOOLBAR ).c_str() );

	fileToolBarImageList.Create( IDB_FILE_TOOLBAR_D, 16, 0, 0xFF00FF );
	fileToolBar.GetToolBarCtrl().SetDisabledImageList( &fileToolBarImageList );

	editToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDIT_TOOLBAR );
	editToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	editToolBar.SetWindowText( rdo::format( IDR_EDIT_TOOLBAR ).c_str() );

	editToolBarImageList.Create( IDB_EDIT_TOOLBAR_D, 16, 0, 0xFF00FF );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	zoomToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	zoomToolBar.LoadToolBar( IDR_ZOOM_TOOLBAR );
	zoomToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	zoomToolBar.SetWindowText( rdo::format( IDR_ZOOM_TOOLBAR ).c_str() );

	zoomToolBarImageList.Create( IDB_ZOOM_TOOLBAR_D, 16, 0, 0xFF00FF );
	zoomToolBar.GetToolBarCtrl().SetDisabledImageList( &zoomToolBarImageList );

	modelToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	modelToolBar.LoadToolBar( IDR_MODEL_TOOLBAR );
	modelToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	modelToolBar.SetWindowText( rdo::format( IDR_MODEL_TOOLBAR ).c_str() );

	modelToolBarImageList.Create( IDB_MODEL_TOOLBAR_D, 16, 0, 0xFF00FF );
	modelToolBar.GetToolBarCtrl().SetDisabledImageList( &modelToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) );
	statusBar.SetPaneInfo( 0, ID_COORD_STATUSBAR           , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFY_STATUSBAR          , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITE_STATUSBAR , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_MODEL_TIME_STATUSBAR      , SBPS_NORMAL , 100 );
	statusBar.SetPaneInfo( 4, ID_MODEL_RUNTYPE_STATUSBAR   , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 5, ID_MODEL_SHOWRATE_STATUSBAR  , SBPS_NORMAL , 120 );
	statusBar.SetPaneInfo( 6, ID_PROGRESSSTATUSBAR         , SBPS_STRETCH, 70 );
	statusBar.setProgressIndicator( ID_PROGRESSSTATUSBAR );

	workspace.Create( rdo::format( ID_DOCK_WORKSPACE ).c_str(), this, 0 );
	workspace.SetBarStyle( workspace.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	output.Create( rdo::format( ID_DOCK_OUTPUT ).c_str(), this, 0 );
	output.SetBarStyle( output.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	fileToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	zoomToolBar.EnableDocking( CBRS_ALIGN_ANY );
	modelToolBar.EnableDocking( CBRS_ALIGN_ANY );
	workspace.EnableDocking( CBRS_ALIGN_ANY );
	output.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &fileToolBar );
	dockControlBarBesideOf( editToolBar, fileToolBar );
	dockControlBarBesideOf( zoomToolBar, editToolBar );
	dockControlBarBesideOf( modelToolBar, zoomToolBar );
	DockControlBar( &workspace, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &output, AFX_IDW_DOCKBAR_BOTTOM );

//	zoomToolBar.SetButtonStyle( 2, TBBS_CHECKBOX );

	modelToolBar.SetButtonStyle( 3, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	modelToolBar.SetButtonStyle( 4, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	modelToolBar.SetButtonStyle( 5, TBBS_CHECKBOX | TBBS_CHECKGROUP );

	tracer->registerClipboardFormat();

	return 0;
}

void RDOStudioMainFrame::OnDestroy()
{
	style_editor.save();
	style_build.save();
	style_debug.save();
	style_trace.save();
	style_results.save();
	style_find.save();
	style_frame.save();
	style_chart.save();

	::OleUninitialize();
	// close model before delete plugins (for PM_MODEL_CLOSE message)
	model->closeModel();
	// delete plugins before delete model
	if ( plugins ) { delete plugins; plugins = NULL; }
	if ( model   ) { delete model; model = NULL; }

	kernel.removeSyncClient( syncObject );
	delete syncObject;
	syncObject = NULL;

	CMDIFrameWnd::OnDestroy();
}

BOOL RDOStudioMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) ) return FALSE;
	return TRUE;
}

#ifdef _DEBUG
void RDOStudioMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void RDOStudioMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif

void RDOStudioMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
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

void RDOStudioMainFrame::OnViewFileToolbar() 
{
	ShowControlBar( &fileToolBar, !(fileToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewEditToolbar() 
{
	ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewZoomToolbar()
{
	ShowControlBar( &zoomToolBar, !(zoomToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewModelToolbar()
{
	ShowControlBar( &modelToolBar, !(modelToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnUpdateViewFileToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( fileToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewEditToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( editToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewZoomToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( zoomToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewModelToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( modelToolBar.GetStyle() & WS_VISIBLE );
}

BOOL RDOStudioMainFrame::OnCmdMsgForDockOnly( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if ( workspace.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( output.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return FALSE;
}

void RDOStudioMainFrame::OnViewWorkspace() 
{
	ShowControlBar( &workspace, !workspace.IsVisible(), false );
}

void RDOStudioMainFrame::OnViewOutput() 
{
	ShowControlBar( &output, !output.IsVisible(), false );
}

void RDOStudioMainFrame::OnWorkspaceShow()
{
	ShowControlBar( &workspace, true, false );
}

void RDOStudioMainFrame::OnOutputShow()
{
	ShowControlBar( &output, true, false );
}

void RDOStudioMainFrame::showWorkspace()
{
	::SendMessage( m_hWnd, WORKSPACE_SHOW_MESSAGE, 0, 0 );
}

void RDOStudioMainFrame::showOutput()
{
	::SendMessage( m_hWnd, OUTPUT_SHOW_MESSAGE, 0, 0 );
}

void RDOStudioMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( workspace.IsVisible() );
}

void RDOStudioMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( output.IsVisible() );
}

void RDOStudioMainFrame::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_MODEL_TIME, model->getModelTime() ).c_str() );
}

void RDOStudioMainFrame::showNewModelTime( const double value )
{
	::SendMessage( statusBar.m_hWnd, SB_SETTEXT, 3, reinterpret_cast<LPARAM>(rdo::format( ID_STATUSBAR_MODEL_TIME, value ).c_str()) );
}

void RDOStudioMainFrame::OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string s = "";
	if ( model->isRunning() ) {
		switch ( model->getShowMode() ) {
			case RDOSimulatorNS::SM_NoShow   : s = rdo::format( ID_STATUSBAR_MODEL_RUNNOSHOW ); break;
			case RDOSimulatorNS::SM_Animation: s = rdo::format( ID_STATUSBAR_MODEL_RUNANIMATION ); break;
			case RDOSimulatorNS::SM_Monitor  : s = rdo::format( ID_STATUSBAR_MODEL_RUNMONITOR ); break;
		}
	}
	pCmdUI->SetText( s.c_str() );
}

void RDOStudioMainFrame::OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string s = "";
	if ( model->isRunning() ) {
		double showRate = model->getShowRate();
		if ( showRate < 1e-10 || showRate > 1e10 ) {
			s = rdo::format( IDS_MODEL_SHOWRATE_E, showRate );
		} else if ( showRate >= 1 ) {
			s = rdo::format( IDS_MODEL_SHOWRATE_FMOREONE, showRate );
		} else {
			s = rdo::format( IDS_MODEL_SHOWRATE_FLESSONE, showRate );
		}
	}
	pCmdUI->SetText( s.c_str() );
}

void RDOStudioMainFrame::OnViewOptions() 
{
	RDOStudioOptions dlg;
	dlg.DoModal();
}

void RDOStudioMainFrame::updateAllStyles() const
{
	model->updateStyleOfAllModel();
	output.updateStyles();
	tracer->updateChartsStyles();
}

void RDOStudioMainFrame::beginProgress( const int lower, const int upper, const int step )
{
	statusBar.setRange( lower, upper );
	statusBar.setStep( step );
	statusBar.setPos( lower );
	statusBar.setProgressVisible( true );
}

void RDOStudioMainFrame::endProgress()
{
	statusBar.setProgressVisible( false );
}

void RDOStudioMainFrame::OnHelpContents()
{
	string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

void RDOStudioMainFrame::OnModelRunNoShow()
{
	model->setShowMode( RDOSimulatorNS::SM_NoShow );
}

void RDOStudioMainFrame::OnModelRunAnimation()
{
	model->setShowMode( RDOSimulatorNS::SM_Animation );
}

void RDOStudioMainFrame::OnModelRunMonitor()
{
	model->setShowMode( RDOSimulatorNS::SM_Monitor );
}

void RDOStudioMainFrame::OnUpdateModelRunNoShow(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning() && model->isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? model->getShowMode() == RDOSimulatorNS::SM_NoShow : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRunAnimation(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning() && model->isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? model->getShowMode() == RDOSimulatorNS::SM_Animation : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRunMonitor(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning() && model->isFrmDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? model->getShowMode() == RDOSimulatorNS::SM_Monitor : 0 );
}

void RDOStudioMainFrame::OnModelShowRateInc()
{
	double showRate = model->getShowRate();
	showRate *= 1.5;
	if ( showRate <= DBL_MAX ) {
		model->setShowRate( showRate );
	}
}

void RDOStudioMainFrame::OnModelShowRateIncFour()
{
	double showRate = model->getShowRate();
	showRate *= 4;
	if ( showRate <= DBL_MAX ) {
		model->setShowRate( showRate );
	}
}

void RDOStudioMainFrame::OnModelShowRateDecFour()
{
	double showRate = model->getShowRate();
	showRate /= 4;
	if ( showRate >= DBL_MIN ) {
		model->setShowRate( showRate );
	}
}

void RDOStudioMainFrame::OnModelShowRateDec()
{
	double showRate = model->getShowRate();
	showRate /= 1.5;
	if ( showRate >= DBL_MIN ) {
		model->setShowRate( showRate );
	}
}

void RDOStudioMainFrame::OnUpdateModelShowRateInc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() * 1.5 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() * 4 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() / 4 >= DBL_MIN );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() / 1.5 >= DBL_MIN );
}

void RDOStudioMainFrame::OnModelFrameNext()
{
	model->showNextFrame();
}

void RDOStudioMainFrame::OnModelFramePrev()
{
	model->showPrevFrame();
}

void RDOStudioMainFrame::OnUpdateModelFrameNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->canShowNextFrame() );
}

void RDOStudioMainFrame::OnUpdateModelFramePrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->canShowPrevFrame() );
}

LRESULT RDOStudioMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	static int k = 0;
	if ( message == WORKSPACE_SHOW_MESSAGE ) {
		OnWorkspaceShow();
	} else if ( message == OUTPUT_SHOW_MESSAGE ) {
		OnOutputShow();
	} else if ( message == FM_KERNEL_NOTIFY ) {
//		TRACE( "\n%d FM_KERNEL_NOTIFY", k );
//		k++;
		syncObject->notify( static_cast<RDOKernel::NotifyType>(wParam) );
//		k--;
	} else if ( message == FM_KERNEL_NOTIFYSTRING ) {
//		TRACE( "\n%d FM_KERNEL_NOTIFYSTRING", k );
//		k++;
		syncObject->notifyString( static_cast<RDOKernel::NotifyStringType>(wParam), *((const std::string*)lParam) );
//		k--;
	} else if ( message == FM_KERNEL_NOTIFYBOOLAND ) {
//		TRACE( "\n%d FM_KERNEL_NOTIFYBOOLAND", k );
//		k++;
		syncObject->notifyBoolAnd( static_cast<RDOKernel::NotifyBoolType>(wParam) );
//		k--;
	} else if ( message == FM_KERNEL_NOTIFYBOOLOR ) {
//		TRACE( "\n%d FM_KERNEL_NOTIFYBOOLOR", k );
//		k++;
		syncObject->notifyBoolOr( static_cast<RDOKernel::NotifyBoolType>(wParam) );
//		k--;
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void RDOStudioMainFrame::OnUpdateViewZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( false );
}

void RDOStudioMainFrame::OnUpdateViewZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( false );
}

void RDOStudioMainFrame::OnUpdateViewZoomAuto(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( false );
	pCmdUI->SetCheck( false );
}

void RDOStudioMainFrame::OnUpdateViewZoomReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( false );
}
