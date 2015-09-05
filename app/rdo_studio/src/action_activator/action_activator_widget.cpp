// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator_widget.h"
// --------------------------------------------------------------------------------

ActionActivatorWidget::ActionActivatorWidget(QWidget* pParent)
    : QWidget(pParent)
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
    deactivate(pEvent);
    QWidget::focusOutEvent(pEvent);
}
