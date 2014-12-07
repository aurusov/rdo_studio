#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDialog>
#include "ui_new_model_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class NewModelDialog
	: public  QDialog
	, private Ui::NewModelDialog
{
Q_OBJECT

public:
	NewModelDialog(QWidget* pParent);
	virtual ~NewModelDialog();

	QString getModelName() const;
	QString getModelPath() const;
	std::size_t getTemplateIndex() const;

private:
	QFont    m_font;
	QFont    m_fontRed;
	QPalette m_palette;
	QPalette m_paletteRed;

	void setPath(const QString& path);

private slots:
	void updateUI();
	void onPathButtonClicked();
};
