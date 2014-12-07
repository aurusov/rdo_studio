#ifndef _RDO_STUDIO_DIALOG_FILE_ASSOCIATION_DIALOG_H_
#define _RDO_STUDIO_DIALOG_FILE_ASSOCIATION_DIALOG_H_

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

#endif // _RDO_STUDIO_DIALOG_FILE_ASSOCIATION_DIALOG_H_
