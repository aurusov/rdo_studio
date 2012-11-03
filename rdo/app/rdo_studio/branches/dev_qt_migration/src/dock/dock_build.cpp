/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_build.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qaction.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qmessagebox.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_build.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "simulator/report/file_message.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------

DockBuild::DockBuild(PTR(QWidget) pParent)
	: QDockWidget("Компилятор", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(150, 300));

	PTR(QVBoxLayout) pLayout = new QVBoxLayout(this);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(pWidget);

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_build.png")));
}

DockBuild::~DockBuild()
{}

void DockBuild::appendString(CREF(tstring) str)
{
	PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(str);
	getContext().appendLine(pLine);
}

void DockBuild::appendString(CREF(rdo::simulation::report::FileMessage) message)
{
	if (message.getType() == rdo::simulation::report::FileMessage::MT_ERROR || (message.getType() == rdo::simulation::report::FileMessage::MT_WARNING && static_cast<PTR(rdoEditCtrl::RDOBuildEditTheme)>(studioApp.getStyle()->style_build.theme)->warning))
	{
		if (message.getText().find("Сработало лицензионное ограничение") != tstring::npos)
		{
			QMessageBox::critical(studioApp.getMainWnd(), "Лицензионное ограничение", message.getText().c_str());
			return;
		}

		PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(message);
		getContext().appendLine(pLine);
	}
}

void DockBuild::clear()
{
	getContext().clearAll();
}

REF(DockBuild::context_type) DockBuild::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
REF(DockBuild::context_type) DockBuild::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
REF(DockBuild::context_type) DockBuild::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}