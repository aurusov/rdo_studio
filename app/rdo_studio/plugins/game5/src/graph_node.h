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

class GraphNode: public QGraphicsObject
{Q_OBJECT

public:
	GraphNode(int graphNode, GraphNode* parentGraphNode, int pathCost, int restPathCost,
	          const QString& moveDirection, int moveCost, int relevantTile, int graphLevel, int tileMoveFrom,
	          int tileMoveTo, const std::vector<unsigned int>& boardState, int width, int height
	);
	~GraphNode();

	enum { Type = rdo::plugin::game5::TypeID::GRAPH_NODE };
	virtual int type() const { return Type; }

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

	static QString generateNodeTextLargeView (int nodeID, int pathCost, int restPathCost, int moveCost,
	                                          int relevantTile, int tileMoveTo, const QString& moveDirection);
	static QString generateNodeTextMediumView(int nodeID, int pathCost, int restPathCost, int moveCost);
	static QString generateNodeTextSmallView (int nodeID);

signals:
	void clickedNode(GraphNode* node);
	void doubleClicked();
	void positionChanged();

private:
	std::list<GraphNode*> childrenList;

	GraphNode* m_pParentGraphNode;
	std::vector<unsigned int> m_boardState;
	int     m_nodeID;
	int     m_pathCost;
	int     m_restPathCost;
	QString m_moveDirection;
	int     m_moveCost;
	int     m_relevantTile;
	int     m_graphLevel;
	int     m_tileMoveFrom;
	int     m_tileMoveTo;
	int     m_graphOnLevelOrder;
	bool    m_relatedToSolutionState;
	bool    m_isChecked;
	double  m_width;
	double  m_height;

	virtual QVariant itemChange           (GraphicsItemChange change, const QVariant &value);
	virtual void     mousePressEvent      (QGraphicsSceneMouseEvent* mEvent);
	virtual void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mEvent);
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_NODE_H_
