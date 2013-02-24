/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerserie.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerSerieFindValue
// --------------------------------------------------------------------------------
class TracerSerieFindValue
{
public:
	TracerSerieFindValue(ChartView* pView)
		: m_pView(pView)
	{}

	rbool operator() (TracerValue* pValue);

private:
	ChartView* m_pView;
};

rbool TracerSerieFindValue::operator() (TracerValue* pValue)
{
	rbool res = pValue && pValue->getModelTime()->time >= m_pView->m_drawFromX.time;
	if (m_pView->doUnwrapTime() && res && (pValue->getModelTime()->time == m_pView->m_drawFromX.time))
	{
		res = pValue->getEventID() >= m_pView->m_drawFromEventIndex;
	}
	return res;
}

// --------------------------------------------------------------------------------
// -------------------- TracerSerie
// --------------------------------------------------------------------------------
TracerSerie::TracerSerie(Kind _serieKind)
	: ChartTreeItem(true)
	, m_kind(_serieKind)
	, m_minValue(0)
	, m_maxValue(0)
	, m_valueCount(0)
{}

TracerSerie::~TracerSerie()
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

CREF(QString) TracerSerie::getTitle() const
{
	return m_title;
}

void TracerSerie::setTitle(CREF(QString) value)
{
	m_title = value;
}

TracerSerie::Kind TracerSerie::getKind() const
{
	return m_kind;
}

rbool TracerSerie::isTemporaryResourceParam() const
{
	return m_kind == SK_PARAM && ((TracerResourceParam*)this)->getResource()->getType()->getKind() == TracerResourceType::RDOTK_TEMPORARY;
}

void TracerSerie::addValue(TracerValue* const pValue)
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

	std::for_each(m_documentList.begin(), m_documentList.end(), std::bind2nd(std::mem_fun1(&RDOStudioChartDoc::newValueToSerieAdded), pValue));
}

void TracerSerie::getValueCount(int& count) const
{
	count = m_valueCount;
}

rbool TracerSerie::empty() const
{
	return m_valueList.empty();
}

TracerSerie::ValuesList::const_iterator TracerSerie::begin() const
{
	return m_valueList.begin();
}

TracerSerie::ValuesList::const_iterator TracerSerie::end() const
{
	return m_valueList.end();
}

void TracerSerie::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	if (!captions.empty())
		captions.clear();

	if (m_kind == TracerSerie::SK_PREVIEW)
	{
		double valoffset = (m_maxValue - m_minValue) / (double)(valueCount - 1);
		double valo = m_minValue;
		tstring formatstr = "%.3f";
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

void TracerSerie::getCaptionsInt(std::vector<tstring>& captions, const int valueCount) const
{
	TracerSerie::getCaptions(captions, valueCount);

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
	tstring formatstr = "%d";
	for (int i = 0; i < real_val_count; i++)
	{
		captions.push_back(rdo::format(formatstr.c_str(), valo));
		valo += valoffset;
	}
}

void TracerSerie::getCaptionsDouble(std::vector<tstring>& captions, const int valueCount) const
{
	TracerSerie::getCaptions(captions, valueCount);

	double valoffset = (m_maxValue - m_minValue) / (double)(valueCount - 1);
	double valo = m_minValue;
	tstring formatstr = "%.3f";
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

void TracerSerie::getCaptionsBool(std::vector<tstring>& captions, const int valueCount) const
{
	TracerSerie::getCaptions(captions, valueCount);
	captions.push_back("FALSE");
	captions.push_back("TRUE");
}

void TracerSerie::getLastValue(TracerValue*& pValue) const
{
	if (!m_valueList.size())
		pValue = NULL;
	else
		pValue = m_valueList.back();
}

void TracerSerie::drawSerie(ChartView* const pView,
                            QPainter& painter,
                            const QRect& rect,
                            const QColor& color,
                            Marker marker,
                            const int markerSize,
                            const rbool draw_marker,
                            const rbool transparent_marker) const
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

		rbool flag = it != m_valueList.end();
		if (flag && !pView->doUnwrapTime())
		{
			flag = !(it == m_valueList.begin() && (*it)->getModelTime()->time > pView->m_drawToX.time);
		}
		else if (flag)
		{
			flag = !(it == m_valueList.begin() && ((*it)->getModelTime()->time > pView->m_drawToX.time || ((*it)->getModelTime()->time == pView->m_drawToX.time && (*it)->getEventID() > pView->m_drawToEventCount)));
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
				flag = (*it)->getModelTime()->time > pView->m_drawFromX.time;
			}
			else if (flag)
			{
				flag = (*it)->getModelTime()->time > pView->m_drawFromX.time || ((*it)->getModelTime()->time == pView->m_drawFromX.time && (*it)->getEventID() > pView->m_drawFromEventIndex);
			}
			if (flag)
				--it;

			int lasty = roundDouble((double)rect.bottom() - double(ky) * ((*it)->getValue() - m_minValue));
			lasty = std::max(lasty, rect.top());
			lasty = std::min(lasty, rect.bottom());
			int lastx = rect.left() + roundDouble(((*it)->getModelTime()->time - pView->m_drawFromX.time) * double(pView->m_timeScale)) - pView->m_chartShift;
			lastx = std::min(lastx, rect.right());

			int ticks = 0;
			TimesList::iterator times_it = pView->m_unwrapTimesList.begin();
			if (pView->doUnwrapTime() && (*it)->getModelTime()->time >= pView->m_drawFromX.time)
			{
				if (*(*it)->getModelTime() == *(*times_it))
				{
					lastx += ((*it)->getEventID() - pView->m_drawFromEventIndex) * pView->m_pStyle->pFontsTicks->tickWidth;
				}
				else
				{
					while (times_it != pView->m_unwrapTimesList.end() && *(*it)->getModelTime() != *(*times_it))
					{
						ticks += (*times_it)->eventCount;
						++times_it;
					}
					lastx += (ticks + (*it)->getEventID() - pView->m_drawFromEventIndex) * pView->m_pStyle->pFontsTicks->tickWidth;
				}
			}
			lastx = std::min(lastx, rect.right());

			QPainterPath path;
			if (lastx >= rect.left() && draw_marker)
			{
				drawMarker(painter, lastx, lasty, marker, markerSize);
				path.moveTo(lastx, lasty);
			}
			else
				path.moveTo(rect.left(), lasty);

			int x = lastx, y = lasty;
			if (pView->doUnwrapTime())
			{
				ticks -= pView->m_drawFromEventIndex;
			}
			++it;
			if (pView->doUnwrapTime() && it != m_valueList.end())
			{
				while (times_it != pView->m_unwrapTimesList.end() && *(*it)->getModelTime() != *(*times_it))
				{
					ticks += (*times_it)->eventCount;
					++times_it;
				}
			}

			while (it != m_valueList.end()
			        && ((!pView->doUnwrapTime() && (*it)->getModelTime()->time <= pView->m_drawToX.time)
			                || (pView->doUnwrapTime()
			                        && ((*it)->getModelTime()->time < pView->m_drawToX.time || ((*it)->getModelTime()->time == pView->m_drawToX.time && (*it)->getEventID() <= pView->m_drawToEventCount)))))
			{
				y = roundDouble((double)rect.bottom() - double(ky) * ((*it)->getValue() - m_minValue));
				y = std::max(y, rect.top());
				y = std::min(y, rect.bottom());
				x = rect.left() + roundDouble(((*it)->getModelTime()->time - pView->m_drawFromX.time) * double(pView->m_timeScale)) - pView->m_chartShift;
				if (pView->doUnwrapTime())
				{
					x += (ticks + (*it)->getEventID()) * pView->m_pStyle->pFontsTicks->tickWidth;
				}
				x = std::min(x, rect.right());
				if (draw_marker)
					drawMarker(painter, x, y, marker, markerSize);
				path.lineTo(x, lasty);
				path.lineTo(x, y);
				lastx = x;
				lasty = y;
				++it;
				if (pView->doUnwrapTime() && it != m_valueList.end())
				{
					while (times_it != pView->m_unwrapTimesList.end() && *(*it)->getModelTime() != *(*times_it))
					{
						ticks += (*times_it)->eventCount;
						++times_it;
					}
				}
			}

			rbool tempres_erased = (m_kind == SK_PARAM && ((TracerResourceParam*)this)->getResource()->isErased());
			rbool need_continue = !pView->doUnwrapTime() ? (m_valueList.size() > 1) : true;
			if (tempres_erased)
			{
				if (!pView->doUnwrapTime())
				{
					need_continue = (it != m_valueList.end() && (*it)->getModelTime()->time > pView->m_drawToX.time);
				}
				else
				{
					need_continue = (it != m_valueList.end()
					        && ((*it)->getModelTime()->time > pView->m_drawToX.time || ((*it)->getModelTime()->time == pView->m_drawToX.time && (*it)->getEventID() > pView->m_drawToEventCount)));
				}
			}

			if (need_continue)
			{
				if (pView->m_drawFromX == pView->m_drawToX)
				{
					x = rect.left() + (pView->m_drawToEventCount - pView->m_drawFromEventIndex) * pView->m_pStyle->pFontsTicks->tickWidth;
					x = std::min(x, rect.right());
				}
				else
				{
					x = rect.right();
				}
				path.lineTo(x, lasty);
			}

			painter.setBrush(Qt::NoBrush);
			painter.drawPath(path);
		}
	}
}

void TracerSerie::drawMarker(QPainter& painter, const int x, const int y, Marker marker, const int markerSize) const
{
	float halfMarkerSize = float(markerSize) / 2.0;
	QRectF rect(x - halfMarkerSize, y - halfMarkerSize, markerSize, markerSize);

	switch (marker)
	{
	case M_CIRCLE:
	{
		painter.drawEllipse(rect);
		break;
	}
	case M_SQUARE:
	{
		painter.drawRect(rect);
		break;
	}
	case M_TRIANG:
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
	case M_CROSS:
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
	}
}

void TracerSerie::addToDoc(RDOStudioChartDoc* const pDocument)
{
	if (pDocument && std::find(m_documentList.begin(), m_documentList.end(), pDocument) == m_documentList.end())
	{
		m_documentList.push_back(pDocument);
	}
}

void TracerSerie::removeFromDoc(RDOStudioChartDoc* const pDocument)
{
	DocumentList::iterator it = std::find(m_documentList.begin(), m_documentList.end(), pDocument);
	if (it != m_documentList.end())
	{
		m_documentList.erase(it);
	}
}

rbool TracerSerie::isInOneOrMoreDocs() const
{
	return !m_documentList.empty();
}

rbool TracerSerie::activateFirstDoc() const
{
	rbool result = false;
	if (!m_documentList.empty())
	{
		RDOStudioChartDoc* pDoc = m_documentList.front();
		if (pDoc)
		{
			ChartView* pView = pDoc->getFirstView();
			ASSERT (pView)
			studioApp.getIMainWnd()->activateSubWindow(pView->parentWidget()->parentWidget());
			result = true;
		}
	}

	return result;
}

TracerSerie::ExportData TracerSerie::exportData()
{
	setlocale(LC_ALL, _T("rus"));

	ExportData exportData;
	exportData.reserve(m_valueList.size() + 1);
	exportData.push_back(QString("%1;%2").arg(QString::fromStdWString(L"время")).arg(m_title));

	BOOST_FOREACH(PTR(TracerValue) pValue, m_valueList)
	{
		exportData.push_back(QString("%1;%2").arg(pValue->getModelTime()->time).arg(pValue->getValue()));
	}

	setlocale(LC_NUMERIC, _T("eng"));

	return exportData;
}
