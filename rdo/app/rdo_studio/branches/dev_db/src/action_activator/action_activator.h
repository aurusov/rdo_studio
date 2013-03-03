/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_ACTION_ACTIVATOR_H_
#define _RDO_STUDIO_ACTION_ACTIVATOR_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QFocusEvent>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class ActionActivator
{
protected:
	ActionActivator();
	virtual ~ActionActivator();

	rbool isActivated() const;

	void activate  (QFocusEvent* pEvent);
	void deactivate(QFocusEvent* pEvent);

	virtual void onUpdateActions(rbool activated) = 0;

	template <typename SlotFun>
	static void updateAction(QAction* pAction, rbool enabled, const typename QtPrivate::FunctionPointer<SlotFun>::Object* pObject, SlotFun pSlot)
	{
		ASSERT(pAction);

		pAction->setEnabled(enabled);
		if (enabled)
		{
			ASSERT(pObject);
			QObject::connect(pAction, &QAction::triggered, pObject, pSlot, Qt::UniqueConnection);
		}
		else
		{
			QObject::disconnect(pAction, &QAction::triggered, NULL, NULL);
		}
	}

private:
	rbool m_activated;
};

#endif // _RDO_STUDIO_ACTION_ACTIVATOR_H_
