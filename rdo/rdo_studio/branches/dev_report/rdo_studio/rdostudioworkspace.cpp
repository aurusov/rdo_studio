#include "stdafx.h"
#include "rdostudioworkspace.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdo_tracer/rdotracer.h"
#include "rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "resource.h"

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

RDOStudioWorkspace::RDOStudioWorkspace():
	RDOStudioDockWnd(),
	frames( NULL )
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
	tab.modifyTabStyle( 0, TCS_MULTILINE );
//	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	RDOTracerTreeCtrl* trace = tracer->createTree();
	trace->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );

	frames = new RDOStudioFrameTreeCtrl;
	frames->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );

	tab.insertItem( trace, rdo::format( IDS_TAB_TRACER ).c_str() );
	tab.insertItem( frames, rdo::format( IDS_TAB_FRAMES ).c_str() );

	studioApp.mainFrame->registerCmdWnd( trace );
	studioApp.mainFrame->registerCmdWnd( frames );

	return 0;
}
