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
#include "utils/src/common/warning_disable.h"
#include <QtPlugin>
#include "utils/src/common/warning_enable.h"
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
	virtual QUuid   getGUID      () const;
	virtual QString getPluginName() const;
	virtual QString getAuthor    () const;
	virtual QString getVersion   () const;

	virtual void pluginStartAction (QWidget* parent);
	virtual void pluginStopAction  (QWidget* parent);

private:
	PluginGame5GenerateSituationDialog* m_generateSituationDlg;
	PluginGame5GraphDialog*             m_graphDlg;
	
	void initDialogs(QWidget* pParent);
	void initToolBar(MainWindow* pParent) const;

private slots:
	void pluginActivation();
	void reemitGraphDlgAction();
	void enablePluginActions();

signals:
	void onGraphDlgAction(const std::vector<unsigned int>& state);
	void setGraphDlgActionEnabled(bool);
	void setGenerateSituationDlgActionEnabled(bool);
};

#endif // _RDO_PLUGIN_PLUGIN_GAME5_H_
