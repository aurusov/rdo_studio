/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_main_wnd.cpp
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_main_wnd.h"
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view_style.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- LogMainWnd
// --------------------------------------------------------------------------------
LogMainWnd::LogMainWnd(QWidget* pParent)
	: QAbstractScrollArea(pParent)
{
	PTR(LogView) pLog = new LogView(this, &g_pApp->getStyle()->style_trace);
	pLog->show();

	setViewport(pLog);
}

bool LogMainWnd::viewportEvent(PTR(QEvent) pEvent)
{
	UNUSED(pEvent);
	return false;
}

void LogMainWnd::focusInEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void LogMainWnd::focusOutEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void LogMainWnd::keyPressEvent(PTR(QKeyEvent) pEvent)
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
