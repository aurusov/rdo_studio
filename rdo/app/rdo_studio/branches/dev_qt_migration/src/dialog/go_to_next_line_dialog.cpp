/*!
  \copyright (c) RDO-Team, 2013
  \file      go_to_next_line_dialog.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dialog/go_to_next_line_dialog.h"
// --------------------------------------------------------------------------------

GoToNextLineDialog::GoToNextLineDialog(PTR(QWidget) pParent, int _line)
	: QDialog(pParent),
	  line(_line)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	lineEdit->setValidator(new QIntValidator(this));

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

void GoToNextLineDialog::okButtonClicked()
{
	line = lineEdit->text().toInt();
	done(Accepted);
}