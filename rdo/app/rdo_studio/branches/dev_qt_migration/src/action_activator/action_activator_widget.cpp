/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator_widget.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/action_activator/action_activator_widget.h"
// --------------------------------------------------------------------------------

ActionActivatorWidget::ActionActivatorWidget(PTR(QWidget) pParent)
	: QWidget        (pParent)
	, ActionActivator(boost::bind(&ActionActivatorWidget::onActivate, this), boost::bind(&ActionActivatorWidget::onDeactivate, this))
{}

ActionActivatorWidget::~ActionActivatorWidget()
{}

void ActionActivatorWidget::focusInEvent(QFocusEvent* pEvent)
{
	QWidget::focusInEvent(pEvent);
	activate(pEvent);
}

void ActionActivatorWidget::focusOutEvent(QFocusEvent* pEvent)
{
	QWidget::focusOutEvent(pEvent);
	deactivate(pEvent);
}
