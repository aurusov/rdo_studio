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

rbool RDOTracerLogCtrlView::getItemColors(int index, RDOLogColorPair* &colors) const
{
	const_cast<CMutex&>(mutex).Lock();

	rbool res = true;
	SubitemColors::List::const_iterator it = m_subitemColors.m_colorList.find(index);
	if (it != m_subitemColors.m_colorList.end())
	{
		colors = (*it).second;
	}
	else
	{
		res = RDOLogCtrl::getItemColors("", colors);
	}

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}

void RDOTracerLogCtrlView::clear()
{
	mutex.Lock();

	RDOLogCtrl::clear();
	m_subitemColors = SubitemColors();

	mutex.Unlock();
}

void RDOTracerLogCtrlView::addStringToLog(CREF(tstring) logStr)
{
	mutex.Lock();
	
	if (!logStr.empty())
	{
		int posstart = logStr.find_first_not_of(' ');
		int posend   = logStr.find_first_of(' ', posstart);
		tstring key  = logStr.substr(posstart, posend - posstart);
		rdo::trim(key);

		RDOLogColorPair* colors = NULL;

		if (logStyle->getItemColors(key, colors))
		{
			m_subitemColors.m_addingSubitems = false;
		}
		else if (m_subitemColors.m_addingSubitems)
		{
			m_subitemColors.m_colorList.insert(SubitemColors::List::value_type(m_strings.count(), m_subitemColors.m_parentColor));
		}

		if (key == "SD")
		{
			m_subitemColors.m_addingSubitems = true;
			logStyle->getItemColors(key, m_subitemColors.m_parentColor);
		}
	}

	RDOLogCtrl::addStringToLog(logStr);

	mutex.Unlock();
}

void RDOTracerLogCtrlView::setStyle(RDOTracerLogStyle* style, rbool needRedraw)
{
	logStyle = style;
	RDOLogCtrl::setStyle(style, needRedraw);
}

void RDOTracerLogCtrlView::OnUpdateCoordStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	if (selectedLine() != -1)
	{
		pCmdUI->SetText(rdo::format("%d: %d", 1, selectedLine() + 1).c_str());
	}
	else
	{
		pCmdUI->SetText("");
	}
}

void RDOTracerLogCtrlView::OnUpdateModifyStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetText(rdo::format(ID_STATUSBAR_READONLY).c_str());
}
