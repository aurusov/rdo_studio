#include "stdafx.h"
#include "rdostudiomainfrm.h"
#include "rdostudioapp.h"
#include "rdostudiomodel.h"
#include "rdostudiooptions.h"
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
IMPLEMENT_DYNAMIC(RDOStudioMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RDOStudioMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RDOStudioMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TOOLBAR_FILETOOLBAR, OnViewFileToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_MODELTOOLBAR, OnViewModelToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnViewEditToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FILETOOLBAR, OnUpdateViewFileToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_MODELTOOLBAR, OnUpdateViewModelToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnUpdateViewEditToolbar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_MODEL_RUNNOSHOW, OnModelRunNoShow)
	ON_COMMAND(ID_MODEL_RUNANIMATION, OnModelRunAnimation)
	ON_COMMAND(ID_MODEL_RUNMONITOR, OnModelRunMonitor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNNOSHOW, OnUpdateModelRunNoShow)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNANIMATION, OnUpdateModelRunAnimation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNMONITOR, OnUpdateModelRunMonitor)
	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowRateInc)
	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowRateIncFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowRateDecFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowRateDec)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowRateInc)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowRateIncFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowRateDecFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowRateDec)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORDSTATUSBAR          , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFYSTATUSBAR         , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITESTATUSBAR, OnUpdateInsertOverwriteStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODELTIMESTATUSBAR      , OnUpdateModelTimeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODELRUNTYPESTATUSBAR   , OnUpdateModelRunTypeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODELSHOWRATE           , OnUpdateModelShowRateStatusBar )
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORDSTATUSBAR,
	ID_MODIFYSTATUSBAR,
	ID_INSERTOVERWRITESTATUSBAR,
	ID_MODELTIMESTATUSBAR,
	ID_MODELRUNTYPESTATUSBAR,
	ID_MODELSHOWRATE,
	ID_PROGRESSSTATUSBAR
};

RDOStudioMainFrame::RDOStudioMainFrame(): CMDIFrameWnd()
{
}

RDOStudioMainFrame::~RDOStudioMainFrame()
{
}

int RDOStudioMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CMDIFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

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
	style_frame.theme->backgroundColor = RGB( 0, 128, 128 );
	style_frame.load();

	style_chart.init( "chart" );
	style_chart.load();

	fileToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	fileToolBar.LoadToolBar( IDR_FILETOOLBAR );
	fileToolBar.GetToolBarCtrl().SetWindowText( format( IDR_FILETOOLBAR ).c_str() );

	fileToolBarImageList.Create( IDB_FILETOOLBAR_D, 16, 0, 0xFF00FF );
	fileToolBar.GetToolBarCtrl().SetDisabledImageList( &fileToolBarImageList );

	modelToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	modelToolBar.LoadToolBar( IDR_MODELTOOLBAR );
	modelToolBar.GetToolBarCtrl().SetWindowText( format( IDR_MODELTOOLBAR ).c_str() );

	modelToolBarImageList.Create( IDB_MODELTOOLBAR_D, 16, 0, 0xFF00FF );
	modelToolBar.GetToolBarCtrl().SetDisabledImageList( &modelToolBarImageList );

	editToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDITTOOLBAR );
	editToolBar.GetToolBarCtrl().SetWindowText( format( IDR_EDITTOOLBAR ).c_str() );

	editToolBarImageList.Create( IDB_EDITTOOLBAR_D, 16, 0, 0xFF00FF );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) );
	statusBar.SetPaneInfo( 0, ID_COORDSTATUSBAR          , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFYSTATUSBAR         , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITESTATUSBAR, SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_MODELTIMESTATUSBAR      , SBPS_NORMAL , 100 );
	statusBar.SetPaneInfo( 4, ID_MODELRUNTYPESTATUSBAR   , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 5, ID_MODELSHOWRATE           , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 6, ID_PROGRESSSTATUSBAR       , SBPS_STRETCH, 70 );

	workspace.Create( format( ID_DOCK_WORKSPACE ).c_str(), this, 0 );
	workspace.SetBarStyle( workspace.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	output.Create( format( ID_DOCK_OUTPUT ).c_str(), this, 0 );
	output.SetBarStyle( output.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	fileToolBar.EnableDocking( CBRS_ALIGN_ANY );
	modelToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	workspace.EnableDocking( CBRS_ALIGN_ANY );
	output.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &fileToolBar );
	dockControlBarBesideOf( modelToolBar, fileToolBar );
	dockControlBarBesideOf( editToolBar, modelToolBar );
	DockControlBar( &workspace, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &output, AFX_IDW_DOCKBAR_BOTTOM );

	modelToolBar.SetButtonStyle( 3, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	modelToolBar.SetButtonStyle( 4, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	modelToolBar.SetButtonStyle( 5, TBBS_CHECKBOX | TBBS_CHECKGROUP );

	tracer->registerClipboardFormat();

	model = new RDOStudioModel;

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
	if ( model ) { delete model; model = NULL; }
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

void RDOStudioMainFrame::OnViewModelToolbar()
{
	ShowControlBar( &modelToolBar, !(modelToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewEditToolbar() 
{
	ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnUpdateViewFileToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( fileToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewModelToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( modelToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewEditToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( editToolBar.GetStyle() & WS_VISIBLE );
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
	pCmdUI->SetText( format( ID_STATUSBAR_MODELTIME, model->getModelTime() ).c_str() );
}

void RDOStudioMainFrame::showNewModelTime( const double value )
{
	::SendMessage( statusBar.m_hWnd, SB_SETTEXT, 3, reinterpret_cast<LPARAM>(format( ID_STATUSBAR_MODELTIME, value ).c_str()) );
}

void RDOStudioMainFrame::OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string s = "";
	if ( model->isRunning() ) {
		switch ( model->getShowMode() ) {
			case RDOSimulatorNS::SM_NoShow   : s = format( ID_STATUSBAR_MODEL_RUNNOSHOW ); break;
			case RDOSimulatorNS::SM_Animation: s = format( ID_STATUSBAR_MODEL_RUNANIMATION ); break;
			case RDOSimulatorNS::SM_Monitor  : s = format( ID_STATUSBAR_MODEL_RUNMONITOR ); break;
		}
	}
	pCmdUI->SetText( s.c_str() );
}

void RDOStudioMainFrame::OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string s = "";
	if ( model->isRunning() ) {
		s = format( IDS_MODELSHOWRATE, model->getShowRate() );
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
	string filename = studioApp.getFullHelpFileName( "RAO-language.chm" );
	if ( filename.empty() ) return;

	HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

void RDOStudioMainFrame::OnModelRunNoShow()
{
	model->setShowMode( RDOSimulatorNS::SM_NoShow );
	tracer->setShowMode( RDOSimulatorNS::SM_NoShow );
}

void RDOStudioMainFrame::OnModelRunAnimation()
{
	model->setShowMode( RDOSimulatorNS::SM_Animation );
	tracer->setShowMode( RDOSimulatorNS::SM_Animation );
}

void RDOStudioMainFrame::OnModelRunMonitor()
{
	model->setShowMode( RDOSimulatorNS::SM_Monitor );
	tracer->setShowMode( RDOSimulatorNS::SM_Monitor );
}

void RDOStudioMainFrame::OnUpdateModelRunNoShow(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? model->getShowMode() == RDOSimulatorNS::SM_NoShow : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRunAnimation(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? model->getShowMode() == RDOSimulatorNS::SM_Animation : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRunMonitor(CCmdUI* pCmdUI)
{
	bool flag = model->isRunning();
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
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() * 1.5 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() * 4 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() / 4 >= DBL_MIN );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() && model->getShowMode() != RDOSimulatorNS::SM_NoShow && model->getShowRate() / 1.5 >= DBL_MIN );
}
