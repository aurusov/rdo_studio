/*!
  \copyright (c) RDO-Team, 2013
  \file      file_association_dialog.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2012
  \brief     
  \indent    4T
*/

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
