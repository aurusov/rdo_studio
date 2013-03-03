/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_tree_ctrl.cpp
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
#include "app/rdo_studio/src/frame/frame_tree_ctrl.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
// --------------------------------------------------------------------------------

FrameTreeCtrl::FrameTreeCtrl(PTR(QWidget) pParent)
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

FrameTreeCtrl::~FrameTreeCtrl()
{}

PTR(QTreeWidgetItem) FrameTreeCtrl::insertFrame(CREF(QString) name)
{
	PTR(QTreeWidgetItem) pItem = new QTreeWidgetItem(m_pRootItem);
	ASSERT(pItem);
	pItem->setText(0, name);
	pItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/tree_frame_item.png")));
	return pItem;
}

void FrameTreeCtrl::clear()
{
	QList<PTR(QTreeWidgetItem)> children = m_pRootItem->takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		m_pRootItem->removeChild(item);
	}
}

void FrameTreeCtrl::focusInEvent(QFocusEvent* pEvent)
{
	parent_type::focusInEvent(pEvent);
	activate(pEvent);
}

void FrameTreeCtrl::focusOutEvent(QFocusEvent* pEvent)
{
	parent_type::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void FrameTreeCtrl::onUpdateActions(rbool activated)
{
	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, &FrameTreeCtrl::onHelpContext
	);
}

void FrameTreeCtrl::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_frame.htm#frame\n");
	g_pApp->callQtAssistant(ba);
}
