/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

ActionActivator::ActionActivator()
	: m_activated(false)
{}

ActionActivator::~ActionActivator()
{}

rbool ActionActivator::isActivated() const
{
	return m_activated;
}

void ActionActivator::activate(QFocusEvent* pEvent)
{
	ASSERT(pEvent);

	if (!m_activated)
	{
		m_activated = true;
		onUpdateActions(m_activated);
	}
}

void ActionActivator::deactivate(QFocusEvent* pEvent)
{
	ASSERT(pEvent);

	if (pEvent->reason() != Qt::PopupFocusReason)
	{
		if (m_activated)
		{
			m_activated = false;
			onUpdateActions(m_activated);
		}
	}
}
