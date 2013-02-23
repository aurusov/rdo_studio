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
#include <boost/foreach.hpp>
#include <boost/range/algorithm/find_if.hpp>
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

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartDocInsertTime
// --------------------------------------------------------------------------------
class RDOStudioChartDocInsertTime
{
public:
	RDOStudioChartDocInsertTime(RDOStudioChartDoc* _doc)
		: doc(_doc)
	{}

	void operator ()(TracerValue* val);

private:
	RDOStudioChartDoc* doc;
};

void RDOStudioChartDocInsertTime::operator ()(TracerValue* val)
{
	if (val)
	{
		TimesList::iterator it = std::find_if(doc->m_insertedIt, doc->m_docTimes.end(), std::bind2nd(std::mem_fun1(&TracerTimeNow::compareTimes), val->modeltime));
		if (it == doc->m_docTimes.end() || (*it) != val->modeltime)
		{
			doc->m_insertedIt = doc->m_docTimes.insert(it, val->modeltime);
			doc->m_ticksCount += val->modeltime->eventCount;
			double offl = 1.7E+308;
			double offr = 1.7E+308;
			if (it != doc->m_docTimes.end())
			{
				offr = (*it)->time - (*doc->m_insertedIt)->time;
			}
			if (doc->m_insertedIt != doc->m_docTimes.begin())
			{
				offl = (*doc->m_insertedIt)->time;
				TimesList::iterator prev_it = doc->m_insertedIt;
				prev_it--;
				offl -= (*prev_it)->time;
			}
			double minoff = std::min(offl, offr);
			if (minoff < doc->m_minTimeOffset)
			{
				doc->m_minTimeOffset = minoff;
			}
		}
	}
	//studioApp.m_pMainFrame->stepProgress();
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartDoc
// --------------------------------------------------------------------------------
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
	g_pTracer->lock();

	m_mutex.Lock();

	for (std::vector<ChartSerie*>::iterator it = m_serieList.begin(); it != m_serieList.end(); it++)
	{
		(*it)->getSerie()->removeFromDoc(this);
		delete (*it);
	}
	if (!m_previewMode)
	{
		g_pTracer->removeChart(this);
	}

	m_mutex.Unlock();

	g_pTracer->unlock();
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
	ASSERT (pStyle)
	BOOST_FOREACH(ChartView * pView, m_viewList)
	{
		pView->setStyle(pStyle);
	}
}

void RDOStudioChartDoc::updateAllViews()
{
	BOOST_FOREACH(ChartView * pView, m_viewList)
	{
		pView->updateView();
	}
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
	//mutex.Lock(); Document is locked from TracerBase::addTime
	
	if (!m_docTimes.empty() && m_docTimes.back() == time)
	{
		m_ticksCount++;
		updateChartViews (UPDATE_TIMETICKS);
	}
	
	//mutex.Unlock();
}

rbool RDOStudioChartDoc::newValueToSerieAdded(TracerValue* val)
{
	//mutex.Lock(); Document is locked from TracerSerie::addValue

	if (m_docTimes.empty())
	{
		m_docTimes.push_back(val->modeltime);
		m_ticksCount += val->modeltime->eventCount;
	}
	else
	{
		TracerTimeNow* last = m_docTimes.back();
		if (last != val->modeltime)
		{
			m_docTimes.push_back(val->modeltime);
			m_ticksCount += val->modeltime->eventCount;
			double off = val->modeltime->time - last->time;
			if (off < m_minTimeOffset)
			{
				m_minTimeOffset = off;
			}
		}
	}
	updateChartViews (UPDATE_NEWVALUE);

	//mutex.Unlock();

	return true;
}

int RDOStudioChartDoc::getMaxMarkerSize() const
{
	const_cast<CMutex&>(m_mutex).Lock();

	int res = 0;
	for (std::vector<ChartSerie*>::const_iterator it = m_serieList.begin(); it != m_serieList.end(); it++)
	{
		if ((*it)->options().markerNeedDraw && (*it)->options().markerSize > res)
		{
			res = (*it)->options().markerSize;
		}
	}

	const_cast<CMutex&>(m_mutex).Unlock();

	return res;
}

void RDOStudioChartDoc::addToViews(ChartView* pWidget)
{
	m_mutex.Lock();

	m_widgetList.push_back(pWidget);

	m_mutex.Unlock();
}

void RDOStudioChartDoc::removeFromViews(ChartView* pWidget)
{
	m_mutex.Lock();

	std::vector<ChartView*>::iterator it = std::find(m_widgetList.begin(), m_widgetList.end(), pWidget);
	if (it != m_widgetList.end())
	{
		m_widgetList.erase(it);
	}

	m_mutex.Unlock();
}

void RDOStudioChartDoc::updateChartViews(ruint updateType) const
{
	const_cast<CMutex&>(m_mutex).Lock();

	for (std::vector<ChartView*>::const_iterator it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
	{
		(*it)->onUserUpdateChartView(updateType);
	}

	const_cast<CMutex&>(m_mutex).Unlock();
}

void RDOStudioChartDoc::addSerie(TracerSerie* const serie)
{
	m_mutex.Lock();

	if (serie && !serieExists(serie))
	{
		ChartSerie* docserie = new ChartSerie(serie);
		docserie->lock();
		ChartSerie::Options options(docserie->options());
		options.color = selectColor();
		options.markerType = selectMarker();
		docserie->setOptions(options);
		m_serieList.push_back(docserie);
		m_insertedIt = m_docTimes.begin();

		if (!m_docTimes.empty() && !serie->empty())
		{
			TimesList::iterator last_doc = m_docTimes.end();
			--last_doc;
			valuesList::const_iterator first_serie = serie->begin();
			if ((*first_serie)->modeltime->time >= (*last_doc)->time)
			{
				m_insertedIt = m_docTimes.end();
				if ((*first_serie)->modeltime->time == (*last_doc)->time)
				{
					m_insertedIt = last_doc;
				}
			}
		}

		try
		{
			studioApp.BeginWaitCursor();

			//int count;
			//serie->getValueCount( count );
			//studioApp.m_pMainFrame->beginProgress( 0, count );

			std::for_each(serie->begin(), serie->end(), RDOStudioChartDocInsertTime(this));

			//studioApp.m_pMainFrame->endProgress();
			studioApp.EndWaitCursor();
		}
		catch (...)
		{
			studioApp.EndWaitCursor();
		}

		serie->addToDoc(this);
		if (m_serieList.size() == 1)
		{
			ChartView* pView = getFirstView();
			ASSERT(pView);
			pView->m_pYAxis = docserie;
		}
		docserie->unlock();
		updateChartViews (UPDATE_NEWSERIE);
	}

	m_mutex.Unlock();
}

/*void RDOStudioChartDoc::removeSerie( TracerSerie* const serie )
 {
 if ( !serie ) return;
 vector<ChartSerie*>::iterator it = find_if( series.begin(), series.end(), bind2nd( mem_fun1(&ChartSerie::isTracerSerie), serie ) );
 if ( it != series.end() ) {
 (*it)->serie->removeFromDoc( this );
 //must be recalc of minTimeOffset && tickscount
 delete (*it);
 series.erase( it );
 updateChartViews();
 }
 }*/

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

TracerSerieMarker RDOStudioChartDoc::selectMarker()
{
	int count = m_serieList.size();
	int mul = count / 4;
	int index = count - mul * 4;
	TracerSerieMarker res = RDOSM_CIRCLE;
	switch (index)
	{
	case 0:
		res = RDOSM_CIRCLE;
		break;
	case 1:
		res = RDOSM_SQUARE;
		break;
	case 2:
		res = RDOSM_TRIANG;
		break;
	case 3:
		res = RDOSM_CROSS;
		break;
	};
	return res;
}

rbool RDOStudioChartDoc::serieExists(const TracerSerie* serie) const
{
	const_cast<CMutex&>(m_mutex).Lock();

	rbool res = boost::range::find_if(m_serieList, boost::bind(&ChartSerie::isTracerSerie, _1, serie)) != m_serieList.end();

	const_cast<CMutex&>(m_mutex).Unlock();

	return res;
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
