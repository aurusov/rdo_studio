/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_results.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_results.h"
// --------------------------------------------------------------------------------

DockResults::DockResults(PTR(QWidget) pParent)
	: DockFocusable(QString::fromStdWString(L"Результаты"), pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 150));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_results.png")));
}

DockResults::~DockResults()
{}

void DockResults::appendString(CREF(tstring) str)
{
	int pos = getContext().getCurrentPos();
	getContext().setCurrentPos(getContext().getLength());
	getContext().setReadOnly  (false);
	getContext().appendText   (QString::fromLocal8Bit(str.c_str()));
	getContext().setReadOnly  (true );
	getContext().setCurrentPos(pos  );
}

void DockResults::clear()
{
	getContext().clearAll();
}

REF(DockResults::context_type) DockResults::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}