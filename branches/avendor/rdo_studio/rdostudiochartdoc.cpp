#include "stdafx.h"
#include "rdostudiochartdoc.h"
#include "./rdo_tracer/rdotracertrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioChartDoc, CDocument)
	//{{AFX_MSG_MAP(RDOStudioChartDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartDoc::RDOStudioChartDoc() : CDocument()
{
	trace.addChart( this );
}

RDOStudioChartDoc::~RDOStudioChartDoc()
{
	trace.removeChart( this );
}

BOOL RDOStudioChartDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioChartDoc::Serialize(CArchive& ar)
{
	if ( ar.IsStoring() ) {
	}
	else {
	}
}

bool RDOStudioChartDoc::serieExists( const RDOTracerSerie* serie ) const
{
	for ( vector< RDOTracerSerie* >::const_iterator it = series.begin(); it != series.end(); it++ )
		if ( (*it) == serie )
			return true;
	return false;
}

void RDOStudioChartDoc::addSerie( RDOTracerSerie* const serie )
{
	if ( serie && !serieExists( serie ) ) {
		series.push_back( serie );
		UpdateAllViews( NULL );
	}
}


#ifdef _DEBUG
void RDOStudioChartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RDOStudioChartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
