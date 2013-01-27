/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/document.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHART_DOCUMENT_H_
#define _RDO_STUDIO_MFC_CHART_DOCUMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/doc_serie.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
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
class RDOStudioDocSerie;
class RDOStudioChartViewStyle;

typedef std::list< TracerTimeNow* > timesList;

class RDOStudioChartDoc
{
friend class RDOStudioChartView;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;

protected:
	CMutex mutex;

	std::vector< RDOStudioDocSerie* > series;
	int getSerieIndex( RDOStudioDocSerie* serie ) const;
	COLORREF selectColor();
	TracerSerieMarker selectMarker();

	timesList docTimes;
	timesList::iterator inserted_it;
	double minTimeOffset;
	int ticksCount;

	rbool previewMode;

	int getMaxMarkerSize() const;

	std::vector< HWND > views_hwnd;
	void removeFromViews( const HWND handle );
	void addToViews( const HWND handle );

	void updateChartViews( const UINT update_type ) const;

	tstring title;

	std::vector<RDOStudioChartView*> m_viewList;

	static ruint s_titleIndex;

public:
	RDOStudioChartDoc( const rbool preview = false );
	virtual ~RDOStudioChartDoc();

	void attachView(RDOStudioChartView* pView);
	RDOStudioChartView* getFirstView();

	tstring     getTitle () const;
	void        setTitle (CREF(tstring) title);
	void        autoTitle();
	static void resetTitleIndex();

	void setStyle(RDOStudioChartViewStyle* pStyle);

	void updateAllViews();

	void addSerie( TracerSerie* const serie );
	//void removeSerie( TracerSerie* const serie );
	rbool serieExists( const TracerSerie* serie ) const;

	void lock() { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };
	void incTimeEventsCount( TracerTimeNow* time );
	rbool newValueToSerieAdded( TracerValue* val );
};

#endif // _RDO_STUDIO_MFC_CHART_DOCUMENT_H_
