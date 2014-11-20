/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_graph_dialog.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/plugins/game5/src/graph_edge.h"
#include "app/rdo_studio/plugins/game5/src/graph_info.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_graph_node_info_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_graph_dialog.h"
// --------------------------------------------------------------------------------

namespace
{
	const int SPACER_X  = 20;
	const int SPACER_Y  = 20;

	const int DPS_CO_INDEX = 4;
	const int DPS_TO_INDEX = 5;
	const int DPS_TT_INDEX = 6;

	bool sortingFunction(GraphNode* first, GraphNode* second)
	{
		const int firstParentOLO  = first ->getParentGraphNode()->getGraphOnLevelOrder();
		const int secondParentOLO = second->getParentGraphNode()->getGraphOnLevelOrder();

		return firstParentOLO < secondParentOLO;
	}
} // end anonymous namespace

PluginGame5GraphDialog::PluginGame5GraphDialog(QWidget * pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint| Qt::WindowCloseButtonHint)
	, m_traceTimeStamp(getTraceTimeStamp())
	, m_clickedNode(NULL)
	, m_nodeWidth(0)
	, m_nodeHeight(0)
{
	setupUi(this);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}

	PluginGame5GraphNodeInfoDialog* nodeInfoDlg = new PluginGame5GraphNodeInfoDialog(this);
	
	connect(this       , &PluginGame5GraphDialog::updateNodeInfoDlg,
	        nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::updateDlg);
	connect(this       , &PluginGame5GraphDialog::showNodeInfoDlg,
	        nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::show);

	connect(nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::updateCheckedNode,
	        this       , &PluginGame5GraphDialog::updateCheckedNode);
}

PluginGame5GraphDialog::~PluginGame5GraphDialog()
{}

QString PluginGame5GraphDialog::getTraceFile() const
{
	MainWindow* pMainWindow = (MainWindow*)(parent());
	rdo::gui::model::Model* pModel = pMainWindow->getModel();
	boost::filesystem::path traceFilePath(pModel->getFullName().toStdString());
	traceFilePath.replace_extension(".trc");

	return QString(traceFilePath.string().c_str());
}

QString PluginGame5GraphDialog::getTraceTimeStamp() const
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readLine());
	}
	const QString timeStamp = trcString.section(' ', -2, -1);
	return timeStamp;
}

std::list<int> PluginGame5GraphDialog::getSolutionNodes() const
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

	QString solutionStr;
	const int begin = trcString.indexOf("SD");
	int end;
	if (begin > 0)
	{
		end = trcString.indexOf("SES", begin);
		solutionStr = trcString.mid(begin, end - begin);
	}

	const QRegExp regExp("(\\d{1,5})((\\s\\d){3}\\s\\s\\d{1,2}\\s\\d{1,2})");
	std::list<int> list;

	int pos = 0;
	while ((pos = regExp.indexIn(solutionStr, pos))!= -1)
	{
		list.push_back(regExp.cap(1).toInt());
		pos += regExp.matchedLength();
	}

	if (!list.empty())
	{
		list.push_front(1);
	}

	trcFile.close();
	return list;
}

std::vector<GraphNodeInfo> PluginGame5GraphDialog::parseTrace(const std::vector<unsigned int>& startBoardState)
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}
	trcFile.close();

	const int begin = trcString.indexOf("SB");
	const int end   = trcString.indexOf(QRegExp("SEN|SES"), begin);
	const QString trcStringShort = trcString.mid(begin, end - begin);

	const QRegExp regExp("(STN|STR)(((\\s)-?\\d+){8}((\\s)(\\s)(\\d+)(\\s)(\\d+)))(((\\nSRK)((\\s)\\d+){4})(\\s\\d+)((\\nSRK)((\\s)\\d+){3})(\\s\\d+))");
	QStringList list;
	int pos = 0;
	while ((pos = regExp.indexIn(trcStringShort, pos))!= -1)
	{
		list << regExp.cap(2) + regExp.cap(16) + regExp.cap(21);
		pos += regExp.matchedLength();
	}

	const QFont sceneFont = graphWidget->scene()->font();
	std::vector<GraphNodeInfo> parsingResult;
	if (!list.empty())
	{
		parsingResult.resize(list.size() + 1);
		parsingResult[0] = GraphNodeInfo(1, 0, 0, 0, "Начало поиска", 0, 0, 0, 0, 0, startBoardState);

		QRect nodeRect = GraphNode::calcNodeRect(parsingResult[0], sceneFont);
		m_nodeWidth  = nodeRect.width();
		m_nodeHeight = nodeRect.height();
	}
	for (const QString& string: list)
	{
		const int graphNodeId       = string.section(" ",  1,  1).toInt();
		const int parentGraphNodeId = string.section(" ",  2,  2).toInt();
		const int pathCost          = string.section(" ",  3,  3).toInt();
		const int restPathCost      = string.section(" ",  4,  4).toInt() - pathCost;
		const int moveDirection     = string.section(" ",  5,  5).toInt();
		const int moveCost          = string.section(" ",  7,  7).toInt();

		const int tileMoveFrom      = string.section(" ", -1, -1).toInt();
		const int tileMoveTo        = string.section(" ", -2, -2).toInt();
		const int relevantTile      = string.section(" ", -4, -4).toInt();

		QString moveDirectionText;
		switch (moveDirection)
		{
			case 1: moveDirectionText = "Вправо"; break;
			case 2: moveDirectionText = "Влево" ; break;
			case 3: moveDirectionText = "Вверх" ; break;
			case 4: moveDirectionText = "Вниз"  ; break;
		}

		const int graphNodeIndex = graphNodeId - 1;
		const int parentGraphNodeIndex = parentGraphNodeId - 1;

		const int graphLevel = parsingResult[parentGraphNodeIndex].m_graphLevel + 1;
		std::vector<unsigned int> boardState = parsingResult[parentGraphNodeIndex].m_boardState;
		std::swap(boardState[tileMoveFrom - 1], boardState[tileMoveTo - 1]);

		parsingResult[graphNodeIndex] = GraphNodeInfo(graphNodeId, parentGraphNodeId, pathCost, restPathCost,
		                                              moveDirectionText, moveCost, relevantTile, graphLevel,
		                                              tileMoveFrom, tileMoveTo, boardState);

		QRect nodeRect = GraphNode::calcNodeRect(parsingResult[graphNodeIndex], sceneFont);
		m_nodeWidth  = std::max(nodeRect.width(), m_nodeWidth);
		m_nodeHeight = std::max(nodeRect.height(), m_nodeHeight);
	}

	for (int nodeId: getSolutionNodes())
	{
		parsingResult[nodeId - 1].m_relatedToSolutionState = true;
	}

	return parsingResult;
}

std::vector<std::vector<GraphNode*> > PluginGame5GraphDialog::generateGraphNodes(std::vector<GraphNodeInfo>& parsingResult) const
{
	std::vector<GraphNode*> tempStorage;
	tempStorage.resize(parsingResult.size());
	auto it = tempStorage.begin();
	while (it != tempStorage.end())
	{
		*it = NULL;
		++it;
	}

	std::vector<std::vector<GraphNode*>> graphTree;
	for (const GraphNodeInfo& info: parsingResult)
	{
		const int nodeGraphLevel = info.m_graphLevel;
		const int currentLevel = graphTree.size() - 1;
		if (currentLevel < nodeGraphLevel)
		{
			graphTree.push_back(std::vector<GraphNode*>());
		}

		GraphNode* parentNode = NULL;
		if (info.m_parentNodeId)
		{
			ASSERT(tempStorage[info.m_parentNodeId - 1]);
			parentNode = tempStorage[info.m_parentNodeId - 1];
		}

		GraphNode* node = new GraphNode(info, parentNode, m_nodeWidth, m_nodeHeight);
		connect(node, &GraphNode::clickedNode  , this, &PluginGame5GraphDialog::updateCheckedNode);
		connect(node, &GraphNode::doubleClicked, this, &PluginGame5GraphDialog::emitShowNodeInfoDlg);
		graphTree[nodeGraphLevel].push_back(node);
		tempStorage[info.m_nodeID - 1] = node;
	}

	for (unsigned int i = 1; i < graphTree.size(); i++)
	{
		std::sort(graphTree[i].begin(), graphTree[i].end(), sortingFunction);
		for (unsigned int j = 0; j < graphTree[i].size(); j++)
		{
			graphTree[i][j]->setGraphOnLevelOrder(j);
		}
	}

	return graphTree;
}

void PluginGame5GraphDialog::drawGraph(const std::vector<std::vector<GraphNode*>>& graph) const
{
	for (unsigned int j = 0; j < graph.back().size(); j++)
	{
		GraphNode* node = graph.back()[j];
		graphWidget->scene()->addItem(node);
		node->setPos((SPACER_X + m_nodeWidth) * (j + 1), (graph.size() - 1) * (SPACER_Y + m_nodeHeight));
	}

	for (int i = (int)graph.size() - 2; i >= 0; i--)
	{
		bool buildFlag   = true;
		unsigned int tempNodeNum = 0;
		unsigned int tempCounter = 0;
		std::vector<UnbuiltRange> unbuiltRangeVector;
		for (unsigned int j = 0; j < graph[i].size(); j++)
		{
			GraphNode* node = graph[i][j];
			if (node->haveChild())
			{
				graphWidget->scene()->addItem(node);
				node->setPos(node->childrenMeanX(), node->childrenMeanY() - (SPACER_Y + m_nodeHeight));
				for (GraphNode* childNode: node->getChildrenList())
				{
					graphWidget->scene()->addItem(new GraphEdge(*node, *childNode));
				}

				if (!buildFlag)
				{
					buildFlag = true;
					UnbuiltRange temp = {tempNodeNum, tempCounter};
					unbuiltRangeVector.push_back(temp);
					tempCounter = 0;
				}
			}
			else
			{
				if (buildFlag)
				{
					buildFlag = false;
					tempNodeNum = j;
				}
				tempCounter++;
			}
		}
		if (!buildFlag)
		{
			buildFlag = true;
			UnbuiltRange temp = {tempNodeNum, tempCounter};
			unbuiltRangeVector.push_back(temp);
			tempCounter = 0;
		}
		for (const UnbuiltRange& unbuiltRange: unbuiltRangeVector)
		{
			const unsigned int endUnbuiltRange = unbuiltRange.firstNode + unbuiltRange.range;
			if (unbuiltRange.firstNode == 0)
			{
				GraphNode* leftNode = graph[i][endUnbuiltRange];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					GraphNode* node = graph[i][k];
					const int segment = unbuiltRange.range - k + unbuiltRange.firstNode;

					graphWidget->scene()->addItem(node);
					node->setPos(leftNode->pos().x() - (SPACER_X + m_nodeWidth) * segment, leftNode->pos().y());
				}
			}
			else if (endUnbuiltRange == graph[i].size())
			{
				GraphNode* rightNode = graph[i][unbuiltRange.firstNode - 1];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					GraphNode* node = graph[i][k];
					const int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene()->addItem(node);
					node->setPos(rightNode->pos().x() + (SPACER_X + m_nodeWidth) * segment, rightNode->pos().y());
				}
			}
			else
			{
				GraphNode* leftNode  = graph[i][unbuiltRange.firstNode - 1];
				GraphNode* rightNode = graph[i][endUnbuiltRange];
				double deltaX =  rightNode->pos().x() - leftNode->pos().x();

				if (deltaX < (unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth))
				{
					for (unsigned int l = endUnbuiltRange; l < graph[i].size(); l++)
					{
						GraphNode* node = graph[i][l];
						node->forceShift((unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth) - deltaX);
					}
					deltaX = rightNode->pos().x() - leftNode->pos().x();
				}

				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					GraphNode* node    = graph[i][k];
					const int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene()->addItem(node);
					node->setPos(leftNode->pos().x() + segment * deltaX/(unbuiltRange.range + 1), leftNode->pos().y());
				}
			}
		}
	}
}

PluginGame5GraphDialog::GraphInfo PluginGame5GraphDialog::getGraphInfo() const
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}
	trcFile.close();

	const int begin = trcString.indexOf(QRegExp("SES|SEN"));
	if (begin >= 0)
	{
		const QString trcStringShort = trcString.mid(begin);

		QString solutionCost       = trcStringShort.section(' ', DPS_CO_INDEX, DPS_CO_INDEX, QString::SectionSkipEmpty);
		QString numberOfOpenNodes  = trcStringShort.section(' ', DPS_TO_INDEX, DPS_TO_INDEX, QString::SectionSkipEmpty);
		QString totalNumberOfNodes = trcStringShort.section(' ', DPS_TT_INDEX, DPS_TT_INDEX, QString::SectionSkipEmpty);
		return GraphInfo(solutionCost, numberOfOpenNodes, totalNumberOfNodes);
	}
	return GraphInfo();
}

void PluginGame5GraphDialog::updateGraph(const std::vector<unsigned int>& startBoardState)
{
	graphWidget->scene()->clear();
	m_clickedNode = NULL;

	std::vector<GraphNodeInfo> parsingResult = parseTrace(startBoardState);
	std::vector<std::vector<GraphNode*>> graph = generateGraphNodes(parsingResult);
	if (!graph.empty())
	{
		drawGraph(graph);
	}

	PluginGame5GraphDialog::GraphInfo graphInfo = getGraphInfo();
	graphWidget->updateGraphInfo(graphInfo.solutionCost, graphInfo.numberOfOpenNodes, graphInfo.totalNumberOfNodes);
	graphWidget->scene()->setSceneRect(graphWidget->scene()->itemsBoundingRect());
	graphWidget->zoomFit();
}

void PluginGame5GraphDialog::onPluginAction(const std::vector<unsigned int>& boardState)
{
	if (m_traceTimeStamp != getTraceTimeStamp())
	{
		m_traceTimeStamp = getTraceTimeStamp();
		updateGraph(boardState);
	}
	exec();
}

void PluginGame5GraphDialog::updateCheckedNode(GraphNode* node)
{
	if (node != m_clickedNode)
	{
		if (m_clickedNode)
		{
			m_clickedNode->setChecked(false);
		}
		node->setChecked(true);
		m_clickedNode = node;
		emit updateNodeInfoDlg(node);
	}
}

void PluginGame5GraphDialog::emitShowNodeInfoDlg()
{
	emit showNodeInfoDlg();
}

PluginGame5GraphDialog::GraphInfo::GraphInfo(QString solutionCost, QString numberOfOpenNodes, QString totalNumberOfNodes)
	: solutionCost(solutionCost)
	, numberOfOpenNodes(numberOfOpenNodes)
	, totalNumberOfNodes(totalNumberOfNodes)
{}
