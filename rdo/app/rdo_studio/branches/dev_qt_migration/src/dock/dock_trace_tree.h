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
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
// --------------------------------------------------------------------------------

class DockTraceTree: public DockBase<RDOTracerTreeCtrl>
{
public:
	DockTraceTree(PTR(QWidget) pParent);
	virtual ~DockTraceTree();

private:
	typedef  DockBase<RDOTracerTreeCtrl>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_TRACE_TREE_H_
