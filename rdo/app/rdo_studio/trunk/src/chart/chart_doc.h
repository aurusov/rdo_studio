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

class TracerTimeNow;
class TracerValue;
class ChartSerie;
class RDOStudioChartViewStyle;

class RDOStudioChartDoc
{
public:
	typedef  std::list<TracerTimeNow*> TimesList;
	typedef  std::vector<ChartSerie*>  SerieList;

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

	void addSerie(CREF(LPTracerSerie) pSerie);
	rbool serieExists(CREF(LPTracerSerie) pSerie) const;

	void incTimeEventsCount(TracerTimeNow* time);
	rbool newValueToSerieAdded(TracerValue* val);

	void addToViews     (ChartView* pWidget);
	void removeFromViews(ChartView* pWidget);

	CREF(TimesList) getTimes    () const;
	CREF(SerieList) getSerieList() const;

	int    getMaxMarkerSize() const;
	int    getTicksCount   () const;
	double getMinTimeOffset() const;

private:
	TimesList            m_docTimes;
	TimesList::iterator  m_insertedIt;
	SerieList            m_serieList;
	double               m_minTimeOffset;
	int                  m_ticksCount;
	rbool                m_previewMode;

	std::vector<ChartView*> m_widgetList;
	tstring                 m_title;
	std::vector<ChartView*> m_viewList;
	static ruint            s_titleIndex;

	int getSerieIndex(ChartSerie* serie) const;
	COLORREF selectColor();
	TracerSerie::Marker selectMarker();

	void updateChartViews(const UINT update_type) const;

	void insertValue(TracerValue* pValue);
};

#endif // _RDO_STUDIO_CHART_DOC_H_
