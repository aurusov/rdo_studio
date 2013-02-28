/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_doc.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_DOC_H_
#define _RDO_STUDIO_TRACER_CHART_DOC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

#define WM_USER_UPDATE_CHART_VIEW WM_USER + 0x156
#define UPDATE_NEWSERIE  0x000
#define UPDATE_NEWVALUE  0x001
#define UPDATE_TIMETICKS 0x002

namespace rdo { namespace gui { namespace tracer {

class Time;
class Value;
class ChartSerie;
class ChartViewStyle;

class ChartDoc
{
public:
	typedef  std::list<Time*>          TimesList;
	typedef  std::vector<ChartSerie*>  SerieList;

	ChartDoc(const rbool preview = false);
	virtual ~ChartDoc();

	void attachView(ChartView* pView);
	ChartView* getFirstView();

	CREF(QString) getTitle () const;
	void          setTitle (CREF(QString) title);
	void          autoTitle();
	static void   resetTitleIndex();

	void setStyle(ChartViewStyle* pStyle);

	void updateAllViews();

	void addSerie(CREF(LPSerie) pSerie);
	rbool serieExists(CREF(LPSerie) pSerie) const;

	void incTimeEventsCount(Time* time);
	rbool newValueToSerieAdded(Value* val);

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
	QString                 m_title;
	std::vector<ChartView*> m_viewList;
	static ruint            s_titleIndex;

	int getSerieIndex(ChartSerie* serie) const;
	COLORREF selectColor();
	Serie::Marker selectMarker();

	void updateChartViews(const UINT update_type) const;

	void insertValue(Value* pValue);
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_DOC_H_
