/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_debug.h"
// --------------------------------------------------------------------------------

DockDebug::DockDebug(PTR(QWidget) pParent)
	: parent_class("Вывод", pParent)
{}

DockDebug::~DockDebug()
{}

void DockDebug::appendString(CREF(tstring) str)
{
	getContext().appendLine(str);
}

void DockDebug::clear()
{
	getContext().clearAll();
}
