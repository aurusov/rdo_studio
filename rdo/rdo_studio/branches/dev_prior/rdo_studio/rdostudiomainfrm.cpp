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
// ---------- RDOToolBar
// ----------------------------------------------------------------------------
void RDOToolBar::init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID )
{
	CreateEx( parent, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	LoadToolBar( tbResID );
	ModifyStyle( 0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	SetWindowText( rdo::format( tbResID ).c_str() );

	disabledImage.Create( tbDisabledImageResID, 16, 0, 0xFF00FF );
	GetToolBarCtrl().SetDisabledImageList( &disabledImage );
}

// ----------------------------------------------------------------------------
// ---------- RDOToolBarModel
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOToolBarModel, RDOToolBar)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void RDOToolBarModel::init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID )
{
	RDOToolBar::init( parent, tbResID, tbDisabledImageResID );
	SetButtonInfo( CommandToIndex(ID_MODEL_SPEED_SLIDER), ID_MODEL_SPEED_SLIDER, TBBS_SEPARATOR, 100 );
	CRect rc;
	GetItemRect( CommandToIndex(ID_MODEL_SPEED_SLIDER), rc );
	rc.top    += 1;
	rc.bottom -= 1;
	slider.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_HORZ, rc, this, ID_MODEL_SPEED_SLIDER );
	slider.SetRange( 0, 100 );
	for ( int i = 1; i <= 101; i += 10 ) {
		slider.SetTic( (int)(log( (double)i ) / log101 * 100) );
	}
	slider.SetPos( 100 );
}

void RDOToolBarModel::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	if ( nSBCode = SB_THUMBTRACK ) {
		model->setSpeed( getSpeed() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
const int WORKSPACE_SHOW_MESSAGE = ::RegisterWindowMessage( "WORKSPACE_SHOW_MESSAGE" );
const int OUTPUT_SHOW_MESSAGE    = ::RegisterWindowMessage( "OUTPUT_SHOW_MESSAGE" );
const int update_timer_ID = 1;

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
	ON_COMMAND(ID_MODEL_RUNTIME_MAXSPEED, OnModelRuntimeMaxSpeed)
	ON_COMMAND(ID_MODEL_RUNTIME_JUMP, OnModelRuntimeJump)
	ON_COMMAND(ID_MODEL_RUNTIME_SYNC, OnModelRuntimeSync)
	ON_COMMAND(ID_MODEL_RUNTIME_PAUSE, OnModelRuntimePause)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_MAXSPEED, OnUpdateModelRuntimeMaxSpeed)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_JUMP, OnUpdateModelRuntimeJump)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_SYNC, OnUpdateModelRuntimeSync)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_PAUSE, OnUpdateModelRuntimePause)
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
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ENTERMENULOOP()
	ON_WM_EXITMENULOOP()
	ON_WM_ENTERIDLE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR           , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR          , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR , OnUpdateInsertOverwriteStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_TIME_STATUSBAR      , OnUpdateModelTimeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_RUNTYPE_STATUSBAR   , OnUpdateModelRunTypeStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_SPEED_STATUSBAR     , OnUpdateModelSpeedStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODEL_SHOWRATE_STATUSBAR  , OnUpdateModelShowRateStatusBar )
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORD_STATUSBAR,
	ID_MODIFY_STATUSBAR,
	ID_INSERTOVERWRITE_STATUSBAR,
	ID_MODEL_TIME_STATUSBAR,
	ID_MODEL_RUNTYPE_STATUSBAR,
	ID_MODEL_SPEED_STATUSBAR,
	ID_MODEL_SHOWRATE_STATUSBAR,
	ID_PROGRESSSTATUSBAR
};

bool RDOStudioMainFrame::close_mode = false;

RDOStudioMainFrame::RDOStudioMainFrame():
	CMDIFrameWnd(),
	update_timer( 0 )
{
}

RDOStudioMainFrame::~RDOStudioMainFrame()
{
}

int RDOStudioMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CMDIFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	// Кто-то должен поднять кернел и треды
	new RDOStudioModel();

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

	fileToolBar.init( this, IDR_FILE_TOOLBAR, IDB_FILE_TOOLBAR_D );
	editToolBar.init( this, IDR_EDIT_TOOLBAR, IDB_EDIT_TOOLBAR_D );
	zoomToolBar.init( this, IDR_ZOOM_TOOLBAR, IDB_ZOOM_TOOLBAR_D );
	modelToolBar.init( this, IDR_MODEL_TOOLBAR, IDB_MODEL_TOOLBAR_D );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) );
	statusBar.SetPaneInfo( 0, ID_COORD_STATUSBAR           , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFY_STATUSBAR          , SBPS_NORMAL , 80 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITE_STATUSBAR , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_MODEL_TIME_STATUSBAR      , SBPS_NORMAL , 100 );
	statusBar.SetPaneInfo( 4, ID_MODEL_RUNTYPE_STATUSBAR   , SBPS_NORMAL , 120 );
	statusBar.SetPaneInfo( 5, ID_MODEL_SPEED_STATUSBAR     , SBPS_NORMAL , 90 );
	statusBar.SetPaneInfo( 6, ID_MODEL_SHOWRATE_STATUSBAR  , SBPS_NORMAL , 140 );
	statusBar.SetPaneInfo( 7, ID_PROGRESSSTATUSBAR         , SBPS_STRETCH, 70 );
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
	studioApp.mainFrame = NULL;

	update_stop();
	style_editor.save();
	style_build.save();
	style_debug.save();
	style_trace.save();
	style_results.save();
	style_find.save();
	style_frame.save();
	style_chart.save();

	::OleUninitialize();
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
	CWnd* wnd = GetFocus();
	if ( wnd ) {
		std::map< HWND, CWnd* >::iterator it = cmd_wnd.begin();
		while ( it != cmd_wnd.end() ) {
			if ( it->first == wnd->m_hWnd ) {
				return it->second->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
			}
			it++;
		}
	}
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
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_MODEL_TIME, model->getTimeNow() ).c_str() );
}

void RDOStudioMainFrame::OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	std::string s = "";
	if ( model->isRunning() ) {
		switch ( model->getRuntimeMode() ) {
			case rdoRuntime::RTM_MaxSpeed  : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_MAXSPEED ); break;
			case rdoRuntime::RTM_Jump      : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_JUMP ); break;
			case rdoRuntime::RTM_Sync      : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_SYNC ); break;
			case rdoRuntime::RTM_Pause     : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_PAUSE ); break;
			case rdoRuntime::RTM_BreakPoint: s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_BREAKPOINT, model->getLastBreakPointName().c_str() ); break;
		}
	}
	pCmdUI->SetText( s.c_str() );
}

void RDOStudioMainFrame::OnUpdateModelSpeedStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	if ( model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed || !model->isRunning() ) {
		pCmdUI->SetText( rdo::format( IDS_MODEL_SPEED, static_cast<int>(model->getSpeed() * 100) ).c_str() );
	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOStudioMainFrame::OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	if ( model->isRunning() ) {
		switch ( model->getRuntimeMode() ) {
			case rdoRuntime::RTM_MaxSpeed:
			case rdoRuntime::RTM_Jump: {
				pCmdUI->SetText( rdo::format( IDS_MODEL_SHOWRATE_S, rdo::format( IDS_INFINITI ).c_str() ).c_str()  );
				break;
			}
			case rdoRuntime::RTM_Pause     :
			case rdoRuntime::RTM_BreakPoint: {
				pCmdUI->SetText( rdo::format( IDS_MODEL_SHOWRATE_S, "0.0" ).c_str() );
				break;
			}
			case rdoRuntime::RTM_Sync: {
				std::string s;
				double showRate = model->getShowRate();
				if ( showRate < 1e-10 || showRate > 1e10 ) {
					s = rdo::format( IDS_MODEL_SHOWRATE_E, showRate );
				} else if ( showRate >= 1 ) {
					s = rdo::format( IDS_MODEL_SHOWRATE_FMOREONE, showRate );
				} else {
					s = rdo::format( IDS_MODEL_SHOWRATE_FLESSONE, showRate );
				}
				pCmdUI->SetText( s.c_str() );
				break;
			}
		}
	} else {
		pCmdUI->SetText( "" );
	}
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
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

void RDOStudioMainFrame::OnModelRuntimeMaxSpeed()
{
	model->setRuntimeMode( rdoRuntime::RTM_MaxSpeed );
}

void RDOStudioMainFrame::OnModelRuntimeJump()
{
	model->setRuntimeMode( rdoRuntime::RTM_Jump );
}

void RDOStudioMainFrame::OnModelRuntimeSync()
{
	model->setRuntimeMode( rdoRuntime::RTM_Sync );
}

void RDOStudioMainFrame::OnModelRuntimePause()
{
	model->setRuntimeMode( rdoRuntime::RTM_Pause );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeMaxSpeed( CCmdUI* pCmdUI )
{
	bool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdoRuntime::RTM_MaxSpeed ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeJump( CCmdUI* pCmdUI )
{
	bool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdoRuntime::RTM_Jump ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeSync( CCmdUI* pCmdUI )
{
	bool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdoRuntime::RTM_Sync ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimePause( CCmdUI* pCmdUI )
{
	bool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && (model->getRuntimeMode() == rdoRuntime::RTM_Pause || model->getRuntimeMode() == rdoRuntime::RTM_BreakPoint) ? 1 : 0 );
}

void RDOStudioMainFrame::OnModelShowRateInc()
{
	model->setShowRate( model->getShowRate() * 1.5 );
}

void RDOStudioMainFrame::OnModelShowRateIncFour()
{
	model->setShowRate( model->getShowRate() * 4 );
}

void RDOStudioMainFrame::OnModelShowRateDecFour()
{
	model->setShowRate( model->getShowRate() / 4 );
}

void RDOStudioMainFrame::OnModelShowRateDec()
{
	model->setShowRate( model->getShowRate() / 1.5 );
}

void RDOStudioMainFrame::OnUpdateModelShowRateInc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdoRuntime::RTM_Sync && model->getShowRate() * 1.5 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdoRuntime::RTM_Sync && model->getShowRate() * 4 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdoRuntime::RTM_Sync && model->getShowRate() / 4 >= DBL_MIN );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdoRuntime::RTM_Sync && model->getShowRate() / 1.5 >= DBL_MIN );
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
	if ( message == WORKSPACE_SHOW_MESSAGE ) {
		OnWorkspaceShow();
	} else if ( message == OUTPUT_SHOW_MESSAGE ) {
		OnOutputShow();
	}
	return CMDIFrameWnd::WindowProc( message, wParam, lParam );
}

void RDOStudioMainFrame::update_start()
{
	update_timer = SetTimer( update_timer_ID, 1000 / 30, NULL );
}

void RDOStudioMainFrame::update_stop()
{
	if ( update_timer ) {
		KillTimer( update_timer );
		update_timer = 0;
	}
}

void RDOStudioMainFrame::OnTimer( UINT nIDEvent )
{
	if ( nIDEvent == update_timer ) {
		model->update();
	}
	CMDIFrameWnd::OnTimer( nIDEvent );
}

void RDOStudioMainFrame::OnClose()
{
	close_mode = true;
	CMDIFrameWnd::OnClose();
	// close_mode объявлена как static, т.к. после удачного закрытия всех документов
	// происходит реальное удаление главного окна и this уже не определен
	close_mode = false;
}

void RDOStudioMainFrame::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CMDIFrameWnd::OnShowWindow( bShow, nStatus );
	if ( plugins ) plugins->saveMainFrameState( bShow ? SW_SHOW : SW_HIDE );
}

void RDOStudioMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CMDIFrameWnd::OnSize( nType, cx, cy );
	if ( plugins ) plugins->saveMainFrameState( nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED ? SW_SHOW : SW_HIDE );
}

void RDOStudioMainFrame::OnEnterMenuLoop( BOOL bIsTrackPopupMenu )
{
	CMDIFrameWnd::OnEnterMenuLoop( bIsTrackPopupMenu );
	model->setGUIPause();
}

void RDOStudioMainFrame::OnExitMenuLoop( BOOL bIsTrackPopupMenu )
{
	CMDIFrameWnd::OnExitMenuLoop( bIsTrackPopupMenu );
	model->setGUIContinue();
}

void RDOStudioMainFrame::OnEnterIdle( UINT nWhy, CWnd* pWho )
{
	CMDIFrameWnd::OnEnterIdle( nWhy, pWho );
//	model->setGUIPause();
}
