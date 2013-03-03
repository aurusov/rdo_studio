/*!
  \copyright (c) RDO-Team, 2013
  \file      new_model_dialog.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QDir>
#include <QFileDialog>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/new_model_dialog.h"
// --------------------------------------------------------------------------------

NewModelDialog::NewModelDialog(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	m_font    = labelError->font();
	m_fontRed = m_font;
	m_fontRed.setBold(true);

	m_palette    = labelError->palette();
	m_paletteRed = m_palette;
	m_paletteRed.setColor(labelError->foregroundRole(), Qt::red);

	connect(modelName, SIGNAL(textChanged(const QString&)), this, SLOT(updateUI()));
	connect(modelPathButton, SIGNAL(clicked(bool)), this, SLOT(onPathButtonClicked()));

	setPath(QDir::homePath());
}

NewModelDialog::~NewModelDialog()
{}

void NewModelDialog::setPath(CREF(QString) path)
{
	QString normPath(path);
	if (!normPath.isEmpty())
	{
		if (normPath.at(normPath.length()-1) != QChar('/') && normPath.at(normPath.length()-1) != QChar('\\'))
		{
			normPath += "/";
		}
	}
	modelPath->setText(normPath);
	updateUI();
}

void NewModelDialog::updateUI()
{
	rbool error = true;
	if (modelName->text().isEmpty())
	{
		labelError->setText("Необходимо указать имя модели");
	}
	else
	{
		QString fullName(modelPath->text() + modelName->text());
		QFileInfo fileInfo(fullName);
		if (fileInfo.exists())
		{
			QString info = fileInfo.isDir()
				? "Такая директория уже существует: '%1'"
				: "Такой файл уже существует: '%1'";

			labelError->setText(info.arg(fullName));
		}
		else
		{
			labelError->setText(QString("Будет создана директория: '%1'").arg(fullName));
			error = false;
		}
	}

	buttonOk->setEnabled(!error);
	labelError->setFont   (error ? m_fontRed    : m_font   );
	labelError->setPalette(error ? m_paletteRed : m_palette);
}

void NewModelDialog::onPathButtonClicked()
{
	QString path = QFileDialog::getExistingDirectory(
		this,
		"Выбор директории для модели",
		modelPath->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	if (!path.isEmpty())
	{
		setPath(path);
	}
}

QString NewModelDialog::getModelName() const
{
	return modelName->text();
}

QString NewModelDialog::getModelPath() const
{
	return modelPath->text();
}

ruint NewModelDialog::getTemplateIndex() const
{
	ruint result = 0;
	if (template2->isChecked())
	{
		result = 2;
	}
	else if (template3->isChecked())
	{
		result = 4;
	}
	else if (template4->isChecked())
	{
		result = 6;
	}
	else
	{
		ASSERT(template1->isChecked());
		result = 0;
	}

	if (comments->isChecked())
	{
		++result;
	}

	return result;
}
