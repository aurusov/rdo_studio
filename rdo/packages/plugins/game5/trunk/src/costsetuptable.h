/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/costsetuptable.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_COSTSETUPTABLE_H_
#define _RDO_PLUGIN_GAME_5_COSTSETUPTABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QTableWidget>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class CostSetupTable: public QTableWidget
{
Q_OBJECT
public:
	CostSetupTable(QWidget* pParent);
	virtual ~CostSetupTable() {};

signals:
	void itemCheckStateChanged(QTableWidgetItem* itm);

private slots:
	void reemitItemCheckStateChanged(QTableWidgetItem* itm);

private:
	std::vector<bool> m_itemsLastState;

	void prepareTable();
	void init();
};

#endif // _RDO_PLUGIN_GAME_5_COSTSETUPTABLE_H_