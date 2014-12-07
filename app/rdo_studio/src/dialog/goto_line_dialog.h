#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDialog>
#include "ui_goto_line_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GoToLineDialog
	: public QDialog
	, private Ui::GoToLineDialog
{
Q_OBJECT

public:
	explicit GoToLineDialog(QWidget* pParent, int line, int lineCount);

	int getLine() const;

private:
	int m_line;

private slots:
	void onOkButtonClicked();
	void onCheckInput(const QString& text);
};
