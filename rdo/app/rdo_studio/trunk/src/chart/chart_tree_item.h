/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree_item.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _APP_RDO_STUDIO_CHART_TREE_ITEM_H_
#define _APP_RDO_STUDIO_CHART_TREE_ITEM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qtreewidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class ChartTreeItem
{
public:
	ChartTreeItem (rbool drawable = false);
	~ChartTreeItem();

	void setCtrlItem(PTR(QTreeWidgetItem) pCtrlItem);
	REF(QTreeWidgetItem) getCtrlItem();

	rbool isDrawable() const;

private:
	PTR(QTreeWidgetItem) m_pCtrlItem;
	rbool                m_drawable; // set to true ONLY for RDOTracerSerie and descendants
};

#endif // _APP_RDO_STUDIO_CHART_TREE_ITEM_H_
