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
//	ON_WM_INITMENUPOPUP()
//	ON_WM_CONTEXTMENU()
//	ON_UPDATE_COMMAND_UI(ID_COORD_STATUSBAR , OnUpdateCoordStatusBar)
//	ON_UPDATE_COMMAND_UI(ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar)
//END_MESSAGE_MAP()

RDOTracerLogCtrlView::RDOTracerLogCtrlView(PTR(QAbstractScrollArea) pParent)
	: RDOLogCtrl(pParent, &studioApp.getStyle()->style_trace)
	, addingSubitems(false)
	, bShowMenu(true)
{
	//! todo qt
	//popupMenu.CreatePopupMenu();

	//if (AfxGetMainWnd())
	//{
	//	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	//	if (mainMenu)
	//	{
	//		rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//		int delta = maximized ? 1 : 0;

	//		appendMenu(mainMenu->GetSubMenu(1 + delta), 4, &popupMenu);
	//		popupMenu.AppendMenu(MF_SEPARATOR);
	//		appendMenu(mainMenu->GetSubMenu(2 + delta), 0, &popupMenu);
	//		appendMenu(mainMenu->GetSubMenu(2 + delta), 1, &popupMenu);
	//		appendMenu(mainMenu->GetSubMenu(2 + delta), 2, &popupMenu);
	//	}
	//}
}

RDOTracerLogCtrlView::~RDOTracerLogCtrlView()
{}

rbool RDOTracerLogCtrlView::getItemColors(int index, RDOLogColorPair* &colors) const
{
	const_cast<CMutex&>(mutex).Lock();

	rbool res = true;
	RDOColorMap::const_iterator it = subitemColors.find(index);
	if (it != subitemColors.end())
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
	subitemColors.clear();

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
			addingSubitems = false;
		}
		else if (addingSubitems)
		{
			subitemColors.insert(RDOColorMap::value_type(m_strings.count(), itemColor));
		}

		if (key == "SD")
		{
			addingSubitems = true;
			logStyle->getItemColors(key, itemColor);
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

//! todo qt
//void RDOTracerLogCtrlView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
//{
//	CWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
//	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
//	if(pwndFrame) pwndFrame->SendMessage(WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu));
//}
//
//void RDOTracerLogCtrlView::OnContextMenu(CWnd* pWnd, CPoint pos)
//{
//	if (!bShowMenu) return;
//	CWnd::OnContextMenu(pWnd, pos);
//	if (popupMenu.m_hMenu) popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
//}

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
