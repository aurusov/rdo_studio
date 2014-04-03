/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
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
#include "app/rdo_studio/src/dock/dock_debug.h"
// --------------------------------------------------------------------------------

DockDebug::DockDebug(QWidget* pParent)
	: DockFocusable("Вывод", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 110));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_debug.png")));
}

DockDebug::~DockDebug()
{}

void DockDebug::appendString(CREF(QString) str)
{
	getContext().appendLine(str);
}

void DockDebug::clear()
{
	getContext().clearAll();
}

REF(DockDebug::context_type) DockDebug::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
