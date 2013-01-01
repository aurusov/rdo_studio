/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

ActionActivator::ActionActivator(CREF(Callback) onActivate, CREF(Callback) onDeactivate)
	: m_onActivate  (onActivate  )
	, m_onDeactivate(onDeactivate)
	, m_activated   (false       )
{}

ActionActivator::~ActionActivator()
{}

void ActionActivator::activate(QFocusEvent* pEvent)
{
	ASSERT(pEvent);

	if (!m_activated)
	{
		m_activated = true;
		m_onActivate();
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
			m_onDeactivate();
		}
	}
}
