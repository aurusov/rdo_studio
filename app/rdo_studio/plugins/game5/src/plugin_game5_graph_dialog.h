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
#include "utils/src/common/warning_disable.h"
#include <vector>
#include "ui_plugin_game5_graph_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
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

	void updateGraph(const std::vector<unsigned int>& startBoardState);

public slots:
	void onPluginAction(const std::vector<unsigned int>& boardState);
	void emitShowNodeInfoDlg();

signals:
	void updateNodeInfoDlg(GraphNode* node);
	void showNodeInfoDlg  ();

private:
	struct UnbuiltRange
	{
		unsigned int firstNode;
		unsigned int range;
	};

	struct GraphInfo
	{
		GraphInfo(QString solutionCost = "", QString numberOfOpenNodes = "", QString totalNumberOfNodes = "");
		const QString solutionCost;
		const QString numberOfOpenNodes;
		const QString totalNumberOfNodes;
	};

	struct GraphNodeInfo
	{
		GraphNodeInfo(int nodeID, int parentNodeId, int pathCost, int restPathCost,
		              const QString& moveDirection, int moveCost, int relevantTile,
		              int graphLevel, int tileMoveFrom, int tileMoveTo,
		              const std::vector<unsigned int>& boardState);
		GraphNodeInfo() {};

		int     m_nodeID;
		int     m_parentNodeId;
		int     m_pathCost;
		int     m_restPathCost;
		QString m_moveDirection;
		int     m_moveCost;
		int     m_relevantTile;
		int     m_graphLevel;
		int     m_tileMoveFrom;
		int     m_tileMoveTo;
		int     m_graphOnLevelOrder;
		bool    m_relatedToSolutionState;
		std::vector<unsigned int> m_boardState;
		GraphNode* m_pNode;
	};

	QString m_traceTimeStamp;
	GraphNode* m_clickedNode;
	int m_nodeWidth;
	int m_nodeHeight;

	std::list<int> getSolutionNodes() const;
	void updateCheckedNode(GraphNode* node);
	QString     getTraceTimeStamp() const;
	QString     getTraceFile() const;
	std::vector<PluginGame5GraphDialog::GraphNodeInfo> parseTrace(const std::vector<unsigned int>& startBoardState);

	PluginGame5GraphDialog::GraphInfo getGraphInfo() const;
};

#endif // _RDO_PLUGIN_GAME5_GRAPH_DIALOG_H_
