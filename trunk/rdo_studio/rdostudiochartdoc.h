#ifndef RDOSTUDIOCHARTDOC_H
#define RDOSTUDIOCHARTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_tracer/rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
class RDOTracerTimeNow;
class RDOTracerValue;
class RDOStudioChartView;

typedef std::list< RDOTracerTimeNow* > timesList;

class RDOStudioDocSerie
{
friend class RDOStudioChartDoc;
protected:
	RDOTracerSerie* serie;
	COLORREF color;
	std::string docSerieTitle;
public:
	RDOStudioDocSerie( RDOTracerSerie* _serie ) : serie( _serie ){};
	~RDOStudioDocSerie() {};
	RDOTracerSerie* getSerie() const { return serie; };
	COLORREF getColor() const { return color; };
	bool operator ==( const RDOTracerSerie* _serie ) const { return serie == _serie; };
	void drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect ) const { serie->drawSerie( view, dc, rect, color ); };
};

class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;
friend class RDOStudioChartDocInsertTime;

protected:
	DECLARE_DYNCREATE(RDOStudioChartDoc)

	std::vector< RDOStudioDocSerie > series;
	COLORREF selectColor();

	timesList docTimes;
	double minTimeOffset;
	int ticksCount;
	void incTimeEventsCount( RDOTracerTimeNow* time );
	//void addSerieTimes( RDOTracerSerie* const serie );
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
	bool serieExists( const RDOTracerSerie* serie ) const { return std::find( series.begin(), series.end(), serie ) != series.end(); };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTDOC_H
