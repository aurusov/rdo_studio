// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_frame.h"
// --------------------------------------------------------------------------------

DockFrame::DockFrame(QWidget* pParent)
	: DockFocusable("Анимация", pParent)
{
	context_type* pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(150, 200));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_frame.png")));
}

DockFrame::~DockFrame()
{}

DockFrame::context_type& DockFrame::getContext()
{
	return *static_cast<context_type*>(widget());
}
