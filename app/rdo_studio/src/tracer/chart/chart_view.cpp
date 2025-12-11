// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <algorithm>
#include <QProcess>
#include <QEvent>
#include <QClipboard>
#include <QPainterPath>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_view.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/tracer/chart/chart_view_style.h"
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/tracer/chart/chart_preferences.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;
using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- ChartView
// --------------------------------------------------------------------------------
ChartView::ChartView(QAbstractScrollArea* pParent, ChartDoc* pDocument, const bool preview)
    : super(pParent)
    , m_pddSerie(NULL)
    , m_bmpRect(0, 0, 0, 0)
    , m_clientRect(0, 0, 0, 0)
    , m_valueCountX(5)
    , m_valueCountY(5)
    , m_timeWrapFlag(true)
    , m_chartRect(0, 0, 0, 0)
    , m_timeScale(0)
    , m_chartShift(0)
    , m_drawFromEventID(0)
    , m_drawToEventCount(0)
    , m_legendRect(0, 0, 0, 0)
    , m_pPopupMenu(NULL)
    , m_zoom(1)
    , m_zoomOld(1)
    , m_zoomAuto(1)
    , m_zoomAutoFlag(false)
    , m_scaleKoeff(1)
    , m_previewMode(preview)
    , m_pStyle(NULL)
    , m_pYAxis(NULL)
    , m_needDrawLegend(true)
    , m_pDocument(pDocument)
{
    setAcceptDrops(true);
    connect(&getHorzScrollBar(), &QScrollBar::valueChanged, this, &ChartView::onHorzScrollBarValueChanged);

    if (m_previewMode)
        m_timeWrapFlag = false;

    if (!m_previewMode)
        setStyle(&g_pApp->getStyle()->style_chart, false);

    if (getDocument())
    {
        recalcLayout();
        updateScrollBars();
    }

    Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
    ASSERT(pMainWindow);

    m_pPopupMenu = new QMenu(pParent);
    m_pPopupMenu->addAction(pMainWindow->actEditCopy);
    m_pPopupMenu->addSeparator();
    m_pPopupMenu->addAction(pMainWindow->actViewZoomInc);
    m_pPopupMenu->addAction(pMainWindow->actViewZoomDec);
    m_pPopupMenu->addAction(pMainWindow->actViewZoomAuto);
    m_pPopupMenu->addAction(pMainWindow->actViewZoomReset);
    m_pPopupMenu->addSeparator();
    m_pPopupMenu->addAction(pMainWindow->actChartTimeWrap);
    m_pPopupMenu->addSeparator();
    m_pPopupMenu->addAction(pMainWindow->actChartOptions);

    attachToDoc();
}

ChartView::~ChartView()
{
    if (getDocument())
        getDocument()->removeFromViews(this);

    delete m_pDocument;
}

int ChartView::getValueCountX() const
{
    return m_valueCountX;
}

void ChartView::setValueCountX(int value)
{
    m_valueCountX = value;
    recalcLayout();
}

int ChartView::getValueCountY() const
{
    return m_valueCountY;
}

void ChartView::setValueCountY(int value)
{
    m_valueCountY = value;
    recalcLayout();
}

void ChartView::setYAxis(ChartSerie* pSerie)
{
    ASSERT(pSerie);
    m_pYAxis = pSerie;
    recalcLayout();
}

bool ChartView::isDrawLegend() const
{
    return m_needDrawLegend;
}

void ChartView::setDrawLegend(bool value)
{
    m_needDrawLegend = value;
}

long double ChartView::timeScale() const
{
    return m_timeScale;
}

int ChartView::chartShift() const
{
    return m_chartShift;
}

const Time& ChartView::drawFromX() const
{
    return m_drawFromX;
}

const Time& ChartView::drawToX() const
{
    return m_drawToX;
}

int ChartView::drawFromEventID() const
{
    return m_drawFromEventID;
}

int ChartView::drawToEventCount() const
{
    return m_drawToEventCount;
}

const ChartDoc::TimesList& ChartView::unwrapTimesList() const
{
    return m_unwrapTimesList;
}

const ChartViewStyle* const ChartView::style() const
{
    return m_pStyle;
}

void ChartView::recalcLayout()
{
    ChartDoc* pDoc = getDocument();

    QFontMetrics titleFontMetrics(m_fontTitle);
    QString str = pDoc->getTitle();
    QSize titleSize = !str.isEmpty()
        ? titleFontMetrics.size(Qt::TextSingleLine, str)
        : QSize(0, 0);
    m_chartRect.setTop(titleSize.height() + 5);

    QFontMetrics axisFontMetrics(m_fontAxis);
    QSize sizeMax(0, 0);
    if (m_pYAxis)
    {
        m_pYAxis->getCaptions(m_captionList, m_valueCountY);
        for (std::vector<std::string>::iterator it = m_captionList.begin(); it != m_captionList.end(); ++it)
        {
            QRect rect = axisFontMetrics.boundingRect(QString::fromStdString(*it));
            if (rect.width() > sizeMax.width())
                sizeMax.setWidth(rect.width());
        }
    }
    m_chartRect.setLeft(sizeMax.width() + 10);

    str = QString("%1").arg(
        !pDoc->getTimes().empty()
            ? pDoc->getTimes().back()->time
            : 0.0,
        0, 'f', 3
    );
    QSize size = axisFontMetrics.boundingRect(str).size();
    m_chartRect.setRight(m_clientRect.right() - size.width() - 5);
    m_chartRect.setBottom(m_clientRect.bottom() - size.height() - 7);

    if (m_needDrawLegend)
    {
        QFontMetrics legendFontMetrics(m_fontLegend);
        int count = 0;
        for (const ChartSerie* const pSerie: pDoc->getSerieList())
        {
            size = pSerie->getLegendSize(legendFontMetrics, m_chartRect);
            if (size.width() && size.height())
            {
                if (size.width() > sizeMax.width())
                    sizeMax.setWidth(size.width());

                if (size.height() > sizeMax.height())
                    sizeMax.setHeight(size.height());

                count++;
            }
        }

        m_legendRect.setTop(m_chartRect.top());
        m_legendRect.setBottom(m_legendRect.top() + count * sizeMax.height() + 3 + pDoc->getMaxMarkerSize() / 2);
        m_legendRect.setLeft(m_chartRect.left() + (m_chartRect.width() - sizeMax.width()) / 2);
        m_legendRect.setRight(m_legendRect.left() + sizeMax.width());
        m_chartRect.setTop(m_chartRect.top() + m_legendRect.height());
        if (m_legendRect.width() > m_chartRect.width())
        {
            m_legendRect.setLeft(m_chartRect.left());
            m_legendRect.setRight(m_chartRect.right());
        }
    }

    if (!pDoc->getTimes().empty())
    {
        double timeRange = pDoc->getTimes().back()->time - pDoc->getTimes().front()->time;
        if (timeRange > 0)
        {
            long double timeScaleAuto = doUnwrapTime() ? (double)(m_chartRect.width() - m_pStyle->pFontsTicks.tickWidth * pDoc->getTicksCount()) / timeRange : (double)m_chartRect.width() / timeRange;
            m_timeScale = (double)m_pStyle->pFontsTicks.tickWidth / pDoc->getMinTimeOffset();
            m_zoomAuto = double(timeScaleAuto) / double(m_timeScale);

            if (m_zoomAutoFlag || (m_zoomAuto > 1 /*&& scale_koeff < auto_zoom*/))
                m_scaleKoeff = m_zoomAuto;

            if (!m_zoomAutoFlag && m_zoomAuto <= 1)
                m_scaleKoeff = m_zoom;
        }
        else
        {
            m_timeScale = 0;
            m_scaleKoeff = 1;
        }
    }
    else
    {
        m_timeScale = 0;
        m_scaleKoeff = 1;
    }

    m_timeScale *= m_scaleKoeff;
}

QScrollBar& ChartView::getHorzScrollBar()
{
    QScrollBar* pScrollBar = static_cast<QAbstractScrollArea*>(parentWidget())->horizontalScrollBar();
    ASSERT(pScrollBar);
    return *pScrollBar;
}

void ChartView::updateScrollBars()
{
    ChartDoc* doc = getDocument();

    int size;
    if (!doc->getTimes().empty())
    {
        size = rdo::roundDouble((doc->getTimes().back()->time - doc->getTimes().front()->time) * double(m_timeScale));
        if (doUnwrapTime())
        {
            size += m_pStyle->pFontsTicks.tickWidth * doc->getTicksCount();
        }
    }
    else
    {
        size = 0;
    }

    m_SM_X.pageSize = m_chartRect.width();
    m_SM_X.posMax   = std::max(0, size - m_chartRect.width());
    m_SM_X.position = std::min(m_SM_X.position, m_SM_X.posMax);

    getHorzScrollBar().setRange   (0, m_SM_X.posMax);
    getHorzScrollBar().setPageStep(m_SM_X.pageSize);
    getHorzScrollBar().setValue   (m_SM_X.position);
}

bool ChartView::scrollHorizontally(int inc)
{
    if (!m_SM_X.applyInc(inc))
        return false;

    getHorzScrollBar().setValue(m_SM_X.position);
    update();
    return true;
}

void ChartView::onHorzScrollBarValueChanged(int value)
{
    if (value < 0)
        return;

    scrollHorizontally(value - m_SM_X.position);
}

void ChartView::keyPressEvent(QKeyEvent* pEvent)
{
    boost::optional<int> position;

    switch (pEvent->key())
    {
    case Qt::Key_Left    : position = std::max(0,             m_SM_X.position - (pEvent->modifiers().testFlag(Qt::ControlModifier) ? m_SM_X.pageSize : 1)); break;
    case Qt::Key_Right   : position = std::min(m_SM_X.posMax, m_SM_X.position + (pEvent->modifiers().testFlag(Qt::ControlModifier) ? m_SM_X.pageSize : 1)); break;
    case Qt::Key_PageUp  : position = std::max(0,             m_SM_X.position - m_SM_X.pageSize); break;
    case Qt::Key_PageDown: position = std::min(m_SM_X.posMax, m_SM_X.position + m_SM_X.pageSize); break;
    case Qt::Key_Home    : position = 0; break;
    case Qt::Key_End     : position = m_SM_X.posMax; break;
    }

    if (position.is_initialized())
        getHorzScrollBar().setValue(position.get());
}

void ChartView::wheelEvent(QWheelEvent*  pEvent)
{
    getHorzScrollBar().setValue(getHorzScrollBar().value() - m_SM_X.pageSize * (pEvent->delta() > 0 ? 1 : -1));
}

void ChartView::contextMenuEvent(QContextMenuEvent* pEvent)
{
    if (!m_previewMode)
        m_pPopupMenu->exec(pEvent->globalPos());
}

void ChartView::dragEnterEvent(QDragEnterEvent* pEvent)
{
    if (pEvent->mimeData()->hasFormat("ChartSerie"))
    {
        QByteArray serieData = pEvent->mimeData()->data("ChartSerie");
        m_pddSerie = (Serie*)serieData.toULongLong();
        if (!getDocument()->serieExists(m_pddSerie))
            pEvent->acceptProposedAction();
    }
}

void ChartView::dropEvent(QDropEvent* pEvent)
{
    getDocument()->addSerie(m_pddSerie);
    m_pddSerie = NULL;
    pEvent->acceptProposedAction();
}

void ChartView::dragLeaveEvent(QDragLeaveEvent*)
{
    m_pddSerie = NULL;
}

bool ChartView::setTo(const int fromMaxPos)
{
    bool res = true;
    int delta = (fromMaxPos - m_SM_X.position - m_chartRect.width()) / m_pStyle->pFontsTicks.tickWidth;
    if (delta >= 0)
    {
        res = false;
        m_drawToX = m_drawFromX;
        m_drawToEventCount = m_drawToX.eventCount - delta;
    }
    return res;
}

void ChartView::setFromTo()
{
    ChartDoc* doc = getDocument();

    m_drawFromX.eventCount = 0;
    m_drawFromEventID = 0;
    m_drawToX.eventCount = 0;
    m_drawToEventCount = 0;
    m_chartShift = 0;
    m_unwrapTimesList.clear();

    if (!doUnwrapTime())
    {
        if (m_timeScale)
        {
            m_drawFromX.time = doc->getTimes().front()->time + (double)m_SM_X.position / double(m_timeScale);
            if (maxXVisible())
            {
                m_drawToX.time = doc->getTimes().back()->time;
            }
            else
            {
                m_drawToX.time = doc->getTimes().front()->time + (double)(m_SM_X.position + m_chartRect.width()) / double(m_timeScale);
            }
        }
        else
        {
            m_drawFromX.time = 0;
            m_drawToX.time = 0;
        }
    }
    else
    {
        int it_pos = 0;
        int it_max_pos = 0;
        bool need_search_to = true;
        int ticks = 0;
        ChartDoc::TimesList::const_iterator it;
        for (it = doc->getTimes().begin(); it != doc->getTimes().end(); ++it)
        {
            it_pos = rdo::roundDouble(((*it)->time - doc->getTimes().front()->time) * double(m_timeScale)) + ticks * m_pStyle->pFontsTicks.tickWidth;
            it_max_pos = it_pos + m_pStyle->pFontsTicks.tickWidth * (*it)->eventCount;
            if (it_pos == m_SM_X.position)
            {
                m_drawFromX = *(*it);
                need_search_to = setTo(it_max_pos);
                break;
            }
            if (it_pos < m_SM_X.position && (it_max_pos >= m_SM_X.position))
            {
                m_drawFromX = *(*it);
                m_drawFromEventID = (m_SM_X.position - it_pos) / m_pStyle->pFontsTicks.tickWidth;
                m_chartShift = m_SM_X.position - (it_pos + m_drawFromEventID * m_pStyle->pFontsTicks.tickWidth);
                need_search_to = setTo(it_max_pos);
                break;
            }
            if (it_pos > m_SM_X.position)
            {
                m_drawFromX.time = (*it)->time - (it_pos - m_SM_X.position) / double(m_timeScale);
                need_search_to = setTo(0);
                break;
            }
            ticks += (*it)->eventCount;
        }
        m_unwrapTimesList.push_back(&m_drawFromX);
        if (need_search_to)
        {
            if (it != doc->getTimes().end() && m_drawFromX == *(*it))
            {
                ticks += (*it)->eventCount;
                it++;
            }
            if (it == doc->getTimes().end() && !doc->getTimes().empty())
            {
                m_drawToX = m_drawFromX;
                int delta = m_drawToX.eventCount * m_pStyle->pFontsTicks.tickWidth - m_chartRect.width();
                m_drawToEventCount = delta >= 0 ? rdo::roundDouble((double)delta / (double)m_pStyle->pFontsTicks.tickWidth) : m_drawToX.eventCount;
                it_max_pos = m_drawToX.eventCount * m_pStyle->pFontsTicks.tickWidth;
                if (it_max_pos > m_chartRect.width())
                {
                    m_drawToEventCount = (it_max_pos - m_chartRect.width()) / m_pStyle->pFontsTicks.tickWidth;
                    if (m_drawToEventCount * m_pStyle->pFontsTicks.tickWidth < m_chartRect.width())
                        m_drawToEventCount++;
                }
                else
                {
                    m_drawToEventCount = m_drawToX.eventCount;
                }

            }
            int pos = m_SM_X.position + m_chartRect.width();
            for (; it != doc->getTimes().end(); ++it)
            {
                it_pos = rdo::roundDouble(((*it)->time - doc->getTimes().front()->time) * double(m_timeScale)) + ticks * m_pStyle->pFontsTicks.tickWidth;
                it_max_pos = it_pos + m_pStyle->pFontsTicks.tickWidth * (*it)->eventCount;
                if (it_pos == pos)
                {
                    m_drawToX = *(*it);
                    break;
                }
                if (it_pos < pos && it_max_pos >= pos)
                {
                    m_drawToX = *(*it);
                    m_drawToEventCount = (pos - it_pos) / m_pStyle->pFontsTicks.tickWidth;
                    if (it_pos + m_drawToEventCount * m_pStyle->pFontsTicks.tickWidth < pos)
                    {
                        m_drawToEventCount++;
                    }
                    break;
                }
                if (it_pos > pos)
                {
                    m_drawToX.time = (*it)->time - (it_pos - pos) / double(m_timeScale);
                    break;
                }
                m_unwrapTimesList.push_back((*it));
                ticks += (*it)->eventCount;
            }
        }
        m_unwrapTimesList.push_back(&m_drawToX);
    }
}

void ChartView::drawTitle(QPainter& painter, const QRect& chartRect)
{
    QRect rect(m_clientRect);
    rect.setTop(0);
    rect.setBottom(chartRect.top());

    painter.setFont(m_fontTitle);
    painter.setPen(m_pStyle->titleFGColor);
    painter.drawText(rect, Qt::AlignHCenter | Qt::AlignTop, getDocument()->getTitle());
}

void ChartView::drawLegend(QPainter& painter, const QRect& legendRect)
{
    ChartDoc* doc = getDocument();
    QRect rect(legendRect);
    painter.setFont(m_fontLegend);
    for (const ChartSerie* const pSerie: doc->getSerieList())
    {
        QSize size = pSerie->drawLegend(painter, rect, m_pStyle->legendFgColor);
        rect.setTop(rect.top() + size.height());
    }
}

void ChartView::drawYAxis(QPainter& painter, const QRect& chartRect, const ChartSerie* axisValues)
{
    QRect tmprect(m_clientRect);
    tmprect.setRight(chartRect.left() - 5);
    tmprect.setLeft(5);
    if (axisValues)
    {
        int count;
        axisValues->getSerie()->getValueCount(count);
        if (count)
        {
            painter.setFont(m_fontAxis);
            painter.setPen(m_pStyle->axisFgColor);

            int count = m_captionList.size();
            int heightoffset = rdo::roundDouble((double)chartRect.height() / (double)(count - 1));
            tmprect.setTop(chartRect.bottom());
            int index = 0;
            for (std::vector<std::string>::iterator it = m_captionList.begin(); it != m_captionList.end(); ++it)
            {
                index++;
                painter.drawText(tmprect, Qt::AlignRight, QString::fromStdString(*it));
                if (index != 1 && index < count)
                {
                    QPainterPath path;
                    path.moveTo(tmprect.right() + 2, tmprect.top());
                    path.lineTo(chartRect.left(), tmprect.top());
                    painter.drawPath(path);
                }

                if (index == count - 1)
                    tmprect.setTop(chartRect.top());
                else
                    tmprect.setTop(tmprect.top() - heightoffset);
            }
        }
    }
}

void ChartView::drawXAxis(QPainter& painter, const QRect& chartRect)
{
    QRect tmprect(chartRect);
    tmprect.setTop(chartRect.bottom() + 3);
    tmprect.setLeft(chartRect.left());
    tmprect.setBottom(m_clientRect.bottom());
    tmprect.setRight(m_clientRect.right() - 5);

    ChartDoc* pDoc = getDocument();
    if (!pDoc->getTimes().empty())
    {
        std::string formatstr = "%.3f";

        painter.setFont(m_fontAxis);
        painter.setPen(m_pStyle->axisFgColor);

        if (!doUnwrapTime())
        {
            double valoffset = 0;
            int widthoffset = chartRect.width() / (m_valueCountX - 1);
            if (m_drawToX != m_drawFromX)
                valoffset = (m_drawToX.time - m_drawFromX.time) / (m_valueCountX - 1);

            double valo = m_drawFromX.time;
            int x = chartRect.left();
            std::string str = rdo::format(formatstr.c_str(), valo);
            tmprect.setLeft(x);
            painter.drawText(tmprect, Qt::AlignLeft, QString::fromStdString(str));
            valo += valoffset;
            x += widthoffset;
            if (valoffset)
            {
                for (int i = 1; i < m_valueCountX; i++)
                {
                    str = rdo::format(formatstr.c_str(), valo);
                    tmprect.setLeft(x);
                    painter.setPen(m_pStyle->axisFgColor);
                    painter.drawText(tmprect, Qt::AlignLeft, QString::fromStdString(str));
                    if (i != m_valueCountX - 1)
                    {
                        QPainterPath path;
                        path.moveTo(x, chartRect.bottom());
                        path.lineTo(x, chartRect.bottom() + 3);
                        painter.setPen(m_pStyle->defaultColor);
                        painter.drawPath(path);
                    }
                    valo += valoffset;
                    x += widthoffset;
                }
            }
        }
        else
        {
            int ticks = 0;
            std::string str;
            int lastx = 0;
            QSize sz;
            for (const Time* const pTime: m_unwrapTimesList)
            {
                tmprect.setLeft(chartRect.left() + (int)((pTime->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks.tickWidth - m_chartShift));
                tmprect.setLeft(std::min(tmprect.left(), chartRect.right() - 1));
                str = rdo::format(formatstr.c_str(), pTime->time);
                if (*pTime == m_drawFromX)
                    tmprect.setLeft(tmprect.left() + m_chartShift);

                if (tmprect.left() > chartRect.right())
                    tmprect.setLeft(chartRect.right());

                if (tmprect.left() > lastx)
                {
                    painter.setPen(m_pStyle->axisFgColor);
                    painter.drawText(tmprect, Qt::AlignLeft, QString::fromStdString(str));
                    if (tmprect.left() != chartRect.left() && tmprect.left() != chartRect.right())
                    {
                        QPainterPath path;
                        path.moveTo(tmprect.left(), chartRect.bottom());
                        path.lineTo(tmprect.left(), chartRect.bottom() + 3);
                        painter.setPen(m_pStyle->defaultColor);
                        painter.drawPath(path);
                    }
                    QSize size = painter.fontMetrics().boundingRect(QString::fromStdString(str)).size();
                    lastx = tmprect.left() + size.width();
                }

                ticks += pTime->eventCount;
                if (*pTime == m_drawFromX)
                    ticks -= m_drawFromEventID;
            }
        }
    }
}

void ChartView::drawGrid(QPainter& painter, const QRect& chartRect)
{
    QRect rect(chartRect);
    rect.adjust(0, 0, -1, -1);
    painter.setPen  (m_pStyle->defaultColor);
    painter.setBrush(m_pStyle->chartBgColor);
    painter.drawRect(rect);

    if (doUnwrapTime())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_pStyle->timeBgColor);

        rect.adjust(1, 1, 0, 0);
        QRect wrapRect(rect);

        int ticks = 0;
        ChartDoc::TimesList::const_iterator it = m_unwrapTimesList.begin();
        if (m_drawFromX == m_drawToX)
            ++it;

        for (; it != m_unwrapTimesList.end(); ++it)
        {
            int width = (*it)->eventCount * m_pStyle->pFontsTicks.tickWidth;

            wrapRect.setLeft(std::max(
                rect.left() + (int)(((*it)->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks.tickWidth - m_chartShift),
                rect.left()
            ));

            if (*(*it) == m_drawFromX)
                width -= m_drawFromEventID * m_pStyle->pFontsTicks.tickWidth + m_chartShift;

            if (*(*it) == m_drawToX)
                width = m_drawToEventCount * m_pStyle->pFontsTicks.tickWidth;

            wrapRect.setRight(std::min(
                wrapRect.left() + width,
                rect.right()
            ));

            if (wrapRect.left() < wrapRect.right())
                painter.drawRect(wrapRect);

            ticks += (*it)->eventCount;
            if (*(*it) == m_drawFromX)
                ticks -= m_drawFromEventID;
        }
    }
}

void ChartView::setZoom(double new_zoom, const bool /*force_update*/)
{
    if (doUnwrapTime() && new_zoom < 1)
        new_zoom = 1;
    else if (!doUnwrapTime() && new_zoom < m_zoomAuto)
        new_zoom = m_zoomAuto;

    m_zoom = new_zoom;
    m_scaleKoeff = m_zoom;
    recalcLayout();
    updateScrollBars();
    update();
}

ChartDoc* ChartView::getDocument()
{
    return m_pDocument;
}

void ChartView::resizeEvent(QResizeEvent* pEvent)
{
    super::resizeEvent(pEvent);

    m_clientRect = QRect(QPoint(0, 0), pEvent->size());
    if (getDocument())
    {
        recalcLayout();
        updateScrollBars();
        //setZoom(zoom);
    }
}

void ChartView::onChartTimeWrap()
{
    m_timeWrapFlag = !m_timeWrapFlag;
    recalcLayout();
    updateScrollBars();
    update();
    onUpdateActions(isActivated());
}

void ChartView::onUserUpdateChartView(ChartDoc::Update updateType)
{
    if (doUnwrapTime() || updateType != ChartDoc::Update::TIMES)
        updateView();
}

void ChartView::onEditCopy()
{
    QApplication::clipboard()->setPixmap(grab());
}

void ChartView::onViewZoomIn()
{
    if (m_zoomAutoFlag)
        m_zoomAutoFlag = !m_zoomAutoFlag;

    const double delta = m_zoom < 1 ? 0.1 : 0.5;

    if (m_zoom + delta > 1 && m_zoom + delta - 1 < delta)
        setZoom(1);
    else
        setZoom(m_zoom + delta);

    onUpdateActions(isActivated());
}

void ChartView::onViewZoomOut()
{
    if (m_zoomAutoFlag)
        m_zoomAutoFlag = !m_zoomAutoFlag;

    double delta = m_zoom > 1 ? -0.5 : -0.1;
    double zoom_new = m_zoom + delta;
    setZoom(zoom_new);
    onUpdateActions(isActivated());
}

void ChartView::onViewZoomReset()
{
    if (m_zoomAutoFlag)
        m_zoomAutoFlag = !m_zoomAutoFlag;

    setZoom(1);
    onUpdateActions(isActivated());
}

const ChartViewStyle& ChartView::getStyle() const
{
    return (*m_pStyle);
}

void ChartView::setFonts(const bool /*needRedraw*/)
{
    if (!m_pStyle)
        return;

    m_fontAxis = QFont(m_pStyle->font.name.c_str());
    m_fontAxis.setBold     (static_cast<int>(m_pStyle->defaultStyle) & static_cast<int>(StyleFont::Style::BOLD)      ? true : false);
    m_fontAxis.setItalic   (static_cast<int>(m_pStyle->defaultStyle) & static_cast<int>(StyleFont::Style::ITALIC)    ? true : false);
    m_fontAxis.setUnderline(static_cast<int>(m_pStyle->defaultStyle) & static_cast<int>(StyleFont::Style::UNDERLINE) ? true : false);
    m_fontAxis.setPointSize(m_pStyle->font.size);

    m_fontTitle = QFont(m_pStyle->font.name.c_str());
    m_fontTitle.setBold     (static_cast<int>(m_pStyle->titleStyle) & static_cast<int>(StyleFont::Style::BOLD)      ? true : false);
    m_fontTitle.setItalic   (static_cast<int>(m_pStyle->titleStyle) & static_cast<int>(StyleFont::Style::ITALIC)    ? true : false);
    m_fontTitle.setUnderline(static_cast<int>(m_pStyle->titleStyle) & static_cast<int>(StyleFont::Style::UNDERLINE) ? true : false);
    m_fontTitle.setPointSize(m_pStyle->pFontsTicks.titleFontSize);

    m_fontLegend = QFont(m_pStyle->font.name.c_str());
    m_fontLegend.setBold     (static_cast<int>(m_pStyle->legendStyle) & static_cast<int>(StyleFont::Style::BOLD)      ? true : false);
    m_fontLegend.setItalic   (static_cast<int>(m_pStyle->legendStyle) & static_cast<int>(StyleFont::Style::ITALIC)    ? true : false);
    m_fontLegend.setUnderline(static_cast<int>(m_pStyle->legendStyle) & static_cast<int>(StyleFont::Style::UNDERLINE) ? true : false);
    m_fontLegend.setPointSize(m_pStyle->pFontsTicks.legendFontSize);
}

void ChartView::setStyle(ChartViewStyle* pStyle, const bool needRedraw)
{
    m_pStyle = pStyle;

    setFonts(false);

    if (needRedraw)
    {
        recalcLayout();
        updateScrollBars();
        update();
    }
}

void ChartView::setPreviwMode(bool value)
{
    m_previewMode = value;
}

void ChartView::onChartOptions()
{
    ChartPreferences dlg(this);
    dlg.exec();
}

void ChartView::updateView()
{
    bool lastvisible = maxXVisible();
    recalcLayout();
    updateScrollBars();
    if (lastvisible && !maxXVisible())
        getHorzScrollBar().setValue(m_SM_X.posMax);

    parentWidget()->update();
    updateScrollBars();
    onUpdateActions(isActivated());
}

void ChartView::attachToDoc()
{
    getDocument()->addToViews(this);
}

void ChartView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    ChartDoc* doc = getDocument();

    painter.fillRect(m_clientRect, m_pStyle->backgroundColor);

    drawTitle(painter, m_chartRect);

    if (!m_chartRect.isEmpty())
    {
        if (m_needDrawLegend)
            drawLegend(painter, m_legendRect);

        painter.setPen(m_pStyle->defaultColor);

        drawYAxis(painter, m_chartRect, m_pYAxis);
        setFromTo();
        drawXAxis(painter, m_chartRect);
        drawGrid(painter, m_chartRect);

        for (const ChartSerie* const pSerie: doc->getSerieList())
            pSerie->drawSerie(this, painter, m_chartRect);
    }
}

void ChartView::onViewZoomAuto()
{
    m_zoomAutoFlag = !m_zoomAutoFlag;
    if (!m_zoomAutoFlag)
    {
        setZoom(m_zoomOld, true);
    }
    else
    {
        m_zoomOld = m_zoom;
        setZoom(m_zoomAuto, true);
    }
    onUpdateActions(isActivated());
}

void ChartView::onHelpKeyword()
{
    QByteArray ba;
    ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
    g_pApp->callQtAssistant(ba);
}

bool ChartView::canUnwrapTime() const
{
    return m_scaleKoeff >= 1 && !m_zoomAutoFlag;
}

bool ChartView::doUnwrapTime() const
{
    return canUnwrapTime() && !m_timeWrapFlag;
}

bool ChartView::minXVisible() const
{
    return m_SM_X.position == 0;
}

bool ChartView::maxXVisible() const
{
    return m_SM_X.position == m_SM_X.posMax;
}

void ChartView::onUpdateActions(bool activated)
{
    MainWindow* pMainWindow = g_pApp->getMainWndUI();
    ASSERT(pMainWindow);

    updateAction(
        pMainWindow->actViewZoomInc,
        activated && !m_zoomAutoFlag && m_zoom < 5,
        this, &ChartView::onViewZoomIn
    );

    updateAction(
        pMainWindow->actViewZoomDec,
        activated && !m_zoomAutoFlag && ((!doUnwrapTime() && m_zoom > m_zoomAuto) || (doUnwrapTime() && m_zoom > 1)),
        this, &ChartView::onViewZoomOut
    );

    updateAction(
        pMainWindow->actViewZoomReset,
        activated && m_zoom != 1,
        this, &ChartView::onViewZoomReset
    );

    pMainWindow->actViewZoomAuto->setCheckable(true);
    pMainWindow->actViewZoomAuto->setChecked(m_zoomAutoFlag);

    updateAction(
        pMainWindow->actViewZoomAuto,
        activated,
        this, &ChartView::onViewZoomAuto
    );

    pMainWindow->actChartTimeWrap->setCheckable(true);
    pMainWindow->actChartTimeWrap->setChecked(!canUnwrapTime() || m_timeWrapFlag);

    updateAction(
        pMainWindow->actChartTimeWrap,
        activated && canUnwrapTime(),
        this, &ChartView::onChartTimeWrap
    );

    updateAction(
        pMainWindow->actChartOptions,
        activated,
        this, &ChartView::onChartOptions
    );

    updateAction(
        pMainWindow->actEditCopy,
        activated,
        this, &ChartView::onEditCopy
    );

    updateAction(
        pMainWindow->actHelpContext,
        activated,
        this, &ChartView::onHelpKeyword
    );
}

// --------------------------------------------------------------------------------
// -------------------- ChartViewMainWnd
// --------------------------------------------------------------------------------
ChartViewMainWnd::ChartViewMainWnd(QWidget* pParent, ChartDoc* pDocument, bool preview)
    : super(pParent)
{
    setMinimumSize(400, 200);

    ChartView* pContext = new ChartView(this, pDocument, preview);
    pContext->show();
    setViewport(pContext);
}

ChartViewMainWnd::~ChartViewMainWnd()
{}

ChartView& ChartViewMainWnd::view()
{
    return *static_cast<ChartView*>(viewport());
}

bool ChartViewMainWnd::viewportEvent(QEvent* /*pEvent*/)
{
    return false;
}

void ChartViewMainWnd::focusInEvent(QFocusEvent* pEvent)
{
    static_cast<QObject*>(viewport())->event(pEvent);
}

void ChartViewMainWnd::focusOutEvent(QFocusEvent* pEvent)
{
    static_cast<QObject*>(viewport())->event(pEvent);
}

void ChartViewMainWnd::keyPressEvent(QKeyEvent* pEvent)
{
    switch (pEvent->key())
    {
    case Qt::Key_Left    :
    case Qt::Key_Right   :
    case Qt::Key_PageUp  :
    case Qt::Key_PageDown:
    case Qt::Key_Home    :
    case Qt::Key_End     :
        static_cast<QObject*>(viewport())->event(pEvent);
        break;

    default:
        super::keyPressEvent(pEvent);
        break;
    }
}
