#ifndef RDOSTUDIOCHARTDOC_H
#define RDOSTUDIOCHARTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

#include "./rdo_tracer/rdotracerserie.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
struct RDOStudioDocSerie {
	RDOTracerSerie* serie;
	COLORREF color;
};

class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;

protected:
	RDOStudioChartDoc();
	DECLARE_DYNCREATE(RDOStudioChartDoc)

	vector <RDOStudioDocSerie> series;
	int findSerie( const RDOTracerSerie* serie ) const;
	bool serieExists( const RDOTracerSerie* serie ) const { return findSerie( serie ) != -1; };
	COLORREF selectColor();

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
	virtual ~RDOStudioChartDoc();

	void addSerie( RDOTracerSerie* const serie );
	void removeSerie( RDOTracerSerie* const serie );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTDOC_H
