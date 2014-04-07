/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/checkable_line_edit.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_CHECKABLE_LINE_EDIT_H
#define _RDO_PLUGIN_GAME_5_CHECKABLE_LINE_EDIT_H

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class CheckableLineEdit: public QWidget
{Q_OBJECT

public:
	CheckableLineEdit(QWidget* parent);
	QCheckBox& getCheckBox() {return *checkBox;};
	QLineEdit& getLineEdit() {return *lineEdit;};

private:
	QCheckBox*    checkBox;
	QLineEdit*    lineEdit;
	const QString defaultText;

private slots:
	void setLineEditState(int state);
};

#endif // _RDO_PLUGIN_GAME_5_CHECKABLE_LINE_EDIT_H
