/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_find.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_find.h"
#include "simulator/report/log_edit_line_info.h"
// --------------------------------------------------------------------------------

DockFind::DockFind(PTR(QWidget) pParent)
	: parent_class(
		"Поиск",
		pParent,
		parent_class::Context::CreateFunction(
			boost::bind<BOOL>(&parent_class::Context::context_type::Create, _1, LPCTSTR(NULL), LPCTSTR(NULL), DWORD(0), CRect(0, 0, 0, 0), _2, UINT(0), static_cast<CCreateContext*>(NULL))
		)
	)
{}

DockFind::~DockFind()
{}

void DockFind::appendString(
	CREF(tstring)                str,
	rdoModelObjects::RDOFileType fileType,
	int lineNumber, int posInLine)
{
	PTR(rdo::simulation::report::LogEditLineInfo) pLine = new rdo::simulation::report::LogEditLineInfo(rdo::simulation::report::FileMessage(str, fileType, lineNumber, posInLine));
	getContext().appendLine(pLine);
}

void DockFind::clear()
{
	getContext().clearAll();
}
