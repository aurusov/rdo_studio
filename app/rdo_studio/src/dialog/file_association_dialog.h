#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDialog>
#include "ui_file_association_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class FileAssociationDialog
	: public QDialog
	, public Ui::FileAssociationDialog
{
Q_OBJECT

public:
	FileAssociationDialog(QWidget* pParent);
	virtual ~FileAssociationDialog();
};
