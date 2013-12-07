/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_DIALOG_H_
#define _RDO_PLUGIN_GAME_5_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
#include "app/rdo_studio/src/model/model.h"
#include "ui_plugin_game5_dialog.h"
// --------------------------------------------------------------------------------

class PluginGame5GenerateSituationDialog
	: public QDialog
	, public Ui_PluginGame5GenerateSituationDialog
{
Q_OBJECT
public:
	PluginGame5GenerateSituationDialog(QWidget* parent);
	~PluginGame5GenerateSituationDialog();

	QString getBoardState();

signals:
	void buttonRandomClicked(bool solvabilityCheck);

public slots:
	void onPlgnAction();

private slots:
	void callTilesOrderDialog();
	void onClickOk();
	void onClickHide(bool state);
	void emitSolvabilityCheck();
	void onItemCheckStateChanged(QTableWidgetItem* item);
	
private:
	std::string evaluateBy();
	std::string activityValue(int tableRow);

	std::string RTPtabText();
	std::string RSStabText();
	std::string PATtabText();
	std::string DPTtabText();
	std::string FUNtabText();

	void clearAllTabs();
	rdo::gui::model::Model* getCurrentModel();
	void updateTabs();

	QStringList parseFunTab();
};

#endif // _RDO_PLUGIN_GAME_5_DIALOG_H_
