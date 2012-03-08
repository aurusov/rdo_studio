#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerSerieFindValue
// --------------------------------------------------------------------------------
class RDOTracerSerieFindValue
{
	RDOStudioChartView* view;
public:
	RDOTracerSerieFindValue( RDOStudioChartView* _view ): view( _view ) {};
	rbool operator ()( RDOTracerValue* val );
};

rbool RDOTracerSerieFindValue::operator ()( RDOTracerValue* val )
{
	rbool res = val && val->modeltime->time >= view->drawFromX.time;
	if ( view->doUnwrapTime() && res && ( val->modeltime->time == view->drawFromX.time ) )
		res = val->eventIndex >= view->drawFromEventIndex;
	return res;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerSerie
// --------------------------------------------------------------------------------
RDOTracerSerie::RDOTracerSerie( RDOTracerSerieKind _serieKind ) :
	RDOTracerTreeItem( true ),
	serieKind( _serieKind ),
	minValue( 0 ),
	maxValue( 0 ),
	value_count( 0 )
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

rbool RDOTracerSerie::isTemporaryResourceParam() const
{
	return serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->getType()->getResTypeKind() == RDOTK_TEMPORARY;
};

tstring RDOTracerSerie::getTitle() const
{
	return title;
}

void RDOTracerSerie::setTitle( CREF(tstring) value )
{
	if ( title != value )
		title = value;
}

void RDOTracerSerie::addValue( RDOTracerValue* const value )
{
	std::vector< RDOStudioChartDoc* >::iterator it;
	for ( it = documents.begin(); it != documents.end(); it++ )
		(*it)->lock();

	mutex.Lock();

	if ( value->value < minValue || values.empty() )
		minValue = value->value;
	if ( value->value > maxValue || values.empty() )
		maxValue = value->value;

	values.push_back( value );

	value_count++;

	std::for_each( documents.begin(), documents.end(), std::bind2nd( std::mem_fun1( &RDOStudioChartDoc::newValueToSerieAdded ), value ) );
	
	mutex.Unlock();

	for ( it = documents.begin(); it != documents.end(); it++ )
		(*it)->unlock();
}

void RDOTracerSerie::getValueCount( int& count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	count = value_count;

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptions( std::vector<tstring> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	if ( !captions.empty() )
		captions.clear();
	if ( serieKind == RDOST_PREVIEW ) {
		double valoffset = ( maxValue - minValue ) / (double)( val_count - 1 );
		double valo = minValue;
		tstring formatstr = "%.3f";
		if ( value_count > 1 ) {
			for ( int i = 0; i < val_count; i++ ) {
				captions.push_back( rdo::format( formatstr.c_str(), valo ) );
				valo += valoffset;
			}
		} else {
			captions.push_back( rdo::format( formatstr.c_str(), valo ) );
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsInt( std::vector<tstring> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	RDOTracerSerie::getCaptions( captions, val_count );

	int real_val_count = val_count;
	if ( ( maxValue - minValue + 1 ) > real_val_count ) {
		while ( (int)(( maxValue - minValue ) / ( real_val_count - 1 )) != (double)(( maxValue - minValue ) / ( real_val_count - 1 )) )
			real_val_count--;
	} else {
		real_val_count = (int)(maxValue - minValue + 1);
	}
	int valo = (int)minValue;
	int valoffset = (int)(( maxValue - minValue ) / ( real_val_count - 1 ));
	tstring formatstr = "%d";
	for ( int i = 0; i < real_val_count; i++ ) {
		captions.push_back( rdo::format( formatstr.c_str(), valo ) );
		valo += valoffset;
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsDouble( std::vector<tstring> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	RDOTracerSerie::getCaptions( captions, val_count );
	
	double valoffset = ( maxValue - minValue ) / (double)( val_count - 1 );
	double valo = minValue;
	tstring formatstr = "%.3f";
	if ( value_count > 1 ) {
		for ( int i = 0; i < val_count; i++ ) {
			captions.push_back( rdo::format( formatstr.c_str(), valo ) );
			valo += valoffset;
		}
	} else {
		captions.push_back( rdo::format( formatstr.c_str(), valo ) );
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::getCaptionsBool( std::vector<tstring> &captions, const int val_count ) const
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

void RDOTracerSerie::drawSerie( RDOStudioChartView* const view, HDC &dc, CRect &rect, const COLORREF color, RDOTracerSerieMarker marker, const int marker_size, const rbool draw_marker, const rbool transparent_marker ) const
{
	const_cast<CMutex&>(mutex).Lock();
	
	if ( !values.empty() ) {
		
		valuesList::const_iterator it = std::find_if( values.begin(), values.end(), RDOTracerSerieFindValue( view ) );

		if ( it == values.end() && !values.empty() && !isTemporaryResourceParam() ) {
			it --;
		}
		
		rbool flag = it != values.end();
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
			
			int lasty = roundDouble( (double)rect.bottom - double(ky) * ( (*it)->value - minValue ) );
			lasty = min( lasty, rect.bottom - 1 );
			int lastx = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * double(view->timeScale) ) - view->chartShift;
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

			HPEN pen = NULL;
			HPEN old_pen = NULL;
			
			HBRUSH brush_marker = NULL;
			HBRUSH old_brush = NULL;
			LOGBRUSH log_brush;
			log_brush.lbStyle = transparent_marker ? BS_HOLLOW : BS_SOLID;
			log_brush.lbColor = color;
			try {
				pen = ::CreatePen( PS_SOLID, 0, color );
				old_pen = (HPEN)::SelectObject( dc, pen );
				brush_marker = ::CreateBrushIndirect( &log_brush );
				old_brush = (HBRUSH)::SelectObject( dc, brush_marker );

				if ( lastx >= rect.left && draw_marker ) {
					drawMarker( dc, lastx, lasty, marker, marker_size );
					::MoveToEx( dc, lastx, lasty, (LPPOINT)NULL );
				}
				else
					::MoveToEx( dc, rect.left, lasty, (LPPOINT)NULL );
				
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
					y = roundDouble( (double)rect.bottom - double(ky) * ( (*it)->value - minValue ) );
					y = min( y, rect.bottom - 1 );
					x = rect.left + roundDouble( ( (*it)->modeltime->time - view->drawFromX.time ) * double(view->timeScale) ) - view->chartShift;
					if ( view->doUnwrapTime() ) {
						x += ( ticks + (*it)->eventIndex ) * view->style->fonts_ticks->tickWidth;
					}
					x = min( x, rect.right - 1 );
					if ( draw_marker )
						drawMarker( dc, x, y, marker, marker_size );
					::LineTo( dc, x, lasty );
					::LineTo( dc, x, y );
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
				
				rbool tempres_erased = ( serieKind == RDOST_RESPARAM && ((RDOTracerResParam*)this)->getResource()->isErased() );
				rbool need_continue = !view->doUnwrapTime() ? ( values.size() > 1 ) : true;
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
					::LineTo( dc, x, lasty );
				}
				
				::SelectObject( dc, old_pen );
				::DeleteObject( pen );
				pen = NULL;

				::SelectObject( dc, old_brush );
				::DeleteObject( brush_marker );
				brush_marker = NULL;
			} catch( ... ) {
				if ( pen ) {
					::SelectObject( dc, old_pen );
					::DeleteObject( pen );
					pen = NULL;
				}
				if ( brush_marker ) {
					::SelectObject( dc, old_brush );
					::DeleteObject( brush_marker );
					brush_marker = NULL;
				}
			}
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerSerie::drawMarker( HDC &dc, const int x, const int y, RDOTracerSerieMarker marker, const int marker_size ) const
{
	CRect rect;
	rect.left = x - marker_size;
	rect.top = y - marker_size;
	rect.bottom = y + marker_size;
	rect.right = x + marker_size;

	switch( marker ) {
		case RDOSM_CIRCLE : {
			::Ellipse( dc, rect.left, rect.top, rect.right, rect.bottom );
			break;
		}
		case RDOSM_SQUARE : {
			::Rectangle( dc, rect.left, rect.top, rect.right, rect.bottom );
			break;
		}
		case RDOSM_RHOMB : {
			POINT pts[4];
			pts[0].x = rect.left + ( rect.right - rect.left ) / 2;
			pts[0].y = rect.top;
			pts[1].x = rect.right;
			pts[1].y = rect.top + ( rect.bottom - rect.top ) / 2;
			pts[2].x = pts[0].x;
			pts[2].y = rect.bottom;
			pts[3].x = rect.left;
			pts[3].y = pts[1].y;
			::Polygon( dc, pts, 4 );
			break;
		}
		case RDOSM_CROSS : {
			POINT pos;
			::GetCurrentPositionEx( dc, &pos );
			
			::MoveToEx( dc, rect.left, rect.top, (LPPOINT)NULL );
			::LineTo( dc, rect.right, rect.bottom + 1 );
			::MoveToEx( dc, rect.left, rect.bottom, (LPPOINT)NULL );
			::LineTo( dc, rect.right, rect.top - 1 );
			::MoveToEx( dc, pos.x, pos.y, (LPPOINT)NULL );
			break;
		}
	}
}

void RDOTracerSerie::addToDoc( RDOStudioChartDoc* const doc )
{
	mutex.Lock();
	
	if ( doc && std::find( documents.begin(), documents.end(), doc ) == documents.end() ) {
		documents.push_back( doc );
	}

	mutex.Unlock();
}

void RDOTracerSerie::removeFromDoc( RDOStudioChartDoc* const doc )
{
	mutex.Lock();
	
	std::vector <RDOStudioChartDoc*>::iterator it = std::find( documents.begin(), documents.end(), doc );
	if ( it != documents.end() ) {
		documents.erase( it );
	}

	mutex.Unlock();
}

rbool RDOTracerSerie::activateFirstDoc() const
{
	const_cast<CMutex&>(mutex).Lock();
	
	rbool res = false;
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
