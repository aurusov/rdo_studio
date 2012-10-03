/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_debug.h"
#include "app/rdo_studio_mfc/src/application.h"
// --------------------------------------------------------------------------------

DockDebug::DockDebug(PTR(QWidget) pParent)
	: QDockWidget("Вывод", pParent)
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
	getContext().setEditorStyle(&studioApp.getStyle()->style_debug);
	getContext().ShowWindow(SW_SHOW);
}

DockDebug::~DockDebug()
{}

REF(DockDebug::Context::context_type) DockDebug::getContext()
{
	ASSERT(m_pContext);
	return m_pContext->getContext();
}

void DockDebug::appendString(CREF(tstring) str)
{
	getContext().appendLine(str);
}

void DockDebug::clear()
{
	getContext().clearAll();
}
