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
