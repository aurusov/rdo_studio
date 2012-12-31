/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_dialog.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      31.12.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

FindDialog::FindDialog(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}
