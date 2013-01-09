/*!
  \copyright (c) RDO-Team, 2013
  \file      goto_line_dialog.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dialog/goto_line_dialog.h"
// --------------------------------------------------------------------------------

GoToLineDialog::GoToLineDialog(PTR(QWidget) pParent, int _line)
	: QDialog(pParent),
	  line(_line)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	lineEdit->setValidator(new QIntValidator(this));

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

void GoToLineDialog::okButtonClicked()
{
	line = lineEdit->text().toInt();
	done(Accepted);
}