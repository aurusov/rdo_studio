// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFontMetrics>
#include <list>
#include <math.h>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_node.h"
// --------------------------------------------------------------------------------

namespace
{
    QFont fontSizeMultiply(const QFont& baseFont, double multiplier)
    {
        QFont newFont(baseFont);
        if (baseFont.pixelSize() != -1)
        {
            newFont.setPixelSize((int)(baseFont.pixelSize() * multiplier));
        }
        if (baseFont.pointSize() != -1)
        {
            newFont.setPointSize((int)(baseFont.pointSize() * multiplier));
        }
        if (baseFont.pointSizeF() != -1)
        {
            newFont.setPointSizeF((int)(baseFont.pointSizeF() * multiplier));
        }
        return newFont;
    }

    const double LEVEL_OF_LOW_DETAIL = 0.8;
    const double LOW_DETAIL_MULTIPLIER = 1.5;
    const double LEVEL_OF_LOWER_DETAIL = 0.6;
    const double LOWER_DETAIL_MULTIPLIER = 3.0;
    const double WIDTH_MARGIN = 4;
    const double HEIGHT_MARGIN = 4;
} // end anonymous namespace

GraphNode::GraphNode(const GraphNodeInfo& info, GraphNode* parentGraphNode, int width, int height)
    : GraphNodeInfo      (info)
    , m_pParentGraphNode (parentGraphNode)
    , m_graphOnLevelOrder(0)
    , m_isChecked        (false)
    , m_width            (width + WIDTH_MARGIN)
    , m_height           (height + HEIGHT_MARGIN)
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
    return QRectF((-m_width  - adjust) / 2, (-m_height - adjust) / 2,
                   m_width + adjust,  m_height + adjust);
}

void GraphNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QFont sceneFont = painter->font();
    QRect nodeRect(int(-m_width / 2.), int(-m_height / 2.), int(m_width), int(m_height));
    painter->setPen(QPen(Qt::black, 0));
    if (m_isChecked)
    {
        painter->setBrush(Qt::darkGreen);
    }
    else if (m_relatedToSolutionState)
    {
        painter->setBrush(Qt::darkGray);
    }
    painter->drawRect(nodeRect);

    if (m_relatedToSolutionState || m_isChecked)
    {
        painter->setPen(QPen(Qt::white, 0));
    }

    const double levelOfDetail = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    if (levelOfDetail < LEVEL_OF_LOW_DETAIL)
    {
        if (levelOfDetail < LEVEL_OF_LOWER_DETAIL)
        {
            QString textStr = generateNodeTextLowerDetalization(m_nodeID);
            painter->setFont(fontSizeMultiply(sceneFont, LOWER_DETAIL_MULTIPLIER));
            painter->drawText(nodeRect, Qt::AlignCenter, textStr);
        }
        else
        {
            QString textStr = generateNodeTextLowDetalization(m_nodeID, m_pathCost, m_restPathCost, m_moveCost);
            painter->setFont(fontSizeMultiply(sceneFont, LOW_DETAIL_MULTIPLIER));
            painter->drawText(nodeRect, Qt::AlignCenter, textStr);
        }
    }
    else
    {
        QString textStr = generateNodeTextNormalDetalization(m_nodeID, m_pathCost, m_restPathCost, m_moveCost, m_relevantTile, m_tileMoveTo, m_moveDirection);
        painter->drawText(nodeRect, Qt::AlignCenter, textStr);
    }
    painter->setFont(sceneFont);
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

const QString& GraphNode::getMoveDirection() const
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
            emit positionChanged();
            break;
        default:
            break;
    }

    return QGraphicsItem::itemChange(change, value);
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
    for (GraphNode* child: childrenList)
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
    for (GraphNode* child: childrenList)
    {
        child->forceShift(deltaX);
    }
    setPos(pos().x() + deltaX, pos().y());
}

void GraphNode::setChecked(bool state)
{
    m_isChecked = state;
    update();
}

QPointF GraphNode::getBorderPointByAngle(double angle) const
{
    double xPos;
    double yPos;
    const double nodeDiagonal = sqrt(((m_height / 2.) * (m_height / 2.)) + ((m_width / 2.) * (m_width / 2.)));
    if (fabs(sin(angle)) * nodeDiagonal < m_height / 2.)
    {
        xPos = cos(angle) > 0 ? m_width / 2. : -m_width / 2.;
        yPos = xPos * sin(angle) / cos(angle);
    }
    else
    {
        yPos = sin(angle) > 0 ? m_height / 2. : -m_height / 2.;
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

QString GraphNode::generateNodeTextNormalDetalization(int nodeID, int pathCost, int restPathCost, int moveCost, int relevantTile, int tileMoveTo, const QString& moveDirection)
{
    return QString("%1 (%2/%3/%4)\nФишка %5 = %6\n%7").arg(
            QString::number(nodeID), QString::number(pathCost), QString::number(restPathCost), QString::number(moveCost),
            QString::number(relevantTile), QString::number(tileMoveTo),
            moveDirection);
}

QString GraphNode::generateNodeTextLowDetalization(int nodeID, int pathCost, int restPathCost, int moveCost)
{
    return QString("%1\n%2/%3/%4").arg(
            QString::number(nodeID),
            QString::number(pathCost), QString::number(restPathCost), QString::number(moveCost));
}

QString GraphNode::generateNodeTextLowerDetalization(int nodeID)
{
    return QString("%1").arg(QString::number(nodeID));
}

QRect GraphNode::calcTextWidth(const QString& text, const QFont& baseFont, Detalization detalization)
{
    QFontMetrics fontMetrics = QFontMetrics(baseFont);
    switch (detalization)
    {
        case Detalization::LOW :
            fontMetrics = QFontMetrics(fontSizeMultiply(baseFont, LOW_DETAIL_MULTIPLIER));
            break;

        case Detalization::LOWER :
            fontMetrics = QFontMetrics(fontSizeMultiply(baseFont, LOWER_DETAIL_MULTIPLIER));
            break;

        case Detalization::NORMAL :
            break;
    }

    return fontMetrics.boundingRect(QRect(), Qt::AlignCenter, text);;
}

QRect GraphNode::calcNodeRect(const GraphNodeInfo& info, const QFont& baseFont)
{
    const QString nodeTextNormalD = generateNodeTextNormalDetalization(
            info.m_nodeID, info.m_pathCost, info.m_restPathCost, info.m_moveCost, info.m_relevantTile, info.m_tileMoveTo, info.m_moveDirection);
    const QString nodeTextLowD = generateNodeTextLowDetalization(
            info.m_nodeID, info.m_pathCost, info.m_restPathCost, info.m_moveCost);
    const QString nodeTextLowerD = generateNodeTextLowerDetalization(info.m_nodeID);

    const QRect nodeRectNormalD = calcTextWidth(nodeTextNormalD, baseFont, Detalization::NORMAL);
    const QRect nodeRectLowD = calcTextWidth(nodeTextLowD, baseFont, Detalization::LOW);
    const QRect nodeRectLowerD = calcTextWidth(nodeTextLowerD, baseFont, Detalization::LOWER);

    QRect nodeRect = QRect();
    const int width = std::max(nodeRectNormalD.width(), std::max(nodeRectLowD.width(), nodeRectLowerD.width()));
    const int height = std::max(nodeRectNormalD.height(), std::max(nodeRectLowD.height(), nodeRectLowerD.height()));
    nodeRect.setWidth(width);
    nodeRect.setHeight(height);

    return nodeRect;
}
