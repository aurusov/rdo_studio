// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <math.h>
#include <QGridLayout>
#include <QAction>
#include <QShortcut>
#include <QMenu>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_widget.h"
// --------------------------------------------------------------------------------

namespace
{
	const double MAX_FACTOR          = 10;
	const double MIN_FACTOR          = 0.01;
	const double SCALE_SPEED         = 1/1200.;
	const double MANUAL_SCALE_FACTOR = 20 * SCALE_SPEED;
} // end anonymous namespace

GraphWidget::GraphWidget(QWidget* pParent)
	: QGraphicsView(pParent)
	, m_graphInfo(this)
	, m_autoScale(true)
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
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, &GraphWidget::callContextMenu);

	zoomInAct = new QAction("Zoom In", this);
	QShortcut* ctrlPlusShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this);
	connect(ctrlPlusShortcut, &QShortcut::activated, zoomInAct, &QAction::trigger);
	QShortcut* ctrlEqualShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal), this);
	connect(ctrlEqualShortcut, &QShortcut::activated, zoomInAct, &QAction::trigger);
	zoomInAct->setStatusTip("Приблизить");
	connect(zoomInAct, &QAction::triggered, this, &GraphWidget::zoomIn);

	zoomOutAct = new QAction("Zoom Out", this);
	QShortcut* ctrlMinusShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this);
	connect(ctrlMinusShortcut, &QShortcut::activated, zoomOutAct, &QAction::trigger);
	QShortcut* ctrlUnderscoreShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Underscore), this);
	connect(ctrlUnderscoreShortcut, &QShortcut::activated, zoomOutAct, &QAction::trigger);
	zoomOutAct->setStatusTip("Отдалить");
	connect(zoomOutAct, &QAction::triggered, this, &GraphWidget::zoomOut);

	zoomFitAct = new QAction("Zoom Fit", this);
	zoomFitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_9));
	zoomFitAct->setStatusTip("Вписать граф в окно");
	connect(zoomFitAct, &QAction::triggered, this, &GraphWidget::zoomFit);

	normalSizeAct = new QAction("Normal Size", this);
	normalSizeAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
	normalSizeAct->setStatusTip("Вернуть масштаб 1:1");
	connect(normalSizeAct, &QAction::triggered, this, &GraphWidget::normalSize);

	addAction(zoomInAct);
	addAction(zoomOutAct);
	addAction(zoomFitAct);
	addAction(normalSizeAct);
}

GraphWidget::~GraphWidget()
{}

void GraphWidget::updateGraphInfo(const QString& solutionCost, const QString& numberOfOpenNodes, const QString& totalNumberOfNodes)
{
	m_graphInfo.update(solutionCost, numberOfOpenNodes, totalNumberOfNodes);
}

void GraphWidget::zoomIn()
{
	scaleView(pow(2., MANUAL_SCALE_FACTOR));
}

void GraphWidget::zoomOut()
{
	scaleView(pow(2., -MANUAL_SCALE_FACTOR));
}

void GraphWidget::zoomFit()
{
	m_autoScale = true;
	fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
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

void GraphWidget::normalSize()
{
	m_autoScale = false;
	setTransform(QTransform());
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
	m_autoScale = false;
	const double factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (MIN_FACTOR < factor && factor < MAX_FACTOR)
	{
		scale(scaleFactor, scaleFactor);
	}
}

void GraphWidget::resizeEvent(QResizeEvent* event)
{
	if (m_autoScale)
	{
		zoomFit();
	}
	QGraphicsView::resizeEvent(event);
}

void GraphWidget::callContextMenu(const QPoint& pos)
{
	QMenu* menu = new QMenu;
	menu->addAction(zoomInAct);
	menu->addAction(zoomOutAct);
	menu->addAction(zoomFitAct);
	menu->addAction(normalSizeAct);
	menu->exec(viewport()->mapToGlobal(pos));
}
