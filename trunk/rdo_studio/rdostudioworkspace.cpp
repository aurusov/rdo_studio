#include "stdafx.h"
#include "rdostudioworkspace.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "./rdo_tracer/tracer_ctrls/rdotracertreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioWorkspace
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioWorkspace, RDOStudioDockWnd)
	//{{AFX_MSG_MAP(RDOStudioWorkspace)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioWorkspace::RDOStudioWorkspace()
{
}

RDOStudioWorkspace::~RDOStudioWorkspace()
{
}

int RDOStudioWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );
	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	RDOTracerTreeCtrl* page1 = tracer.createTree();
	CEdit* page2 = new CEdit;

	page1->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );
	page2->Create( NULL, CRect(0, 0, 0, 0), &tab, 0 );

	page2->ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

	tab.insertItem( page1, "Tracer" );
	tab.insertItem( page2, "Frames" );

	return 0;
}

BOOL RDOStudioWorkspace::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioDockWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
