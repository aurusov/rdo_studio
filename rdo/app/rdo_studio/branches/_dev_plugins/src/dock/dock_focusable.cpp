/*!
  \copyright (c) RDO-Team, 2013
  \file      dock_focusable.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
// --------------------------------------------------------------------------------

DockFocusable::DockFocusable(const QString& title, QWidget* pParent)
	: super(title, pParent)
{
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(onVisibilityChanged(bool)));
}

DockFocusable::~DockFocusable()
{}

void DockFocusable::onVisibilityChanged(bool visible)
{
	if (visible)
	{
		widget()->setFocus();
	}
	else
	{
		widget()->clearFocus();
	}
}
