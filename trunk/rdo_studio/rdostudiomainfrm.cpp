#include "stdafx.h"
#include "rdostudiomainfrm.h"
#include "rdostudioapp.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "rdo_edit/rdoeditoredit.h"
#include "resource.h"

#include <rdokernel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOStudioMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RDOStudioMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RDOStudioMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TOOLBAR_PROJECTTOOLBAR, OnViewProjectToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnViewEditToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_PROJECTTOOLBAR, OnUpdateViewProjectToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDITTOOLBAR, OnUpdateViewEditToolbar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORDSTATUSBAR          , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFYSTATUSBAR         , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITESTATUSBAR, OnUpdateInsertOverwriteStatusBar )
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

	projectToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	projectToolBar.LoadToolBar( IDR_PROJECTTOOLBAR );
	projectToolBar.GetToolBarCtrl().SetWindowText( studioApp.sprintf( IDR_PROJECTTOOLBAR ).c_str() );

	projectToolBarImageList.Create( IDB_PROJECTTOOLBAR_D, 16, 0, 0xFF00FF );
	projectToolBar.GetToolBarCtrl().SetDisabledImageList( &projectToolBarImageList );

	editToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDITTOOLBAR );
	editToolBar.GetToolBarCtrl().SetWindowText( studioApp.sprintf( IDR_EDITTOOLBAR ).c_str() );

	editToolBarImageList.Create( IDB_EDITTOOLBAR_D, 16, 0, 0xFF00FF );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, 5 );
	statusBar.SetPaneInfo( 0, ID_COORDSTATUSBAR          , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 1, ID_MODIFYSTATUSBAR         , SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITESTATUSBAR, SBPS_NORMAL , 70 );
	statusBar.SetPaneInfo( 3, ID_MODELTIMESTATUSBAR      , SBPS_NORMAL , 100 );
	statusBar.SetPaneInfo( 4, ID_MODELRUNTYPESTATUSBAR   , SBPS_STRETCH, 70 );

	workspace.Create( studioApp.sprintf( ID_DOCK_WORKSPACE ).c_str(), this, -1 );
	workspace.SetBarStyle( workspace.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	output.Create( studioApp.sprintf( ID_DOCK_OUTPUT ).c_str(), this, -1 );
	output.SetBarStyle( output.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	projectToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	workspace.EnableDocking( CBRS_ALIGN_ANY );
	output.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &projectToolBar );
	dockControlBarBesideOf( editToolBar, projectToolBar );
	DockControlBar( &workspace, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &output, AFX_IDW_DOCKBAR_BOTTOM );

	trace.registerClipboardFormat();

	kernel.setNotifyReflect( RDOKernel::buildString, buildNotify );
	kernel.setNotifyReflect( RDOKernel::debugString, debugNotify );

	return 0;
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

void RDOStudioMainFrame::OnViewProjectToolbar() 
{
	ShowControlBar( &projectToolBar, !(projectToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewEditToolbar() 
{
	ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnUpdateViewProjectToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( projectToolBar.GetStyle() & WS_VISIBLE );
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
	string str = "";
	RDOStudioModelDoc* doc = studioApp.getModelDoc();
	if ( doc ) {
		RDOStudioModelView* view = doc->getView();
		if ( view ) {
			RDOEditorEdit* edit = view->getEdit();
			if ( edit ) {
				int x = edit->getCurrentColumnNumber() + 1;
				int y = edit->getCurrentLineNumber() + 1;
				str = studioApp.sprintf( "%d: %d", x, y );
			}
		}
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioMainFrame::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	RDOStudioModelDoc* doc = studioApp.getModelDoc();
	if ( doc ) {
		RDOStudioModelView* view = doc->getView();
		if ( view ) {
			RDOEditorEdit* edit = view->getEdit();
			if ( edit ) {
				if ( edit->isReadOnly() ) {
					str = studioApp.sprintf( ID_STATUSBAR_READONLY );
				} else if ( edit->isModify() ) {
					str = studioApp.sprintf( ID_STATUSBAR_MODIFIED );
				}
			}
		}
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioMainFrame::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	RDOStudioModelDoc* doc = studioApp.getModelDoc();
	if ( doc ) {
		RDOStudioModelView* view = doc->getView();
		if ( view ) {
			RDOEditorEdit* edit = view->getEdit();
			if ( edit && edit->isOverwrite() ) {
				str = studioApp.sprintf( ID_STATUSBAR_OVERWRITE );
			}
		}
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioMainFrame::buildNotify( string str )
{
	studioApp.mainFrame->output.appendStringToBuild( str );
}

void RDOStudioMainFrame::debugNotify( string str )
{
	studioApp.mainFrame->output.appendStringToDebug( str );
}
