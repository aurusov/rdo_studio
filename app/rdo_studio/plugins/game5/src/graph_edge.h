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
#include "app/rdo_studio/plugins/game5/src/graph_items_types.h"
// --------------------------------------------------------------------------------

class GraphNode;

class GraphEdge : public QGraphicsItem
{
public:
	GraphEdge (GraphNode& sourceNode, GraphNode& destNode);
	~GraphEdge();

	void adjust();

	enum { Type = rdo::plugin::game5::TypeID::GRAPH_EDGE };
	virtual int type() const { return Type; }

private:
	GraphNode& source;
	GraphNode& dest;

	QPointF sourcePoint;
	QPointF destPoint;
	const double arrowSize;
	const double pointSize;
	const double penWidth;

	virtual QRectF boundingRect() const;
	virtual void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_EDGE_H_
