/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree_item.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_TREE_ITEM_H_
#define _RDO_STUDIO_TRACER_CHART_TREE_ITEM_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QTreeWidget>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartTreeItem: public rdo::counter_reference
{
DECLARE_FACTORY(ChartTreeItem)
public:
	void setCtrlItem(PTR(QTreeWidgetItem) pCtrlItem);
	REF(QTreeWidgetItem) getCtrlItem();

	rbool isDrawable() const;

protected:
	ChartTreeItem (rbool drawable = false);
	~ChartTreeItem();

private:
	PTR(QTreeWidgetItem) m_pCtrlItem;
	rbool                m_drawable; // set to true ONLY for TracerSerie and descendants
};

typedef  rdo::intrusive_ptr<ChartTreeItem>  LPChartTreeItem;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_TREE_ITEM_H_
