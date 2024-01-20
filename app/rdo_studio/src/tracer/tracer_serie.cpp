// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <boost/bind.hpp>
#include <QPainterPath>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
#include "app/rdo_studio/src/tracer/tracer_resource.h"
#include "app/rdo_studio/src/tracer/tracer_resource_type.h"
#include "app/rdo_studio/src/tracer/chart/chart_view.h"
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- TracerSerieFindValue
// --------------------------------------------------------------------------------
class TracerSerieFindValue
{
public:
    TracerSerieFindValue(ChartView* pView)
        : m_pView(pView)
    {}

    bool operator() (Value* pValue);

private:
    ChartView* m_pView;
};

bool TracerSerieFindValue::operator() (Value* pValue)
{
    bool res = pValue && pValue->getModelTime()->time >= m_pView->drawFromX().time;
    if (m_pView->doUnwrapTime() && res && (pValue->getModelTime()->time == m_pView->drawFromX().time))
    {
        res = pValue->getEventID() >= m_pView->drawFromEventID();
    }
    return res;
}

// --------------------------------------------------------------------------------
// -------------------- TracerSerie
// --------------------------------------------------------------------------------
Serie::Serie(Kind kind)
    : ChartTreeItem(true)
    , m_minValue(0)
    , m_maxValue(0)
    , m_kind(kind)
    , m_valueCount(0)
{}

Serie::~Serie()
{
    ValuesList::iterator it = m_valueList.begin();
    while (it != m_valueList.end())
    {
        delete *it;
        it++;
    }
    m_valueList.clear();
    m_documentList.clear();
}

const QString& Serie::getTitle() const
{
    return m_title;
}

void Serie::setTitle(const QString& value)
{
    m_title = value;
}

Serie::Kind Serie::getKind() const
{
    return m_kind;
}

bool Serie::isTemporaryResourceParam() const
{
    return m_kind == Kind::PARAM && ((Param*)this)->getResource()->getType()->getKind() == ResourceType::Kind::TEMPORARY;
}

void Serie::addValue(Value* const pValue)
{
    DocumentList::iterator it;

    if (pValue->getValue() < m_minValue || m_valueList.empty())
    {
        m_minValue = pValue->getValue();
    }
    if (pValue->getValue() > m_maxValue || m_valueList.empty())
    {
        m_maxValue = pValue->getValue();
    }

    m_valueList.push_back(pValue);

    m_valueCount++;

    std::for_each(m_documentList.begin(), m_documentList.end(), boost::bind(&ChartDoc::newValueToSerieAdded, _1, pValue));
}

void Serie::getValueCount(int& count) const
{
    count = m_valueCount;
}

bool Serie::empty() const
{
    return m_valueList.empty();
}

Serie::ValuesList::const_iterator Serie::begin() const
{
    return m_valueList.begin();
}

Serie::ValuesList::const_iterator Serie::end() const
{
    return m_valueList.end();
}

void Serie::getCaptions(std::vector<std::string>& captions, const int valueCount) const
{
    if (!captions.empty())
        captions.clear();

    if (m_kind == Serie::Kind::PREVIEW)
    {
        double valoffset = (m_maxValue - m_minValue) / (double)(valueCount - 1);
        double valo = m_minValue;
        std::string formatstr = "%.3f";
        if (m_valueCount > 1)
        {
            for (int i = 0; i < valueCount; i++)
            {
                captions.push_back(rdo::format(formatstr.c_str(), valo));
                valo += valoffset;
            }
        }
        else
        {
            captions.push_back(rdo::format(formatstr.c_str(), valo));
        }
    }
}

void Serie::getCaptionsInt(std::vector<std::string>& captions, const int valueCount) const
{
    Serie::getCaptions(captions, valueCount);

    int real_val_count = valueCount;
    if ((m_maxValue - m_minValue + 1) > real_val_count)
    {
        while ((int)((m_maxValue - m_minValue) / (real_val_count - 1)) != (double)((m_maxValue - m_minValue) / (real_val_count - 1)))
            real_val_count--;
    }
    else
    {
        real_val_count = (int)(m_maxValue - m_minValue + 1);
    }
    int valo = (int)m_minValue;
    int valoffset = (int)((m_maxValue - m_minValue) / (real_val_count - 1));
    std::string formatstr = "%d";
    for (int i = 0; i < real_val_count; i++)
    {
        captions.push_back(rdo::format(formatstr.c_str(), valo));
        valo += valoffset;
    }
}

void Serie::getCaptionsDouble(std::vector<std::string>& captions, const int valueCount) const
{
    Serie::getCaptions(captions, valueCount);

    double valoffset = (m_maxValue - m_minValue) / (double)(valueCount - 1);
    double valo = m_minValue;
    std::string formatstr = "%.3f";
    if (m_valueCount > 1)
    {
        for (int i = 0; i < valueCount; i++)
        {
            captions.push_back(rdo::format(formatstr.c_str(), valo));
            valo += valoffset;
        }
    }
    else
    {
        captions.push_back(rdo::format(formatstr.c_str(), valo));
    }
}

void Serie::getCaptionsBool(std::vector<std::string>& captions, const int valueCount) const
{
    Serie::getCaptions(captions, valueCount);
    captions.push_back("FALSE");
    captions.push_back("TRUE");
}

void Serie::getLastValue(Value*& pValue) const
{
    if (!m_valueList.size())
        pValue = NULL;
    else
        pValue = m_valueList.back();
}

void Serie::drawSerie(ChartView* const pView,
                            QPainter& painter,
                            const QRect& rect,
                            const QColor& color,
                            Marker marker,
                            const int markerSize,
                            const bool draw_marker,
                            const bool transparent_marker) const
{
    if (!m_valueList.empty())
    {
        painter.setPen(color);
        painter.setBrush(QBrush(color, transparent_marker ? Qt::NoBrush : Qt::SolidPattern));

        ValuesList::const_iterator it = std::find_if(m_valueList.begin(), m_valueList.end(), TracerSerieFindValue(pView));

        if (it == m_valueList.end() && !m_valueList.empty() && !isTemporaryResourceParam())
        {
            --it;
        }

        bool flag = it != m_valueList.end();
        if (flag && !pView->doUnwrapTime())
        {
            flag = !(it == m_valueList.begin() && (*it)->getModelTime()->time > pView->drawToX().time);
        }
        else if (flag)
        {
            flag = !(it == m_valueList.begin() && ((*it)->getModelTime()->time > pView->drawToX().time || ((*it)->getModelTime()->time == pView->drawToX().time && (*it)->getEventID() > pView->drawToEventCount())));
        }

        if (flag)
        {

            long double ky;
            if (m_maxValue != m_minValue)
                ky = rect.height() / (m_maxValue - m_minValue);
            else
                ky = 0;

            flag = it != m_valueList.begin();
            if (flag && !pView->doUnwrapTime())
            {
                flag = (*it)->getModelTime()->time > pView->drawFromX().time;
            }
            else if (flag)
            {
                flag = (*it)->getModelTime()->time > pView->drawFromX().time || ((*it)->getModelTime()->time == pView->drawFromX().time && (*it)->getEventID() > pView->drawFromEventID());
            }
            if (flag)
                --it;

            int lastY = rdo::roundDouble((double)rect.bottom() - double(ky) * ((*it)->getValue() - m_minValue));
            lastY = std::max(lastY, rect.top());
            lastY = std::min(lastY, rect.bottom());
            int lastX = rect.left() + rdo::roundDouble(((*it)->getModelTime()->time - pView->drawFromX().time) * double(pView->timeScale())) - pView->chartShift();
            lastX = std::min(lastX, rect.right());

            int ticks = 0;
            ChartDoc::TimesList::const_iterator timeIt = pView->unwrapTimesList().begin();
            if (pView->doUnwrapTime() && (*it)->getModelTime()->time >= pView->drawFromX().time)
            {
                if (*(*it)->getModelTime() == *(*timeIt))
                {
                    lastX += ((*it)->getEventID() - pView->drawFromEventID()) * pView->style()->pFontsTicks.tickWidth;
                }
                else
                {
                    while (timeIt != pView->unwrapTimesList().end() && *(*it)->getModelTime() != *(*timeIt))
                    {
                        ticks += (*timeIt)->eventCount;
                        ++timeIt;
                    }
                    lastX += (ticks + (*it)->getEventID() - pView->drawFromEventID()) * pView->style()->pFontsTicks.tickWidth;
                }
            }
            lastX = std::min(lastX, rect.right());

            QPainterPath path;
            if (lastX >= rect.left() && draw_marker)
            {
                drawMarker(painter, lastX, lastY, marker, markerSize);
                path.moveTo(lastX, lastY);
            }
            else
                path.moveTo(rect.left(), lastY);

            int x = lastX, y = lastY;
            if (pView->doUnwrapTime())
            {
                ticks -= pView->drawFromEventID();
            }
            ++it;
            if (pView->doUnwrapTime() && it != m_valueList.end())
            {
                while (timeIt != pView->unwrapTimesList().end() && *(*it)->getModelTime() != *(*timeIt))
                {
                    ticks += (*timeIt)->eventCount;
                    ++timeIt;
                }
            }

            while (it != m_valueList.end()
                    && ((!pView->doUnwrapTime() && (*it)->getModelTime()->time <= pView->drawToX().time)
                            || (pView->doUnwrapTime()
                                    && ((*it)->getModelTime()->time < pView->drawToX().time || ((*it)->getModelTime()->time == pView->drawToX().time && (*it)->getEventID() <= pView->drawToEventCount())))))
            {
                y = rdo::roundDouble((double)rect.bottom() - double(ky) * ((*it)->getValue() - m_minValue));
                y = std::max(y, rect.top());
                y = std::min(y, rect.bottom());
                x = rect.left() + rdo::roundDouble(((*it)->getModelTime()->time - pView->drawFromX().time) * double(pView->timeScale())) - pView->chartShift();
                if (pView->doUnwrapTime())
                {
                    x += (ticks + (*it)->getEventID()) * pView->style()->pFontsTicks.tickWidth;
                }
                x = std::min(x, rect.right());
                if (draw_marker)
                    drawMarker(painter, x, y, marker, markerSize);
                path.lineTo(x, lastY);
                path.lineTo(x, y);
                lastX = x;
                lastY = y;
                ++it;
                if (pView->doUnwrapTime() && it != m_valueList.end())
                {
                    while (timeIt != pView->unwrapTimesList().end() && *(*it)->getModelTime() != *(*timeIt))
                    {
                        ticks += (*timeIt)->eventCount;
                        ++timeIt;
                    }
                }
            }

            bool tempResourceErased = (m_kind == Kind::PARAM && ((Param*)this)->getResource()->isErased());
            bool needContinue = !pView->doUnwrapTime() ? (m_valueList.size() > 1) : true;
            if (tempResourceErased)
            {
                if (!pView->doUnwrapTime())
                {
                    needContinue = (it != m_valueList.end() && (*it)->getModelTime()->time > pView->drawToX().time);
                }
                else
                {
                    needContinue = (it != m_valueList.end()
                            && ((*it)->getModelTime()->time > pView->drawToX().time || ((*it)->getModelTime()->time == pView->drawToX().time && (*it)->getEventID() > pView->drawToEventCount())));
                }
            }

            if (needContinue)
            {
                if (pView->drawFromX() == pView->drawToX())
                {
                    x = rect.left() + (pView->drawToEventCount() - pView->drawFromEventID()) * pView->style()->pFontsTicks.tickWidth;
                    x = std::min(x, rect.right());
                }
                else
                {
                    x = rect.right();
                }
                path.lineTo(x, lastY);
            }

            painter.setBrush(Qt::NoBrush);
            painter.drawPath(path);
        }
    }
}

void Serie::drawMarker(QPainter& painter, const int x, const int y, Marker marker, const int markerSize) const
{
    float halfMarkerSize = float(markerSize) / 2.0f;
    QRectF rect(x - halfMarkerSize, y - halfMarkerSize, markerSize, markerSize);

    switch (marker)
    {
    case Marker::CIRCLE:
        painter.drawEllipse(rect);
        break;

    case Marker::SQUARE:
        painter.drawRect(rect);
        break;

    case Marker::TRIANG:
    {
        QPolygonF polygon;
        polygon << QPointF(rect.left () + halfMarkerSize, rect.top());
        polygon << QPointF(rect.right(),                  rect.bottom());
        polygon << QPointF(rect.left (),                  rect.bottom());
        polygon << QPointF(rect.left () + halfMarkerSize, rect.top());

        QPainterPath path;
        path.addPolygon(polygon);
        painter.drawPath(path);
        break;
    }
    case Marker::CROSS:
    {
        int delta = halfMarkerSize == int(halfMarkerSize)
            ? 1
            : 0;

        QPainterPath path;
        path.moveTo(QPointF(x - halfMarkerSize - delta, y - halfMarkerSize - delta));
        path.lineTo(QPointF(x + halfMarkerSize, y + halfMarkerSize));
        path.moveTo(QPointF(x - halfMarkerSize - delta, y + halfMarkerSize + delta));
        path.lineTo(QPointF(x + halfMarkerSize, y - halfMarkerSize));
        painter.drawPath(path);
        break;
    }
    default:
        break;
    }
}

void Serie::addToDoc(ChartDoc* const pDocument)
{
    if (pDocument && std::find(m_documentList.begin(), m_documentList.end(), pDocument) == m_documentList.end())
    {
        m_documentList.push_back(pDocument);
    }
}

void Serie::removeFromDoc(ChartDoc* const pDocument)
{
    DocumentList::iterator it = std::find(m_documentList.begin(), m_documentList.end(), pDocument);
    if (it != m_documentList.end())
    {
        m_documentList.erase(it);
    }
}

bool Serie::isInOneOrMoreDocs() const
{
    return !m_documentList.empty();
}

bool Serie::activateFirstDoc() const
{
    bool result = false;
    if (!m_documentList.empty())
    {
        ChartDoc* pDoc = m_documentList.front();
        if (pDoc)
        {
            ChartView* pView = pDoc->getFirstView();
            ASSERT (pView)
            g_pApp->getIMainWnd()->activateSubWindow(pView->parentWidget()->parentWidget());
            result = true;
        }
    }

    return result;
}

Serie::ExportData Serie::exportData()
{
//    setlocale(LC_ALL, "rus");

    ExportData exportData;
    exportData.reserve(m_valueList.size() + 1);
    exportData.push_back(QString("%1;%2").arg("время").arg(m_title));

    for (Value* pValue: m_valueList)
    {
        exportData.push_back(QString("%1;%2").arg(pValue->getModelTime()->time).arg(pValue->getValue()));
    }

//    setlocale(LC_NUMERIC, "eng");

    return exportData;
}
