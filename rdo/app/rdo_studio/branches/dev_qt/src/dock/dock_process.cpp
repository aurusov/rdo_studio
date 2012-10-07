/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_process.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_process.h"
// --------------------------------------------------------------------------------

DockProcess::DockProcess(PTR(QWidget) pParent)
	: parent_class(
		"РДО-Процесс",
		pParent,
		parent_class::Context::CreateFunction(
			boost::bind<BOOL>(&parent_class::Context::context_type::Create, _1, LPCTSTR(NULL), LPCTSTR(NULL), DWORD(0), CRect(0, 0, 0, 0), _2, UINT(0), static_cast<CCreateContext*>(NULL))
		),
		QSize(150, 300)
	)
{}

DockProcess::~DockProcess()
{}
