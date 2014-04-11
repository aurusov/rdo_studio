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
	const int MARGIN_X2 = 10;
	const int SPACER_X  = 20;
	const int SPACER_Y  = 20;

	std::pair<int, int> calcNodeSize(const QFont& font, int nodeID      , int pathCost  , int restPathCost,
	                                 int moveCost     , int relevantTile, int tileMoveTo, const QString& moveDirection
	)
	{
		QString nodeMultiLine = GraphNode::generateNodeTextLargeView(nodeID, pathCost, restPathCost, moveCost,
		                                                       relevantTile, tileMoveTo, moveDirection
		);
		QFontMetrics fontMetrics = QFontMetrics(font);
		int nodeWidth = 0;
		for(const auto& nodeStr: nodeMultiLine.split("\n"))
		{
			const int tempWidth = fontMetrics.width(nodeStr);
			nodeWidth = std::max(tempWidth, nodeWidth);
		}
		nodeWidth += MARGIN_X2;
		const int nodeHeight = fontMetrics.height() * 3 + fontMetrics.leading() * 2 + MARGIN_X2;
		return std::make_pair(nodeWidth, nodeHeight);
	}
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
	
	connect(this, &PluginGame5GraphDialog::updateNodeInfoDlg, nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::updateDlg);
	connect(this, &PluginGame5GraphDialog::showNodeInfoDlg  , nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::show);

	connect(nodeInfoDlg, &PluginGame5GraphNodeInfoDialog::updateCheckedNode, this, &PluginGame5GraphDialog::updateCheckedNode);
}

PluginGame5GraphDialog::~PluginGame5GraphDialog()
{
}

QString PluginGame5GraphDialog::getTraceFile()
{
	MainWindow* pMainWindow = (MainWindow*)(parent());
	rdo::gui::model::Model* pModel = pMainWindow->getModel();
	QString traceFile = pModel->getFullName().section('.', 0, -2) + ".trc";
	return traceFile;
}

QString PluginGame5GraphDialog::getTraceTimeStamp()
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readLine());
	}
	QString timeStamp = trcString.section(' ', -2, -1);
	return timeStamp;
}

std::vector<int> PluginGame5GraphDialog::getSolutionNodes()
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

	QString solutionStr;
	int begin = trcString.indexOf("SD");
	int end;
	if (begin > 0)
	{
		end = trcString.indexOf("SES", begin);
		solutionStr = trcString.mid(begin + 2, end - begin - 2);
	}

	QRegExp regExp("(\\d{1,5})((\\s\\d){3}\\s\\s\\d{1,2}\\s\\d{1,2})");
	std::vector<int> list;
	list.push_back(1);

	int pos=0;
	while((pos = regExp.indexIn(solutionStr, pos))!= -1)
	{
		list.push_back(regExp.cap(1).toInt());
		pos += regExp.matchedLength();
	}

	trcFile.close();
	return list;
}

QStringList PluginGame5GraphDialog::parseTrace()
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

	int begin = trcString.indexOf("SB");
	int end   = trcString.indexOf("SEN", begin + 2);
	QString trcStringS;
	trcStringS = trcString.mid(begin + 2, end - begin - 2);

	QRegExp regExp("(STN|STR)(((\\s)-?\\d{1,5}){8}((\\s)(\\s)(\\d{1,2})(\\s)(\\d{1,2})))(((\\nSRK)((\\s)\\d){4})(\\s\\d{1,2})((\\nSRK)((\\s)\\d){3})(\\s\\d{1,2}))?");
	QStringList list;
	int pos=0;
	while((pos = regExp.indexIn(trcStringS, pos))!= -1)
	{
		list << regExp.cap(2) + regExp.cap(16) + regExp.cap(21);
		pos += regExp.matchedLength();
	}
	trcFile.close();
	return list;
}

QString PluginGame5GraphDialog::getTraceInfo()
{
	QFile trcFile(getTraceFile());
	QString trcString;
	if (trcFile.open(QIODevice::ReadOnly))
	{
		trcString = QString(trcFile.readAll());
	}

	int begin = trcString.indexOf("DPS_C");
	QString trcStringShort = trcString.mid(begin);

	trcFile.close();
	return trcStringShort;
}

int PluginGame5GraphDialog::parseTraceInfo(const QString& key)
{
	QString regExpPat = "(" + key + "\\s)(\\d{1,5})";
	QRegExp regExp(regExpPat);
	int value = 0;
	QString str = getTraceInfo(); 

	int pos = 0;
	while((pos = regExp.indexIn(str, pos))!= -1)
	{
		value = regExp.cap(2).toInt();
		pos += regExp.matchedLength();
	}

	return value;
}

void PluginGame5GraphDialog::updateGraph(const std::vector<unsigned int>& startBoardState)
{
	graphWidget->scene->clear();//@todo (*)Хранить и рисовать 2 разных объекта
	m_graph.clear();            //Очистка вектора, объекты удалены при очистке сцены
	m_clickedNode = NULL;

	GraphNode* leftNode;
	GraphNode* rightNode;
	std::vector<std::vector<int>> paintLevel;
	QStringList parsingResult = parseTrace();
	//@todo Во время парсинга разделять строку на атрибуты GraphNode и запоминать самые широкие
	//заодно сделать (*)
	const std::pair<int, int> nodeSize = calcNodeSize(graphWidget->scene->font(), 999, 999, 999, 999, 999, 999, "Вправо");
	m_nodeWidth  = nodeSize.first;
	m_nodeHeight = nodeSize.second;

	m_graph.resize(parsingResult.size() + 2);
	m_graph[1] = new GraphNode(1, NULL, 0, 0, "Начало поиска", 0, 0, 0, 0, 0, startBoardState, m_nodeWidth, m_nodeHeight);
	for(const auto& string: parsingResult)
	{
		int graphNode        = string.section(" ",  1,  1).toInt();
		int parentGraphNode  = string.section(" ",  2,  2).toInt();
		int pathCost         = string.section(" ",  3,  3).toInt();
		int restPathCost     = string.section(" ",  4,  4).toInt() - pathCost;
		int moveDirection    = string.section(" ",  5,  5).toInt();
		int moveCost         = string.section(" ",  7,  7).toInt();

		int tileMoveFrom     = string.section(" ", -1, -1).toInt();
		int tileMoveTo       = string.section(" ", -2, -2).toInt();
		int relevantTile     = string.section(" ", -4, -4).toInt();

		QString moveDirectionText;
		switch (moveDirection)
		{
			case 1: moveDirectionText = "Вправо"; break;
			case 2: moveDirectionText = "Влево" ; break;
			case 3: moveDirectionText = "Вверх" ; break;
			case 4: moveDirectionText = "Вниз"  ; break;
		}

		int graphLevel = m_graph[parentGraphNode]->getGraphLevel() + 1;
		std::vector<unsigned int> boardState = m_graph[parentGraphNode]->getBoardState();
		unsigned int temp = boardState[tileMoveFrom - 1];
		boardState[tileMoveFrom - 1] = boardState[tileMoveTo - 1];
		boardState[tileMoveTo - 1]   = temp;
		//std::swap(boardState[tileMoveFrom], boardState[tileMoveTo]);
		
		m_graph[graphNode] = new GraphNode(graphNode, m_graph[parentGraphNode], pathCost, restPathCost,
		                                   moveDirectionText, moveCost, relevantTile, graphLevel,
		                                   tileMoveFrom, tileMoveTo, boardState, m_nodeWidth, m_nodeHeight
		);
	}

	for (unsigned int i = 1; i < m_graph.size(); i++)
	{
		int graphLevel = m_graph[i]->getGraphLevel();
		connect(m_graph[i], &GraphNode::clickedNode  , this, &PluginGame5GraphDialog::updateCheckedNode);
		connect(m_graph[i], &GraphNode::doubleClicked, this, &PluginGame5GraphDialog::emitShowNodeInfoDlg);
		int currentLevel = paintLevel.size() - 1;
		if (currentLevel < graphLevel)
		{
			paintLevel.push_back(std::vector<int>());
		}
		paintLevel[graphLevel].push_back(i);
	}

	for (unsigned int i = 1; i < paintLevel.size(); i++)
	{
		quickSort(paintLevel[i]);
		for (unsigned int j = 0; j < paintLevel[i].size(); j++)
		{
			m_graph[paintLevel[i][j]]->setGraphOnLevelOrder(j);
		}
	}

	for(auto node: getSolutionNodes())
	{
		m_graph[node]->setRelatedToSolution(true);
	}
	
	for (unsigned int j = 0; j < paintLevel.back().size(); j++)
	{
		int node = paintLevel.back()[j];

		graphWidget->scene->addItem(m_graph[node]);
		m_graph[node]->setPos((SPACER_X + m_nodeWidth) * (j + 1), (paintLevel.size() - 1) * (SPACER_Y + m_nodeHeight));
	}
	leftNode  = m_graph[paintLevel.back().front()];
	rightNode = m_graph[paintLevel.back().back()];
	
	for (int i = (int)paintLevel.size() - 2; i >= 0; i--)
	{
		bool buildFlag   = true;
		unsigned int tempNodeNum = 0;
		unsigned int tempCounter = 0;
		std::vector<UnbuiltRange> unbuiltRangeVector;
		for (unsigned int j = 0; j < paintLevel[i].size(); j++)
		{
			int node = paintLevel[i][j];
			if (m_graph[node]->haveChild())
			{
				graphWidget->scene->addItem(m_graph[node]);
				m_graph[node]->setPos(m_graph[node]->childrenMeanX(), m_graph[node]->childrenMeanY() - (SPACER_Y + m_nodeHeight));
				for(auto childNode: m_graph[node]->getChildrenList())
				{
					graphWidget->scene->addItem(new GraphEdge(*m_graph[node], *childNode));
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
		for(const auto& unbuiltRange: unbuiltRangeVector)
		{
			unsigned int endUnbuiltRange = unbuiltRange.firstNode + unbuiltRange.range;
			if (unbuiltRange.firstNode == 0)
			{
				int temp  = paintLevel[i][endUnbuiltRange];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					int node  = paintLevel[i][k];
					int segment = unbuiltRange.range - k + unbuiltRange.firstNode;

					graphWidget->scene->addItem(m_graph[node]);
					m_graph[node]->setPos(m_graph[temp]->pos().x() - (SPACER_X + m_nodeWidth) * segment, m_graph[temp]->pos().y());
				}
				if (leftNode->pos().x() > m_graph[paintLevel[i][unbuiltRange.firstNode]]->pos().x())
				{
					leftNode = m_graph[paintLevel[i][unbuiltRange.firstNode]];
				}
			}
			else if (endUnbuiltRange == paintLevel[i].size())
			{
				int temp = paintLevel[i][unbuiltRange.firstNode - 1];
				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					int node  = paintLevel[i][k];
					int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene->addItem(m_graph[node]);
					m_graph[node]->setPos(m_graph[temp]->pos().x() + (SPACER_X + m_nodeWidth) * segment, m_graph[temp]->pos().y());
				}
				if (rightNode->pos().x() < m_graph[paintLevel[i][endUnbuiltRange - 1]]->pos().x())
				{
					rightNode = m_graph[paintLevel[i][endUnbuiltRange - 1]];
				}
			}
			else
			{
				int temp1  = paintLevel[i][unbuiltRange.firstNode - 1];
				int temp2  = paintLevel[i][endUnbuiltRange];
				double deltaX =  m_graph[temp2]->pos().x() - m_graph[temp1]->pos().x();

				if (deltaX < (unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth))
				{
					for (unsigned int l = endUnbuiltRange; l < paintLevel[i].size(); l++)
					{
						int node    = paintLevel[i][l];
						m_graph[node]->forceShift((unbuiltRange.range + 1) * (SPACER_X + m_nodeWidth) - deltaX);
					}
					deltaX = m_graph[temp2]->pos().x() - m_graph[temp1]->pos().x();
				}

				for (unsigned int k = unbuiltRange.firstNode; k < endUnbuiltRange; k++)
				{
					int node    = paintLevel[i][k];
					int segment = k - unbuiltRange.firstNode + 1;

					graphWidget->scene->addItem(m_graph[node]);
					m_graph[node]->setPos(m_graph[temp1]->pos().x() + segment * deltaX/(unbuiltRange.range + 1), m_graph[temp1]->pos().y());
				}
			}
		}
	}
	graphWidget->scene->addItem(new GraphInfo(parseTraceInfo("DPS_CO"), parseTraceInfo("DPS_TO"), parseTraceInfo("DPS_TT"),
	                                          graphWidget->scene->sceneRect().x(), graphWidget->scene->sceneRect().y()));
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
	SortStruct s(this);
	std::sort(vector.begin(), vector.end(), s);
}
