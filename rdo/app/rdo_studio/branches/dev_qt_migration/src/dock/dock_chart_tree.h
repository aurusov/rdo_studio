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
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree.h"
// --------------------------------------------------------------------------------

class DockTraceTree: public QDockWidget
{
public:
	typedef  RDOTracerTreeCtrl  context_type;

	DockTraceTree(PTR(QWidget) pParent);
	virtual ~DockTraceTree();
};

#endif // _RDO_STUDIO_DOCK_TRACE_TREE_H_
