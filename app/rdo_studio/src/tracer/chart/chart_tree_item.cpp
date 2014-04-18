/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree_item.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

ChartTreeItem::ChartTreeItem(bool drawable)
	: m_pCtrlItem(NULL    )
	, m_drawable (drawable)
{}

ChartTreeItem::~ChartTreeItem()
{}

void ChartTreeItem::setCtrlItem(QTreeWidgetItem* pCtrlItem)
{
	m_pCtrlItem = pCtrlItem;
}

QTreeWidgetItem& ChartTreeItem::getCtrlItem()
{
	ASSERT(m_pCtrlItem)
	return *m_pCtrlItem;
}

bool ChartTreeItem::isDrawable() const
{
	return m_drawable;
}
