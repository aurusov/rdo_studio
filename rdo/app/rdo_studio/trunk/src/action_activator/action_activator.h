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
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include <QFocusEvent>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class ActionActivator
{
public:
	template <typename SlotFun>
	static void updateAction(QAction* pAction, bool enabled, const typename QtPrivate::FunctionPointer<SlotFun>::Object* pObject, SlotFun pSlot)
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

	template <typename F>
	static void updateAction(QAction* pAction, bool enabled, const F& functor)
	{
		ASSERT(pAction);

		pAction->setEnabled(enabled);
		if (enabled)
		{
			QObject::connect(pAction, &QAction::triggered, functor);
		}
		else
		{
			QObject::disconnect(pAction, &QAction::triggered, NULL, NULL);
		}
	}

protected:
	ActionActivator();
	virtual ~ActionActivator();

	bool isActivated() const;

	void activate  (QFocusEvent* pEvent);
	void deactivate(QFocusEvent* pEvent);

	virtual void onUpdateActions(bool activated) = 0;

private:
	bool m_activated;
};

#endif // _RDO_STUDIO_ACTION_ACTIVATOR_H_
