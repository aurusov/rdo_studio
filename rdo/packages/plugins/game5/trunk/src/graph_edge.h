/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_edge.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_EDGE_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_EDGE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsItem>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GraphNode;

class GraphEdge : public QGraphicsItem
{
public:
	GraphEdge (GraphNode *sourceNode, GraphNode *destNode);
	~GraphEdge();

	void adjust();

	enum { Type = UserType + 2 };
	virtual int type() const { return Type; }

protected:
	virtual QRectF boundingRect() const;
	virtual void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	GraphNode* source;
	GraphNode* dest;

	QPointF sourcePoint;
	QPointF destPoint;
	double arrowSize;
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_EDGE_H_