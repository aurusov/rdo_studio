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
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_find.h"
#include "simulator/report/log_edit_line_info.h"
// --------------------------------------------------------------------------------

DockFind::DockFind(PTR(QWidget) pParent)
	: parent_class("Поиск", pParent)
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
