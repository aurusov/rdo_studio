#include "stdafx.h"
#include "rdostudiomainfrm.h"
#include "rdostudiomodel.h"
#include "rdostudiooptions.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "resource.h"

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
	ON_COMMAND(ID_VIEW_TOOLBAR_BUILDTOOLBAR, OnViewBuildToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnViewEditToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FILETOOLBAR, OnUpdateViewFileToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_BUILDTOOLBAR, OnUpdateViewBuildToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnUpdateViewEditToolbar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORDSTATUSBAR          , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFYSTATUSBAR         , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITESTATUSBAR, OnUpdateInsertOverwriteStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODELTIMESTATUSBAR      , OnUpdateModelTimeStatusBar )
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORDSTATUSBAR,
	ID_MODIFYSTATUSBAR,
	ID_INSERTOVERWRITESTATUSBAR,
	ID_MODELTIMESTATUSBAR,
	ID_MODELRUNTYPESTATUSBAR
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

	style_editor.init( "editorStyle" );
	style_editor.load();

	style_build.init( "buildStyle" );
	style_build.window->showHorzScrollBar = false;
	style_build.window->wordWrap          = true;
	style_build.load();

	style_debug.init( "debugStyle" );
	style_debug.window->showHorzScrollBar = false;
	style_debug.window->wordWrap          = true;
	style_debug.load();

	style_results.init( "resultsStyle" );
	style_results.load();

	style_find.init( "findStyle" );
	style_find.load();

	fileToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	fileToolBar.LoadToolBar( IDR_FILETOOLBAR );
	fileToolBar.GetToolBarCtrl().SetWindowText( format( IDR_FILETOOLBAR ).c_str() );

	fileToolBarImageList.Create( IDB_FILETOOLBAR_D, 16, 0, 0xFF00FF );
	fileToolBar.GetToolBarCtrl().SetDisabledImageList( &fileToolBarImageList );

	buildToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	buildToolBar.LoadToolBar( IDR_BUILDTOOLBAR );
	buildToolBar.GetToolBarCtrl().SetWindowText( format( IDR_BUILDTOOLBAR ).c_str() );

	buildToolBarImageList.Create( IDB_BUILDTOOLBAR_D, 16, 0, 0xFF00FF );
	buildToolBar.GetToolBarCtrl().SetDisabledImageList( &buildToolBarImageList );

	editToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDITTOOLBAR );
	editToolBar.GetToolBarCtrl().SetWindowText( format( IDR_EDITTOOLBAR ).c_str() );

	editToolBarImageList.Create( IDB_EDITTOOLBAR_D, 16, 0, 0xFF00FF );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, 5 );
	statusBar.SetPaneInfo( 0, ID_COORDSTATUSBAR          , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFYSTATUSBAR         , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITESTATUSBAR, SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_MODELTIMESTATUSBAR      , SBPS_NORMAL , 140 );
	statusBar.SetPaneInfo( 4, ID_MODELRUNTYPESTATUSBAR   , SBPS_STRETCH, 70 );

	workspace.Create( format( ID_DOCK_WORKSPACE ).c_str(), this, -1 );
	workspace.SetBarStyle( workspace.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	output.Create( format( ID_DOCK_OUTPUT ).c_str(), this, -1 );
	output.SetBarStyle( output.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	fileToolBar.EnableDocking( CBRS_ALIGN_ANY );
	buildToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	workspace.EnableDocking( CBRS_ALIGN_ANY );
	output.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &fileToolBar );
	dockControlBarBesideOf( buildToolBar, fileToolBar );
	dockControlBarBesideOf( editToolBar, buildToolBar );
	DockControlBar( &workspace, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &output, AFX_IDW_DOCKBAR_BOTTOM );

	trace.registerClipboardFormat();

	model = new RDOStudioModel;

	return 0;
}

void RDOStudioMainFrame::OnDestroy()
{
	style_editor.save();
	style_build.save();
	style_debug.save();
	style_results.save();
	style_find.save();

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

void RDOStudioMainFrame::OnViewBuildToolbar() 
{
	ShowControlBar( &buildToolBar, !(buildToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewEditToolbar() 
{
	ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnUpdateViewFileToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( fileToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewBuildToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( buildToolBar.GetStyle() & WS_VISIBLE );
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
	string str = "";
	if ( model->isRunning() ) {
		str = format( ID_STATUSBAR_MODELTIME, model->getModelTime() );
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioMainFrame::OnViewOptions() 
{
	RDOStudioOptions dlg;
	dlg.DoModal();
}

void RDOStudioMainFrame::updateAllStyles() const
{
	model->updateStyleOfAllModel();
}
