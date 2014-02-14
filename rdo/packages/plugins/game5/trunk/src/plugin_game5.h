/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_PLUGIN_GAME5_H_
#define _RDO_PLUGIN_PLUGIN_GAME5_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtPlugin>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_graph_dialog.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/plugins/plugin_interface.h"
// --------------------------------------------------------------------------------

class PluginGame5
	: public QObject
	, public PluginInterface
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "RDO_PLUGIN_INTERFACE" FILE "plugin_game5.json")
Q_INTERFACES(PluginInterface)

public:
	virtual QUuid   getGUID      ();
	virtual QString getPluginName();
	virtual QString getAuthor    ();
	virtual QString getVersion   ();

	virtual void pluginStartAction (QWidget* parent);
	virtual void pluginStopAction  (QWidget* parent);

signals:
	void onGraphDlgAction(QString string);
	void setGraphDlgActionEnabled(bool);

private slots:
	void pluginSlot();
	void reemitGraphDlgAction();
	void reemitGraphDlgActionEnabled();

private:
	PluginGame5GenerateSituationDialog* genSitDlg;
	PluginGame5GraphDialog*             graphDlg;
	
	void initDialogs(QWidget* pParent);
	void initToolBar(MainWindow* pParent);
	void backUpModel(rdo::gui::model::Model* pModel);
};

#endif // _RDO_PLUGIN_PLUGIN_GAME5_H_