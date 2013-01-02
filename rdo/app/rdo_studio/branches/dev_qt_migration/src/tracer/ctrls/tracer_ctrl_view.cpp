/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_view.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
#include <QtGui/qmessagebox.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
RDOTracerLogCtrl::RDOTracerLogCtrl(PTR(QWidget) pParent)
	: QAbstractScrollArea(pParent)
{
	PTR(RDOTracerLogCtrlView) pLog = new RDOTracerLogCtrlView(this);
	pLog->show();

	setViewport(pLog);
}

rbool RDOTracerLogCtrl::viewportEvent(PTR(QEvent) pEvent)
{
	UNUSED(pEvent);
	return false;
}

void RDOTracerLogCtrl::focusInEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void RDOTracerLogCtrl::focusOutEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void RDOTracerLogCtrl::keyPressEvent(PTR(QKeyEvent) pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Up      :
	case Qt::Key_Down    :
	case Qt::Key_PageUp  :
	case Qt::Key_PageDown:
	case Qt::Key_Home    :
	case Qt::Key_End     :
		static_cast<QObject*>(viewport())->event(pEvent);
		break;

	default:
		QAbstractScrollArea::keyPressEvent(pEvent);
		break;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrlView
// --------------------------------------------------------------------------------
//! @todo qt
//BEGIN_MESSAGE_MAP(RDOTracerLogCtrlView, RDOLogCtrl)
//	ON_UPDATE_COMMAND_UI(ID_COORD_STATUSBAR , OnUpdateCoordStatusBar)
//	ON_UPDATE_COMMAND_UI(ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar)
//END_MESSAGE_MAP()

RDOTracerLogCtrlView::RDOTracerLogCtrlView(PTR(QAbstractScrollArea) pParent)
	: RDOLogCtrl(pParent, &studioApp.getStyle()->style_trace)
{}

RDOTracerLogCtrlView::~RDOTracerLogCtrlView()
{}

void RDOTracerLogCtrlView::OnUpdateCoordStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	//if (selectedLine() != -1)
	//{
	//	pCmdUI->SetText(rdo::format("%d: %d", 1, selectedLine() + 1).c_str());
	//}
	//else
	//{
	//	pCmdUI->SetText("");
	//}
}

void RDOTracerLogCtrlView::OnUpdateModifyStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetText(rdo::format(ID_STATUSBAR_READONLY).c_str());
}
