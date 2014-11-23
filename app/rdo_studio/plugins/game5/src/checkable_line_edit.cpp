/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/checkable_line_edit.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QHBoxLayout>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/checkable_line_edit.h"
// --------------------------------------------------------------------------------

namespace
{
	const QString DEFAULT_TEXT = "1";
} // end anonymous namespace

CheckableLineEdit::CheckableLineEdit(QWidget* parent)
	: QWidget(parent)
	, defaultText(DEFAULT_TEXT)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);

	checkBox = new QCheckBox("", this);
	lineEdit = new QLineEdit(defaultText, this);
	lineEdit->setEnabled(false);

	layout->addWidget(checkBox);
	layout->addWidget(lineEdit);
	connect(checkBox, &QCheckBox::stateChanged, this, &CheckableLineEdit::setLineEditState);
}

void CheckableLineEdit::setLineEditState(int state)
{
	if (state == Qt::Checked)
	{
		lineEdit->setEnabled(true);
		lineEdit->setFocus();
	}
	else
	{
		lineEdit->setText(defaultText);
		lineEdit->setEnabled(false);
	}
}
