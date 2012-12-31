/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_dialog.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      31.12.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_DIALOG_FIND_DIALOG_H_
#define _RDO_STUDIO_MFC_DIALOG_FIND_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_find_dialog.h"
// --------------------------------------------------------------------------------

class FindDialog
	: public  QDialog
	, private Ui::FindDialog
{
Q_OBJECT

public:
	explicit FindDialog(PTR(QWidget) pParent = NULL);
};

#endif // _RDO_STUDIO_MFC_DIALOG_FIND_DIALOG_H_
