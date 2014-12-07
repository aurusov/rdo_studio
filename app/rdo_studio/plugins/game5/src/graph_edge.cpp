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
	, penWidth (2         )
{
	setAcceptedMouseButtons(Qt::NoButton);
	adjust();
	connect(&source, &GraphNode::positionChanged, this, &GraphEdge::adjust);
	connect(&dest  , &GraphNode::positionChanged, this, &GraphEdge::adjust);
}

GraphEdge::~GraphEdge()
{}

void GraphEdge::adjust()
{
	prepareGeometryChange();
	const double angle = -QLineF(source.pos(), dest.pos()).angle() * Pi / 180.;

	sourcePoint = mapFromItem(&source, source.getBorderPointByAngle(angle));
	destPoint   = mapFromItem(&dest, dest.getBorderPointByAngle(angle + Pi));
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
	if (line.length() > 2 * arrowSize)
	{
		const double angle = -line.angle() * Pi / 180.;

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
