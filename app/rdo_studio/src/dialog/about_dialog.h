#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDialog>
#include "ui_about_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class AboutDialog: public QDialog, private Ui::AboutDialog
{
Q_OBJECT

public:
	explicit AboutDialog(QWidget* pParent = NULL);
};
