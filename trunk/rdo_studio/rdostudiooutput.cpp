#include "stdafx.h"
#include "rdostudiooutput.h"
#include "rdo_edit/rdoeditorsciedit.h"
#include "rdo_edit/rdoeditorscilog.h"
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
}

int RDOStudioOutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, -1 );
	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	build   = new RDOEditorSciLog;
	debug   = new RDOEditorSciEdit;
	tracer  = (CWnd*)::trace.createLog();
	results = new RDOEditorEdit;

	build->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	debug->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	tracer->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );
	results->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), -1 );

	buildStyle.init( "buildStyle" );
	buildStyle.window->showHorzScrollBar = false;
	buildStyle.window->wordWrap          = true;
	buildStyle.load();
	build->setEditorStyle( &buildStyle );
//	build->setReadOnly( true );

	debugStyle.init( "debugStyle" );
	debugStyle.window->showHorzScrollBar = false;
	debugStyle.window->wordWrap          = true;
	debugStyle.load();
	debug->setEditorStyle( &debugStyle );
//	debug->setReadOnly( true );

	resultsStyle.init( "resultsStyle" );
	resultsStyle.window->showHorzScrollBar = false;
	resultsStyle.window->wordWrap          = true;
	resultsStyle.load();
	results->setEditorStyle( &resultsStyle );
	results->setViewMargin( false );
	results->setViewFoldMargin( false );
//	results->setReadOnly( true );

	tab.insertItem( build, "Build" );
	tab.insertItem( debug, "Debug" );
	tab.insertItem( tracer, "Tracer" );
	tab.insertItem( results, "Results" );

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

void RDOStudioOutput::appendStringToDebug( const string& str )
{
	debug->appendText( str );
}
