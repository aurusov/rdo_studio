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
	mutex.Lock();

	valuesList::iterator it = values.begin();
	while ( it != values.end() ) {
		delete *it;
		it++;
	}
	values.clear();
	documents.clear();

	mutex.Unlock();
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

void RDOTracerSerie::addValue( RDOTracerValue* const value )
{
	for ( vector< RDOStudioChartDoc* >::iterator it = documents.begin(); it != documents.end(); it++ )
		(*it)->lock();

	mutex.Lock();

	values.push_back( value );
	if ( value->value < minValue || values.empty() )
		minValue = value->value;
	if ( value->value > maxValue || values.empty() )
		maxValue = value->value;
	for_each( documents.begin(), documents.end(), bind2nd( mem_fun1( &RDOStudioChartDoc::newValueToSerieAdded ), value ) );
	
	mutex.Unlock();

	for ( it = documents.begin(); it != documents.end(); it++ )
		(*it)->unlock();
}

void RDOTracerSerie::getValueCount( int& count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	count = values.size();

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptions( vector<string> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	if ( !captions.empty() )
		captions.clear();
	if ( serieKind == RDOST_PREVIEW ) {
		double valoffset = ( maxValue - minValue ) / (double)( val_count - 1 );
		double valo = minValue;
		string formatstr = "%.3f";
		for ( int i = 0; i < val_count; i++ ) {
			captions.push_back( format( formatstr.c_str(), valo ) );
			valo += valoffset;
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsInt( vector<string> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	RDOTracerSerie::getCaptions( captions, val_count );

	int real_val_count = val_count;
	if ( ( maxValue - minValue + 1 ) > real_val_count ) {
		while ( (int)(( maxValue - minValue ) / ( real_val_count - 1 )) != (double)(( maxValue - minValue ) / ( real_val_count - 1 )) )
			real_val_count--;
	} else {
		real_val_count = maxValue - minValue + 1;
	}
	int valo = minValue;
	int valoffset = ( maxValue - minValue ) / ( real_val_count - 1 );
	string formatstr = "%d";
	for ( int i = 0; i < real_val_count; i++ ) {
		captions.push_back( format( formatstr.c_str(), valo ) );
		valo += valoffset;
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsDouble( vector<string> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	RDOTracerSerie::getCaptions( captions, val_count );
	
	double valoffset = ( maxValue - minValue ) / (double)( val_count - 1 );
	double valo = minValue;
	string formatstr = "%.3f";
	for ( int i = 0; i < val_count; i++ ) {
		captions.push_back( format( formatstr.c_str(), valo ) );
		valo += valoffset;
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsBool( vector<string> &captions, const int val_count ) const
{
	RDOTracerSerie::getCaptions( captions, val_count );
	captions.push_back( "FALSE" );
	captions.push_back( "TRUE" );
}

void RDOTracerSerie::getLastValue( RDOTracerValue*& val ) const
{
	const_cast<CMutex&>(mutex).Lock();

	if ( !values.size() )
		val = NULL;
	else
		val = values.back();

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color, RDOTracerSerieMarker marker, const int marker_size, const bool draw_marker, const bool transparent_marker ) const
{
	const_cast<CMutex&>(mutex).Lock();
	
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
			int lastx = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * view->timeScale ) - view->chartShift;
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

			if ( lastx >= rect.left && draw_marker ) {
				drawMarker( dc, lastx, lasty, color, marker, marker_size, transparent_marker );
				dc.MoveTo( lastx, lasty );
			}
			else
				dc.MoveTo( rect.left, lasty );
			
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
				x = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * view->timeScale ) - view->chartShift;
				if ( view->doUnwrapTime() ) {
					x += ( ticks + (*it)->eventIndex ) * view->style->fonts_ticks->tickWidth;
				}
				x = min( x, rect.right - 1 );
				if ( draw_marker )
					drawMarker( dc, x, y, color, marker, marker_size, transparent_marker );
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

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::drawMarker( CDC &dc, const int x, const int y, const COLORREF color, RDOTracerSerieMarker marker, const int marker_size, const bool transparent_marker ) const
{
	CRect rect;
	rect.left = x - marker_size;
	rect.top = y - marker_size;
	rect.bottom = y + marker_size;
	rect.right = x + marker_size;
	CPen pen;
	pen.CreatePen( PS_SOLID, 0, color );
	CPen* pOldPen = dc.SelectObject( &pen );
	CBrush brush;
	LOGBRUSH log_brush;
	log_brush.lbStyle = transparent_marker ? BS_HOLLOW : BS_SOLID;
	log_brush.lbColor = color;
	brush.CreateBrushIndirect( &log_brush );
	CBrush* old_brush = dc.SelectObject( &brush );
	switch( marker ) {
		case RDOSM_CIRCLE : {
			drawSircle( dc, rect, color );
			break;
		}
		case RDOSM_SQUARE : {
			drawSquare( dc, rect, color );
			break;
		}
		case RDOSM_RHOMB : {
			drawRhomb( dc, rect, color );
			break;
		}
		case RDOSM_CROSS : {
			drawCross( dc, rect, color );
			break;
		}
	}
	dc.SelectObject( &old_brush );
	dc.SelectObject( pOldPen );
}

void RDOTracerSerie::drawSircle( CDC &dc, CRect& rect, const COLORREF color ) const
{
	dc.Ellipse( &rect );
}

void RDOTracerSerie::drawSquare( CDC &dc, CRect& rect, const COLORREF color ) const
{
	dc.Rectangle( &rect );
}

void RDOTracerSerie::drawRhomb( CDC &dc, CRect& rect, const COLORREF color ) const
{
	CPoint pts[4];
	pts[0].x = rect.left + ( rect.right - rect.left ) / 2;
	pts[0].y = rect.top;
	pts[1].x = rect.right;
	pts[1].y = rect.top + ( rect.bottom - rect.top ) / 2;
	pts[2].x = pts[0].x;
	pts[2].y = rect.bottom;
	pts[3].x = rect.left;
	pts[3].y = pts[1].y;
	dc.Polygon( pts, 4 );
}

void RDOTracerSerie::drawCross( CDC &dc, CRect& rect, const COLORREF color ) const
{
	CPoint pos = dc.GetCurrentPosition();
	dc.MoveTo( rect.left, rect.top );
	dc.LineTo( rect.right, rect.bottom + 1 );
	dc.MoveTo( rect.left, rect.bottom );
	dc.LineTo( rect.right, rect.top - 1 );
	dc.MoveTo( pos );
}

void RDOTracerSerie::addToDoc( RDOStudioChartDoc* const doc )
{
	mutex.Lock();
	
	if ( doc && find( documents.begin(), documents.end(), doc ) == documents.end() ) {
		documents.push_back( doc );
	}

	mutex.Unlock();
}

void RDOTracerSerie::removeFromDoc( RDOStudioChartDoc* const doc )
{
	mutex.Lock();
	
	vector <RDOStudioChartDoc*>::iterator it = find( documents.begin(), documents.end(), doc );
	if ( it != documents.end() ) {
		documents.erase( it );
	}

	mutex.Unlock();
}

bool RDOTracerSerie::activateFirstDoc() const
{
	const_cast<CMutex&>(mutex).Lock();
	
	bool res = false;
	if ( !documents.empty() ) {
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
	}

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}
