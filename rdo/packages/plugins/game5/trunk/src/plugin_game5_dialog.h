/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_TEST_GAME_5_DIALOG_H_
#define _RDO_PLUGIN_TEST_GAME_5_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QDialog>
#include <QtPlugin>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
#include "app/rdo_studio/src/model/model.h"
#include "ui_plugin_game5_dialog.h"
// --------------------------------------------------------------------------------

class game5Dialog: public QDialog, public Ui_RAOgame5PluginDialog
{
Q_OBJECT
public:
	game5Dialog(QWidget * parent);
	~game5Dialog() {};

signals:
	void buttonRandomClicked(bool solvabilityCheck);

private slots:
	void callDialog ();
	void onClickOk  ();
	void onClickHide(bool state);
	void emitSolvabilityCheck();
	void onItemCheckStateChanged(QTableWidgetItem * item);
	
private:
	std::string evaluateBy();
	std::string activityValue(int tableRow);

	std::string RTPtabText();
	std::string RSStabText();
	std::string PATtabText();
	std::string DPTtabText();
	std::string FUNtabText();

	void backUpModel (rdo::gui::model::Model* pModel);
	void clearAllTabs(rdo::gui::model::Model* pModel);
};

#endif // _RDO_PLUGIN_TEST_GAME_5_DIALOG_H_
