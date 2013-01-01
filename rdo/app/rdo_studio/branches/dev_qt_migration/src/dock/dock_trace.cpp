/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_trace.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

DockTrace::DockTrace(PTR(QWidget) pParent)
	: DockFocusable("Трассировка", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 150));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_trace.png")));

	tracer->setLog(&getContext());
}

DockTrace::~DockTrace()
{}

REF(DockTrace::context_type) DockTrace::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
