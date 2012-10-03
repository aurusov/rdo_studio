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
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_build.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "simulator/report/file_message.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------

DockBuild::DockBuild(PTR(QWidget) pParent)
	: QDockWidget("Компилятор", pParent)
{
	PTR(QWidget) pDocWidget = new QWidget();
	setWidget(pDocWidget);

	m_pContext = new Context(pDocWidget);
	m_pContext->setMinimumSize(300, 150);
	//! @todo qt
	//m_pContext->setPopupMenu(&popupMenu);

	PTR(QVBoxLayout) pOutputLayout = new QVBoxLayout(pDocWidget);
	pOutputLayout->setSpacing(0);
	pOutputLayout->setContentsMargins(0, 0, 0, 0);
	pOutputLayout->addWidget(m_pContext);

	m_pContext->init();
	getContext().setEditorStyle(&studioApp.getStyle()->style_build);
	getContext().ShowWindow(SW_SHOW);
}

DockBuild::~DockBuild()
{}

REF(DockBuild::Context::context_type) DockBuild::getContext()
{
	ASSERT(m_pContext);
	return m_pContext->getContext();
}

void DockBuild::appendString(CREF(tstring) str)
{
	PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(str);
	getContext().appendLine(pLine);
}

void DockBuild::appendString(CREF(rdo::simulation::report::FileMessage) message)
{
	if (message.getType() == rdo::simulation::report::FileMessage::MT_ERROR || (message.getType() == rdo::simulation::report::FileMessage::MT_WARNING && static_cast<PTR(rdoEditCtrl::RDOBuildEditTheme)>(studioApp.getStyle()->style_build.theme)->warning))
	{
		PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(message);
		getContext().appendLine(pLine);
	}
}

void DockBuild::clear()
{
	getContext().clearAll();
}
