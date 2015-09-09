// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_trace.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

DockTrace::DockTrace(QWidget* pParent)
    : DockFocusable("Трассировка", pParent)
{
    context_type* pWidget = new context_type(this);
    pWidget->setMinimumSize(QSize(300, 110));

    setWidget(pWidget);

    toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_trace.png")));

    g_pTracer->setLog(&getContext());
}

DockTrace::~DockTrace()
{}

DockTrace::context_type& DockTrace::getContext()
{
    return *static_cast<context_type*>(widget());
}
