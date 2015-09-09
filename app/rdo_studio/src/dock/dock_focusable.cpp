// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
// --------------------------------------------------------------------------------

DockFocusable::DockFocusable(const QString& title, QWidget* pParent)
    : super(title, pParent)
{
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(onVisibilityChanged(bool)));
}

DockFocusable::~DockFocusable()
{}

void DockFocusable::onVisibilityChanged(bool visible)
{
    if (visible)
    {
        widget()->setFocus();
    }
    else
    {
        widget()->clearFocus();
    }
}
