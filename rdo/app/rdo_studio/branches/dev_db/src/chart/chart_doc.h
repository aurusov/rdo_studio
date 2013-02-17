/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_doc.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_DOC_H_
#define _RDO_STUDIO_CHART_DOC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_serie.h"
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

#define WM_USER_UPDATE_CHART_VIEW WM_USER + 0x156
#define UPDATE_NEWSERIE  0x000
#define UPDATE_NEWVALUE  0x001
#define UPDATE_TIMETICKS 0x002

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartDoc
// --------------------------------------------------------------------------------
class TracerTimeNow;
class TracerValue;
class ChartSerie;
class RDOStudioChartViewStyle;

typedef std::list<TracerTimeNow*> TimesList;

class RDOStudioChartDoc
{
friend class ChartView;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;

public:
	RDOStudioChartDoc(const rbool preview = false);
	virtual ~RDOStudioChartDoc();

	void attachView(ChartView* pView);
	ChartView* getFirstView();

	tstring getTitle () const;
	void    setTitle (CREF(tstring) title);
	void    autoTitle();
	static void resetTitleIndex();

	void setStyle(RDOStudioChartViewStyle* pStyle);

	void updateAllViews();

	void addSerie(TracerSerie* const serie);
	//void removeSerie( TracerSerie* const serie );
	rbool serieExists(const TracerSerie* serie) const;

	void lock()
	{
		m_mutex.Lock();
	}

	void unlock()
	{
		m_mutex.Unlock();
	}

	void incTimeEventsCount(TracerTimeNow* time);
	rbool newValueToSerieAdded(TracerValue* val);

private:
	CMutex m_mutex;

	std::vector<ChartSerie*> m_serieList;
	int getSerieIndex(ChartSerie* serie) const;
	COLORREF selectColor();
	TracerSerieMarker selectMarker();

	TimesList           m_docTimes;
	TimesList::iterator m_insertedIt;
	double              m_minTimeOffset;
	int                 m_ticksCount;
	rbool               m_previewMode;

	int getMaxMarkerSize() const;

	std::vector<HWND> m_viewHwndList;
	void removeFromViews(const HWND handle);
	void addToViews(const HWND handle);

	void updateChartViews(const UINT update_type) const;

	tstring                 m_title;
	std::vector<ChartView*> m_viewList;
	static ruint            s_titleIndex;
};

#endif // _RDO_STUDIO_CHART_DOC_H_
