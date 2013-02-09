/*!
  \copyright (c) RDO-Team, 2013
  \file      goto_line_dialog.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/goto_line_dialog.h"
// --------------------------------------------------------------------------------

GoToLineDialog::GoToLineDialog(PTR(QWidget) pParent, int line, int lineCount)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_line (line   )
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	label->setText(QString("Номер строки (1-%1):").arg(lineCount));

	lineEdit->setValidator(new QIntValidator(1, lineCount, this));
	lineEdit->setText(QString::number(m_line));
	lineEdit->setFocus();
	lineEdit->selectAll();

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
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
