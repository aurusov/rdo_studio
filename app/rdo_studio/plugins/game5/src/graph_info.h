/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_info.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsItem>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_items_types.h"
// --------------------------------------------------------------------------------

class GraphInfo;

class GraphInfo : public QGraphicsItem
{
public:
	GraphInfo  (int solutionCost, int numOfOpenNodes, int totalNumOfNodes, double x, double y);
	~GraphInfo();

	enum { Type = UserType + TypeID::GRAPH_INFO };
	virtual int type() const { return Type; }

private:
	const int m_solutionCost;
	const int m_numOfOpenNodes;
	const int m_totalNumOfNodes;

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_
