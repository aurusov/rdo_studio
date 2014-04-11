/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_node.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_NODE_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_NODE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsObject>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_items_types.h"
// --------------------------------------------------------------------------------

class GraphWidget;
class QGraphicsSceneMouseEvent;
class GraphEdge;

class GraphNode: public QGraphicsObject
{Q_OBJECT

public:
	GraphNode(int graphNode, GraphNode* parentGraphNode, int pathCost, int restPathCost,
	          int moveDirection, int moveCost, int relevantTile, int graphLevel,
	          int tileMoveFrom, int tileMoveTo, const std::vector<unsigned int>& boardState
	);
	~GraphNode();

	enum { Type = UserType + TypeID::GRAPH_NODE };
	virtual int type() const { return Type; }

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	void setGraphOnLevelOrder(int value);
	void setRelatedToSolution(bool value);

	const std::list<GraphNode*>&     getChildrenList   () const;
	GraphNode*                       getParentGraphNode() const;
	const std::vector<unsigned int>& getBoardState     () const;

	int  getNodeID           () const;
	int  getPathCost         () const;
	int  getRestPathCost     () const;
	int  getMoveDirection    () const;
	int  getMoveCost         () const;
	int  getRelevantTile     () const;
	int  getGraphLevel       () const;
	int  getGraphOnLevelOrder() const;
	int  getTileMoveFrom     () const;
	int  getTileMoveTo       () const;
	bool isRelatedToSolution () const;

	void addEdge(GraphEdge* edge);

	bool   haveChild ()    const;
	double childrenMeanX() const;
	double childrenMeanY() const;
	void   addChild  (GraphNode* child);

	void   forceShift(double deltaX);
	void   setChecked(bool state);

	QPointF getBorderPointByAngle(double angle) const;
signals:
	void clickedNode(GraphNode* node);
	void doubleClicked();

private:
	std::list<GraphEdge*> edgeList;
	std::list<GraphNode*> childrenList;

	GraphNode* m_pParentGraphNode;
	std::vector<unsigned int> m_boardState;
	int  m_nodeID;
	int  m_pathCost;
	int  m_restPathCost;
	int  m_moveDirection;
	int  m_moveCost;
	int  m_relevantTile;
	int  m_graphLevel;
	int  m_tileMoveFrom;
	int  m_tileMoveTo;
	int  m_graphOnLevelOrder;
	bool m_relatedToSolutionState;
	bool isChecked;

	double height;
	double width;

	virtual QVariant itemChange           (GraphicsItemChange change, const QVariant &value);
	virtual void     mousePressEvent      (QGraphicsSceneMouseEvent* mEvent);
	virtual void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mEvent);
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_NODE_H_
