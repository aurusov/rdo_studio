/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_doc.cpp
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_doc.h"
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ruint RDOStudioChartDoc::s_titleIndex = 0;

RDOStudioChartDoc::RDOStudioChartDoc(const rbool preview)
	: m_minTimeOffset(1.7E+308)
	, m_ticksCount(0)
	, m_previewMode(preview)
{
	if (!m_previewMode)
	{
		g_pTracer->addChart(this);
	}
}

RDOStudioChartDoc::~RDOStudioChartDoc()
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

CREF(RDOStudioChartDoc::TimesList) RDOStudioChartDoc::getTimes() const
{
	return m_docTimes;
}

CREF(RDOStudioChartDoc::SerieList) RDOStudioChartDoc::getSerieList() const
{
	return m_serieList;
}

void RDOStudioChartDoc::attachView(ChartView* pView)
{
	m_viewList.push_back(pView);
}

ChartView* RDOStudioChartDoc::getFirstView()
{
	return m_viewList.empty() ? NULL : m_viewList.front();
}

void RDOStudioChartDoc::setStyle(RDOStudioChartViewStyle* pStyle)
{
	ASSERT(pStyle);
	boost::range::for_each(m_viewList, boost::bind(&ChartView::setStyle, _1, pStyle, true));
}

void RDOStudioChartDoc::updateAllViews()
{
	boost::range::for_each(m_viewList, boost::bind(&ChartView::updateView, _1));
}

int RDOStudioChartDoc::getSerieIndex(ChartSerie* serie) const
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

void RDOStudioChartDoc::incTimeEventsCount(TracerTimeNow* time)
{
	if (!m_docTimes.empty() && m_docTimes.back() == time)
	{
		m_ticksCount++;
		updateChartViews (UPDATE_TIMETICKS);
	}
}

rbool RDOStudioChartDoc::newValueToSerieAdded(TracerValue* val)
{
	if (m_docTimes.empty())
	{
		m_docTimes.push_back(val->getModelTime());
		m_ticksCount += val->getModelTime()->eventCount;
	}
	else
	{
		TracerTimeNow* last = m_docTimes.back();
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
	updateChartViews (UPDATE_NEWVALUE);

	return true;
}

int RDOStudioChartDoc::getMaxMarkerSize() const
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

int RDOStudioChartDoc::getTicksCount() const
{
	return m_ticksCount;
}

double RDOStudioChartDoc::getMinTimeOffset() const
{
	return m_minTimeOffset;
}

void RDOStudioChartDoc::addToViews(ChartView* pWidget)
{
	m_widgetList.push_back(pWidget);
}

void RDOStudioChartDoc::removeFromViews(ChartView* pWidget)
{
	std::vector<ChartView*>::iterator it = std::find(m_widgetList.begin(), m_widgetList.end(), pWidget);
	if (it != m_widgetList.end())
	{
		m_widgetList.erase(it);
	}
}

void RDOStudioChartDoc::updateChartViews(ruint updateType) const
{
	boost::range::for_each(m_widgetList, boost::bind(&ChartView::onUserUpdateChartView, _1, updateType));
}

void RDOStudioChartDoc::addSerie(CREF(LPTracerSerie) pSerie)
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
			TracerSerie::ValuesList::const_iterator first_serie = pSerie->begin();
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
			studioApp.BeginWaitCursor();

			std::for_each(pSerie->begin(), pSerie->end(), boost::bind(&RDOStudioChartDoc::insertValue, this, _1));

			studioApp.EndWaitCursor();
		}
		catch (...)
		{
			studioApp.EndWaitCursor();
		}

		pSerie->addToDoc(this);
		if (m_serieList.size() == 1)
		{
			ChartView* pView = getFirstView();
			ASSERT(pView);
			pView->setYAxis(pDocSerie);
		}
		updateChartViews(UPDATE_NEWSERIE);
	}
}

COLORREF RDOStudioChartDoc::selectColor()
{
	int count = m_serieList.size();
	int mul = count / 15;
	int index = count - mul * 16;
	COLORREF res = RGB(0, 0, 0);
	switch (index)
	{
	case 0:
		res = RGB(0x00, 0x80, 0x00);
		break;
	case 1:
		res = RGB(0x00, 0x00, 0x80);
		break;
	case 2:
		res = RGB(0x80, 0x80, 0x80);
		break;
	case 3:
		res = RGB(0x80, 0x00, 0x80);
		break;
	case 4:
		res = RGB(0xFF, 0x00, 0x00);
		break;
	case 5:
		res = RGB(0x00, 0xFF, 0x00);
		break;
	case 6:
		res = RGB(0x00, 0x00, 0x00);
		break;
	case 7:
		res = RGB(0x80, 0x80, 0x00);
		break;
	case 8:
		res = RGB(0xC0, 0xC0, 0xC0);
		break;
	case 9:
		res = RGB(0x80, 0x00, 0x00);
		break;
	case 10:
		res = RGB(0x00, 0x80, 0x80);
		break;
	case 11:
		res = RGB(0xFF, 0xFF, 0x00);
		break;
	case 12:
		res = RGB(0x00, 0x00, 0xFF);
		break;
	case 13:
		res = RGB(0xFF, 0x00, 0xFF);
		break;
	case 14:
		res = RGB(0x00, 0xFF, 0xFF);
		break;
	};
	return res;
}

TracerSerie::Marker RDOStudioChartDoc::selectMarker()
{
	int count = m_serieList.size();
	int mul = count / 4;
	int index = count - mul * 4;
	TracerSerie::Marker res = TracerSerie::M_CIRCLE;
	switch (index)
	{
	case 0:
		res = TracerSerie::M_CIRCLE;
		break;
	case 1:
		res = TracerSerie::M_SQUARE;
		break;
	case 2:
		res = TracerSerie::M_TRIANG;
		break;
	case 3:
		res = TracerSerie::M_CROSS;
		break;
	};
	return res;
}

rbool RDOStudioChartDoc::serieExists(CREF(LPTracerSerie) pSerie) const
{
	return boost::range::find_if(m_serieList, boost::bind(&ChartSerie::isTracerSerie, _1, pSerie)) != m_serieList.end();
}

tstring RDOStudioChartDoc::getTitle() const
{
	return m_title;
}

void RDOStudioChartDoc::setTitle(CREF(tstring) title)
{
	this->m_title = title;
	getFirstView()->parentWidget()->setWindowTitle(QString::fromLocal8Bit(rdo::format(IDS_CHART_TITLE, this->m_title.c_str()).c_str()));
}

void RDOStudioChartDoc::autoTitle()
{
	tstring title = rdo::format("График%d", ++s_titleIndex);
	setTitle(title);
}

void RDOStudioChartDoc::resetTitleIndex()
{
	s_titleIndex = 0;
}

void RDOStudioChartDoc::insertValue(TracerValue* pValue)
{
	if (pValue)
	{
		RDOStudioChartDoc::TimesList::iterator it = std::find_if(m_insertedIt, m_docTimes.end(), std::bind2nd(std::mem_fun1(&TracerTimeNow::compareTimes), pValue->getModelTime()));
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
