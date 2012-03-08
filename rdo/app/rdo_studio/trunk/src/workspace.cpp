/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      workspace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// --------------------------------------------------------------------------------
// -------------------- RDOStudioWorkspace
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioWorkspace, RDOStudioDockWnd)
	ON_WM_CREATE()
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
#ifdef PROCGUI_ENABLE
	pagectrl = new RPPageCtrl;
	pagectrl->Create( "", "", 0 , CRect(0, 0, 0, 0), &tab, 0);
#endif
	tab.insertItem( trace, rdo::format( IDS_TAB_TRACER ).c_str() );
	tab.insertItem( frames, rdo::format( IDS_TAB_FRAMES ).c_str() );
#ifdef PROCGUI_ENABLE
	tab.insertItem( pagectrl, rdo::format( IDS_TAB_PAGECTRL ).c_str() );
#endif
	studioApp.mainFrame->registerCmdWnd( trace );
	studioApp.mainFrame->registerCmdWnd( frames );
#ifdef PROCGUI_ENABLE
	studioApp.mainFrame->registerCmdWnd( pagectrl );
#endif
	return 0;
}
