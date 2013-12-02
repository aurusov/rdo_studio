/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_info.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_
#define _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QGraphicsItem>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GraphInfo;

class GraphInfo : public QGraphicsItem
{
public:
	GraphInfo  (int solutionCost, int numOfOpenNodes, int totalNumOfNodes, double x, double y);
	~GraphInfo();

	enum { Type = UserType + 3 };
	virtual int type() const { return Type; }

protected:
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
	int m_solutionCost;
	int m_numOfOpenNodes;
	int m_totalNumOfNodes;
};

#endif // _RDO_PLUGIN_GAME_5_GRAPH_INFO_H_