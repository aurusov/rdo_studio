#ifndef RDOSTUDIOCHARTDOC_H
#define RDOSTUDIOCHARTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

#include "./rdo_tracer/rdotracerrdoclasses.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;

protected:
	RDOStudioChartDoc();
	DECLARE_DYNCREATE(RDOStudioChartDoc)

	vector <RDOTracerSerie*> series;
	bool serieExists( const RDOTracerSerie* serie ) const;

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

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTDOC_H
