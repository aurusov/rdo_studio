#ifndef RDOSTUDIOCHARTDOC_H
#define RDOSTUDIOCHARTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodocserie.h"
#include "rdo_tracer/rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
class RDOTracerTimeNow;
class RDOTracerValue;
class RDOStudioDocSerie;

typedef std::list< RDOTracerTimeNow* > timesList;

class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartOptionsChart;

protected:
	DECLARE_DYNCREATE(RDOStudioChartDoc)

	std::vector< RDOStudioDocSerie* > series;
	int getSerieIndex( RDOStudioDocSerie* serie ) const;
	COLORREF selectColor();
	RDOTracerSerieMarker selectMarker();

	timesList docTimes;
	double minTimeOffset;
	int ticksCount;
	void incTimeEventsCount( RDOTracerTimeNow* time );
	bool newValueToSerieAdded( RDOTracerValue* val );

	bool previewMode;

public:
	//{{AFX_VIRTUAL(RDOStudioChartDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioChartDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartDoc( const bool preview = false );
	virtual ~RDOStudioChartDoc();

	void addSerie( RDOTracerSerie* const serie );
	void removeSerie( RDOTracerSerie* const serie );
	bool serieExists( const RDOTracerSerie* serie ) const { return std::find_if( series.begin(), series.end(), std::bind2nd( std::mem_fun1(&RDOStudioDocSerie::isTracerSerie), serie ) ) != series.end(); };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTDOC_H
