#include "stdafx.h"
#include "rdostudiochartdoc.h"
#include "rdostudiochartview.h"
#include "rdo_tracer/rdotracertrace.h"
#include "rdo_tracer/rdotracerserie.h"
#include "rdo_tracer/rdotracervalues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDocInsertTime
// ----------------------------------------------------------------------------
class RDOStudioChartDocInsertTime
{
	RDOStudioChartDoc* doc;
public:
	RDOStudioChartDocInsertTime( RDOStudioChartDoc* _doc ): doc( _doc ) {};
	void operator ()( RDOTracerValue* val );
};

void RDOStudioChartDocInsertTime::operator ()( RDOTracerValue* val )
{
	if( val ) {
		//timesList::iterator it = find_if( doc->docTimes.begin(), doc->docTimes.end(), bind2nd( greater_equal<RDOTracerTimeNow*>(), val->modeltime ) );
		timesList::iterator it = find_if( doc->docTimes.begin(), doc->docTimes.end(), bind2nd( mem_fun1( &RDOTracerTimeNow::compareTimes ), val->modeltime ) );
		if ( it != doc->docTimes.end() && (*it) == val->modeltime )
			return;
		timesList::iterator inserted_it = doc->docTimes.insert( it, val->modeltime );
		doc->ticksCount += val->modeltime->eventCount;
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

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioChartDoc, CDocument)
	//{{AFX_MSG_MAP(RDOStudioChartDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartDoc::RDOStudioChartDoc( const bool preview )
	: CDocument(),
	minTimeOffset( 1.7E+308 ),
	ticksCount( 0 ),
	previewMode( preview )
{
	if ( !previewMode )
		tracer.addChart( this );
}

RDOStudioChartDoc::~RDOStudioChartDoc()
{
	for ( vector< RDOStudioDocSerie* >::iterator it = series.begin(); it != series.end(); it++ ) {
		(*it)->serie->removeFromDoc( this );
		delete (*it);
	}
	if ( !previewMode )
		tracer.removeChart( this );
}

int RDOStudioChartDoc::getSerieIndex( RDOStudioDocSerie* serie ) const
{
	int res = -1;
	int index = 0;
	for ( vector< RDOStudioDocSerie* >::const_iterator it = series.begin(); it != series.end(); it++ ) {
		if ( serie == (*it) )
			res = index;
		index ++;
	}
	return res;
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

void RDOStudioChartDoc::incTimeEventsCount( RDOTracerTimeNow* time )
{
	if ( docTimes.back() == time )
		ticksCount ++;
}

bool RDOStudioChartDoc::newValueToSerieAdded( RDOTracerValue* val )
{
	if ( docTimes.empty() ) {
		docTimes.push_back( val->modeltime );
		ticksCount += val->modeltime->eventCount;
		return true;
	}
	RDOTracerTimeNow* last = docTimes.back();
	if ( last != val->modeltime ) {
		docTimes.push_back( val->modeltime );
		ticksCount += val->modeltime->eventCount;
		double off = val->modeltime->time - last->time;
		if ( off < minTimeOffset )
			minTimeOffset = off;
	}
	return true;
}

void RDOStudioChartDoc::addSerie( RDOTracerSerie* const serie )
{
	if ( serie && !serieExists( serie ) ) {
		RDOStudioDocSerie* docserie = new RDOStudioDocSerie( serie );
		//docserie.serie = serie;
		docserie->color = selectColor();
		docserie->marker = selectMarker();
		//docserie->docSerieTitle = serie->getTitle();
		series.push_back( docserie );
		for_each( serie->begin(), serie->end(), RDOStudioChartDocInsertTime( this ) );
		serie->addToDoc( this );
		if ( series.size() == 1 ) {
			POSITION pos = GetFirstViewPosition();
			while (pos != NULL) {
				CView* pView = GetNextView( pos );
				if ( pView && pView->IsKindOf( RUNTIME_CLASS(RDOStudioChartView) ) )
					static_cast<RDOStudioChartView*>( pView )->yAxis = docserie;
			}
		}
		UpdateAllViews( NULL );
	}
}

void RDOStudioChartDoc::removeSerie( RDOTracerSerie* const serie )
{
	if ( !serie ) return;
	vector<RDOStudioDocSerie*>::iterator it = find_if( series.begin(), series.end(), bind2nd( mem_fun1(&RDOStudioDocSerie::isTracerSerie), serie ) );
	if ( it != series.end() ) {
		(*it)->serie->removeFromDoc( this );
		//must be recalc of minTimeOffset && tickscount
		delete (*it);
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

RDOTracerSerieMarker RDOStudioChartDoc::selectMarker()
{
	int count = series.size();
	int mul = count / 4;
	int index = count - mul * 4;
	RDOTracerSerieMarker res = RDOSM_CIRCLE;
	switch ( index ) {
		case 0  : res = RDOSM_CIRCLE; break;
		case 1  : res = RDOSM_SQUARE; break;
		case 2  : res = RDOSM_RHOMB; break;
		case 3  : res = RDOSM_CROSS; break;
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
