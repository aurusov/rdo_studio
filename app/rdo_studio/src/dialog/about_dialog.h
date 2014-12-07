#ifndef _RDO_STUDIO_DIALOG_ABOUT_DIALOG_H_
#define _RDO_STUDIO_DIALOG_ABOUT_DIALOG_H_

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

#endif // _RDO_STUDIO_DIALOG_ABOUT_DIALOG_H_
