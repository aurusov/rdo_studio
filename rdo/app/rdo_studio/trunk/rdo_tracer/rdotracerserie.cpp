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
	TracerSerieFindValue(ChartView* _view)
		: view(_view)
	{}

	rbool operator ()(TracerValue* val);

private:
	ChartView* view;
};

rbool TracerSerieFindValue::operator ()(TracerValue* val)
{
	rbool res = val && val->modeltime->time >= view->m_drawFromX.time;
	if (view->doUnwrapTime() && res && (val->modeltime->time == view->m_drawFromX.time))
	{
		res = val->eventIndex >= view->m_drawFromEventIndex;
	}
	return res;
}

// --------------------------------------------------------------------------------
// -------------------- TracerSerie
// --------------------------------------------------------------------------------
TracerSerie::TracerSerie(TracerSerieKind _serieKind)
	: ChartTreeItem(true)
	, serieKind(_serieKind)
	, minValue(0)
	, maxValue(0)
	, value_count(0)
{}

TracerSerie::~TracerSerie()
{
	valuesList::iterator it = values.begin();
	while (it != values.end())
	{
		delete *it;
		it++;
	}
	values.clear();
	documents.clear();
}

rbool TracerSerie::isTemporaryResourceParam() const
{
	return serieKind == RDOST_RESPARAM && ((TracerResParam*)this)->getResource()->getType()->getResTypeKind() == RDOTK_TEMPORARY;
}

tstring TracerSerie::getTitle() const
{
	return title;
}

void TracerSerie::setTitle(CREF(tstring) value)
{
	if (title != value)
		title = value;
}

void TracerSerie::addValue(TracerValue* const value)
{
	std::vector<RDOStudioChartDoc*>::iterator it;

	if (value->value < minValue || values.empty())
		minValue = value->value;
	if (value->value > maxValue || values.empty())
		maxValue = value->value;

	values.push_back(value);

	value_count++;

	std::for_each(documents.begin(), documents.end(), std::bind2nd(std::mem_fun1(&RDOStudioChartDoc::newValueToSerieAdded), value));
}

void TracerSerie::getValueCount(int& count) const
{
	count = value_count;
}

void TracerSerie::getCaptions(std::vector<tstring> &captions, const int val_count) const
{
	if (!captions.empty())
		captions.clear();
	if (serieKind == RDOST_PREVIEW)
	{
		double valoffset = (maxValue - minValue) / (double)(val_count - 1);
		double valo = minValue;
		tstring formatstr = "%.3f";
		if (value_count > 1)
		{
			for (int i = 0; i < val_count; i++)
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

void TracerSerie::getCaptionsInt(std::vector<tstring> &captions, const int val_count) const
{
	TracerSerie::getCaptions(captions, val_count);

	int real_val_count = val_count;
	if ((maxValue - minValue + 1) > real_val_count)
	{
		while ((int)((maxValue - minValue) / (real_val_count - 1)) != (double)((maxValue - minValue) / (real_val_count - 1)))
			real_val_count--;
	}
	else
	{
		real_val_count = (int)(maxValue - minValue + 1);
	}
	int valo = (int)minValue;
	int valoffset = (int)((maxValue - minValue) / (real_val_count - 1));
	tstring formatstr = "%d";
	for (int i = 0; i < real_val_count; i++)
	{
		captions.push_back(rdo::format(formatstr.c_str(), valo));
		valo += valoffset;
	}
}

void TracerSerie::getCaptionsDouble(std::vector<tstring> &captions, const int val_count) const
{
	TracerSerie::getCaptions(captions, val_count);

	double valoffset = (maxValue - minValue) / (double)(val_count - 1);
	double valo = minValue;
	tstring formatstr = "%.3f";
	if (value_count > 1)
	{
		for (int i = 0; i < val_count; i++)
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

void TracerSerie::getCaptionsBool(std::vector<tstring> &captions, const int val_count) const
{
	TracerSerie::getCaptions(captions, val_count);
	captions.push_back("FALSE");
	captions.push_back("TRUE");
}

void TracerSerie::getLastValue(TracerValue*& val) const
{
	if (!values.size())
		val = NULL;
	else
		val = values.back();
}

void TracerSerie::drawSerie(ChartView* const view,
                            QPainter& painter,
                            const QRect& rect,
                            const QColor& color,
                            TracerSerieMarker marker,
                            const int markerSize,
                            const rbool draw_marker,
                            const rbool transparent_marker) const
{
	if (!values.empty())
	{
		painter.setPen(color);
		painter.setBrush(QBrush(color, transparent_marker ? Qt::NoBrush : Qt::SolidPattern));

		valuesList::const_iterator it = std::find_if(values.begin(), values.end(), TracerSerieFindValue(view));

		if (it == values.end() && !values.empty() && !isTemporaryResourceParam())
		{
			--it;
		}

		rbool flag = it != values.end();
		if (flag && !view->doUnwrapTime())
		{
			flag = !(it == values.begin() && (*it)->modeltime->time > view->m_drawToX.time);
		}
		else if (flag)
		{
			flag = !(it == values.begin() && ((*it)->modeltime->time > view->m_drawToX.time || ((*it)->modeltime->time == view->m_drawToX.time && (*it)->eventIndex > view->m_drawToEventCount)));
		}

		if (flag)
		{

			long double ky;
			if (maxValue != minValue)
				ky = rect.height() / (maxValue - minValue);
			else
				ky = 0;

			flag = it != values.begin();
			if (flag && !view->doUnwrapTime())
			{
				flag = (*it)->modeltime->time > view->m_drawFromX.time;
			}
			else if (flag)
			{
				flag = (*it)->modeltime->time > view->m_drawFromX.time || ((*it)->modeltime->time == view->m_drawFromX.time && (*it)->eventIndex > view->m_drawFromEventIndex);
			}
			if (flag)
				--it;

			int lasty = roundDouble((double)rect.bottom() - double(ky) * ((*it)->value - minValue));
			lasty = std::max(lasty, rect.top());
			lasty = std::min(lasty, rect.bottom());
			int lastx = rect.left() + roundDouble(((*it)->modeltime->time - view->m_drawFromX.time) * double(view->m_timeScale)) - view->m_chartShift;
			lastx = std::min(lastx, rect.right());

			int ticks = 0;
			TimesList::iterator times_it = view->m_unwrapTimesList.begin();
			if (view->doUnwrapTime() && (*it)->modeltime->time >= view->m_drawFromX.time)
			{
				if (*(*it)->modeltime == *(*times_it))
				{
					lastx += ((*it)->eventIndex - view->m_drawFromEventIndex) * view->m_pStyle->pFontsTicks->tickWidth;
				}
				else
				{
					while (times_it != view->m_unwrapTimesList.end() && *(*it)->modeltime != *(*times_it))
					{
						ticks += (*times_it)->eventCount;
						++times_it;
					}
					lastx += (ticks + (*it)->eventIndex - view->m_drawFromEventIndex) * view->m_pStyle->pFontsTicks->tickWidth;
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
			if (view->doUnwrapTime())
			{
				ticks -= view->m_drawFromEventIndex;
			}
			++it;
			if (view->doUnwrapTime() && it != values.end())
			{
				while (times_it != view->m_unwrapTimesList.end() && *(*it)->modeltime != *(*times_it))
				{
					ticks += (*times_it)->eventCount;
					++times_it;
				}
			}

			while (it != values.end()
			        && ((!view->doUnwrapTime() && (*it)->modeltime->time <= view->m_drawToX.time)
			                || (view->doUnwrapTime()
			                        && ((*it)->modeltime->time < view->m_drawToX.time || ((*it)->modeltime->time == view->m_drawToX.time && (*it)->eventIndex <= view->m_drawToEventCount)))))
			{
				y = roundDouble((double)rect.bottom() - double(ky) * ((*it)->value - minValue));
				y = std::max(y, rect.top());
				y = std::min(y, rect.bottom());
				x = rect.left() + roundDouble(((*it)->modeltime->time - view->m_drawFromX.time) * double(view->m_timeScale)) - view->m_chartShift;
				if (view->doUnwrapTime())
				{
					x += (ticks + (*it)->eventIndex) * view->m_pStyle->pFontsTicks->tickWidth;
				}
				x = std::min(x, rect.right());
				if (draw_marker)
					drawMarker(painter, x, y, marker, markerSize);
				path.lineTo(x, lasty);
				path.lineTo(x, y);
				lastx = x;
				lasty = y;
				++it;
				if (view->doUnwrapTime() && it != values.end())
				{
					while (times_it != view->m_unwrapTimesList.end() && *(*it)->modeltime != *(*times_it))
					{
						ticks += (*times_it)->eventCount;
						++times_it;
					}
				}
			}

			rbool tempres_erased = (serieKind == RDOST_RESPARAM && ((TracerResParam*)this)->getResource()->isErased());
			rbool need_continue = !view->doUnwrapTime() ? (values.size() > 1) : true;
			if (tempres_erased)
			{
				if (!view->doUnwrapTime())
				{
					need_continue = (it != values.end() && (*it)->modeltime->time > view->m_drawToX.time);
				}
				else
				{
					need_continue = (it != values.end()
					        && ((*it)->modeltime->time > view->m_drawToX.time || ((*it)->modeltime->time == view->m_drawToX.time && (*it)->eventIndex > view->m_drawToEventCount)));
				}
			}

			if (need_continue)
			{
				if (view->m_drawFromX == view->m_drawToX)
				{
					x = rect.left() + (view->m_drawToEventCount - view->m_drawFromEventIndex) * view->m_pStyle->pFontsTicks->tickWidth;
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

void TracerSerie::drawMarker(QPainter& painter, const int x, const int y, TracerSerieMarker marker, const int markerSize) const
{
	float halfMarkerSize = float(markerSize) / 2.0;
	QRectF rect(x - halfMarkerSize, y - halfMarkerSize, markerSize, markerSize);

	switch (marker)
	{
	case RDOSM_CIRCLE:
	{
		painter.drawEllipse(rect);
		break;
	}
	case RDOSM_SQUARE:
	{
		painter.drawRect(rect);
		break;
	}
	case RDOSM_TRIANG:
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
	case RDOSM_CROSS:
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

void TracerSerie::addToDoc(RDOStudioChartDoc* const doc)
{
	if (doc && std::find(documents.begin(), documents.end(), doc) == documents.end())
	{
		documents.push_back(doc);
	}
}

void TracerSerie::removeFromDoc(RDOStudioChartDoc* const doc)
{
	std::vector<RDOStudioChartDoc*>::iterator it = std::find(documents.begin(), documents.end(), doc);
	if (it != documents.end())
	{
		documents.erase(it);
	}
}

rbool TracerSerie::activateFirstDoc() const
{
	rbool result = false;
	if (!documents.empty())
	{
		RDOStudioChartDoc* pDoc = documents.front();
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
	exportData.reserve(values.size() + 1);
	exportData.push_back(rdo::format("%s;%s", "время", title.c_str()));

	BOOST_FOREACH(PTR(TracerValue) pValue, values)
	{
		exportData.push_back(rdo::format("%f;%f", pValue->getModelTime()->time, pValue->value));
	}

	setlocale(LC_NUMERIC, _T("eng"));

	return exportData;
}
