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
// --------------------------------------------------------------------------------

class GraphWidget;
class QGraphicsSceneMouseEvent;
class GraphEdge;

class GraphNode: public QGraphicsObject
{
Q_OBJECT

private:
	typedef std::list<GraphNode*> NodeList;
	typedef std::list<GraphEdge*> EdgeList;

public:
	GraphNode(int graphNode, GraphNode* parentGraphNode, int pathCost, int restPathCost,
	          int moveDirection, int moveCost, int relevantTile, int graphLevel,
	          int tileMoveFrom, int tileMoveTo, const QString& boardState
	);
	~GraphNode();

	enum { Type = UserType + 1 };
	virtual int type() const { return Type; }

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	void setGraphOnLevelOrder(int value);
	void setRelatedToSolution(bool value);
		
	GraphNode*      getParentGraphNode  () const;
	int             getGraphNode        () const;
	int             getPathCost         () const;
	int             getRestPathCost     () const;
	int             getMoveDirection    () const;
	int             getMoveCost         () const;
	int             getRelevantTile     () const;
	int             getGraphLevel       () const;
	int             getGraphOnLevelOrder() const;
	int             getTileMoveFrom     () const;
	int             getTileMoveTo       () const;
	bool            isRelatedToSolution () const;
	const QString&  getBoardState       () const;
	const NodeList& getChildrenList() const;

	void addEdge(GraphEdge* edge);

	bool   haveChild ()    const;
	double childrenMeanX() const;
	double childrenMeanY() const;
	void   addChild  (GraphNode* child);

	void   forceShift(double deltaX);
	void   setChecked(bool state);

signals:
	void clickedNode(GraphNode* node);
	void doubleClicked();

protected:
	virtual QVariant itemChange           (GraphicsItemChange change, const QVariant &value);
	virtual void     mousePressEvent      (QGraphicsSceneMouseEvent* mEvent);
	virtual void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mEvent);

private:
	EdgeList edgeList;
	NodeList childrenList;

	GraphNode* m_pParentGraphNode;
	QString    m_boardState;
	int  m_graphNode;
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
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_NODE_H_