#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsObject>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_items_types.h"
// --------------------------------------------------------------------------------

class GraphNode;

class GraphEdge : public QGraphicsObject
{Q_OBJECT

public:
     GraphEdge(GraphNode& sourceNode, GraphNode& destNode);
    ~GraphEdge();

    virtual int type() const { return rdo::plugin::game5::TypeID::GRAPH_EDGE; }

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

private slots:
    void adjust();
};
