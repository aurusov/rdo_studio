#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include "ui_plugin_game5_graph_node_info_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
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
