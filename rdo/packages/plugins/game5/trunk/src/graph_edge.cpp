/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_edge.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QPainter>
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
#include "app/rdo_studio/plugins/game5/src/graph_edge.h"
// --------------------------------------------------------------------------------

static const double Pi = 3.14159265358979323846264338327950288419717;

GraphEdge::GraphEdge(GraphNode* sourceNode, GraphNode* destNode)
	: arrowSize(10        )
	, source   (sourceNode)
	, dest     (destNode  )
{
	setAcceptedMouseButtons(0);
	adjust();
	sourceNode->addEdge(this);
	destNode->addEdge(this);
}

GraphEdge::~GraphEdge()
{
}

void GraphEdge::adjust()
{
	if (!source || !dest)
		return;

	prepareGeometryChange();

	sourcePoint = mapFromItem(source, 0,  10);
	destPoint   = mapFromItem(dest  , 0, -10);
}

QRectF GraphEdge::boundingRect() const
{
	if (!source || !dest)
		return QRectF();

	double penWidth = 1;
	double extra = (penWidth + arrowSize) / 2.0;

	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
	                                  destPoint.y() - sourcePoint.y()))
	    .normalized()
	    .adjusted(-extra, -extra, extra, extra);
}

void GraphEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	if (!source || !dest)
		return;

	QLineF line(sourcePoint, destPoint);

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);

	if (line.length() > 10.)
	{
		double angle = ::asin(line.dy() / line.length());
		if (line.dx() < 0)
		{
			angle = Pi - angle;
		}

		QPointF destArrowP1 = destPoint - QPointF(cos(angle - Pi / 12.) * arrowSize,
																sin(angle - Pi / 12.) * arrowSize);
		QPointF destArrowP2 = destPoint - QPointF(cos(angle + Pi / 12.) * arrowSize,
																sin(angle + Pi / 12.) * arrowSize);

		painter->setBrush(Qt::black);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
}
