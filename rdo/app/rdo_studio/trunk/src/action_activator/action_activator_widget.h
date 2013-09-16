/*!
  \copyright (c) RDO-Team, 2013
  \file      action_activator_widget.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_ACTION_ACTIVATOR_WIDGET_H_
#define _RDO_STUDIO_ACTION_ACTIVATOR_WIDGET_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

class ActionActivatorWidget
	: public QWidget
	, public ActionActivator
{
protected:
	ActionActivatorWidget(QWidget* pParent);
	virtual ~ActionActivatorWidget();

	virtual void focusInEvent (QFocusEvent* pEvent);
	virtual void focusOutEvent(QFocusEvent* pEvent);
};

#endif // _RDO_STUDIO_ACTION_ACTIVATOR_WIDGET_H_
