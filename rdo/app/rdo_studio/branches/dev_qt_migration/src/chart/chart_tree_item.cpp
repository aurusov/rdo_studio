/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree_item.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

RDOTracerTreeItem::RDOTracerTreeItem(rbool drawable)
	: m_pCtrlItem(NULL    )
	, m_drawable (drawable)
{}

RDOTracerTreeItem::~RDOTracerTreeItem()
{}

void RDOTracerTreeItem::setCtrlItem(PTR(QTreeWidgetItem) pCtrlItem)
{
	m_pCtrlItem = pCtrlItem;
}

REF(QTreeWidgetItem) RDOTracerTreeItem::getCtrlItem()
{
	ASSERT(m_pCtrlItem)
	return *m_pCtrlItem;
}

rbool RDOTracerTreeItem::isDrawable() const
{
	return m_drawable;
}
