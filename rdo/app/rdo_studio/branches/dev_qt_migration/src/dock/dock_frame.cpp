/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_frame.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <QtGui/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_frame.h"
// --------------------------------------------------------------------------------

DockFrame::DockFrame(PTR(QWidget) pParent)
	: parent_class(
		"Анимация",
		pParent,
		parent_class::Context::CreateFunction(
			boost::bind<BOOL>(&parent_class::Context::context_type::Create, _1, DWORD(0), CRect(0, 0, 0, 0), _2, UINT(0))
		),
		QSize(150, 300)
	)
{
	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_frame.png")));
}

DockFrame::~DockFrame()
{}
