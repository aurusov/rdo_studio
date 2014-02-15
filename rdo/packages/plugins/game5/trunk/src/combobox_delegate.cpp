/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/combobox_delegate.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QComboBox>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/combobox_delegate.h"
// --------------------------------------------------------------------------------
 
ComboBoxDelegate::ComboBoxDelegate(const ComboBoxDelegate::ItemList& comboBoxItems , QObject* parent)
	: QItemDelegate(parent)
	, comboBoxItems(comboBoxItems)
{
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent,
                                        const QStyleOptionViewItem& /* option */,
                                        const QModelIndex&          /*index   */) const
{
	QComboBox* comboBox = new QComboBox(parent);
	for (unsigned int i = 0; i < comboBoxItems.size(); i++)
	{
		comboBox->addItem(QString::fromStdString(comboBoxItems[i]));
	}

	connect(comboBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &ComboBoxDelegate::emitCommitData);

	return comboBox;
}

void ComboBoxDelegate::setEditorData(QWidget* editor,
                                     const QModelIndex& index) const
{
	QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
	if (!comboBox)
	{
		return;
	}

	int pos = comboBox->findText(index.model()->data(index).toString(), Qt::MatchExactly);
	comboBox->setCurrentIndex(pos);
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                    const QModelIndex& index) const
{
	QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
	if (!comboBox)
	{
		return;
	}

	model->setData(index, comboBox->currentText());
}

void ComboBoxDelegate::emitCommitData()
{
	emit commitData(qobject_cast<QWidget*>(sender()));
}
