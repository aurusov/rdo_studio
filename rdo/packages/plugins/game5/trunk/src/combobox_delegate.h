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
#include <QItemDelegate>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------
 
class ComboBoxDelegate : public QItemDelegate
{
Q_OBJECT

public:
	typedef std::vector<std::string> itemList;

public:
	ComboBoxDelegate(const ComboBoxDelegate::itemList& comboBoxItems, QObject* parent = 0);

	QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option,
	                      const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData (QWidget* editor, QAbstractItemModel* model,
	                   const QModelIndex& index) const;

private slots:
	void emitCommitData();

private:
	itemList comboBoxItems;
};

#endif // _RDO_PLUGIN_GAME_5_COMBOBOX_DELEGATE_H_