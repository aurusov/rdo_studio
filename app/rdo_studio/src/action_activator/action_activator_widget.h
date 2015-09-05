#pragma once

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
