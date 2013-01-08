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
#include <QtGui/qevent.h>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class ActionActivator
{
protected:
	typedef  boost::function<void ()>  Callback;

	ActionActivator(CREF(Callback) onActivate, CREF(Callback) onDeactivate);
	virtual ~ActionActivator();

	rbool isActivated() const;

	void activate  (QFocusEvent* pEvent);
	void deactivate(QFocusEvent* pEvent);

	static void updateAction(QAction* pAction, rbool enabled, QObject* pObject, CREF(tstring) method);

private:
	Callback  m_onActivate;
	Callback  m_onDeactivate;
	rbool     m_activated;
};

#endif // _RDO_STUDIO_ACTION_ACTIVATOR_H_
