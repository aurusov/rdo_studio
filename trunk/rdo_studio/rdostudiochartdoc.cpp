#include "stdafx.h"
#include "rdostudiochartdoc.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "./rdo_tracer/rdotracerserie.h"
#include "./rdo_tracer/rdotracervalues.h"

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

/*int RDOStudioChartDoc::findSerie( const RDOTracerSerie* serie ) const
{
	int res = -1;
	if ( !serie ) return res;
	for ( vector<RDOStudioDocSerie>::const_iterator it = series.begin(); it != series.end(); it++ ) {
		res ++;
		if ( (*it).serie == serie )
			break;
	}
	if ( it == series.end() ) res = -1;
	return res;
}*/

void insertTime::operator ()( RDOTracerValue* val )
{
	if( val ) {
		timesList::iterator it = find( times->begin(), times->end(), val->modeltime );
		if ( it != times->end() )
			return;
		it = find_if( times->begin(), times->end(), checkMoreThen( val->modeltime ) );
		times->insert( it, val->modeltime );
	}
}

bool checkMoreThen::operator ()( RDOTracerTimeNow* val )
{
	bool res = false;
	if( val ) {
		res = val->time > checktime->time;
	}
	return res;
}

void RDOStudioChartDoc::addSerieTimes( RDOTracerSerie* const serie )
{
	/*int count = serie->getValueCount();
	if ( !count )
		return;
	list< RDOTracerValue* >::const_iterator it = serie->getValueIterator( 0 );
	RDOTracerTimeNow* last = NULL;
	for ( int i = 0; i < count; i++, it++ ) {
		RDOTracerTimeNow* timenow = (*it)->modeltime;
		if ( last != timenow )
			docTimes.push_back( timenow );
		last = timenow;
	}*/
	for_each( serie->begin(), serie->end(), insertTime( &docTimes ) );
}

void RDOStudioChartDoc::addSerie( RDOTracerSerie* const serie )
{
	if ( serie && !serieExists( serie ) ) {
		RDOStudioDocSerie docserie( serie );
		//docserie.serie = serie;
		docserie.color = selectColor();
		series.push_back( docserie );
		addSerieTimes( serie );
		serie->addToDoc( this );
		UpdateAllViews( NULL );
	}
}

void RDOStudioChartDoc::removeSerie( RDOTracerSerie* const serie )
{
	if ( !serie ) return;
	vector<RDOStudioDocSerie>::iterator it = find( series.begin(), series.end(), serie );
	if ( it != series.end() ) {
		(*it).serie->removeFromDoc( this );
		series.erase( it );
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
