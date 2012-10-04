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
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_trace.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

DockTrace::DockTrace(PTR(QWidget) pParent)
	: parent_class("Трассировка", pParent)
{
	tracer->setLog(&getContext());
	getContext().setStyle(&studioApp.getStyle()->style_trace);
}

DockTrace::~DockTrace()
{}
