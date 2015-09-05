#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsObject>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_items_type.h"
#include "app/rdo_studio/plugins/game5/src/graph_node_info.h"
// --------------------------------------------------------------------------------

class GraphNode
    : public  QGraphicsObject
    , private GraphNodeInfo
{Q_OBJECT

public:
    GraphNode(const GraphNodeInfo& info, GraphNode* parentGraphNode, int width, int height);
    ~GraphNode();

    enum class Detalization
    {
        NORMAL,
        LOW,
        LOWER
    };

    virtual int type() const { return static_cast<int>(rdo::plugin::game5::GraphItemType::NODE); }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void setGraphOnLevelOrder(int value);
    void setRelatedToSolution(bool value);

    const std::list<GraphNode*>&     getChildrenList   () const;
    GraphNode*                       getParentGraphNode() const;
    const std::vector<unsigned int>& getBoardState     () const;

    int             getNodeID           () const;
    int             getPathCost         () const;
    int             getRestPathCost     () const;
    const QString&  getMoveDirection    () const;
    int             getMoveCost         () const;
    int             getRelevantTile     () const;
    int             getGraphLevel       () const;
    int             getGraphOnLevelOrder() const;
    int             getTileMoveFrom     () const;
    int             getTileMoveTo       () const;
    bool            isRelatedToSolution () const;

    bool   haveChild ()    const;
    double childrenMeanX() const;
    double childrenMeanY() const;
    void   addChild  (GraphNode* child);

    void   forceShift(double deltaX);
    void   setChecked(bool state);

    QPointF getBorderPointByAngle(double angle) const;

    static QString generateNodeTextNormalDetalization(int nodeID, int pathCost, int restPathCost, int moveCost,
            int relevantTile, int tileMoveTo, const QString& moveDirection);
    static QString generateNodeTextLowDetalization(int nodeID, int pathCost, int restPathCost, int moveCost);
    static QString generateNodeTextLowerDetalization(int nodeID);
    static QRect calcTextWidth(const QString& text, const QFont& baseFont, Detalization detalization);
    static QRect calcNodeRect(const GraphNodeInfo& info, const QFont& baseFont);

signals:
    void clickedNode(GraphNode* node);
    void doubleClicked();
    void positionChanged();

private:
    std::list<GraphNode*> childrenList;
    GraphNode* m_pParentGraphNode;

    int     m_graphOnLevelOrder;
    bool    m_isChecked;
    double  m_width;
    double  m_height;

    virtual QVariant itemChange           (GraphicsItemChange change, const QVariant &value);
    virtual void     mousePressEvent      (QGraphicsSceneMouseEvent* mEvent);
    virtual void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mEvent);
};
