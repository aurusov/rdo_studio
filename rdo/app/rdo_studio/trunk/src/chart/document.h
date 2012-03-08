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
class RDOTracerTimeNow;
class RDOTracerValue;
class RDOStudioDocSerie;

typedef std::list< RDOTracerTimeNow* > timesList;

class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;

DECLARE_DYNCREATE(RDOStudioChartDoc)

protected:
	
	
	CMutex mutex;
	
	std::vector< RDOStudioDocSerie* > series;
	int getSerieIndex( RDOStudioDocSerie* serie ) const;
	COLORREF selectColor();
	RDOTracerSerieMarker selectMarker();

	timesList docTimes;
	timesList::iterator inserted_it;
	double minTimeOffset;
	int ticksCount;

	bool previewMode;

	int getMaxMarkerSize() const;

	std::vector< HWND > views_hwnd;
	void removeFromViews( const HWND handle );
	void addToViews( const HWND handle );

	void updateChartViews( const UINT update_type ) const;

	tstring title;

public:
	virtual void SetTitle( LPCTSTR lpszTitle );

private:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartDoc( const bool preview = false );
	virtual ~RDOStudioChartDoc();

	void addSerie( RDOTracerSerie* const serie );
	//void removeSerie( RDOTracerSerie* const serie );
	bool serieExists( const RDOTracerSerie* serie ) const;

	tstring getTitle() const { return title; };
	
	void lock() { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };
	void incTimeEventsCount( RDOTracerTimeNow* time );
	bool newValueToSerieAdded( RDOTracerValue* val );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // _RDO_STUDIO_MFC_CHART_DOCUMENT_H_
