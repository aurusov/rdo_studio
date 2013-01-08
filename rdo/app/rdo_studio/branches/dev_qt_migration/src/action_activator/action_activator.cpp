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
#include <QtGUI/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/action_activator/action_activator.h"
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

void ActionActivator::updateAction(QAction* pAction, rbool enabled, QObject* pObject, CREF(tstring) method)
{
	ASSERT(pAction);

	pAction->setEnabled(enabled);
	if (enabled)
	{
		ASSERT(pObject);
		ASSERT(!method.empty());
		tstring formattedMethod = rdo::format("1%s %s", method.c_str(), QLOCATION);
		QObject::connect(pAction, SIGNAL(triggered(bool)), pObject, qFlagLocation(formattedMethod.c_str()), Qt::UniqueConnection);
	}
	else
	{
		QObject::disconnect(pAction, SIGNAL(triggered(bool)), NULL, NULL);
	}
}
