#ifndef _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGroupBox>
#include <QLabel>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GraphInfo;

class GraphInfo : public QGroupBox
{
public:
	 GraphInfo(QWidget* parent = 0);
	~GraphInfo();

	void update(const QString& solutionCost, const QString& numOfOpenNodes, const QString& totalNumOfNodes);

private:
	QLabel* m_solutionCostLabel;
	QLabel* m_solutionCostValue;
	QLabel* m_numberOfOpenNodesLabel;
	QLabel* m_numberOfOpenNodesValue;
	QLabel* m_totalNumberOfNodesLabel;
	QLabel* m_totalNumberOfNodesValue;
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_
