/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_widget.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <math.h>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_widget.h"
// --------------------------------------------------------------------------------

GraphWidget::GraphWidget(QWidget* pParent)
	: QGraphicsView(pParent)
	, scene        (NULL   )
{
	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);

	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
}

GraphWidget::~GraphWidget()
{
}

void GraphWidget::wheelEvent(QWheelEvent* wEvent)
{
	if (wEvent->modifiers() & Qt::SHIFT)
	{
		scaleView(pow((double)2, wEvent->delta() / 2400.0));
	}
	else
	{
			QGraphicsView::wheelEvent(wEvent);
	}
}

void GraphWidget::keyPressEvent(QKeyEvent* kEvent)
{
	QGraphicsView::keyPressEvent(kEvent);
	if (kEvent->key() == Qt::Key_Control)
	{
		setDragMode(ScrollHandDrag);
		setInteractive(false);
	}
}

void GraphWidget::keyReleaseEvent(QKeyEvent* kEvent)
{
	QGraphicsView::keyReleaseEvent(kEvent);
	if (kEvent->key() == Qt::Key_Control)
	{
		setDragMode(NoDrag);
		setInteractive(true);
	}
}

void GraphWidget::scaleView(double scaleFactor)
{
	double factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (0.5 < factor && factor < 4)
	{
		scale(scaleFactor, scaleFactor);
	}
}