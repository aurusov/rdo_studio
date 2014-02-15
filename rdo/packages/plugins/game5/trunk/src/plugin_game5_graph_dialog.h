/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_graph_dialog.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME5_GRAPH_DIALOG_H_
#define _RDO_PLUGIN_GAME5_GRAPH_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/warning_disable.h"
#include "ui_plugin_game5_graph_dialog.h"
#include "utils/src/common/warning_enable.h"
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
// --------------------------------------------------------------------------------

class PluginGame5GraphDialog
	: public QDialog
	, public Ui_PluginGame5GraphDialog
{

Q_OBJECT
public:
	PluginGame5GraphDialog(QWidget* parent);
	~PluginGame5GraphDialog();

	void updateGraph(const QString& startBoardState);

public slots:
	void onPluginAction(QString boardState);
	void emitShowNodeInfoDlg();

signals:
	void updateNodeInfoDlg(GraphNode* node);
	void showNodeInfoDlg  ();

private:
	struct UnbuiltRange
	{
		int firstNode;
		int range;
	};

	struct SortStruct
	{
		SortStruct(PluginGame5GraphDialog* pDlg) : m_pDlg(pDlg) {};
		PluginGame5GraphDialog* m_pDlg;

		bool operator() (int i, int j)
		{
			int iParentOLO = m_pDlg->m_graph[i]->getParentGraphNode()->getGraphOnLevelOrder();
			int jParentOLO = m_pDlg->m_graph[j]->getParentGraphNode()->getGraphOnLevelOrder();

			return iParentOLO < jParentOLO;
		}
	};
	void quickSort(std::vector<int>& vector);

	QString m_traceTimeStamp;
	GraphNode* m_clickedNode;
	std::vector<GraphNode*> m_graph;

	std::vector<int> getSolutionNodes();
	void updateCheckedNode(GraphNode* node);
	QString     getTraceTimeStamp();
	QString     getTraceFile();
	QStringList parseTrace();

	QString getTraceInfo();
	int parseTraceInfo(const QString& key);
};

#endif // _RDO_PLUGIN_GAME5_GRAPH_DIALOG_H_
