/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/cost_setup_table.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QHeaderView>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/cost_setup_table.h"
#include "app/rdo_studio/plugins/game5/src/combobox_delegate.h"
// --------------------------------------------------------------------------------

CostSetupTable::CostSetupTable(QWidget * pParent)
	: QTableWidget(pParent)
	, m_itemsLastState()
{
	m_itemsLastState.resize(4);
	prepareTable();
	init();
	resizeRowsToContents();

	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(reemitItemCheckStateChanged(QTableWidgetItem*)));
}

void CostSetupTable::prepareTable()
{
	setRowCount   (4);
	setColumnCount(3);
	setFixedHeight(117);

	verticalHeader()->hide();
	QStringList tableColumnHeaders;
	tableColumnHeaders << "Движение" << "Вычисление" << "Стоимость";
	setHorizontalHeaderLabels(tableColumnHeaders);
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	ComboBoxDelegate::itemList calcOrderState;
	calcOrderState.push_back("after" );
	calcOrderState.push_back("before");
	ComboBoxDelegate * comboDelegate = new ComboBoxDelegate(calcOrderState, this);
	setItemDelegateForColumn(1, comboDelegate);

	setSelectionMode(QAbstractItemView::NoSelection);
}

void CostSetupTable::init()
{
	setItem(0,0,new QTableWidgetItem("Вправо"));
	setItem(1,0,new QTableWidgetItem("Влево" ));
	setItem(2,0,new QTableWidgetItem("Вверх" ));
	setItem(3,0,new QTableWidgetItem("Вниз"  ));

	for (int i = 0; i < 4; i++)
	{
		QTableWidgetItem* calcOrderItem = new QTableWidgetItem("after");
		setItem(i, 1, calcOrderItem);
		openPersistentEditor(calcOrderItem);

		QTableWidgetItem* costValueItem = new QTableWidgetItem("1");
		costValueItem->setFlags((costValueItem->flags() & ~Qt::ItemIsEditable) | Qt::ItemIsUserCheckable );
		costValueItem->setCheckState(Qt::Unchecked);
		m_itemsLastState[i] = false;
		setItem(i, 2, costValueItem);
	}
}

void CostSetupTable::reemitItemCheckStateChanged(QTableWidgetItem* itm)
{
	if ((itm->flags() & Qt::ItemIsUserCheckable) && ((itm->checkState() == Qt::Checked) != m_itemsLastState[itm->row()]))
	{
		m_itemsLastState[itm->row()] = (itm->checkState() == Qt::Checked);
		emit itemCheckStateChanged(itm);
	}
}
