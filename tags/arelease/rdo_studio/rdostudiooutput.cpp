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

	RDOEditorSciLog*  page1 = new RDOEditorSciLog;
	RDOEditorSciEdit* page2 = new RDOEditorSciEdit;
	CWnd*             page3 = (CWnd*)trace.createLog();
	RDOEditorEdit*    page4 = new RDOEditorEdit;

	page1->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page2->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page3->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page4->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );

	buildStyle.init( "buildStyle" );
	buildStyle.window->showHorzScrollBar = false;
	buildStyle.window->wordWrap          = true;
	buildStyle.load();
	page1->setEditorStyle( &buildStyle );
//	page1->setReadOnly( true );

	debugStyle.init( "debugStyle" );
	debugStyle.window->showHorzScrollBar = false;
	debugStyle.window->wordWrap          = true;
	debugStyle.load();
	page2->setEditorStyle( &debugStyle );
//	page2->setReadOnly( true );

	resultsStyle.init( "resultsStyle" );
	resultsStyle.window->showHorzScrollBar = false;
	resultsStyle.window->wordWrap          = true;
	resultsStyle.load();
	page4->setEditorStyle( &resultsStyle );
	page4->setViewMargin( false );
	page4->setViewFoldMargin( false );
//	page4->setReadOnly( true );

	tab.insertItem( page1, "Build" );
	tab.insertItem( page2, "Debug" );
	tab.insertItem( page3, "Tracer" );
	tab.insertItem( page4, "Results" );

	return 0;
}

BOOL RDOStudioOutput::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioDockWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
