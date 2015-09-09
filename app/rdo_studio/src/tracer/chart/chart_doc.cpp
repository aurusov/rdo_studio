// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
#include "app/rdo_studio/src/tracer/chart/chart_view.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/tracer/tracer_serie.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

std::size_t ChartDoc::s_titleIndex = 0;

ChartDoc::ChartDoc(const bool preview)
    : m_minTimeOffset(1.7E+308)
    , m_ticksCount(0)
    , m_previewMode(preview)
{
    if (!m_previewMode)
    {
        g_pTracer->addChart(this);
    }
}

ChartDoc::~ChartDoc()
{
    for (std::vector<ChartSerie*>::iterator it = m_serieList.begin(); it != m_serieList.end(); it++)
    {
        (*it)->getSerie()->removeFromDoc(this);
    }
    if (!m_previewMode)
    {
        g_pTracer->removeChart(this);
    }
}

const ChartDoc::TimesList& ChartDoc::getTimes() const
{
    return m_docTimes;
}

const ChartDoc::SerieList& ChartDoc::getSerieList() const
{
    return m_serieList;
}

void ChartDoc::attachView(ChartView* pView)
{
    m_viewList.push_back(pView);
}

ChartView* ChartDoc::getFirstView()
{
    return m_viewList.empty() ? NULL : m_viewList.front();
}

void ChartDoc::setStyle(ChartViewStyle* pStyle)
{
    ASSERT(pStyle);
    boost::range::for_each(m_viewList, boost::bind(&ChartView::setStyle, _1, pStyle, true));
}

void ChartDoc::updateAllViews()
{
    boost::range::for_each(m_viewList, boost::bind(&ChartView::updateView, _1));
}

int ChartDoc::getSerieIndex(ChartSerie* serie) const
{
    int res = -1;
    int index = 0;
    for (std::vector<ChartSerie*>::const_iterator it = m_serieList.begin(); it != m_serieList.end(); it++)
    {
        if (serie == (*it))
        {
            res = index;
        }
        index++;
    }
    return res;
}

void ChartDoc::incTimeEventsCount(Time* time)
{
    if (!m_docTimes.empty() && m_docTimes.back() == time)
    {
        m_ticksCount++;
        updateChartViews(Update::TIMES);
    }
}

bool ChartDoc::newValueToSerieAdded(Value* val)
{
    if (m_docTimes.empty())
    {
        m_docTimes.push_back(val->getModelTime());
        m_ticksCount += val->getModelTime()->eventCount;
    }
    else
    {
        Time* last = m_docTimes.back();
        if (last != val->getModelTime())
        {
            m_docTimes.push_back(val->getModelTime());
            m_ticksCount += val->getModelTime()->eventCount;
            double off = val->getModelTime()->time - last->time;
            if (off < m_minTimeOffset)
            {
                m_minTimeOffset = off;
            }
        }
    }
    updateChartViews(Update::NEW_VALUE);

    return true;
}

int ChartDoc::getMaxMarkerSize() const
{
    int res = 0;
    for (std::vector<ChartSerie*>::const_iterator it = m_serieList.begin(); it != m_serieList.end(); it++)
    {
        if ((*it)->options().markerNeedDraw && (*it)->options().markerSize > res)
        {
            res = (*it)->options().markerSize;
        }
    }

    return res;
}

int ChartDoc::getTicksCount() const
{
    return m_ticksCount;
}

double ChartDoc::getMinTimeOffset() const
{
    return m_minTimeOffset;
}

void ChartDoc::addToViews(ChartView* pWidget)
{
    m_widgetList.push_back(pWidget);
}

void ChartDoc::removeFromViews(ChartView* pWidget)
{
    std::vector<ChartView*>::iterator it = std::find(m_widgetList.begin(), m_widgetList.end(), pWidget);
    if (it != m_widgetList.end())
    {
        m_widgetList.erase(it);
    }
}

void ChartDoc::updateChartViews(Update updateType) const
{
    boost::range::for_each(m_widgetList, boost::bind(&ChartView::onUserUpdateChartView, _1, updateType));
}

void ChartDoc::addSerie(const LPSerie& pSerie)
{
    if (pSerie && !serieExists(pSerie))
    {
        ChartSerie* pDocSerie = new ChartSerie(pSerie);
        ChartSerie::Options options(pDocSerie->options());
        options.color = selectColor();
        options.markerType = selectMarker();
        pDocSerie->setOptions(options);
        m_serieList.push_back(pDocSerie);
        m_insertedIt = m_docTimes.begin();

        if (!m_docTimes.empty() && !pSerie->empty())
        {
            TimesList::iterator lastDocIt = m_docTimes.end();
            --lastDocIt;
            Serie::ValuesList::const_iterator first_serie = pSerie->begin();
            if ((*first_serie)->getModelTime()->time >= (*lastDocIt)->time)
            {
                m_insertedIt = m_docTimes.end();
                if ((*first_serie)->getModelTime()->time == (*lastDocIt)->time)
                {
                    m_insertedIt = lastDocIt;
                }
            }
        }

        try
        {
            // TODO qt
            //g_pApp->BeginWaitCursor();

            std::for_each(pSerie->begin(), pSerie->end(), boost::bind(&ChartDoc::insertValue, this, _1));

            // TODO qt
            //g_pApp->EndWaitCursor();
        }
        catch (...)
        {
            // TODO qt
            //g_pApp->EndWaitCursor();
        }

        pSerie->addToDoc(this);
        if (m_serieList.size() == 1)
        {
            ChartView* pView = getFirstView();
            ASSERT(pView);
            pView->setYAxis(pDocSerie);
        }
        updateChartViews(Update::NEW_SERIE);
    }
}

QColor ChartDoc::selectColor() const
{
    int count = m_serieList.size();
    int mul = count / 15;
    int index = count - mul * 16;
    QColor res = QColor(0, 0, 0);
    switch (index)
    {
    case 0:
        res = QColor(0x00, 0x80, 0x00);
        break;
    case 1:
        res = QColor(0x00, 0x00, 0x80);
        break;
    case 2:
        res = QColor(0x80, 0x80, 0x80);
        break;
    case 3:
        res = QColor(0x80, 0x00, 0x80);
        break;
    case 4:
        res = QColor(0xFF, 0x00, 0x00);
        break;
    case 5:
        res = QColor(0x00, 0xFF, 0x00);
        break;
    case 6:
        res = QColor(0x00, 0x00, 0x00);
        break;
    case 7:
        res = QColor(0x80, 0x80, 0x00);
        break;
    case 8:
        res = QColor(0xC0, 0xC0, 0xC0);
        break;
    case 9:
        res = QColor(0x80, 0x00, 0x00);
        break;
    case 10:
        res = QColor(0x00, 0x80, 0x80);
        break;
    case 11:
        res = QColor(0xFF, 0xFF, 0x00);
        break;
    case 12:
        res = QColor(0x00, 0x00, 0xFF);
        break;
    case 13:
        res = QColor(0xFF, 0x00, 0xFF);
        break;
    case 14:
        res = QColor(0x00, 0xFF, 0xFF);
        break;
    };
    return res;
}

Serie::Marker ChartDoc::selectMarker()
{
    int count = m_serieList.size();
    int mul = count / 4;
    int index = count - mul * 4;
    Serie::Marker res = Serie::Marker::CIRCLE;
    switch (index)
    {
    case 0:
        res = Serie::Marker::CIRCLE;
        break;
    case 1:
        res = Serie::Marker::SQUARE;
        break;
    case 2:
        res = Serie::Marker::TRIANG;
        break;
    case 3:
        res = Serie::Marker::CROSS;
        break;
    };
    return res;
}

bool ChartDoc::serieExists(const LPSerie& pSerie) const
{
    return boost::range::find_if(m_serieList, boost::bind(&ChartSerie::isTracerSerie, _1, pSerie)) != m_serieList.end();
}

const QString& ChartDoc::getTitle() const
{
    return m_title;
}

void ChartDoc::setTitle(const QString& title)
{
    m_title = title;
    getFirstView()->parentWidget()->setWindowTitle(QString("график: %1").arg(m_title));
}

void ChartDoc::autoTitle()
{
    QString title = QString("График%1").arg(++s_titleIndex);
    setTitle(title);
}

void ChartDoc::resetTitleIndex()
{
    s_titleIndex = 0;
}

void ChartDoc::insertValue(Value* pValue)
{
    if (pValue)
    {
        ChartDoc::TimesList::iterator it = std::find_if(m_insertedIt, m_docTimes.end(), boost::bind(&Time::compareTimes, _1, pValue->getModelTime()));
        if (it == m_docTimes.end() || (*it) != pValue->getModelTime())
        {
            m_insertedIt = m_docTimes.insert(it, pValue->getModelTime());
            m_ticksCount += pValue->getModelTime()->eventCount;
            double offl = 1.7E+308;
            double offr = 1.7E+308;
            if (it != m_docTimes.end())
            {
                offr = (*it)->time - (*m_insertedIt)->time;
            }
            if (m_insertedIt != m_docTimes.begin())
            {
                offl = (*m_insertedIt)->time;
                TimesList::iterator prev_it = m_insertedIt;
                prev_it--;
                offl -= (*prev_it)->time;
            }
            double minoff = std::min(offl, offr);
            if (minoff < m_minTimeOffset)
            {
                m_minTimeOffset = minoff;
            }
        }
    }
}
