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

GoToLineDialog::GoToLineDialog(PTR(QWidget) pParent, int line, int lineCount)
	: QDialog(pParent)
	, m_line (line   )
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	label->setText(QString("(1-%1)").arg(lineCount));

	lineEdit->setValidator(new QIntValidator(1, lineCount, this));
	lineEdit->setText(QString::number(m_line));

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void GoToLineDialog::onOkButtonClicked()
{
	m_line = lineEdit->text().toInt();
	done(Accepted);
}

int GoToLineDialog::getLine() const
{
	return m_line;
}
