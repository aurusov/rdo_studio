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
#include <QGridLayout>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_widget.h"
// --------------------------------------------------------------------------------

namespace
{
	const double MAX_FACTOR          = 10;
	const double MIN_FACTOR          = 0.1;
	const double SCALE_SPEED         = 1/2400.;
	const double MANUAL_SCALE_FACTOR = 20 * SCALE_SPEED;
} // end anonymous namespace

GraphWidget::GraphWidget(QWidget* pParent)
	: QGraphicsView(pParent)
	, m_graphInfo(this)
{
	QGraphicsScene* pScene = new QGraphicsScene(this);
	pScene->setItemIndexMethod(QGraphicsScene::NoIndex);

	setScene(pScene);

	QGridLayout* graphWidgetLayout = new QGridLayout(this);
	graphWidgetLayout->setContentsMargins(0, 0, 0, 0);

	QSpacerItem* verticalSpacer   = new QSpacerItem(20, 40, QSizePolicy::Minimum  , QSizePolicy::Expanding);
	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	graphWidgetLayout->addWidget(&m_graphInfo    , 0, 0, 1, 1);
	graphWidgetLayout->addItem  (verticalSpacer  , 1, 0, 1, 1);
	graphWidgetLayout->addItem  (horizontalSpacer, 0, 1, 2, 1);

	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
}

GraphWidget::~GraphWidget()
{}

void GraphWidget::updateGraphInfo(const QString& solutionCost, const QString& numberOfOpenNodes, const QString& totalNumberOfNodes)
{
	m_graphInfo.update(solutionCost, numberOfOpenNodes, totalNumberOfNodes);
}

void GraphWidget::fitScene()
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	const double factor = transform().mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor > MAX_FACTOR)
	{
		scale(MAX_FACTOR / factor, MAX_FACTOR / factor);
	}
	if (factor < MIN_FACTOR)
	{
		scale(MIN_FACTOR / factor, MIN_FACTOR / factor);
	}
}

void GraphWidget::wheelEvent(QWheelEvent* wEvent)
{
	if (wEvent->modifiers() & Qt::SHIFT)
	{
		scaleView(pow(2., wEvent->delta() * SCALE_SPEED));
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
		m_dragModeCtrl = true;
	}
	if (kEvent->modifiers() & Qt::CTRL)
	{
		if (kEvent->key() == Qt::Key_0)
		{
			setTransform(QTransform());
		}
		if (kEvent->key() == Qt::Key_Minus)
		{
			scaleView(pow(2., -MANUAL_SCALE_FACTOR));
		}
		if (kEvent->key() == Qt::Key_Plus)
		{
			scaleView(pow(2., MANUAL_SCALE_FACTOR));
		}
		if (kEvent->key() == Qt::Key_9)
		{
			fitScene();
		}
	}

}

void GraphWidget::keyReleaseEvent(QKeyEvent* kEvent)
{
	QGraphicsView::keyReleaseEvent(kEvent);
	if (kEvent->key() == Qt::Key_Control)
	{
		if (!m_dragModeClick)
		{
			setDragMode(NoDrag);
			setInteractive(true);
		}
		m_dragModeCtrl = false;
	}
}

void GraphWidget::mousePressEvent(QMouseEvent* mEvent)
{
	if (mEvent->button() == Qt::MouseButton::LeftButton && !itemAt(mEvent->pos()))
	{
		setDragMode(ScrollHandDrag);
		m_dragModeClick = true;
	}
	QGraphicsView::mousePressEvent(mEvent);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* mEvent)
{
	if (mEvent->button() == Qt::MouseButton::LeftButton)
	{
		if (!m_dragModeCtrl)
		{
			setDragMode(NoDrag);
		}
		m_dragModeClick = false;
	}
	QGraphicsView::mouseReleaseEvent(mEvent);
}

void GraphWidget::scaleView(double scaleFactor)
{
	const double factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (MIN_FACTOR < factor && factor < MAX_FACTOR)
	{
		scale(scaleFactor, scaleFactor);
	}
}
