/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_node.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <list>
#include <math.h>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
#include "app/rdo_studio/plugins/game5/src/graph_edge.h"
// --------------------------------------------------------------------------------

namespace
{
	const double HEIGHT = 20;
	const double WIDTH  = 20;
} // end anonymous namespace

GraphNode::GraphNode(int nodeID, GraphNode* parentGraphNode, int pathCost, int restPathCost,
                     int moveDirection, int moveCost, int relevantTile, int graphLevel,
                     int tileMoveFrom, int tileMoveTo, const std::vector<unsigned int>& boardState
)
	: m_pParentGraphNode      (parentGraphNode)
	, m_boardState            (boardState     )
	, m_nodeID                (nodeID         )
	, m_pathCost              (pathCost       )
	, m_restPathCost          (restPathCost   )
	, m_moveDirection         (moveDirection  )
	, m_moveCost              (moveCost       )
	, m_relevantTile          (relevantTile   )
	, m_graphLevel            (graphLevel     )
	, m_tileMoveFrom          (tileMoveFrom   )
	, m_tileMoveTo            (tileMoveTo     )
	, m_graphOnLevelOrder     (0              )
	, m_relatedToSolutionState(false          )
	, isChecked               (false          )
	, height                  (HEIGHT         )
	, width                   (WIDTH          )
{
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);

	if (parentGraphNode)
	{
		parentGraphNode->addChild(this);
	}
}

GraphNode::~GraphNode()
{}

QRectF GraphNode::boundingRect() const
{
	double adjust = 2;
	return QRectF((-width  - adjust) / 2, (-height - adjust) / 2,
	               width + adjust,  height + adjust);
}

void GraphNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QRect nodeRect(-width / 2, -height / 2, width, height);
	painter->setPen(QPen(Qt::black, 0));
	if (isChecked)
	{
		painter->setBrush(Qt::darkGreen);
	}
	else if (m_relatedToSolutionState)
	{
		painter->setBrush(Qt::darkGray);
	}
	painter->drawRect(nodeRect);

	if (m_relatedToSolutionState || isChecked)
	{
		painter->setPen(QPen(Qt::white, 0));
	}
	painter->drawText(nodeRect, Qt::AlignCenter, QString::number(m_nodeID));
}

int GraphNode::getNodeID() const
{
	return m_nodeID;
}

int GraphNode::getPathCost() const
{
	return m_pathCost;
}

int GraphNode::getRestPathCost() const
{
	return m_restPathCost;
}

int GraphNode::getMoveDirection() const
{
	return m_moveDirection;
}

int GraphNode::getMoveCost() const
{
	return m_moveCost;
}

int GraphNode:: getRelevantTile() const
{
	return m_relevantTile;
}

int GraphNode::getGraphLevel() const
{
	return m_graphLevel;
}

int GraphNode::getGraphOnLevelOrder() const
{
	return m_graphOnLevelOrder;
}

int GraphNode::getTileMoveFrom() const
{
	return m_tileMoveFrom;
}
int GraphNode::getTileMoveTo() const
{
	return m_tileMoveTo;
}

const std::vector<unsigned int>& GraphNode::getBoardState() const
{
	return m_boardState;
}

GraphNode* GraphNode::getParentGraphNode() const
{
	return m_pParentGraphNode;
}

bool GraphNode::isRelatedToSolution() const
{
	return m_relatedToSolutionState;
}

void GraphNode::setRelatedToSolution(bool value)
{
	m_relatedToSolutionState = value;
}

void GraphNode::setGraphOnLevelOrder(int value)
{
	m_graphOnLevelOrder = value;
}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
		case ItemPositionHasChanged:
			for(auto edge: edgeList)
			{
				edge->adjust();
			}
			break;
		default:
			break;
	}

	return QGraphicsItem::itemChange(change, value);
}

void GraphNode::addEdge(GraphEdge* edge)
{
	edgeList.push_back(edge);
}

void GraphNode::addChild(GraphNode* child)
{
	childrenList.push_back(child);
}

bool GraphNode::haveChild() const
{
	return !childrenList.empty();
}

double GraphNode::childrenMeanX() const
{
	if (!haveChild())
		return 0.;

	double value(0);
	for(const auto child: childrenList)
	{
		value += child->pos().x();
	}

	return value / childrenList.size();
}

double GraphNode::childrenMeanY() const
{
	double value(0);
	if (haveChild())
		value = (*childrenList.begin())->pos().y();
	return value;
}

const std::list<GraphNode*>& GraphNode::getChildrenList() const
{
	return childrenList;
}

void GraphNode::forceShift(double deltaX)
{
	for(auto child: childrenList)
	{
		child->forceShift(deltaX);
	}
	setPos(pos().x() + deltaX, pos().y());
}

void GraphNode::setChecked(bool state)
{
	isChecked = state;
	update();
}

QPointF GraphNode::getBorderPointByAngle(double angle) const
{
	double xPos;
	double yPos;
	const double nodeDiagonal = sqrt(((height / 2.) * (height / 2.)) + ((width / 2.) * (width / 2.)));
	if (fabs(sin(angle)) * nodeDiagonal < height / 2.)
	{
		xPos = cos(angle) > 0 ? width / 2. : -width / 2.;
		yPos = xPos * sin(angle) / cos(angle);
	}
	else
	{
		yPos = sin(angle) > 0 ? height / 2. : -height / 2.;
		xPos = yPos * cos(angle) / sin(angle);
	}
	return QPointF(xPos, yPos);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent* mEvent)
{
	if (!(mEvent->modifiers()))
	{
		if (mEvent->button() == Qt::LeftButton)
		{
			emit clickedNode(this);
		}
		QGraphicsItem::mousePressEvent(mEvent);
	}
}

void GraphNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mEvent)
{
	emit doubleClicked();
	QGraphicsItem::mouseDoubleClickEvent(mEvent);
}
