/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/cost_setup_table.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_COST_SETUP_TABLE_H_
#define _RDO_PLUGIN_GAME_5_COST_SETUP_TABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QTableWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class CostSetupTable: public QTableWidget
{
Q_OBJECT
public:
	CostSetupTable(QWidget* pParent);
	virtual ~CostSetupTable();

signals:
	void itemCheckStateChanged(QTableWidgetItem* itm);

private slots:
	void reemitItemCheckStateChanged(QTableWidgetItem* itm);

private:
	std::vector<bool> m_itemsLastState;

	void prepareTable();
	void init();
};

#endif // _RDO_PLUGIN_GAME_5_COST_SETUP_TABLE_H_