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
	for ( vector<RDOStudioDocSerie>::const_iterator it = series.begin(); it != series.end(); it++ )
		if ( (*it).serie == serie )
			return true;
	return false;
}

void RDOStudioChartDoc::addSerie( RDOTracerSerie* const serie )
{
	if ( serie && !serieExists( serie ) ) {
		RDOStudioDocSerie docserie;
		docserie.serie = serie;
		docserie.color = selectColor();
		series.push_back( docserie );
		UpdateAllViews( NULL );
	}
}


COLORREF RDOStudioChartDoc::selectColor()
{
	int count = series.size();
	int mul = count / 16;
	int index = count - mul * 16;
	COLORREF res = RGB( 0, 0, 0 );
	switch ( index ) {
		case 0  : res = RGB( 0x00, 0x80, 0x00 ); break;
		case 1  : res = RGB( 0x00, 0x00, 0x80 ); break;
		case 2  : res = RGB( 0x80, 0x80, 0x80 ); break;
		case 3  : res = RGB( 0x80, 0x00, 0x80 ); break;
		case 4  : res = RGB( 0xFF, 0x00, 0x00 ); break;
		case 5  : res = RGB( 0x00, 0xFF, 0x00 ); break;
		case 6  : res = RGB( 0x00, 0x00, 0x00 ); break;
		case 7  : res = RGB( 0xFF, 0xFF, 0xFF ); break;
		case 8  : res = RGB( 0x80, 0x80, 0x00 ); break;
		case 9  : res = RGB( 0xC0, 0xC0, 0xC0 ); break;
		case 10 : res = RGB( 0x80, 0x00, 0x00 ); break;
		case 11 : res = RGB( 0x00, 0x80, 0x80 ); break;
		case 12 : res = RGB( 0xFF, 0xFF, 0x00 ); break;
		case 13 : res = RGB( 0x00, 0x00, 0xFF ); break;
		case 14 : res = RGB( 0xFF, 0x00, 0xFF ); break;
		case 15 : res = RGB( 0x00, 0xFF, 0xFF ); break;
	};
	return res;
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
