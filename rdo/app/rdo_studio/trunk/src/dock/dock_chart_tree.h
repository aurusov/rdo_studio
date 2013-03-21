/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace_tree.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_TRACE_TREE_H_
#define _RDO_STUDIO_DOCK_TRACE_TREE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/tracer/chart/chart_tree.h"
// --------------------------------------------------------------------------------

class DockChartTree: public DockFocusable
{
public:
	typedef  rdo::gui::tracer::ChartTree  context_type;

	DockChartTree(QWidget* pParent);
	virtual ~DockChartTree();
};

#endif // _RDO_STUDIO_DOCK_TRACE_TREE_H_
