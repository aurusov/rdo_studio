#include "stdafx.h"
#include "rdotracerserie.h"
#include "rdotracervalues.h"
#include "rdotracerresource.h"
#include "rdotracerrestype.h"
#include "../rdostudiochartview.h"
#include "../rdostudiochartdoc.h"
#include "../rdostudioapp.h"
#include "../rdostudiomainfrm.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerieFindValue
// ----------------------------------------------------------------------------
class RDOTracerSerieFindValue
{
	RDOStudioChartView* view;
public:
	RDOTracerSerieFindValue( RDOStudioChartView* _view ): view( _view ) {};
	bool operator ()( RDOTracerValue* val );
};

bool RDOTracerSerieFindValue::operator ()( RDOTracerValue* val )
{
	bool res = val && val->modeltime->time >= view->drawFromX.time;
	if ( view->doUnwrapTime() && res && ( val->modeltime->time == view->drawFromX.time ) )
		res = val->eventIndex >= view->drawFromEventIndex;
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerie
// ----------------------------------------------------------------------------
RDOTracerSerie::RDOTracerSerie( RDOTracerSerieKind _serieKind ) :
	RDOTracerTreeItem( true ),
	serieKind( _serieKind ),
	minValue( 0 ),
	maxValue( 0 )
{
}

RDOTracerSerie::~RDOTracerSerie()
{
	valuesList::iterator it = values.begin();
	while ( it != values.end() ) {
		delete *it;
		it++;
	}
	values.clear();
	documents.clear();
};

bool RDOTracerSerie::isTemporaryResourceParam() const
{
	return serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->getType()->getResTypeKind() == RDOTK_TEMPORARY;
};

string RDOTracerSerie::getTitle() const
{
	return title;
}

void RDOTracerSerie::setTitle( const string& value )
{
	if ( title != value )
		title = value;
}

int RDOTracerSerie::addValue( RDOTracerValue* const value )
{
	values.push_back( value );
	if ( value->value < minValue || values.empty() )
		minValue = value->value;
	if ( value->value > maxValue || values.empty() )
		maxValue = value->value;
	for_each( documents.begin(), documents.end(), bind2nd( mem_fun1( &RDOStudioChartDoc::newValueToSerieAdded ), value ) );
	return values.size() - 1;
}

int RDOTracerSerie::getValueCount() const
{
	return values.size();
}

RDOTracerValue* RDOTracerSerie::getLastValue() const
{
	if ( !values.size() )
		return NULL;
	return values.back();
}

void RDOTracerSerie::drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color ) const
{
	int oldBkMode = dc.SetBkMode( TRANSPARENT );
	CPen pen;
	pen.CreatePen( PS_SOLID, 0, color );
	CPen* pOldPen = dc.SelectObject( &pen );

	if ( !values.empty() ) {
		
		valuesList::const_iterator it = find_if( values.begin(), values.end(), RDOTracerSerieFindValue( view ) );

		if ( it == values.end() && !values.empty() && !isTemporaryResourceParam() ) {
			it --;
		}
		
		bool flag = it != values.end();
		if ( flag && !view->doUnwrapTime() ) {
			flag = !( it == values.begin() && (*it)->modeltime->time > view->drawToX.time );
		} else if ( flag ) {
			flag = !( it == values.begin() && ( (*it)->modeltime->time > view->drawToX.time || ( (*it)->modeltime->time == view->drawToX.time && (*it)->eventIndex > view->drawToEventCount ) ) );
		}

		if ( flag ) {
			
			long double ky;
			if ( maxValue != minValue )
				ky = rect.Height() / ( maxValue - minValue );
			else
				ky = 0;
			
			flag = it != values.begin();
			if ( flag && !view->doUnwrapTime() ) {
				flag = (*it)->modeltime->time > view->drawFromX.time;
			} else if ( flag ) {
				flag = (*it)->modeltime->time > view->drawFromX.time || ( (*it)->modeltime->time == view->drawFromX.time && (*it)->eventIndex > view->drawFromEventIndex );
			}
			if ( flag )
				it --;
			
			int lasty = roundDouble( (double)rect.bottom - ky * ( (*it)->value - minValue ) );
			lasty = min( lasty, rect.bottom - 1 );
			int lastx = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * view->timeScale );
			lastx = min( lastx, rect.right - 1 );
			
			int ticks = 0;
			timesList::iterator times_it = view->unwrapTimesList.begin();
			if ( view->doUnwrapTime() && (*it)->modeltime->time >= view->drawFromX.time ) {
				if ( *(*it)->modeltime == *(*times_it) ) {
					lastx += ( (*it)->eventIndex - view->drawFromEventIndex ) * view->style->fonts_ticks->tickWidth;
				} else {
					while ( times_it != view->unwrapTimesList.end() && *(*it)->modeltime != *(*times_it) ) {
						ticks += (*times_it)->eventCount;
						times_it ++;
					}
					lastx += ( ticks + (*it)->eventIndex - view->drawFromEventIndex ) * view->style->fonts_ticks->tickWidth;
				}
			}
			lastx = min( lastx, rect.right - 1 );

			if ( lastx >= rect.left )
				drawMarker( dc, lastx, lasty, color );
			else
				lastx = rect.left;
			dc.MoveTo( lastx, lasty );
			
			int x = lastx, y = lasty;
			if ( view->doUnwrapTime() ) {
				ticks -= view->drawFromEventIndex;
			}
			it ++;
			if ( view->doUnwrapTime() && it != values.end() ) {
				while ( times_it != view->unwrapTimesList.end() && *(*it)->modeltime != *(*times_it) ) {
					ticks += (*times_it)->eventCount;
					times_it ++;
				}
			}

			while ( it != values.end() && ( (!view->doUnwrapTime() && (*it)->modeltime->time <= view->drawToX.time) || (view->doUnwrapTime() && ((*it)->modeltime->time < view->drawToX.time || ((*it)->modeltime->time == view->drawToX.time && (*it)->eventIndex <= view->drawToEventCount) )) ) ) {
				y = roundDouble( (double)rect.bottom - ky * ( (*it)->value - minValue ) );
				y = min( y, rect.bottom - 1 );
				x = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * view->timeScale );
				if ( view->doUnwrapTime() ) {
					x += ( ticks + (*it)->eventIndex ) * view->style->fonts_ticks->tickWidth;
				}
				x = min( x, rect.right - 1 );
				drawMarker( dc, x, y, color );
				dc.LineTo( x, lasty );
				dc.LineTo( x, y );
				lastx = x;
				lasty = y;
				it ++;
				if ( view->doUnwrapTime() && it != values.end() ) {
					while ( times_it != view->unwrapTimesList.end() && *(*it)->modeltime != *(*times_it) ) {
						ticks += (*times_it)->eventCount;
						times_it ++;
					}
				}
			}
			
			bool tempres_erased = ( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() );
			bool need_continue = !view->doUnwrapTime() ? ( values.size() > 1 ) : true;
			if ( tempres_erased ) {
				if ( !view->doUnwrapTime() ) {
					need_continue = ( it != values.end() && (*it)->modeltime->time > view->drawToX.time );
				} else {
					need_continue = ( it != values.end() && ( (*it)->modeltime->time > view->drawToX.time || ( (*it)->modeltime->time == view->drawToX.time && (*it)->eventIndex > view->drawToEventCount ) ) );
				}
			}

			if ( need_continue ) {
				if ( view->drawFromX == view->drawToX ) {
					x = rect.left + ( view->drawToEventCount - view->drawFromEventIndex ) * view->style->fonts_ticks->tickWidth;
					x = min( x, rect.right - 1 );
				} else {
					x = rect.right - 1;
				}
				dc.LineTo( x, lasty );
			}
		}
	}
	dc.SelectObject( pOldPen );
	dc.SetBkMode( oldBkMode );
}

void RDOTracerSerie::drawMarker( CDC &dc, const int x, const int y, const COLORREF color ) const
{
	CPen pen;
	CRect rect;
	rect.left = x - 3;
	rect.top = y - 3;
	rect.bottom = y + 3;
	rect.right = x + 3;
	pen.CreatePen( PS_SOLID, 0, color );
	CPen* pOldPen = dc.SelectObject( &pen );
	dc.Ellipse( &rect );
	dc.SelectObject( pOldPen );
}

int RDOTracerSerie::addToDoc( RDOStudioChartDoc* const doc )
{
	int res = -1;
	if ( !doc ) return res;
	if ( find( documents.begin(), documents.end(), doc ) == documents.end() ) {
		documents.push_back( doc );
		res = documents.size() - 1;
	}
	return res;
}

void RDOTracerSerie::removeFromDoc( RDOStudioChartDoc* const doc )
{
	vector <RDOStudioChartDoc*>::iterator it = find( documents.begin(), documents.end(), doc );
	if ( it != documents.end() ) {
		documents.erase( it );
	}
}

bool RDOTracerSerie::activateFirstDoc() const
{
	bool res = false;
	if ( documents.empty() ) return res;
	RDOStudioChartDoc* doc = documents.front();
	if ( doc ) {
		POSITION pos = doc->GetFirstViewPosition();
		RDOStudioChartView* view = NULL;
		if ( pos )
			view = static_cast<RDOStudioChartView*>(doc->GetNextView( pos ));
		if ( view ) {
			studioApp.mainFrame->MDIActivate( view->GetParentFrame() );
			res = true;
		}
	}
	return res;
}
