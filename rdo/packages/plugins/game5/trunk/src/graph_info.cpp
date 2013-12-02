/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_info.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QPainter>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_info.h"
// --------------------------------------------------------------------------------

GraphInfo::GraphInfo(int solutionCost, int numOfOpenNodes, int totalNumOfNodes, double x, double y)
	: m_solutionCost   (solutionCost   )
	, m_numOfOpenNodes (numOfOpenNodes )
	, m_totalNumOfNodes(totalNumOfNodes)
{
	setAcceptedMouseButtons(0);
	setPos(x, y);
}

GraphInfo::~GraphInfo()
{
}

QRectF GraphInfo::boundingRect() const
{
	double adjust = 2;
	return QRectF(-adjust, -adjust, 200 + 2 * adjust, 70 + 2 * adjust);
}

void GraphInfo::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QRect nodeRect(0, 0, 200, 70);
	painter->setBrush(Qt::white);
	painter->setPen(QPen(Qt::white, 0));
	painter->setOpacity(0.4);
	painter->drawRect(nodeRect);

	QString str  = "Стоимость решения:___________" + QString::number(m_solutionCost)    + "\n";
	        str += "Количество раскрытых вершин:_" + QString::number(m_numOfOpenNodes)  + "\n";
	        str += "Количество вершин в графе:___" + QString::number(m_totalNumOfNodes) + "\n";

	painter->setOpacity(1.);
	painter->setPen(QPen(Qt::black, 0));
	painter->drawText(nodeRect, Qt::AlignLeft, str);
}
