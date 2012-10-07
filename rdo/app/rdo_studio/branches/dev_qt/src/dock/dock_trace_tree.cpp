/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace_tree.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_trace_tree.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

DockTraceTree::DockTraceTree(PTR(QWidget) pParent)
	: parent_class(
		"Графики",
		pParent,
		parent_class::Context::CreateFunction(
			boost::bind<BOOL>(&parent_class::Context::context_type::Create, _1, DWORD(0), CRect(0, 0, 0, 0), _2, UINT(0))
		),
		QSize(150, 300)
	)
{
	tracer->setTree(&getContext());
}

DockTraceTree::~DockTraceTree()
{}
