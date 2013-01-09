/*!
  \copyright (c) RDO-Team, 2013
  \file      go_to_next_line_dialog.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_
#define _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_go_to_next_line_dialog.h"
// --------------------------------------------------------------------------------

class GoToNextLineDialog
	: public QDialog
	, private Ui::GoToNextLineDialog
{
Q_OBJECT

public:
	explicit GoToNextLineDialog(PTR(QWidget) pParent = NULL, int _line = -1);

	int line;

private slots:
	void okButtonClicked();

};

#endif // _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_