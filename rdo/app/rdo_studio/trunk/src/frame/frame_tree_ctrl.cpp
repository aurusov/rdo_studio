/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/frame/tree_ctrl.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     Дерево кадров анимации
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/tree_ctrl.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
// --------------------------------------------------------------------------------

RDOStudioFrameTreeCtrl::RDOStudioFrameTreeCtrl(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setColumnCount    (1);
	setHeaderHidden   (true);
	setRootIsDecorated(false);

	m_pRootItem = new QTreeWidgetItem(this);
	m_pRootItem->setText(0, QString::fromStdWString(L"Кадры"));
	m_pRootItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/tree_frame_root.png")));
	addTopLevelItem(m_pRootItem);
}

RDOStudioFrameTreeCtrl::~RDOStudioFrameTreeCtrl()
{}

PTR(QTreeWidgetItem) RDOStudioFrameTreeCtrl::insertFrame(CREF(QString) name)
{
	PTR(QTreeWidgetItem) pItem = new QTreeWidgetItem(m_pRootItem);
	ASSERT(pItem);
	pItem->setText(0, name);
	pItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/tree_frame_item.png")));
	return pItem;
}

void RDOStudioFrameTreeCtrl::clear()
{
	QList<PTR(QTreeWidgetItem)> children = m_pRootItem->takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		m_pRootItem->removeChild(item);
	}
}

void RDOStudioFrameTreeCtrl::focusInEvent(QFocusEvent* pEvent)
{
	parent_type::focusInEvent(pEvent);
	activate(pEvent);
}

void RDOStudioFrameTreeCtrl::focusOutEvent(QFocusEvent* pEvent)
{
	parent_type::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void RDOStudioFrameTreeCtrl::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, "onHelpContext()"
	);
}

void RDOStudioFrameTreeCtrl::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_frame.htm#frame\n");
	studioApp.callQtAssistant(ba);
}
