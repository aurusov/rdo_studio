#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsView>
#include <QWheelEvent>
#include "utils/src/common/warning_enable.h"
// ---------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_info.h"
// --------------------------------------------------------------------------------

class GraphWidget : public QGraphicsView
{
public:
     GraphWidget(QWidget* parent = 0);
    ~GraphWidget();

    void updateGraphInfo(const QString& solutionCost, const QString& numOfOpenNodes, const QString& totalNumberOfNodes);
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void normalSize();

private:
    GraphInfo m_graphInfo;
    bool m_dragModeCtrl;
    bool m_dragModeClick;
    bool m_autoScale;

    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* zoomFitAct;
    QAction* normalSizeAct;

    virtual void wheelEvent       (QWheelEvent* wEvent);
    virtual void keyPressEvent    (QKeyEvent*   kEvent);
    virtual void keyReleaseEvent  (QKeyEvent*   kEvent);
    virtual void mousePressEvent  (QMouseEvent* mEvent);
    virtual void mouseReleaseEvent(QMouseEvent* mEvent);

    void scaleView(double scaleFactor);

    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void callContextMenu(const QPoint &pos);
};
