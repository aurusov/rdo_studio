/*!
  \copyright (c) RDO-Team, 2013
  \file      file_association_dialog.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/file_association_dialog.h"
// --------------------------------------------------------------------------------

FileAssociationDialog::FileAssociationDialog(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);
}

FileAssociationDialog::~FileAssociationDialog()
{}
