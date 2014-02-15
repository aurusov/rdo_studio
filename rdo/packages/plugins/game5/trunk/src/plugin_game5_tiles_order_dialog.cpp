/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <vector>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
// --------------------------------------------------------------------------------

TilesOrderDialog::TilesOrderDialog(QWidget* pParent, const QString& lineEditText)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	QRegExpValidator * validator = new QRegExpValidator();
	validator->setRegExp(QRegExp(validatorRegExpPattern(lineEditText.length() / 2)));

	lineEditPosition->setValidator(validator);
	lineEditPosition->setText(lineEditText);

	connect(buttonCancel, &QPushButton::clicked, this, &TilesOrderDialog::reject);
	connect(buttonOk    , &QPushButton::clicked, this, &TilesOrderDialog::onOkClick);
	
	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

TilesOrderDialog::~TilesOrderDialog()
{
}

void TilesOrderDialog::onOkClick()
{
	emit tilesOrderCommited(lineEditPosition->text());
	accept();
}

QString TilesOrderDialog::validatorRegExpPattern(int value)
{
	QString singleRegExp;
	int leastBit = value % 10;
	int highBit  = value / 10;
	if (highBit == 0)
	{
		singleRegExp += "[0-" + QString::number(leastBit) + "]";
	}
	else
	{
		if (highBit == 1)
		{
			singleRegExp += "[0-9]|[0-1][0-" + QString::number(leastBit) + "]";
		}
		else
		{
			singleRegExp += "[0-9]|[0-" + QString::number(highBit - 1) + "][0-9]|[0-" + QString::number(highBit) + "][0-" + QString::number(leastBit) + "]";
		}
	}

	QString regExpString = "(((" + singleRegExp + ")(\\s)+)|((\\s)+(" + singleRegExp + ")(\\s)+))*($|((" + singleRegExp + ")$))";
	return regExpString;
}
