/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_graph_node_info_dialog.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME5_GRAPH_NODE_INFO_DIALOG_H_
#define _RDO_PLUGIN_GAME5_GRAPH_NODE_INFO_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui_plugin_game5_graph_node_info_dialog.h"
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
// --------------------------------------------------------------------------------

class PluginGame5GraphNodeInfoDialog
	: public QDialog
	, public Ui_PluginGame5GraphNodeInfoDialog
{
Q_OBJECT
public:
	PluginGame5GraphNodeInfoDialog(QWidget* parent);
	~PluginGame5GraphNodeInfoDialog();

public slots:
	void updateDlg(GraphNode* node);

signals:
	void updateCheckedNode(GraphNode* node);

private slots:
	void nextNode();
	void prevNode();

private:
	GraphNode* m_pNode;
};

#endif // _RDO_PLUGIN_GAME5_GRAPH_NODE_INFO_DIALOG_H_
