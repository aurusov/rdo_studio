/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_results.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_results.h"
// --------------------------------------------------------------------------------

DockResults::DockResults(QWidget* pParent)
	: DockFocusable("Результаты", pParent)
{
	context_type* pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 110));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_results.png")));
}

DockResults::~DockResults()
{}

void DockResults::appendString(const QString& str)
{
	int pos = getContext().getCurrentPos();
	getContext().setCurrentPos(getContext().getLength());
	getContext().setReadOnly  (false);
	getContext().appendText   (str  );
	getContext().setReadOnly  (true );
	getContext().setCurrentPos(pos  );
}

void DockResults::clear()
{
	getContext().clearAll();
}

DockResults::context_type& DockResults::getContext()
{
	return *static_cast<context_type*>(widget());
}