/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/checkable_line_edit.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief
  \indent    4T
*/

#ifndef CHECKABLE_LINE_EDIT_H
#define CHECKABLE_LINE_EDIT_H

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class CheckableLineEdit: public QWidget
{
	Q_OBJECT

public:
	CheckableLineEdit(QWidget* parent, const QString& defaultText = "1");
	const QCheckBox* getCheckBox() {return checkBox;};
	const QLineEdit* getLineEdit() {return lineEdit;};

private:
	QCheckBox*    checkBox;
	QLineEdit*    lineEdit;
	const QString defaultText;

private slots:
	void setLineEditState(const int& state);
};

#endif // CHECKABLE_LINE_EDIT_H
