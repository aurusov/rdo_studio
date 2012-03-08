/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/view.cpp
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/src/chart/view_style.h"
#include "app/rdo_studio_mfc/src/chart/doc_serie.h"
#include "app/rdo_studio_mfc/src/chart/options.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartView
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartView, RDOStudioView)

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP(RDOStudioChartView, RDOStudioView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_CHART_TIMEWRAP, OnChartTimewrap)
	ON_UPDATE_COMMAND_UI(ID_CHART_TIMEWRAP, OnUpdateChartTimewrap)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnChartZoomZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnChartZoomZoomout)
	ON_COMMAND(ID_VIEW_ZOOMRESET, OnChartZoomResetzoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateChartZoomZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateChartZoomZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMRESET, OnUpdateChartZoomResetzoom)
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_CHART_OPTIONS, OnChartOptions)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(ID_VIEW_ZOOMAUTO, OnViewZoomauto)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMAUTO, OnUpdateViewZoomauto)
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	ON_MESSAGE(WM_USER_UPDATE_CHART_VIEW, OnUserUpdateChartView)
END_MESSAGE_MAP()

RDOStudioChartView::RDOStudioChartView( const rbool preview )
	: RDOStudioView(),
	bmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	dragedSerie( NULL ),
	valueCountX( 5 ),
	valueCountY( 5 ),
	yAxis( NULL ),
	timeWrap( true ),
	chartRect( 0, 0, 0, 0 ),
	xMax( 0 ),
	xPos( 0 ),
	timeScale( 0 ),
	drawFromEventIndex( 0 ),
	drawToEventCount( 0 ),
	chartShift( 0 ),
	zoom( 1 ),
	old_zoom( 1 ),
	auto_zoom( 1 ),
	zoomAuto( false ),
	scale_koeff( 1 ),
	style( NULL ),
	previewMode( preview ),
	legendRect( 0, 0, 0, 0 ),
	needDrawLegend( true ),
	hbmp( NULL),
	hbmpInit( NULL ),
	hfontTitle( NULL),
	hfontLegend( NULL),
	hfontAxis( NULL),
	hfontInit( NULL),
	hdc( NULL ),
	saved_hdc( 0 ),
	hmemdc( NULL ),
	saved_hmemdc( 0 ),
	hwnd( NULL )
{
	if ( previewMode )
		timeWrap = false;
}

RDOStudioChartView::~RDOStudioChartView()
{
}

BOOL RDOStudioChartView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !RDOStudioView::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL /*| WS_VSCROLL*/ | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//Setting class style CS_OWNDC to avoid DC releasing
	//and selecting previous objects into it
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int RDOStudioChartView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( !target.Register( this ) )
		return -1;
	if ( RDOStudioView::OnCreate( lpCreateStruct ) == -1 ) return -1;
	
	//Remembering handle to the window in hwnd member
	hwnd = GetSafeHwnd();
	//Remembering handle to the private device context in hdc member
	if ( hwnd )
		hdc = ::GetDC( hwnd );
	if ( !hdc ) return -1;

	//Creating memory device context to draw on bitmap
	hmemdc = ::CreateCompatibleDC( hdc );
	if ( !hmemdc ) return -1;
	//Saving the own DC and memory DC states to restore them
	//before deleting the memory DC or destroying window
	saved_hdc    = ::SaveDC( hdc );
	saved_hmemdc = ::SaveDC( hmemdc );
	if ( !saved_hdc || !saved_hmemdc ) return -1;
	//Setting background mode one time in initialization.
	//We have private DC, so we needn't to reset it each time
	//we paint
	::SetBkMode( hmemdc, TRANSPARENT );
	//Remembering default font to select it into DC
	//when destroying window or setting new font
	hfontInit = (HFONT)::GetCurrentObject( hmemdc, OBJ_FONT );
	//Remembering default bmp to select it into DC
	//when resizing window
	hbmpInit  = (HBITMAP)::GetCurrentObject( hmemdc, OBJ_BITMAP );

	if ( !previewMode )
		setStyle( &studioApp.mainFrame->style_chart, false );

	if ( GetDocument() ) {
		recalcLayout();
		updateScrollBars();
	}

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	
	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 3 + delta ), 6, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 3 + delta ), 7, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 3 + delta ), 8, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 3 + delta ), 9, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 3, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 5, &popupMenu );

	return 0;
}

void RDOStudioChartView::recalcLayout()
{
	RDOStudioChartDoc* doc = GetDocument();
	doc->mutex.Lock();

	mutex.Lock();
	
	::SelectObject( hmemdc, hfontTitle );
	
	CRect tmprect;
	tmprect.CopyRect( &newClientRect );
	tstring str = doc->getTitle();
	::DrawText( hmemdc, str.c_str(), str.length(), tmprect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );
	chartRect.top = tmprect.Height() + 5;

	::SelectObject( hmemdc, hfontAxis );

	SIZE sz;
	SIZE size_max;
	size_max.cx = 0;
	size_max.cy = 0;
	if ( yAxis ) {
		yAxis->getCaptions( captions, valueCountY );
		for ( std::vector<tstring>::iterator it = captions.begin(); it != captions.end(); it++ ) {
			::GetTextExtentPoint32( hmemdc, (*it).c_str(), (*it).length(), &sz );
			if ( sz.cx > size_max.cx ) size_max.cx = sz.cx;
		}
	}
	chartRect.left = size_max.cx + 10;

	str = !doc->docTimes.empty() ? rdo::format( "%.3f", doc->docTimes.back()->time ) : rdo::format( "%.3f", 0 );
	::GetTextExtentPoint32( hmemdc, str.c_str(), str.length(), &sz );
	chartRect.right = newClientRect.right - sz.cx - 5;

	chartRect.bottom = newClientRect.bottom - sz.cy - 7;

	if ( needDrawLegend ) {
		
		::SelectObject( hmemdc, hfontLegend );

		int count = 0;
		for ( std::vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
			(*it)->getLegendExtent( hmemdc, chartRect, sz );
			if ( sz.cx && sz.cy ) {
				if ( sz.cx > size_max.cx )
					size_max.cx = sz.cx;
				if ( sz.cy > size_max.cy )
					size_max.cy = sz.cy;
				count ++;
			}
		}

		legendRect.top = chartRect.top;
		legendRect.bottom = legendRect.top + count * size_max.cy + 3 + doc->getMaxMarkerSize() / 2;
		legendRect.left = chartRect.left + ( chartRect.Width() - size_max.cx ) / 2;
		legendRect.right = legendRect.left + size_max.cx;
		chartRect.top += legendRect.Height();
	}

	if ( !doc->docTimes.empty() ) {
		double timeRange = doc->docTimes.back()->time - doc->docTimes.front()->time;
		if ( timeRange > 0 ) {
			long double timeScaleAuto = doUnwrapTime() ? (double)( chartRect.Width() - style->fonts_ticks->tickWidth * doc->ticksCount ) / timeRange : (double)chartRect.Width() / timeRange;
			timeScale = (double)style->fonts_ticks->tickWidth / doc->minTimeOffset;
			auto_zoom = double(timeScaleAuto) / double(timeScale);
			/*if ( doUnwrapTime() && auto_zoom < 0 ) {
				auto_zoom = 1;
			}*/
			if ( zoomAuto || ( auto_zoom > 1 /*&& scale_koeff < auto_zoom*/ ) ) {
				scale_koeff = auto_zoom;
			}
			if ( !zoomAuto && auto_zoom <= 1 ) {
				scale_koeff = zoom;
			}
		} else {
			timeScale = 0;
			scale_koeff = 1;
		}
	} else {
		timeScale = 0;
		scale_koeff = 1;
	}

	timeScale *= scale_koeff;
	
	mutex.Unlock();

	doc->mutex.Unlock();
}

void RDOStudioChartView::setScrollPos( UINT nSBCode, UINT nPos, const rbool need_update )
{
	if ( nSBCode == SB_HORZ )
		xPos = nPos;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_POS;
	si.nPos   = xPos;
	SetScrollInfo( nSBCode, &si, TRUE );

	if ( need_update ) {
		Invalidate();
		updateWindow();
	}
}

void RDOStudioChartView::updateScrollBars( const rbool need_update )
{
	RDOStudioChartDoc* doc = GetDocument();
	doc->mutex.Lock();

	int size;
	if ( !doc->docTimes.empty() ) {
		size = roundDouble( ( doc->docTimes.back()->time - doc->docTimes.front()->time ) * double(timeScale) );
		if ( doUnwrapTime() )
			size += style->fonts_ticks->tickWidth * doc->ticksCount;
	} else {
		size = 0;
	}

	xMax = max ( 0, size - chartRect.Width() );
	xPos = min ( xPos, xMax );
	
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = size - 1;
	si.nPage  = chartRect.Width(); 
	si.nPos   = xPos;
	SetScrollInfo( SB_HORZ, &si, need_update );

	doc->mutex.Unlock();
}

rbool RDOStudioChartView::setTo( const int from_max_pos )
{
	rbool res = true;
	int delta = ( from_max_pos - xPos - chartRect.Width() ) / style->fonts_ticks->tickWidth;
	if ( delta >= 0 ) {
		res = false;
		drawToX = drawFromX;
		drawToEventCount = drawToX.eventCount - delta;
	}
	return res;
}

void RDOStudioChartView::setFromTo()
{
	RDOStudioChartDoc* doc = GetDocument();

	drawFromX.eventCount = 0;
	drawFromEventIndex = 0;
	drawToX.eventCount = 0;
	drawToEventCount = 0;
	chartShift = 0;
	unwrapTimesList.clear();

	if ( !doUnwrapTime() ) {
		if ( timeScale ) {
			drawFromX.time = doc->docTimes.front()->time + (double)xPos / double(timeScale);
			if ( maxXVisible() )
				drawToX.time = doc->docTimes.back()->time;
			else
				drawToX.time = doc->docTimes.front()->time + (double)( xPos + chartRect.Width() ) / double(timeScale);
		} else {
			drawFromX.time = 0;
			drawToX.time = 0;
		}
	} else {
		int it_pos = 0;
		int it_max_pos = 0;
		rbool need_search_to = true;
		int ticks = 0;
		timesList::iterator it;
		for( it = doc->docTimes.begin(); it != doc->docTimes.end(); it++ ) {
			it_pos = roundDouble( ( (*it)->time - doc->docTimes.front()->time ) * double(timeScale) ) + ticks * style->fonts_ticks->tickWidth;
			it_max_pos = it_pos + style->fonts_ticks->tickWidth * (*it)->eventCount;
			if ( it_pos == xPos ) {
				drawFromX = *(*it);
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos < xPos && ( it_max_pos >= xPos ) ) {
				drawFromX = *(*it);
				drawFromEventIndex = ( xPos - it_pos ) / style->fonts_ticks->tickWidth;
				chartShift = xPos - ( it_pos + drawFromEventIndex * style->fonts_ticks->tickWidth );
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos > xPos ) {
				drawFromX.time = (*it)->time - ( it_pos - xPos ) / double(timeScale);
				need_search_to = setTo( 0 );
				break;
			}
			ticks += (*it)->eventCount;
		}
		unwrapTimesList.push_back( &drawFromX );
		if ( need_search_to ) {
			if ( it != doc->docTimes.end() && drawFromX == *(*it) ) {
				ticks += (*it)->eventCount;
				it ++;
			}
			if ( it == doc->docTimes.end() && !doc->docTimes.empty() ) {
				drawToX = drawFromX;
				int delta = drawToX.eventCount * style->fonts_ticks->tickWidth - chartRect.Width();
				drawToEventCount = delta >= 0 ? roundDouble( (double)delta / (double)style->fonts_ticks->tickWidth ) : drawToX.eventCount;
				it_max_pos = drawToX.eventCount * style->fonts_ticks->tickWidth;
				if ( it_max_pos > chartRect.Width() ) {
					drawToEventCount = ( it_max_pos - chartRect.Width() ) / style->fonts_ticks->tickWidth;
					if ( drawToEventCount * style->fonts_ticks->tickWidth < chartRect.Width() )
						drawToEventCount ++;
				} else {
					drawToEventCount = drawToX.eventCount;
				}

			}
			int pos = xPos + chartRect.Width();
			for( ; it != doc->docTimes.end(); it++ ) {
				it_pos = roundDouble( ( (*it)->time - doc->docTimes.front()->time ) * double(timeScale) ) + ticks * style->fonts_ticks->tickWidth;
				it_max_pos = it_pos + style->fonts_ticks->tickWidth * (*it)->eventCount;
				if ( it_pos == pos ) {
					drawToX = *(*it);
					break;
				}
				if ( it_pos < pos && it_max_pos >= pos ) {
					drawToX = *(*it);
					drawToEventCount = ( pos - it_pos ) / style->fonts_ticks->tickWidth;
					if ( it_pos + drawToEventCount * style->fonts_ticks->tickWidth < pos )
						drawToEventCount ++;
					break;
				}
				if ( it_pos > pos ) {
					drawToX.time = (*it)->time - ( it_pos - pos ) / double(timeScale);
					break;
				}
				unwrapTimesList.push_back( (*it) );
				ticks += (*it)->eventCount;
			}
		}
		unwrapTimesList.push_back( &drawToX );
	}
}

void RDOStudioChartView::drawTitle( CRect& chartRect )
{
	CRect tmprect;
	tmprect.CopyRect( &newClientRect );
	tmprect.top = 0;
	tmprect.bottom = chartRect.top;
	
	::SelectObject( hmemdc, hfontTitle );
	::SetTextColor( hmemdc, style->getTheme()->titleFGColor );
	
	tstring str = GetDocument()->getTitle();
	::DrawText( hmemdc, str.c_str(), str.length(), tmprect, DT_CENTER | DT_WORDBREAK );
}

void RDOStudioChartView::drawLegend( CRect& legendRect )
{
	RDOStudioChartDoc* doc = GetDocument();
	CRect tmprect;
	tmprect.CopyRect( &legendRect );
	SIZE size;
	::SelectObject( hmemdc, hfontLegend );
	for ( std::vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
		(*it)->drawInLegend( hmemdc, tmprect, style->getTheme()->legendFgColor, size );
		tmprect.top += size.cy;
	}
}

void RDOStudioChartView::drawYAxis( CRect& chartRect, const RDOStudioDocSerie* axisValues )
{
	CRect tmprect;
	tmprect.CopyRect( &newClientRect );
	tmprect.right = chartRect.left - 5;
	tmprect.left  = 5;
	if ( axisValues ) {
		int count;
		axisValues->getSerie()->getValueCount( count );
		if ( count ) {
			
			::SelectObject( hmemdc, hfontAxis );
			::SetTextColor( hmemdc, style->getTheme()->axisFgColor );
			
			int count = captions.size();
			int heightoffset = roundDouble( (double)chartRect.Height() / (double)( count - 1 ) );
			tmprect.top = chartRect.bottom;
			int index = 0;
			for ( std::vector<tstring>::iterator it = captions.begin(); it != captions.end(); it++ ) {
				index ++;
				::DrawText( hmemdc, (*it).c_str(), (*it).length(), tmprect, DT_RIGHT );
				if ( index != 1 && index < count ) {
					::MoveToEx( hmemdc, tmprect.right + 2, tmprect.top, (LPPOINT)NULL );
					::LineTo( hmemdc, chartRect.left, tmprect.top );
				}
				if ( index == count - 1 )
					tmprect.top = chartRect.top;
				else
					tmprect.top -= heightoffset;
			}
		}
	}
}

void RDOStudioChartView::drawXAxis( CRect& chartRect )
{	
	CRect tmprect;
	tmprect.CopyRect( &chartRect );
	tmprect.top = chartRect.bottom + 3;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = newClientRect.right - 5;
	
	RDOStudioChartDoc* doc = GetDocument();
	if ( !doc->docTimes.empty() ) {
		tstring formatstr = "%.3f";
		
		::SelectObject( hmemdc, hfontAxis );
		::SetTextColor( hmemdc, style->getTheme()->axisFgColor );
		
		if( !doUnwrapTime() ) {
			double valoffset = 0;
			int widthoffset = chartRect.Width() / ( valueCountX - 1 );
			if ( drawToX != drawFromX ) {
				valoffset = ( drawToX.time - drawFromX.time ) / ( valueCountX - 1 );
			}
			double valo = drawFromX.time;
			int x = chartRect.left;
			tstring str = rdo::format( formatstr.c_str(), valo );
			tmprect.left = x;
			::DrawText( hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT );
			valo += valoffset;
			x += widthoffset;
			if ( valoffset ) {
				for ( int i = 1; i < valueCountX; i++ ) {
					str = rdo::format( formatstr.c_str(), valo );
					tmprect.left = x;
					::DrawText( hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT );
					if ( i != valueCountX - 1 ) {
						::MoveToEx( hmemdc, x, chartRect.bottom, (LPPOINT)NULL );
						::LineTo( hmemdc, x, chartRect.bottom + 3 );
					}
					valo += valoffset;
					x += widthoffset;
				}
			}
		} else {
			int ticks = 0;
			tstring str;
			int lastx = 0;
			SIZE sz;
			for( timesList::iterator it = unwrapTimesList.begin(); it != unwrapTimesList.end(); it++ ) {
				tmprect.left = chartRect.left + (LONG)(( (*it)->time - unwrapTimesList.front()->time ) * timeScale + ticks * style->fonts_ticks->tickWidth - chartShift);
				tmprect.left = min( tmprect.left, chartRect.right - 1 );
				str = rdo::format( formatstr.c_str(), (*it)->time );
				if ( *(*it) == drawFromX ) {
					tmprect.left += chartShift;
				}
				if ( tmprect.left > chartRect.right )
					tmprect.left = chartRect.right;
				
				
				if ( tmprect.left > lastx ) {
					::DrawText( hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT );
					if ( tmprect.left != chartRect.left && tmprect.left != chartRect.right ) {
						::MoveToEx( hmemdc, tmprect.left, chartRect.bottom, (LPPOINT)NULL );
						::LineTo( hmemdc, tmprect.left, chartRect.bottom + 3 );
					}
					::GetTextExtentPoint32( hmemdc, str.c_str(), str.length(), &sz );
					lastx = tmprect.left + sz.cx;
				}

				ticks += (*it)->eventCount;
				if ( *(*it) == drawFromX ) {
					ticks -= drawFromEventIndex;
				}
			}
		}
	}
}

void RDOStudioChartView::drawGrid( CRect& chartRect )
{
	HBRUSH brush_chart = NULL;
	HBRUSH old_brush = NULL;
	try {
		brush_chart = ::CreateSolidBrush( style->getTheme()->chartBgColor );
		old_brush = (HBRUSH)::SelectObject( hmemdc, brush_chart );
		::Rectangle( hmemdc, chartRect.left, chartRect.top, chartRect.right, chartRect.bottom );
		::SelectObject( hmemdc, old_brush );
		::DeleteObject( brush_chart );
		brush_chart = NULL;
	} catch( ... ) {
		if ( brush_chart ) {
			::SelectObject( hmemdc, old_brush );
			::DeleteObject( brush_chart );
			brush_chart = NULL;
		}
	}

	if ( doUnwrapTime() ) {
		CRect rect;
		rect.CopyRect( chartRect );
		rect.InflateRect( -1, -1 );
		CRect tmprect;
		tmprect.CopyRect( &rect );
		
		::IntersectClipRect( hmemdc, rect.left, rect.top, rect.right, rect.bottom );
		int ticks = 0;
		timesList::iterator it = unwrapTimesList.begin();
		if ( drawFromX == drawToX ) {
			it ++;
		}
		//For drawing solid rect
		::SetBkColor( hmemdc, style->getTheme()->timeBgColor );
		for( ; it != unwrapTimesList.end(); it++ ) {
			int width = (*it)->eventCount * style->fonts_ticks->tickWidth;
			tmprect.left = rect.left + (LONG)(( (*it)->time - unwrapTimesList.front()->time ) * timeScale + ticks * style->fonts_ticks->tickWidth - chartShift);
			if ( tmprect.left < rect.left )
				tmprect.left = rect.left;
			if ( *(*it) == drawFromX ) {
				width -= drawFromEventIndex * style->fonts_ticks->tickWidth + chartShift;
			}
			if ( *(*it) == drawToX ) {
				width = drawToEventCount * style->fonts_ticks->tickWidth;
			}
			tmprect.right = tmprect.left + width;
			if ( tmprect.right > rect.right )
				tmprect.right = rect.right;
			//MFC's FillSolidRect do the same thing
			::ExtTextOut( hmemdc, 0, 0, ETO_OPAQUE, tmprect, NULL, 0, NULL );
			ticks += (*it)->eventCount;
			if ( *(*it) == drawFromX ) {
				ticks -= drawFromEventIndex;
			}
		}
		::SelectClipRgn( hmemdc, NULL );
	}
}

void RDOStudioChartView::copyToClipboard()
{
	if ( !OpenClipboard() || !::EmptyClipboard() )
		return;
	
	mutex.Lock();
	
	if ( hmemdc ) {
		HBITMAP hbm = ::CreateCompatibleBitmap( hmemdc, newClientRect.Width(), newClientRect.Height() );
		if ( hbm ) {
			HDC hdcDst = ::CreateCompatibleDC( hmemdc );
			if ( hdcDst ) {
				HGDIOBJ oldObj = ::SelectObject( hdcDst, hbm );

				::BitBlt( hdcDst, 0, 0, newClientRect.Width(), newClientRect.Height(),
					hmemdc, newClientRect.left, newClientRect.top, SRCCOPY );
				
				::SelectObject( hdcDst, oldObj );
				::DeleteDC( hdcDst );
				
				::SetClipboardData( CF_BITMAP, hbm );
			} else {
				::DeleteObject( hbm );
			}
		}
	}

	CloseClipboard();
	
	mutex.Unlock();
}

void RDOStudioChartView::setZoom( double new_zoom, const rbool force_update )
{
	UNUSED(force_update);

	/*scale_koeff = new_zoom;
	if ( scale_koeff < auto_zoom ) {
		scale_koeff = auto_zoom;
	}*/
	if ( doUnwrapTime() && new_zoom < 1 ) {
		new_zoom = 1;
	} else if ( !doUnwrapTime() && new_zoom < auto_zoom ) {
		new_zoom = auto_zoom;
	}
	//if ( zoom != new_zoom || scale_koeff != new_zoom || force_update ) {
		zoom = new_zoom;
		scale_koeff = zoom;
		recalcLayout();
		updateScrollBars();
		Invalidate();
		updateWindow();
	//}
}

void RDOStudioChartView::onDraw()
{
	//Document and view are locked from OnPaint()
	
	RDOStudioChartDoc* doc = GetDocument();
	
	CRect rect;
	rect.CopyRect( &newClientRect );

	//MFC's FillSolidRect do the same thing
	::SetBkColor( hmemdc, style->theme->backgroundColor );
	::ExtTextOut( hmemdc, 0, 0, ETO_OPAQUE, newClientRect, NULL, 0, NULL );

	drawTitle( chartRect );

	if ( needDrawLegend )
		drawLegend( legendRect );
	
	if ( chartRect.Height() > 0 && chartRect.Width() > 0 ) {

		HPEN pen_chart = NULL;
		HPEN old_pen   = NULL;
		try {
			pen_chart = ::CreatePen( PS_SOLID, 0, style->getTheme()->defaultColor );
			old_pen   = (HPEN)::SelectObject( hmemdc, pen_chart );
			
			drawYAxis( chartRect, yAxis );

			setFromTo();
		
			drawXAxis( chartRect );

			drawGrid( chartRect );

			::SelectObject( hmemdc, old_pen );
			::DeleteObject( pen_chart );
			pen_chart = NULL;
		} catch( ... ) {
			if ( pen_chart ) {
				::SelectObject( hmemdc, old_pen );
				::DeleteObject( pen_chart );
				pen_chart = NULL;
			}
		}

		for ( std::vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ )
			(*it)->drawSerie( this, hmemdc, chartRect );
	}

	::BitBlt( hdc, 0, 0, newClientRect.Width(), newClientRect.Height(), hmemdc, 0, 0, SRCCOPY );
}

void RDOStudioChartView::OnDraw(CDC* pDC)
{	
	//Document and view are locked from OnPaint()
	
	RDOStudioView::OnDraw( pDC );
}

BOOL RDOStudioChartView::OnPreparePrinting( CPrintInfo* pInfo )
{
	// default preparation
	return DoPreparePrinting( pInfo );
}

void RDOStudioChartView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
	/// @todo add extra initialization before printing
}

void RDOStudioChartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	/// @todo add cleanup after printing
}

DROPEFFECT RDOStudioChartView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	UNUSED(dwKeyState);
	UNUSED(point     );

	HGLOBAL glb = NULL;
	UINT format = tracer->getClipboardFormat();
	if ( pDataObject->IsDataAvailable( CLIPFORMAT(format) ) ) {
		glb = pDataObject->GetGlobalData( CLIPFORMAT(format) );
		if ( glb ) {
			dragedSerie = *(RDOTracerSerie**)::GlobalLock( glb );
			::GlobalUnlock( glb );
			::GlobalFree( glb );
		}
		if ( !GetDocument()->serieExists( dragedSerie ) )
			return DROPEFFECT_COPY;
		else
			dragedSerie = NULL;
	}
	return DROPEFFECT_NONE;
}

void RDOStudioChartView::OnDragLeave()
{
	dragedSerie = NULL;
}

DROPEFFECT RDOStudioChartView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	UNUSED(dwKeyState);
	UNUSED(point     );

	if ( pDataObject->IsDataAvailable( CLIPFORMAT(tracer->getClipboardFormat()) ) && dragedSerie )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

BOOL RDOStudioChartView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	UNUSED(pDataObject);
	UNUSED(dropEffect );
	UNUSED(point      );

	GetDocument()->addSerie( dragedSerie );
	dragedSerie = NULL;
	return TRUE;
}

#ifdef _DEBUG
void RDOStudioChartView::AssertValid() const
{
	RDOStudioView::AssertValid();
}

void RDOStudioChartView::Dump(CDumpContext& dc) const
{
	RDOStudioView::Dump(dc);
}

RDOStudioChartDoc* RDOStudioChartView::GetDocument()
{
	if ( !m_pDocument )
		return NULL;
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioChartDoc)));
	return (RDOStudioChartDoc*)m_pDocument;
}
#endif

BOOL RDOStudioChartView::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED(pDC);
	return TRUE;
}

void RDOStudioChartView::OnSize(UINT nType, int cx, int cy) 
{
	UNUSED(nType);
	UNUSED(cx   );
	UNUSED(cy   );

	mutex.Lock();

	GetClientRect( &newClientRect );

	if ( newClientRect.Width() > bmpRect.Width() || newClientRect.Height() > bmpRect.Height() )
	{
		if ( hbmp ) {
			::SelectObject( hmemdc, hbmpInit );
			::DeleteObject( hbmp );
		}
		hbmp = CreateCompatibleBitmap( hdc, newClientRect.Width(), newClientRect.Height() );
		::SelectObject( hmemdc, hbmp );
		bmpRect = newClientRect;
	}

	if ( GetDocument() ) {
		recalcLayout();
		updateScrollBars( false );
		//setZoom( zoom );
	}
	
	mutex.Unlock();
}

void RDOStudioChartView::OnChartTimewrap() 
{
	timeWrap = !timeWrap;
	recalcLayout();
	updateScrollBars();
	Invalidate();
	updateWindow();
}

void RDOStudioChartView::OnUpdateChartTimewrap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( !canUnwrapTime() || timeWrap );
	pCmdUI->Enable( canUnwrapTime() );
}

void RDOStudioChartView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNUSED(pSender);
	UNUSED(lHint  );
	UNUSED(pHint  );

	updateView();
}

LRESULT RDOStudioChartView::OnUserUpdateChartView(WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);

	if ( doUnwrapTime() || wParam != UPDATE_TIMETICKS )
		updateView();
	return 0;
}

void RDOStudioChartView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	int inc = 0;
	SCROLLINFO si;
	si.cbSize = sizeof( si );

	switch( nSBCode ) {
		case SB_PAGEUP:
			inc = -chartRect.Width();
			break; 

		case SB_PAGEDOWN:
			inc = chartRect.Width();
			break;

		case SB_LINEUP:
			inc = -style->fonts_ticks->tickWidth;
			break;

		case SB_LINEDOWN:
			inc = style->fonts_ticks->tickWidth;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			inc = si.nTrackPos - xPos;
			break;
		}
		default:
			inc = 0;
	}

	if ( !inc ) return;

	// If applying the horizontal scrolling increment does not 
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc == max ( -xPos, min ( inc, xMax - xPos ) ) ) {
		xPos += inc;
		
		setScrollPos( SB_HORZ, xPos );
	}
}

void RDOStudioChartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UNUSED(nRepCnt);
	UNUSED(nFlags );

	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;
	rbool ctrl = ( ( ::GetKeyState( VK_CONTROL ) & 0x80000000 ) != 0 );
	rbool side = false;
	int pos = 0;
	
	switch ( nChar ) {
		case VK_LEFT: {
			if ( ctrl ) {
				side = true;
			} else {
				scrollNotify = SB_LINEUP;
				msg = WM_HSCROLL;
			}
			break;
		}
		case VK_RIGHT: {
			if ( ctrl ) {
				side = true;
				pos = xMax;
			} else {
				scrollNotify = SB_LINEDOWN;
				msg = WM_HSCROLL;
			}
			break;
		}
		default:
			break;
	}

	if ( scrollNotify != 0xFFFF ) 
		::SendMessage( m_hWnd, msg, MAKELONG(scrollNotify, pos), NULL );

	if ( side )
		setScrollPos( SB_HORZ, pos );
}

void RDOStudioChartView::OnEditCopy() 
{
	copyToClipboard();
}

void RDOStudioChartView::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	RDOStudioView::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOStudioChartView::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	if ( previewMode ) return;
	RDOStudioView::OnContextMenu( pWnd, pos );
	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

void RDOStudioChartView::OnChartZoomZoomin() 
{
	if ( zoomAuto )
		zoomAuto = !zoomAuto;
	double delta = zoom < 1 ? 0.1 : 0.5;
	if ( zoom + delta > 1 && zoom + delta - 1 < delta )
		setZoom( 1 );
	else
		setZoom( zoom + delta );
}

void RDOStudioChartView::OnChartZoomZoomout() 
{
	if ( zoomAuto )
		zoomAuto = !zoomAuto;
	double delta = zoom > 1 ? -0.5 : -0.1;
	double zoom_new = zoom + delta;
	setZoom( zoom_new );
}

void RDOStudioChartView::OnChartZoomResetzoom() 
{
	if ( zoomAuto )
		zoomAuto = !zoomAuto;
	setZoom( 1 );
}

void RDOStudioChartView::OnUpdateChartZoomZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !zoomAuto && zoom < 5 );
}

void RDOStudioChartView::OnUpdateChartZoomZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !zoomAuto && ( ( !doUnwrapTime() && zoom > auto_zoom ) || ( doUnwrapTime() && zoom > 1 ) ) );
}

void RDOStudioChartView::OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( zoom != 1 );
}

const RDOStudioChartViewStyle& RDOStudioChartView::getStyle() const
{
	return (*style);
}

int RDOStudioChartView::OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message )
{
	if ( previewMode ) {
		return CWnd::OnMouseActivate( pDesktopWnd, nHitTest, message );
	} else {
		return RDOStudioView::OnMouseActivate( pDesktopWnd, nHitTest, message );
	}
}

void RDOStudioChartView::setFonts( const rbool needRedraw )
{
	UNUSED(needRedraw);

	if ( !style ) return;
	
	mutex.Lock();
	
	LOGFONT lf;
	RDOStudioChartViewTheme* chart_theme = static_cast<RDOStudioChartViewTheme*>(style->theme);
	::SelectObject( hdc, hfontInit );

	if ( !hfontAxis || ::DeleteObject( hfontAxis ) ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		lf.lfHeight    = -MulDiv( style->font->size, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->defaultStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->defaultStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->defaultStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
		strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)
		hfontAxis = ::CreateFontIndirect( &lf );
	}

	if ( !hfontTitle || ::DeleteObject( hfontTitle ) ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		lf.lfHeight    = -MulDiv( style->fonts_ticks->titleFontSize, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->titleStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->titleStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->titleStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
		strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)

		hfontTitle = ::CreateFontIndirect( &lf );
	}
	if ( !hfontLegend || ::DeleteObject( hfontLegend ) ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		lf.lfHeight    = -MulDiv( style->fonts_ticks->legendFontSize, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->legendStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->legendStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->legendStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
		strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)

		hfontLegend = ::CreateFontIndirect( &lf );
	}

	mutex.Unlock();
}

void RDOStudioChartView::setStyle( RDOStudioChartViewStyle* _style, const rbool needRedraw )
{
	style = _style;

	setFonts( false );
	
	/*if ( previewMode ) {
		auto_zoom = 1;
		setZoom( 1 );
	}*/

	if ( needRedraw ) {
		recalcLayout();
		updateScrollBars();
		Invalidate();
		updateWindow();
	}
}

void RDOStudioChartView::OnChartOptions() 
{
	RDOStudioChartOptions dlg( this );
	dlg.DoModal();
}

void RDOStudioChartView::updateWindow()
{
	CRgn updateRgn;
	int rgn_type = GetUpdateRgn( &updateRgn );
	if ( rgn_type != NULLREGION && rgn_type != ERROR )
		SendNotifyMessage( WM_PAINT, 0, 0 );
}

void RDOStudioChartView::updateView()
{
	GetDocument()->lock();
	rbool lastvisible = maxXVisible();
	recalcLayout();
	updateScrollBars( false );
	if ( lastvisible  && !maxXVisible() ) {
		setScrollPos( SB_HORZ, xMax, false );
	}
	RDOStudioView::OnUpdate( NULL, 0, NULL );
	updateScrollBars( true );
	GetDocument()->unlock();
}

void RDOStudioChartView::OnDestroy() 
{
	if ( GetDocument() )
		GetDocument()->removeFromViews( GetSafeHwnd() );
	if ( hdc ) {
		::RestoreDC( hdc, saved_hdc );
	}
	if ( hmemdc ) {
		::RestoreDC( hmemdc, saved_hmemdc );
		::DeleteDC( hmemdc );
	}
	if ( hfontTitle ) {
		::DeleteObject( hfontTitle );
	}
	if ( hfontLegend ) {
		::DeleteObject( hfontLegend );
	}
	if ( hfontAxis ) {
		::DeleteObject( hfontAxis );
	}
	if ( hbmp ) {
		::DeleteObject( hbmp );
	}
	RDOStudioView::OnDestroy();
}

void RDOStudioChartView::OnInitialUpdate() 
{
	GetDocument()->addToViews( GetSafeHwnd() );
	RDOStudioView::OnInitialUpdate();	
}

void RDOStudioChartView::OnPaint() 
{
	RDOStudioChartDoc* doc = GetDocument();
	doc->mutex.Lock();

	mutex.Lock();

	//MFC's realization
	/*PAINTSTRUCT ps;
	CDC* dc = BeginPaint( &ps );
	OnPrepareDC( dc );
	OnDraw( dc );
	EndPaint( &ps );*/

	PAINTSTRUCT ps;
	::BeginPaint( hwnd, &ps );
	onDraw();
	::EndPaint( hwnd, &ps );

	mutex.Unlock();

	doc->mutex.Unlock();
}

void RDOStudioChartView::OnViewZoomauto() 
{
	zoomAuto = !zoomAuto;
	if ( !zoomAuto )
		setZoom( old_zoom, true );
	else {
		old_zoom = zoom;
		setZoom( auto_zoom, true );
	}
}

void RDOStudioChartView::OnUpdateViewZoomauto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( zoomAuto );
}

void RDOStudioChartView::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_model_chart.htm";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
