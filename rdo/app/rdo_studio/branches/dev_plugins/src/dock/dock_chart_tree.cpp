/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_chart_tree.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_chart_tree.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

DockChartTree::DockChartTree(QWidget* pParent)
	: DockFocusable("Графики", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(150, 200));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_chart.png")));

	g_pTracer->setTree(pWidget);
}

DockChartTree::~DockChartTree()
{}
