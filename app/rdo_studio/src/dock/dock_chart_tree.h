#pragma once

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
