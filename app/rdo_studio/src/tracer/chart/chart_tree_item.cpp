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
