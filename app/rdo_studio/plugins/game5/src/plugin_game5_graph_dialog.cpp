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
} // end anonymous namespace

PluginGame5GraphDialog::PluginGame5GraphDialog(QWidget * pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_traceTimeStamp(getTraceTimeStamp())
	, m_clickedNode(NULL)
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

std::vector<int> PluginGame5GraphDialog::getSolutionNodes() const
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
	std::vector<int> list;
	list.push_back(1);

	int pos = 0;
	while ((pos = regExp.indexIn(solutionStr, pos))!= -1)
	{
		list.push_back(regExp.cap(1).toInt());
		pos += regExp.matchedLength();
	}

	trcFile.close();
	return list;
}

QStringList PluginGame5GraphDialog::parseTrace() const
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

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
	trcFile.close();
	return list;
}

PluginGame5GraphDialog::GraphInfo PluginGame5GraphDialog::getGraphInfo() const
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

	const int begin = trcString.indexOf(QRegExp("SES|SEN"));
	const QString trcStringShort = trcString.mid(begin);

	QString solutionCost       = trcStringShort.section(' ', DPS_CO_INDEX, DPS_CO_INDEX, QString::SectionSkipEmpty);
	QString numberOfOpenNodes  = trcStringShort.section(' ', DPS_TO_INDEX, DPS_TO_INDEX, QString::SectionSkipEmpty);
	QString totalNumberOfNodes = trcStringShort.section(' ', DPS_TT_INDEX, DPS_TT_INDEX, QString::SectionSkipEmpty);

	trcFile.close();
	return GraphInfo(solutionCost, numberOfOpenNodes, totalNumberOfNodes);
}

void PluginGame5GraphDialog::updateGraph(const std::vector<unsigned int>& startBoardState)
{
	graphWidget->scene()->clear();//@todo (*)Хранить и рисовать 2 разных объекта
	m_graphNodeList.clear();      //Очистка вектора, объекты удалены при очистке сцены
	m_clickedNode = NULL;

	std::vector<std::vector<int>> paintLevel;
	QStringList parsingResult = parseTrace();
	//@todo Во время парсинга разделять строку на атрибуты GraphNode и запоминать самые широкие
	//заодно сделать (*)
	const QString nodeText = GraphNode::generateNodeTextLargeView(999, 999, 999, 999, 999, 999, "Начало поиска");
	const QFontMetrics fontMetrics = QFontMetrics(graphWidget->scene()->font());
	const QRect nodeRect = fontMetrics.boundingRect(QRect(), Qt::AlignCenter, nodeText);
	m_nodeWidth  = nodeRect.width();
	m_nodeHeight = nodeRect.height();

	m_graphNodeList.resize(parsingResult.size() + 1);
	m_graphNodeList[0] = new GraphNode(1, NULL, 0, 0, "Начало поиска", 0, 0, 0, 0, 0, startBoardState, m_nodeWidth, m_nodeHeight);
	for (const auto& string: parsingResult)
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

		const int graphLevel = m_graphNodeList[parentGraphNodeIndex]->getGraphLevel() + 1;
		std::vector<unsigned int> boardState = m_graphNodeList[parentGraphNodeIndex]->getBoardState();
		std::swap(boardState[tileMoveFrom - 1], boardState[tileMoveTo - 1]);

		m_graphNodeList[graphNodeIndex] = new GraphNode(graphNodeId, m_graphNodeList[parentGraphNodeIndex],
		                                                pathCost, restPathCost, moveDirectionText, moveCost,
		                                                relevantTile, graphLevel, tileMoveFrom, tileMoveTo,
		                                                boardState, m_nodeWidth, m_nodeHeight);
	}

	for (unsigned int i = 0; i < m_graphNodeList.size(); i++)
	{
		connect(m_graphNodeList[i], &GraphNode::clickedNode  , this, &PluginGame5GraphDialog::updateCheckedNode);
		connect(m_graphNodeList[i], &GraphNode::doubleClicked, this, &PluginGame5GraphDialog::emitShowNodeInfoDlg);

		const int nodeGraphLevel = m_graphNodeList[i]->getGraphLevel();
		const int currentLevel = paintLevel.size() - 1;
		if (currentLevel < nodeGraphLevel)
		{
			paintLevel.push_back(std::vector<int>());
		}
		paintLevel[nodeGraphLevel].push_back(i);
	}

	for (unsigned int i = 1; i < paintLevel.size(); i++)
	{
		quickSort(paintLevel[i]);
		for (unsigned int j = 0; j < paintLevel[i].size(); j++)
		{
			m_graphNodeList[paintLevel[i][j]]->setGraphOnLevelOrder(j);
		}
	}

	for (int nodeId: getSolutionNodes())
	{
		m_graphNodeList[nodeId - 1]->setRelatedToSolution(true);
	}
	
	for (unsigned int j = 0; j < paintLevel.back().size(); j++)
	{
		const int node = paintLevel.back()[j];

		graphWidget->scene()->addItem(m_graphNodeList[node]);
		m_graphNodeList[node]->setPos((SPACER_X + m_nodeWidth) * (j + 1), (paintLevel.size() - 1) * (SPACER_Y + m_nodeHeight));
	}
	
	for (int i = (int)paintLevel.size() - 2; i >= 0; i--)
	{
		bool buildFlag   = true;
		unsigned int tempNodeNum = 0;
		unsigned int tempCounter = 0;
		std::vector<UnbuiltRange> unbuiltRangeVector;
		for (unsigned int j = 0; j < paintLevel[i].size(); j++)
		{
			const int node = paintLevel[i][j];
			if (m_graphNodeList[node]->haveChild())
			{
				graphWidget->scene()->addItem(m_graphNodeList[node]);
				m_graphNodeList[node]->setPos(m_graphNodeList[node]->childrenMeanX(), m_graphNodeList[node]->childrenMeanY() - (SPACER_Y + m_nodeHeight));
				for (auto childNode: m_graphNodeList[node]->getChildrenList())
				{
					graphWidget->scene()->addItem(new GraphEdge(*m_graphNodeList[node], *childNode));
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
				const int temp = paintLevel[i][endUnbuiltRange];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					const int node = paintLevel[i][k];
					const int segment = unbuiltRange.range - k + unbuiltRange.firstNode;

					graphWidget->scene()->addItem(m_graphNodeList[node]);
					m_graphNodeList[node]->setPos(m_graphNodeList[temp]->pos().x() - (SPACER_X + m_nodeWidth) * segment, m_graphNodeList[temp]->pos().y());
				}
			}
			else if (endUnbuiltRange == paintLevel[i].size())
			{
				const int temp = paintLevel[i][unbuiltRange.firstNode - 1];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					const int node = paintLevel[i][k];
					const int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene()->addItem(m_graphNodeList[node]);
					m_graphNodeList[node]->setPos(m_graphNodeList[temp]->pos().x() + (SPACER_X + m_nodeWidth) * segment, m_graphNodeList[temp]->pos().y());
				}
			}
			else
			{
				const int temp1 = paintLevel[i][unbuiltRange.firstNode - 1];
				const int temp2 = paintLevel[i][endUnbuiltRange];
				double deltaX =  m_graphNodeList[temp2]->pos().x() - m_graphNodeList[temp1]->pos().x();

				if (deltaX < (unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth))
				{
					for (unsigned int l = endUnbuiltRange; l < paintLevel[i].size(); l++)
					{
						const int node = paintLevel[i][l];
						m_graphNodeList[node]->forceShift((unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth) - deltaX);
					}
					deltaX = m_graphNodeList[temp2]->pos().x() - m_graphNodeList[temp1]->pos().x();
				}

				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					const int node    = paintLevel[i][k];
					const int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene()->addItem(m_graphNodeList[node]);
					m_graphNodeList[node]->setPos(m_graphNodeList[temp1]->pos().x() + segment * deltaX/(unbuiltRange.range + 1), m_graphNodeList[temp1]->pos().y());
				}
			}
		}
	}
	PluginGame5GraphDialog::GraphInfo graphInfo = getGraphInfo();
	graphWidget->updateGraphInfo(graphInfo.solutionCost, graphInfo.numberOfOpenNodes, graphInfo.totalNumberOfNodes);
	graphWidget->scene()->setSceneRect(graphWidget->scene()->itemsBoundingRect());
	graphWidget->fitScene();
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

void PluginGame5GraphDialog::quickSort(std::vector<int>& vector)
{
	SortStruct sortStruct(this);
	std::sort(vector.begin(), vector.end(), sortStruct);
}

PluginGame5GraphDialog::GraphInfo::GraphInfo(QString solutionCost, QString numberOfOpenNodes, QString totalNumberOfNodes)
	: solutionCost(solutionCost)
	, numberOfOpenNodes(numberOfOpenNodes)
	, totalNumberOfNodes(totalNumberOfNodes)
{}
