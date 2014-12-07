// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/file_association_dialog.h"
// --------------------------------------------------------------------------------

FileAssociationDialog::FileAssociationDialog(QWidget* pParent)
    : QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setupUi(this);
}

FileAssociationDialog::~FileAssociationDialog()
{}
