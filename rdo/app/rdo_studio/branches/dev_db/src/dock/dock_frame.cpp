/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_frame.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QAction>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_frame.h"
// --------------------------------------------------------------------------------

DockFrame::DockFrame(PTR(QWidget) pParent)
	: DockFocusable("Анимация", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(150, 300));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_frame.png")));
}

DockFrame::~DockFrame()
{}

REF(DockFrame::context_type) DockFrame::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
