/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_core_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

ActionActivator::ActionActivator()
	: m_activated(false)
{}

ActionActivator::~ActionActivator()
{}

bool ActionActivator::isActivated() const
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
