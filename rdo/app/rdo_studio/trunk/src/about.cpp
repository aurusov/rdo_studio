/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/about.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.08.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/about.h"
// --------------------------------------------------------------------------------

About::About(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_ui   (new Ui::AboutDialog)
{
	m_ui->setupUi(this);
}
