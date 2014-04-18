/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_build.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QAction>
#include <QMessageBox>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_build.h"
#include "app/rdo_studio/src/application.h"
#include "simulator/report/src/file_message.h"
#include "simulator/report/src/build_edit_line_info.h"
// --------------------------------------------------------------------------------

DockBuild::DockBuild(QWidget* pParent)
	: DockFocusable("Компилятор", pParent)
{
	context_type* pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 110));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_build.png")));
}

DockBuild::~DockBuild()
{}

void DockBuild::appendString(const QString& str)
{
	rdo::simulation::report::BuildEditLineInfo* pLine = new rdo::simulation::report::BuildEditLineInfo(str.toStdString());
	getContext().appendLine(pLine);
}

void DockBuild::appendString(const rdo::simulation::report::FileMessage& message)
{
	QString qMessage = QString::fromStdString(message.getText());
	if (qMessage.contains("Сработало лицензионное ограничение"))
	{
		QMessageBox::critical(g_pApp->getMainWnd(), "Лицензионное ограничение", qMessage);
		return;
	}

	if (message.getType() == rdo::simulation::report::FileMessage::MT_ERROR || (message.getType() == rdo::simulation::report::FileMessage::MT_WARNING && static_cast<rdo::gui::editor::BuildStyle>(g_pApp->getStyle()->style_build).warning))
	{
		rdo::simulation::report::BuildEditLineInfo* pLine = new rdo::simulation::report::BuildEditLineInfo(message);
		getContext().appendLine(pLine);
	}
}

void DockBuild::clear()
{
	getContext().clearAll();
}

DockBuild::context_type& DockBuild::getContext()
{
	return *static_cast<context_type*>(widget());
}