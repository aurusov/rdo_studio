/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/combobox_delegate.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_COMBOBOX_DELEGATE_H_
#define _RDO_PLUGIN_GAME_5_COMBOBOX_DELEGATE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QItemDelegate>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------
 
class ComboBoxDelegate : public QItemDelegate
{
Q_OBJECT

public:
	typedef std::vector<std::string> ItemList;

public:
	ComboBoxDelegate(const ComboBoxDelegate::ItemList& comboBoxItems, QObject* parent = 0);

	virtual QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option,
	                              const QModelIndex& index) const;
	virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
	virtual void setModelData (QWidget* editor, QAbstractItemModel* model,
	                           const QModelIndex& index) const;

private slots:
	void emitCommitData();

private:
	ItemList comboBoxItems;
};

#endif // _RDO_PLUGIN_GAME_5_COMBOBOX_DELEGATE_H_