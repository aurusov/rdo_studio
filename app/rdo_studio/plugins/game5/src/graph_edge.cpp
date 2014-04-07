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
#include "utils/src/common/warning_disable.h"
#include <QPainter>
#include <boost/math/constants/constants.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
#include "app/rdo_studio/plugins/game5/src/graph_edge.h"
// --------------------------------------------------------------------------------

namespace
{
	const double Pi = boost::math::constants::pi<double>();
} // end anonymous namespace

GraphEdge::GraphEdge(GraphNode& sourceNode, GraphNode& destNode)
	: source   (sourceNode)
	, dest     (destNode  )
	, arrowSize(10        )
	, pointSize(2         )
	, penWidth (1         )
{
	setAcceptedMouseButtons(Qt::NoButton);
	adjust();
	sourceNode.addEdge(this);
	destNode.addEdge(this);
}

GraphEdge::~GraphEdge()
{}

void GraphEdge::adjust()
{
	prepareGeometryChange();

	sourcePoint = mapFromItem(&source, 0,  10);
	destPoint   = mapFromItem(&dest  , 0, -10);
}

QRectF GraphEdge::boundingRect() const
{
	const double extra = (penWidth + arrowSize) / 2.0;

	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
	                                  destPoint.y() - sourcePoint.y()))
	    .normalized()
	    .adjusted(-extra, -extra, extra, extra);
}

void GraphEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QLineF line(sourcePoint, destPoint);

	painter->setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);

	painter->setBrush(Qt::black);
	if (line.length() > arrowSize)
	{
		double angle = ::asin(line.dy() / line.length());
		if (line.dx() < 0)
		{
			angle = Pi - angle;
		}

		const QPointF destArrowP1 = destPoint - QPointF(cos(angle - Pi / 12.) * arrowSize,
		                                                sin(angle - Pi / 12.) * arrowSize);
		const QPointF destArrowP2 = destPoint - QPointF(cos(angle + Pi / 12.) * arrowSize,
		                                                sin(angle + Pi / 12.) * arrowSize);

		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
	else
	{
		painter->drawEllipse(destPoint, pointSize, pointSize);
	}
}
