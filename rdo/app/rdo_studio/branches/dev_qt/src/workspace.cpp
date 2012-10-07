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
#include <QtGui/qevent.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
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
RDOStudioWorkspace::RDOStudioWorkspace(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setMinimumSize(200, 150);

	m_thisCWnd.Attach(winId());

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), &m_thisCWnd, 0 );
	tab.modifyTabStyle( 0, TCS_MULTILINE );

#ifdef PROCGUI_ENABLE
	pagectrl = new RPPageCtrl;
	pagectrl->Create( "", "", 0 , CRect(0, 0, 0, 0), &tab, 0);
	tab.insertItem( pagectrl, rdo::format( IDS_TAB_PAGECTRL ).c_str() );
#endif
//! @todo qt
//	studioApp.m_pMainFrame->registerCmdWnd( trace );
//	studioApp.m_pMainFrame->registerCmdWnd( frames );
#ifdef PROCGUI_ENABLE
//! @todo qt
//	studioApp.m_pMainFrame->registerCmdWnd( pagectrl );
#endif
}

RDOStudioWorkspace::~RDOStudioWorkspace()
{}

void RDOStudioWorkspace::resizeEvent(PTR(QResizeEvent) event)
{
	parent_type::resizeEvent(event);
	QSize size(event->size());
	tab.MoveWindow(0, 0, size.width(), size.height());
}
