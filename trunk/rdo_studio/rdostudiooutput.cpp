#include "stdafx.h"
#include "rdostudiooutput.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "edit_ctrls/rdobuildedit.h"
#include "edit_ctrls/rdodebugedit.h"
#include "edit_ctrls/rdofindedit.h"
#include "rdo_edit/rdoeditoredit.h"
#include "./rdo_tracer/rdotracertrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOStudioOutput
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioOutput, RDOStudioDockWnd)
	//{{AFX_MSG_MAP(RDOStudioOutput)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioOutput::RDOStudioOutput()
{
}

RDOStudioOutput::~RDOStudioOutput()
{
	eraseMenu( &popupMenu );
}

int RDOStudioOutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, -1 );
	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 7, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 9, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 2 + delta ), 10, &popupMenu );

	build   = new RDOBuildEdit;
	debug   = new RDODebugEdit;
	tracer  = (CWnd*)::trace.createLog();
	results = new RDOEditorEdit;
	find    = new RDOFindEdit;

	build->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	debug->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	tracer->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	results->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	find->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );

	buildStyle.init( "buildStyle" );
	buildStyle.window->showHorzScrollBar = false;
	buildStyle.window->wordWrap          = true;
	buildStyle.load();
	build->setEditorStyle( &buildStyle );
	build->setReadOnly( true );
	build->setPopupMenu( &popupMenu );

	debugStyle.init( "debugStyle" );
	debugStyle.window->showHorzScrollBar = false;
	debugStyle.window->wordWrap          = true;
	debugStyle.load();
	debug->setEditorStyle( &debugStyle );
	debug->setReadOnly( true );
	debug->setPopupMenu( &popupMenu );

	resultsStyle.init( "resultsStyle" );
	resultsStyle.window->showHorzScrollBar = false;
	resultsStyle.window->wordWrap          = true;
	resultsStyle.load();
	results->setEditorStyle( &resultsStyle );
	results->setViewMargin( false );
	results->setViewFoldMargin( false );
	results->setReadOnly( true );
	results->setPopupMenu( &popupMenu );

	findStyle.init( "findStyle" );
	findStyle.load();
	find->setEditorStyle( &findStyle );
	find->setReadOnly( true );
	find->setPopupMenu( &popupMenu );

	tab.insertItem( build, "Build" );
	tab.insertItem( debug, "Debug" );
	tab.insertItem( tracer, "Tracer" );
	tab.insertItem( results, "Results" );
	tab.insertItem( find, "Find in Model" );

	return 0;
}

BOOL RDOStudioOutput::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioDockWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RDOStudioOutput::showBuild()
{
	build->SetFocus();
}

void RDOStudioOutput::showDebug()
{
	debug->SetFocus();
}

void RDOStudioOutput::showTracer()
{
	tab.setCurrentItem( 2 );
	tracer->SetFocus();
}

void RDOStudioOutput::showResults()
{
	results->SetFocus();
}

void RDOStudioOutput::showFind()
{
	find->SetFocus();
}

void RDOStudioOutput::clearBuild()
{
	build->clearAll();
}

void RDOStudioOutput::clearDebug()
{
	debug->clearAll();
}

void RDOStudioOutput::clearResults()
{
	results->clearAll();
}

void RDOStudioOutput::clearFind()
{
	find->clearAll();
}

void RDOStudioOutput::appendStringToBuild( const string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const bool error ) const
{
	RDOBuildEditLineInfo* line = new RDOBuildEditLineInfo( str, fileType, lineNumber, error );
	build->appendLine( line );
}

void RDOStudioOutput::appendStringToDebug( const string& str ) const
{
	debug->appendLine( str );
}

void RDOStudioOutput::appendStringToFind( const string& str, const rdoModelObjects::RDOFileType fileType, const int lineNumber, const int posInLine ) const
{
	RDOLogEditLineInfo* line = new RDOLogEditLineInfo( str, fileType, lineNumber, posInLine );
	find->appendLine( line );
}
