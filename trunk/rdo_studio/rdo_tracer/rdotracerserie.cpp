#include "stdafx.h"
#include "rdotracerserie.h"
#include "rdotracervalues.h"
#include "rdotracerresource.h"
#include "rdotracerrestype.h"
#include "../rdostudiochartview.h"
#include "../rdostudiochartdoc.h"

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
	if( val && val->modeltime->time >= view->drawFromX )
		return true;
	return false;
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

/*list< RDOTracerValue* >::const_iterator RDOTracerSerie::findValue( RDOTracerValue* const value ) const
{
	//int res = -1;
	//for ( list< RDOTracerValue* >::const_iterator it = values.begin(); it != values.end(); it++ ) {
	//	res ++;
	//	if ( (*it) == value ) {
	//		break;
	//	}
	//}
	//if ( it == values.end() ) res = -1;
	//return res;
	list< RDOTracerValue* >::const_iterator it = find( values.begin(), values.end(), value );
	if ( it == values.end() ) return -1;
	return it;
}*/

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

/*RDOTracerValue* RDOTracerSerie::getValue( const int index ) const
{
	if ( index >= values.size() || index < 0 )
		return NULL;
	list< RDOTracerValue* >::const_iterator it = values.begin();
	for ( int i = 0; i < index; i++ )
		it++;
	return (*it);
}*/

/*list< RDOTracerValue* >::const_iterator RDOTracerSerie::getValueIterator( const int index ) const
{
	list< RDOTracerValue* >::const_iterator it = values.begin();
	if ( index >= values.size() || index < 0 )
		return it;
	for ( int i = 0; i < index; i++ )
		it++;
	return it;
}*/

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

void RDOTracerSerie::getFromToValues( RDOTracerTimeNow* const from, const int fromEvent, RDOTracerTimeNow* const to, const int toEvent, RDOTracerValue* &begin, RDOTracerValue* &end ) const
{
	begin = NULL;
	end = NULL;
	/*if ( values.empty() )
		return;

	vector <RDOTracerValue*>::const_iterator it = values.begin();
	while ( it != values.end() && ( (*it)->modeltime->time < from->time || ( (*it)->modeltime->time == from->time && (*it)->eventIndex <= fromEvent ) ) ) {
		it ++;
	}
	if ( it = values.end() )
		it --;
	bool more = (*it)->modeltime->time > from->time || ( (*it)->modeltime->time == from->time && (*it)->eventIndex > fromEvent );
	if ( it != values.begin() && more )
		it --;
	begin = (*it);

	it = values.end();
	it --;
	while ( it >= values.begin() && ( (*it)->modeltime->time > to->time || ( (*it)->modeltime->time == to->time && (*it)->eventIndex >= toEvent ) ) ) {
		it --;
	}
	bool less = (*it)->modeltime->time < to->time || ( (*it)->modeltime->time == to->time && (*it)->eventIndex < fromEvent );
	if ( it < values.end() - 1 && less )
		it ++;
	end = (*it);*/
}

void RDOTracerSerie::getMinMaxValues( RDOTracerValue* const begin, RDOTracerValue* const end, double &min, double &max ) const
{
	/*if ( values.empty() ) {
		min = 0;
		max = 0;
		return;
	}
	
	for ( vector <RDOTracerValue*>::const_iterator it = values.begin(); it != values.end(); it++ ) {
		if ( (*it) == begin ) {
			break;
		}
	}
	
	min = (*it)->value;
	max = min;
	
	if ( it!= values.end() ) {
		while ( it != values.end() ) {
			if ( (*it)->value > max  )
				max = (*it)->value;
			if ( (*it)->value < min )
				min = (*it)->value;
			if ( (*it) == end )
				break;
			it ++;
		}
	}*/
}

void RDOTracerSerie::drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color ) const
{
	int oldBkMode = dc.SetBkMode( TRANSPARENT );
	CPen pen;
	pen.CreatePen( PS_SOLID, 0, color );
	CPen* pOldPen = dc.SelectObject( &pen );
	/*long double ky;
	if ( maxValue != minValue )
		ky = rect.Height() / ( maxValue - minValue );
	else
		ky = 0;
	int count = values.size();
	if ( count ) {
		int prevx = rect.left + view->timeScale * values.at( 0 )->modeltime->time;
		if ( !view->timeWrap )
			prevx += view->tickWidth * values.at( 0 )->eventIndex;
		int prevy = rect.bottom - ky * values.at( 0 )->value;
		if ( !view->timeWrap && !isTemporaryResourceParam() ) {
			dc.MoveTo( rect.left, prevy );
			dc.LineTo( prevx, prevy );
		}
		dc.MoveTo( prevx, prevy );
		int x = prevx, y = prevy;
		RDOTracerValue* val = NULL;
		for ( int i = 1; i < count; i++ ) {
			val = values.at( i );
			y = min( rect.bottom - ky * val->value, rect.bottom );
			x = rect.left;
			int wrapoffset = 0;
			if ( !view->timeWrap )
				wrapoffset = view->tickWidth * ( val->modeltime->overallCount - val->modeltime->eventCount + val->eventIndex );
			x = min( rect.left + view->timeScale * val->modeltime->time + wrapoffset, rect.right );
			dc.LineTo( x, prevy );
			dc.LineTo( x, y );
			prevx = x;
			prevy = y;
		}
		if ( !( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() ) && x < rect.right ) {
			x = rect.right;
			if ( !view->timeWrap && count == 1 )
				x = rect.left + view->tickWidth * values.at( 0 )->modeltime->eventCount;
			else if ( count == 1 )
				x = prevx;
				
			dc.LineTo( x, y );
		}
	}*/
	
	//new
	/*RDOTracerValue* start = NULL;
	RDOTracerValue* end = NULL;
	getFromToValues( view->drawFromX, view->drawFromXEventIndex, view->drawToX, view->drawToXEventIndex, start, end );
	double min = 0;
	double max = 0;
	getMinMaxValues( start, end, min, max );
	long double ky = 0;
	if ( min != max )
		ky = rect.Height() / ( max - min );
	if ( !values.empty() ) {
		//int prevx = rect.left + view->timeScale * values.at( 0 )->modeltime->time;
		//if ( !view->timeWrap )
		//	prevx += view->tickWidth * values.at( 0 )->eventIndex;
		int prevx = rect.left;
		if ( isTemporaryResourceParam() )
			prevx += view->timeScale * start->modeltime->time;
		if ( !view->timeWrap )
			prevx += view->tickWidth * start->eventIndex;
		int prevy = rect.bottom - ky * ( start->value - min );
		//if ( !view->timeWrap && !isTemporaryResourceParam() ) {
		//	dc.MoveTo( rect.left, prevy );
		//	dc.LineTo( prevx, prevy );
		//}
		dc.MoveTo( prevx, prevy );
		int x = prevx, y = prevy;
		RDOTracerValue* val = NULL;
		int startindex = findValue( start ) + 1;
		int endindex = findValue( end );
		for ( int i = startindex; i <= endindex; i++ ) {
			val = values.at( i );
			y = min( rect.bottom - ky * ( val->value - min ), rect.bottom );
			x = rect.left;
			int wrapoffset = 0;
			if ( !view->timeWrap )
				wrapoffset = view->tickWidth * ( val->modeltime->overallCount - val->modeltime->eventCount + val->eventIndex );
			x = max( rect.left, min( rect.left + view->timeScale * ( val->modeltime->time - view->drawFromX->time ) - view->pixelsToChart + wrapoffset, rect.right ) );
			dc.LineTo( x, prevy );
			dc.LineTo( x, y );
			prevx = x;
			prevy = y;
		}
		if ( !( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() ) && x < rect.right ) {
			x = rect.right;
			//if ( !view->timeWrap && count == 1 )
			//	x = rect.left + view->tickWidth * values.at( 0 )->modeltime->eventCount;
			//else if ( count == 1 )
			//	x = prevx;
				
			dc.LineTo( x, y );
		}
	}*/
	if ( !values.empty() ) {
		
		valuesList::const_iterator it = find_if( values.begin(), values.end(), RDOTracerSerieFindValue( view ) );
		
		if ( it != values.end() && !( it == values.begin() && (*it)->modeltime->time > view->drawToX ) ) {
			
			long double ky;
			if ( maxValue != minValue )
				ky = rect.Height() / ( maxValue - minValue );
			else
				ky = 0;
			
			if ( it != values.begin() && (*it)->modeltime->time > view->drawFromX )
				it --;
			
			int lasty = roundDouble( rect.bottom - ky * ( (*it)->value - minValue ) );
			int lastx = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX ) * view->timeScale );
			if ( lastx >= rect.left )
				drawMarker( dc, lastx, lasty, color );
			else
				lastx = rect.left;
			dc.MoveTo( lastx, lasty );
			
			int x = lastx, y = lasty;
			it ++;
			while ( it != values.end() && (*it)->modeltime->time <= view->drawToX ) {
				y = roundDouble( rect.bottom - ky * ( (*it)->value - minValue ) );
				x = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX ) * view->timeScale );
				drawMarker( dc, x, y, color );
				dc.LineTo( x, lasty );
				dc.LineTo( x, y );
				lastx = x;
				lasty = y;
				it ++;
			}
			
			bool tempres_erased = ( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() );
			bool need_continue = true;
			if ( tempres_erased )
				need_continue = ( it != values.end() && (*it)->modeltime->time > view->drawToX );

			if ( need_continue )
				dc.LineTo( rect.right, lasty );
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
