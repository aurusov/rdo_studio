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
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view_style.h"
#include "app/rdo_studio_mfc/src/application.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
RDOTracerLogCtrl::RDOTracerLogCtrl(PTR(QWidget) pParent)
	: QAbstractScrollArea(pParent)
{
	PTR(RDOLogCtrl) pLog = new RDOLogCtrl(this, &studioApp.getStyle()->style_trace);
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
