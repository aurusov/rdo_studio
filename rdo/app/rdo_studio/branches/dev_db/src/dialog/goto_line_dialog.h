/*!
  \copyright (c) RDO-Team, 2013
  \file      goto_line_dialog.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_
#define _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_goto_line_dialog.h"
// --------------------------------------------------------------------------------

class GoToLineDialog
	: public QDialog
	, private Ui::GoToLineDialog
{
Q_OBJECT

public:
	explicit GoToLineDialog(PTR(QWidget) pParent, int line, int lineCount);

	int getLine() const;

private:
	int m_line;

private slots:
	void onOkButtonClicked();
};

#endif // _RDO_STUDIO_GO_TO_NEXT_LINE_DIALOG_H_
