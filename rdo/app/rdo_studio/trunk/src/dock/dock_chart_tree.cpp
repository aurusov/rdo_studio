/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace_tree.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_chart_tree.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

DockChartTree::DockChartTree(PTR(QWidget) pParent)
	: DockFocusable(QString::fromStdWString(L"Графики"), pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(200, 300));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_chart.png")));

	g_pTracer->setTree(pWidget);
}

DockChartTree::~DockChartTree()
{}
