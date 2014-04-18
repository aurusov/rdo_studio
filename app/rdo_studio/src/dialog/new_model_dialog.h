/*!
  \copyright (c) RDO-Team, 2013
  \file      new_model_dialog.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DIALOG_NEW_MODEL_DIALOG_H_
#define _RDO_STUDIO_DIALOG_NEW_MODEL_DIALOG_H_

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

#endif // _RDO_STUDIO_DIALOG_NEW_MODEL_DIALOG_H_
