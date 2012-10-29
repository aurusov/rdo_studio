/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/document.cpp
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartDocInsertTime
// --------------------------------------------------------------------------------
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
		timesList::iterator it = std::find_if( doc->inserted_it, doc->docTimes.end(), std::bind2nd( std::mem_fun1( &RDOTracerTimeNow::compareTimes ), val->modeltime ) );
		if ( it == doc->docTimes.end() || (*it) != val->modeltime ) {
			doc->inserted_it = doc->docTimes.insert( it, val->modeltime );
			doc->ticksCount += val->modeltime->eventCount;
			double offl = 1.7E+308;
			double offr = 1.7E+308;
			if ( it != doc->docTimes.end() )
				offr = (*it)->time - (*doc->inserted_it)->time;
			if ( doc->inserted_it != doc->docTimes.begin() ) {
				offl = (*doc->inserted_it)->time;
				timesList::iterator prev_it = doc->inserted_it;
				prev_it --;
				offl -= (*prev_it)->time;
			}
			double minoff = min( offl, offr );
			if ( minoff < doc->minTimeOffset )
				doc->minTimeOffset = minoff;
		}
	}
	//studioApp.m_pMainFrame->stepProgress();
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartDoc
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioChartDoc, CDocument)
END_MESSAGE_MAP()

RDOStudioChartDoc::RDOStudioChartDoc( const rbool preview )
	: CDocument(),
	minTimeOffset( 1.7E+308 ),
	ticksCount( 0 ),
	previewMode( preview )
{
	if ( !previewMode )
		tracer->addChart( this );
}

RDOStudioChartDoc::~RDOStudioChartDoc()
{
	tracer->lock();

	mutex.Lock();

	for ( std::vector< RDOStudioDocSerie* >::iterator it = series.begin(); it != series.end(); it++ ) {
		(*it)->serie->removeFromDoc( this );
		delete (*it);
	}
	if ( !previewMode )
		tracer->removeChart( this );

	mutex.Unlock();

	tracer->unlock();
}

int RDOStudioChartDoc::getSerieIndex( RDOStudioDocSerie* serie ) const
{
	int res = -1;
	int index = 0;
	for ( std::vector< RDOStudioDocSerie* >::const_iterator it = series.begin(); it != series.end(); it++ ) {
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
	//mutex.Lock(); Document is locked from RDOTracerBase::addTime

	if ( !docTimes.empty() && docTimes.back() == time ) {
		ticksCount ++;
		updateChartViews( UPDATE_TIMETICKS );
	}

	//mutex.Unlock();
}

rbool RDOStudioChartDoc::newValueToSerieAdded( RDOTracerValue* val )
{
	//mutex.Lock(); Document is locked from RDOTracerSerie::addValue
	
	if ( docTimes.empty() ) {
		docTimes.push_back( val->modeltime );
		ticksCount += val->modeltime->eventCount;
	} else {
		RDOTracerTimeNow* last = docTimes.back();
		if ( last != val->modeltime ) {
			docTimes.push_back( val->modeltime );
			ticksCount += val->modeltime->eventCount;
			double off = val->modeltime->time - last->time;
			if ( off < minTimeOffset )
				minTimeOffset = off;
		}
	}
	updateChartViews( UPDATE_NEWVALUE );

	//mutex.Unlock();

	return true;
}

int RDOStudioChartDoc::getMaxMarkerSize() const
{
	const_cast<CMutex&>(mutex).Lock();
	
	int res = 0;
	for ( std::vector< RDOStudioDocSerie* >::const_iterator it = series.begin(); it != series.end(); it++ ) {
		if ( (*it)->needDrawMarker && (*it)->marker_size > res ) res = (*it)->marker_size;
	}

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}

void RDOStudioChartDoc::addToViews( const HWND handle )
{
	mutex.Lock();

	views_hwnd.push_back( handle );

	mutex.Unlock();
}

void RDOStudioChartDoc::removeFromViews( const HWND handle )
{
	mutex.Lock();

	std::vector< HWND >::iterator it = std::find( views_hwnd.begin(), views_hwnd.end(), handle );
	if ( it != views_hwnd.end() )
		views_hwnd.erase( it );

	mutex.Unlock();
}

void RDOStudioChartDoc::updateChartViews( const UINT update_type ) const
{
	const_cast<CMutex&>(mutex).Lock();

	for( std::vector< HWND >::const_iterator it = views_hwnd.begin(); it != views_hwnd.end(); it++ ) {
		::SendNotifyMessage( (*it), WM_USER_UPDATE_CHART_VIEW, WPARAM( update_type ), 0 );
	}
	
	const_cast<CMutex&>(mutex).Unlock();
}

void RDOStudioChartDoc::addSerie( RDOTracerSerie* const serie )
{
	mutex.Lock();
	
	if ( serie && !serieExists( serie ) ) {
		RDOStudioDocSerie* docserie = new RDOStudioDocSerie( serie );
		docserie->lock();
		docserie->color = selectColor();
		docserie->marker = selectMarker();
		series.push_back( docserie );
		inserted_it = docTimes.begin();

		if ( !docTimes.empty() && !serie->empty() ) {
			timesList::iterator last_doc = docTimes.end();
			last_doc --;
			valuesList::const_iterator first_serie = serie->begin();
			if ( (*first_serie)->modeltime->time >= (*last_doc)->time ) {
				inserted_it = docTimes.end();
				if ( (*first_serie)->modeltime->time == (*last_doc)->time ) {
					inserted_it = last_doc;
				}
			}
		}

		try {
			studioApp.BeginWaitCursor();
			
			//int count;
			//serie->getValueCount( count );
			//studioApp.m_pMainFrame->beginProgress( 0, count );

			std::for_each( serie->begin(), serie->end(), RDOStudioChartDocInsertTime( this ) );

			//studioApp.m_pMainFrame->endProgress();
			studioApp.EndWaitCursor();
		} catch( ... ) {
			studioApp.EndWaitCursor();
		}

		serie->addToDoc( this );
		if ( series.size() == 1 ) {
			POSITION pos = GetFirstViewPosition();
			while (pos != NULL) {
				CView* pView = GetNextView( pos );
				if ( pView && pView->IsKindOf( RUNTIME_CLASS(RDOStudioChartView) ) )
					static_cast<RDOStudioChartView*>( pView )->yAxis = docserie;
			}
		}
		docserie->unlock();
		updateChartViews( UPDATE_NEWSERIE );
	}

	mutex.Unlock();
}

/*void RDOStudioChartDoc::removeSerie( RDOTracerSerie* const serie )
{
	if ( !serie ) return;
	vector<RDOStudioDocSerie*>::iterator it = find_if( series.begin(), series.end(), bind2nd( mem_fun1(&RDOStudioDocSerie::isTracerSerie), serie ) );
	if ( it != series.end() ) {
		(*it)->serie->removeFromDoc( this );
		//must be recalc of minTimeOffset && tickscount
		delete (*it);
		series.erase( it );
		updateChartViews();
	}
}*/

COLORREF RDOStudioChartDoc::selectColor()
{
	int count = series.size();
	int mul = count / 15;
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
		case 7  : res = RGB( 0x80, 0x80, 0x00 ); break;
		case 8  : res = RGB( 0xC0, 0xC0, 0xC0 ); break;
		case 9  : res = RGB( 0x80, 0x00, 0x00 ); break;
		case 10 : res = RGB( 0x00, 0x80, 0x80 ); break;
		case 11 : res = RGB( 0xFF, 0xFF, 0x00 ); break;
		case 12 : res = RGB( 0x00, 0x00, 0xFF ); break;
		case 13 : res = RGB( 0xFF, 0x00, 0xFF ); break;
		case 14 : res = RGB( 0x00, 0xFF, 0xFF ); break;
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

rbool RDOStudioChartDoc::serieExists( const RDOTracerSerie* serie ) const
{
	const_cast<CMutex&>(mutex).Lock();

	rbool res = std::find_if( series.begin(), series.end(), std::bind2nd( std::mem_fun1(&RDOStudioDocSerie::isTracerSerie), serie ) ) != series.end();

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}

void RDOStudioChartDoc::SetTitle( LPCTSTR lpszTitle )
{
	title = lpszTitle;
	CDocument::SetTitle( rdo::format( IDS_CHART_TITLE, title.c_str() ).c_str() );
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