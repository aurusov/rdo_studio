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

class insertTime
{
	RDOStudioChartDoc* doc;
public:
	insertTime( RDOStudioChartDoc* _doc ): doc( _doc ) {};
	void operator ()( RDOTracerValue* val );
};

void insertTime::operator ()( RDOTracerValue* val )
{
	if( val ) {
		//timesList::iterator it = find( times->begin(), times->end(), val->modeltime );
		//if ( it != times->end() )
		//	return;
		//it = find_if( times->begin(), times->end(), checkMoreThen( val->modeltime ) );
		//timesList::iterator it = find_if( times->begin(), times->end(), bind2nd( mem_fun1( &RDOTracerTimeNow::checkTimeMoreOrEqual ), val->modeltime ) );
		timesList::iterator it = find_if( doc->docTimes.begin(), doc->docTimes.end(), bind2nd( greater_equal<RDOTracerTimeNow*>(), val->modeltime ) );
		if ( it != doc->docTimes.end() && (*it) == val->modeltime )
			return;
		timesList::iterator inserted_it = doc->docTimes.insert( it, val->modeltime );
		double offl = 1.7E+308;
		double offr = 1.7E+308;
		if ( it != doc->docTimes.end() )
			offr = (*it)->time - (*inserted_it)->time;
		if ( inserted_it != doc->docTimes.begin() ) {
			offl = (*inserted_it)->time;
			inserted_it --;
			offl -= (*inserted_it)->time;
		}
		double minoff = min( offl, offr );
		if ( minoff < doc->minTimeOffset )
			doc->minTimeOffset = minoff;
	}
}

/*bool checkMoreThen::operator ()( RDOTracerTimeNow* val )
{
	bool res = false;
	if( val ) {
		res = val->time > checktime->time;
	}
	return res;
}*/

/*void RDOStudioChartDoc::insertTime( RDOTracerValue* val )
{
	timesList::iterator it = find_if( docTimes.begin(), docTimes.end(), bind2nd( greater_equal<RDOTracerTimeNow*>(), val->modeltime ) );
	if ( it != docTimes.end() && (*it) == val->modeltime )
		return;
	timesList::iterator inserted_it = docTimes.insert( it, val->modeltime );
}*/

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
	for_each( serie->begin(), serie->end(), insertTime( this ) );
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
