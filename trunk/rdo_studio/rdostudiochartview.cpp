#include "stdafx.h"
#include "rdostudiochartview.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartView, CView)

BEGIN_MESSAGE_MAP(RDOStudioChartView, CView)
	//{{AFX_MSG_MAP(RDOStudioChartView)
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
	ON_COMMAND(ID_VIEW_ZOOMAUTO, OnChartZoomZoomauto)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateChartZoomZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateChartZoomZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMRESET, OnUpdateChartZoomResetzoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMAUTO, OnUpdateChartZoomZoomauto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartView::RDOStudioChartView()
	: CView(),
	bmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	dragedSerie( NULL ),
	valueCountX( 5 ),
	valueCountY( 5 ),
	yaxis( NULL ),
	tickWidth( 5 ),
	timeColor( RGB( 0xE7, 0xF8, 0xF8 ) ),
	timeWrap( true ),
	/*leftMargin( 0 ),
	topMargin( 0 ),
	rightMargin( 0 ),
	bottomMargin( 0 ),*/
	chartRect( 0, 0, 0, 0 ),
	xMax( 0 ),
	xPos( 0 ),
	//timeRange( 0 ),
	timeScale( 0 ),
	/*drawFromX( NULL ),
	drawFromXEventIndex( 0 ),
	drawToX( NULL ),
	drawToXEventIndex( 0 ),
	pixelsToChart( 0 ),*/
	drawFromX( 0 ),
	drawToX( 0 ),/*,
	xAxisOffset( 0 ),
	yAxisOffset( 0 )*/
	zoom( 1 ),
	old_zoom( 1 ),
	auto_zoom( 1 ),
	zoomAuto( false )
{
}

RDOStudioChartView::~RDOStudioChartView()
{
}

BOOL RDOStudioChartView::PreCreateWindow(CREATESTRUCT& cs) 
{
//	return CView::PreCreateWindow(cs);
	if ( !CView::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL /*| WS_VSCROLL*/ | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

void RDOStudioChartView::recalcLayout()
{
	CClientDC cldc( this );
	CDC dc;
	dc.CreateCompatibleDC( &cldc );

	RDOStudioChartDoc* doc = GetDocument();
	
	string str = doc->GetTitle();

	chartRect.top = dc.GetTextExtent( str.c_str() ).cy;

	double val = 0;

	chartRect.left = dc.GetTextExtent( format( "%.3f", val ).c_str() ).cx + 5;

	if ( !doc->docTimes.empty() )
		val = doc->docTimes.back()->time;
	else
		val = 0;
	chartRect.right = newClientRect.right - dc.GetTextExtent( format( "%.3f", val ).c_str() ).cx - 5;

	chartRect.bottom = newClientRect.bottom - chartRect.top - 2 - 3;
	
	if ( !doc->docTimes.empty() ) {
		double timeRange = doc->docTimes.back()->time - doc->docTimes.front()->time;
		if ( timeRange > 0 ) {
			//long double timeScaleAuto = doUnwrapTime() ? (double)( chartRect.Width() - tickWidth * doc->docTimes.back()->overallCount ) / timeRange : (double)chartRect.Width() / timeRange;
			long double timeScaleAuto = (double)chartRect.Width() / timeRange;
			timeScale = (double)tickWidth / doc->minTimeOffset;
			auto_zoom = timeScaleAuto / timeScale;
			if ( zoomAuto || auto_zoom > 1 ) {
				timeScale = timeScaleAuto;
			}
		} else {
			timeScale = 0;
		}
	} else {
		timeScale = 0;
	}

	if ( !zoomAuto ) {
		timeScale *= zoom;
	} else {
		zoom = timeScale > 0 ? auto_zoom : 1;
	}
}

void RDOStudioChartView::setScrollPos( UINT nSBCode, UINT nPos )
{
	if ( nSBCode == SB_HORZ )
		xPos = nPos;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_POS;
	si.nPos   = xPos;
	SetScrollInfo( nSBCode, &si, TRUE );

	Invalidate();
	UpdateWindow();
}

void RDOStudioChartView::updateScrollBars()
{
	//xPageSize = newClientRect.Width() / charWidth;
	//yPageSize = newClientRect.Height() / lineHeight;

	/*yMax = max ( 0, stringsCount - yPageSize );
	yPos = min ( yPos, yMax );
	int mul = yPageSize;
	if ( mul * lineHeight < newClientRect.Height() )
		mul++;
	lastViewableLine = yPos + mul - 1;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0; 
	
	si.nMax   = stringsCount - 1; 
	si.nPage  = yPageSize; 
	si.nPos   = yPos; 
	SetScrollInfo( SB_VERT, &si, TRUE );*/
	
	RDOStudioChartDoc* doc = GetDocument();
	int size;
	if ( !doc->docTimes.empty() )
		//size = roundDouble( GetDocument()->docTimes.back()->time * /*5 / GetDocument()->minTimeOffset*/timeScale );
		size = roundDouble( ( doc->docTimes.back()->time - doc->docTimes.front()->time ) * timeScale );
	else
		size = 0;
	if ( doUnwrapTime() )
		size += tickWidth * doc->docTimes.back()->overallCount;
	//int chartwidth = /*newClientRect.Width() - leftMargin - rightMargin*/chartRect.Width();
	xMax = max ( 0, size - chartRect.Width() );
	xPos = min ( xPos, xMax );
	
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = size - 1;
	si.nPage  = chartRect.Width(); 
	si.nPos   = xPos;
	SetScrollInfo( SB_HORZ, &si, TRUE );
}

/*void RDOStudioChartView::prepareDrawing( CDC &dc, CRect& chartRect )
{
	timeRange = tracer.getMaxTime()->time;
	
	if ( timeRange > 0 ) {
		CRect backuprect;
		backuprect.CopyRect( &chartRect );
		calcYAxisOffset( dc );
		chartRect.left += yAxisOffset;
		calcXAxisOffset( dc );
		chartRect.right -= xAxisOffset;
		timeScale = timeWrap ? chartRect.Width() / timeRange : ( chartRect.Width() - tickWidth * tracer.getMaxTime()->overallCount ) / timeRange;
		if ( tracer.getMinTimeDifference() * timeScale < 5 ) {
			chartRect.CopyRect( &backuprect );
			timeScale = 5 / tracer.getMinTimeDifference();
			
			drawToX = tracer.getMaxTime();
			drawToXEventIndex = tracer.getMaxTime()->eventCount;
			
			calcYAxisOffset( dc );
			chartRect.left += yAxisOffset;
			calcXAxisOffset( dc );
			chartRect.right -= xAxisOffset;
			
			int count = tracer.getTimesCount();
			int x = chartRect.right;
			RDOTracerTimeNow* timenow = NULL;
			RDOTracerTimeNow* prev = tracer.getMaxTime();
			int offset = timeWrap ? 0 : tickWidth * prev->eventCount;
			x -= offset;
			for ( int i = count - 2; i >= 0; i-- ) {
				timenow = tracer.getTime( i );
				offset = ( prev->time - timenow->time ) * timeScale;
				if ( !timeWrap )
					offset += tickWidth * timenow->eventCount;
				x -= offset;
				if ( x <= chartRect.left ) {
					drawFromX = timenow;
					pixelsToChart = chartRect.left - x;
					drawFromXEventIndex = ( pixelsToChart ) / tickWidth;
					break;
				}
				prev = timenow;
			}
			
			timeRange = drawToX->time - drawFromX->time;
		} else {
			drawFromX = tracer.getTime( 0 );
			drawFromXEventIndex = 0;
			drawToX = tracer.getMaxTime();
			drawToXEventIndex = tracer.getMaxTime()->eventCount;
		}
	} else {
		timeScale = 0;
		drawFromX = tracer.getTime( 0 );
		drawFromXEventIndex = 0;
		drawToX = tracer.getTime( 0 );
		drawToXEventIndex = tracer.getTime( 0 )->eventCount;
		calcYAxisOffset( dc );
		chartRect.left += yAxisOffset;
		calcXAxisOffset( dc );
		chartRect.right -= xAxisOffset;
	}
}*/

/*void RDOStudioChartView::calcYAxisOffset( CDC &dc )
{
	CSize size;
	//temporary
	string str = format( "%.3f", 0 );
	size = dc.GetTextExtent( str.c_str() );
	yAxisOffset = size.cx + 2 + 3;
}*/

/*void RDOStudioChartView::calcXAxisOffset( CDC &dc )
{
	CSize size;
	//temporary
	string str = format( "%.3f", tracer.getMaxTime()->time );
	size = dc.GetTextExtent( str.c_str() );
	xAxisOffset = size.cx;
}*/

bool RDOStudioChartView::setTo( const int from_max_pos )
{
	bool res = true;
	int delta = ( from_max_pos - xPos - chartRect.Width() ) / tickWidth;
	if ( delta >= 0 ) {
		res = false;
		drawToX = drawFromX;
		if ( delta > 0 ) {
			//drawToX.overallCount = drawToX.eventCount - delta;
			drawToX.eventCount -= delta;
			//drawToX.overallCount -= delta;
		}
	}
	return res;
}

void RDOStudioChartView::setFromTo()
{
	RDOStudioChartDoc* doc = GetDocument();

	drawFromX.eventCount = 0;
	drawFromX.overallCount = 0;
	drawToX.eventCount = 0;
	drawToX.overallCount = 0;
	unwrapTimesList.clear();

	if ( !doUnwrapTime() ) {
		if ( timeScale ) {
			drawFromX.time = doc->docTimes.front()->time + (double)xPos / timeScale;
			if ( maxXVisible() )
				drawToX.time = doc->docTimes.back()->time;
			else
				drawToX.time = doc->docTimes.front()->time + (double)( xPos + chartRect.Width() ) / timeScale;
		} else {
			drawFromX.time = 0;
			drawToX.time = 0;
		}
	} else {
		int it_pos = 0;
		int it_max_pos = 0;
		bool need_search_to = true;
		for( timesList::iterator it = doc->docTimes.begin(); it != doc->docTimes.end(); it++ ) {
			it_pos = roundDouble( (*it)->time * timeScale ) + tickWidth * ( (*it)->overallCount - (*it)->eventCount );
			it_max_pos = it_pos + tickWidth * (*it)->eventCount;
			if ( it_pos == xPos ) {
				drawFromX = *(*it);
				//drawFromX.overallCount = 0;
				unwrapTimesList.push_back( &drawFromX );
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos < xPos && ( it_max_pos > xPos ) ) {
				//drawFromX = *(*it);
				int delta = ( xPos - it_pos ) / tickWidth;
				drawFromX.time = (*it)->time + ( xPos - it_pos ) / timeScale;
				drawFromX.eventCount = (*it)->eventCount - delta;
				drawFromX.overallCount = drawFromX.eventCount;
				unwrapTimesList.push_back( &drawFromX );
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos > xPos ) {
				drawFromX.time = (*it)->time - ( it_pos - xPos ) / timeScale;
				drawFromX.overallCount = (*it)->overallCount - (*it)->eventCount;
				unwrapTimesList.push_back( &drawFromX );
				break;
			}
		}
		if ( need_search_to ) {
			int pos = xPos + chartRect.Width();
			for( ++it; it != doc->docTimes.end(); it++ ) {
				it_pos = roundDouble( (*it)->time * timeScale ) + tickWidth * ( (*it)->overallCount - (*it)->eventCount );
				it_max_pos = it_pos + tickWidth * (*it)->eventCount;
				if ( it_pos == pos ) {
					drawToX = *(*it);
					drawToX.overallCount = 0;
					break;
				}
				if ( it_pos < pos && it_max_pos >= pos ) {
					drawToX = *(*it);
					drawToX.overallCount = ( it_max_pos - pos ) / tickWidth;
					break;
				}
				if ( it_pos > pos ) {
					drawToX.time = (*it)->time - ( it_pos - pos ) / timeScale;
					break;
				}
				unwrapTimesList.push_back( (*it) );
			}
		}
		unwrapTimesList.push_back( &drawToX );
	}
}

void RDOStudioChartView::drawYAxis( CDC &dc, CRect& chartRect, const RDOTracerSerie* axisValues)
{
	CRect tmprect;
	CSize size;

	tmprect.CopyRect( &newClientRect );
	tmprect.left = 5;
	tmprect.right = chartRect.left;
	if ( axisValues && axisValues->getValueCount() ) {
		string formatstr = "%.3f";
		/*if ( yaxis->getSerieKind() == RDOST_OPERATION )
			formatstr = "%d";
		if ( yaxis->getSerieKind() == RDOST_RESPARAM ) {
			RDOTracerResParam* resparam = (RDOTracerResParam*)yaxis;
			RDOTracerResParamType type = resparam->getParamInfo()->getParamType();
			if ( type == RDOPT_INTEGER )
				formatstr = "%d";
			else if ( type == RDOPT_ENUMERATIVE )
				//format = "%s";
				formatstr = "%d";
		}*/
		/*RDOTracerValue* start = NULL;
		RDOTracerValue* end = NULL;
		axisValues->getFromToValues( drawFromX, drawFromXEventIndex, drawToX, drawToXEventIndex, start, end );
		double min = 0;
		double max = 0;
		axisValues->getMinMaxValues( start, end, min, max );
		double valoffset = 0;
		if ( min != max )
			valoffset = ( max - min ) / ( valueCountY - 1 );
		int heightoffset = chartRect.Height() / ( valueCountY - 1 );
		double valo = min ? min : 0;
		int y = chartRect.bottom;
		//int maxy = 0;
		for ( int i = 0; i < valueCountY; i++ ) {
			string str = format( formatstr.c_str(), valo );
			size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
			tmprect.top = y - size.cy / 2;
			tmprect.bottom += size.cy / 2;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
			valo += valoffset;
			y -= heightoffset;
		}

		//chartRect.left += ( maxy + 2 );
		
		y = chartRect.bottom;
		for ( i = 1; i < valueCountY - 1; i++ ) {
			y -= heightoffset;
			dc.MoveTo( chartRect.left, y );
			dc.LineTo( chartRect.left + 3, y );
		}
		//chartRect.left += 3;*/
		double valoffset = 0;
		double min = axisValues->getMinValue();
		double max = axisValues->getMaxValue();
		int heightoffset = chartRect.Height() / ( valueCountY - 1 );
		if ( max != min ) {
			valoffset = ( max - min ) / ( valueCountY - 1 );
		}
		double valo = min;
		int y = chartRect.bottom;
		string str = format( formatstr.c_str(), valo );
		size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
		tmprect.top = y /*- size.cy / 2*/;
		//tmprect.bottom += size.cy / 2;
		dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		valo += valoffset;
		y -= heightoffset;
		if ( valoffset ) {
			for ( int i = 1; i < valueCountY; i++ ) {
				str = format( formatstr.c_str(), valo );
			//size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
				tmprect.top = y /*- size.cy / 2*/;
				//tmprect.bottom += size.cy / 2;
				dc.DrawText( str.c_str(), tmprect, DT_LEFT );
				dc.MoveTo( chartRect.left, y );
				dc.LineTo( chartRect.left - 3, y );
				valo += valoffset;
				y -= heightoffset;
			}
		}
	}

}

void RDOStudioChartView::drawXAxis( CDC &dc, CRect& chartRect )
{	
	CRect tmprect;
	CSize size;
	/*tmprect.top = chartRect.bottom;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = chartRect.right;*/
	tmprect.CopyRect( &chartRect );
	
	tmprect.top = chartRect.bottom;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = newClientRect.right - 5;
	
	/*string str = format( "%.3f", drawFromX->time );
	dc.DrawText( str.c_str(), tmprect, DT_LEFT );
	//CSize size = dc.GetTextExtent( str.c_str() );
	//rect.left += size.cx / 2;
	if ( timeRange > 0 ) {
		//rect.right -= size.cx / 2;
		//chartRect.right -= size.cx;
		int x = chartRect.left;
		int widthoffset = chartRect.Width() / ( valueCountX - 1 );
		double valoffset = timeRange / ( valueCountX - 1 );
		double tempo = drawFromX->time;
		for ( int i = 1; i < valueCountX - 1; i++ ) {
			tempo += valoffset;
			x += widthoffset;
			dc.MoveTo( x, chartRect.bottom );
			dc.LineTo( x, chartRect.bottom - 3 );
			str = format( "%.3f", tempo );
			//size = dc.GetTextExtent( str.c_str() );
			//tmprect.left = x - size.cx / 2;
			tmprect.left = x;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		}
		str = format( "%.3f", drawToX->time );
		//size = dc.GetTextExtent( str.c_str() );
		//tmprect.left += widthoffset;
		//tmprect.right = tmprect.left + size.cx;
		dc.DrawText( str.c_str(), tmprect, DT_RIGHT );
	}
	chartRect.bottom -= 2;*/
	RDOStudioChartDoc* doc = GetDocument();
	if ( !doc->docTimes.empty() ) {
		double valoffset = 0;
		setFromTo();
		if( doUnwrapTime() )
			return;
		int widthoffset = chartRect.Width() / ( valueCountX - 1 );
		if ( drawToX != drawFromX ) {
			valoffset = ( drawToX.time - drawFromX.time ) / ( valueCountX - 1 );
		}
		double valo = drawFromX.time;
		int x = chartRect.left;
		string formatstr = "%.3f";
		string str = format( formatstr.c_str(), valo );
		size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
		tmprect.left = x /*- size.cy / 2*/;
		//tmprect.bottom += size.cy / 2;
		dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		valo += valoffset;
		x += widthoffset;
		if ( valoffset ) {
			for ( int i = 1; i < valueCountX; i++ ) {
				str = format( formatstr.c_str(), valo );
			//size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
				tmprect.left = x /*- size.cy / 2*/;
				//tmprect.bottom += size.cy / 2;
				dc.DrawText( str.c_str(), tmprect, DT_LEFT );
				dc.MoveTo( x, chartRect.bottom );
				dc.LineTo( x, chartRect.bottom - 3 );
				valo += valoffset;
				x += widthoffset;
			}
		}
	}
}

void RDOStudioChartView::drawGrid(	CDC &dc, CRect& chartRect )
{
	dc.Rectangle( chartRect );

	if ( doUnwrapTime() ) {
		CRect rect;
		rect.CopyRect( chartRect );
		rect.InflateRect( -1, -1 );
		CRect tmprect;
		tmprect.CopyRect( &rect );
		
		int ticks = 0;
		for( timesList::iterator it = unwrapTimesList.begin(); it != unwrapTimesList.end(); it++ ) {
			RDOTracerTimeNow* time = (*it);
			int width = (*it)->eventCount * tickWidth;
			//tmprect.left = rect.left + roundDouble( (*it)->time * timeScale ) + (*it)->overallCount * tickWidth - width - xPos;
			tmprect.left = rect.left + roundDouble( ( (*it)->time - unwrapTimesList.front()->time ) * timeScale ) + ticks * tickWidth;
			tmprect.right = tmprect.left + width;
			dc.FillSolidRect( &tmprect, timeColor );
			dc.Rectangle( &tmprect );
			ticks = (*it)->overallCount;
		}


		/*int count = tracer.getTimesCount();
		RDOTracerTimeNow* lasttime = NULL;
		for ( int i = 0; i < count; i++ ) {
			RDOTracerTimeNow* timeNow = tracer.getTime( i );
			if ( timeNow->eventCount ) {
				int ticks = ( lasttime ) ? lasttime->overallCount : 0;
				tmprect.left = chartRect.left + min( timeScale * timeNow->time + tickWidth * ticks, chartRect.right );
				tmprect.right = min( tmprect.left + tickWidth * timeNow->eventCount, chartRect.right );
				dc.FillSolidRect( &tmprect, timeColor );
			}
			lasttime = timeNow;
		}*/
	}
}

void RDOStudioChartView::copyToClipboard()
{
	if ( !OpenClipboard() || !::EmptyClipboard() )
		return;
	
	HDC hdcSrc = ::GetDC( GetSafeHwnd() );
	if ( hdcSrc ) {
		HBITMAP hbm = ::CreateCompatibleBitmap( hdcSrc, newClientRect.Width(), newClientRect.Height() );
		if ( hbm ) {
			HDC hdcDst = ::CreateCompatibleDC(hdcSrc);
			if ( hdcDst ) {
				HGDIOBJ oldObj = ::SelectObject( hdcDst, hbm );

				::BitBlt( hdcDst, 0, 0, newClientRect.Width(), newClientRect.Height(),
					hdcSrc, newClientRect.left, newClientRect.top, SRCCOPY );
				
				::SelectObject( hdcDst, oldObj );
				::DeleteDC( hdcDst );
				
				::SetClipboardData( CF_BITMAP, hbm );
			} else {
				::DeleteObject( hbm );
			}
		}

		::ReleaseDC( GetSafeHwnd(), hdcSrc );
	}

	CloseClipboard();
}

void RDOStudioChartView::setZoom( const double new_zoom )
{
	if ( zoomAuto )
		zoomAuto = !zoomAuto;
	if ( zoom != new_zoom ) {
		zoom = new_zoom;
		recalcLayout();
		updateScrollBars();
		Invalidate();
		UpdateWindow();
	}
}

void RDOStudioChartView::OnDraw(CDC* pDC)
{
	
	
	RDOStudioChartDoc* pDoc = GetDocument();

	
	//temporary
	if ( pDoc->series.size() )
		yaxis = pDoc->series.at( 0 ).getSerie();
	//temporary

	//GetClientRect( &newClientRect );
	CRect rect;
	rect.CopyRect( &newClientRect );

	CDC dc;
	dc.CreateCompatibleDC( pDC );
	CBitmap* pOldBitmap = dc.SelectObject( &bmp );
	

	int oldBkMode = dc.SetBkMode( TRANSPARENT );

	CBrush brush( RGB( 255, 255, 255 ) );
	CBrush* pOldBrush = dc.SelectObject( &brush );
	CPen penBlack;
	penBlack.CreatePen( PS_SOLID, 0, RGB( 0, 0, 0 ) );
	CPen* pOldPen = dc.SelectObject( &penBlack );
	
	dc.FillSolidRect( newClientRect, RGB( 255, 255, 255 ) );

	string str = pDoc->GetTitle();
	dc.DrawText( str.c_str(), rect, DT_CENTER | DT_CALCRECT );
	dc.DrawText( str.c_str(), newClientRect, DT_CENTER );
	int height = rect.Height();
	//rect.top = height;
	//rect.bottom = newClientRect.Height() - height;
	//rect.left = 5;
	//rect.right = newClientRect.right - 5;
	
	if ( chartRect.Height() > 0 && chartRect.Width() > 0 ) {

		//prepareDrawing( dc, rect );
/*		rect.left = leftMargin;
		rect.top = topMargin;
		rect.bottom = newClientRect.bottom - bottomMargin;
		rect.right = newClientRect.right - rightMargin;*/
		
		//temporary
		/*timeRange = tracer.getMaxTime()->time;
		drawToX = timeRange;

		rect.left += calcYAxisOffset();
		rect.right -= calcXAxisOffset();*/
		
		drawYAxis( dc, chartRect, yaxis );
		
		drawXAxis( dc, chartRect );

		/*if ( timeWrap )
			timeScale = ( timerange > 0 ) ? rect.Width() / timerange : 0;
		else
			timeScale = ( timerange > 0 ) ? ( rect.Width() - tickWidth * tracer.getMaxTime()->overallCount ) / timerange : 0;*/

		drawGrid( dc, chartRect );

		if ( !doUnwrapTime() ) {
		for ( vector< RDOStudioDocSerie >::iterator it = pDoc->series.begin(); it != pDoc->series.end(); it++ )
			it->getSerie()->drawSerie( this, dc, chartRect, it->getColor() );
		}
	}

	pDC->BitBlt( 0, 0, newClientRect.Width(), newClientRect.Height(), &dc, 0, 0, SRCCOPY );
	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );
	dc.SetBkMode( oldBkMode );
	dc.SelectObject( pOldBitmap );
}

BOOL RDOStudioChartView::OnPreparePrinting( CPrintInfo* pInfo )
{
	// default preparation
	return DoPreparePrinting( pInfo );
}

void RDOStudioChartView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
	// TODO: add extra initialization before printing
}

void RDOStudioChartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

DROPEFFECT RDOStudioChartView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	HGLOBAL glb = NULL;
	UINT format = tracer.getClipboardFormat();
	if ( pDataObject->IsDataAvailable( format ) ) {
		glb = pDataObject->GetGlobalData( format );
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
	if ( pDataObject->IsDataAvailable( tracer.getClipboardFormat() ) && dragedSerie )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

BOOL RDOStudioChartView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	GetDocument()->addSerie( dragedSerie );
	dragedSerie = NULL;
	return TRUE;
}

int RDOStudioChartView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( !target.Register( this ) )
		return -1;
	//return CView::OnCreate( lpCreateStruct );
	if ( CView::OnCreate( lpCreateStruct ) == -1 ) return -1;
	//setFont( logStyle->font, false );
	recalcLayout();
	updateScrollBars();

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	
	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 4, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 5, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 7, &popupMenu );

	return 0;
}

#ifdef _DEBUG
void RDOStudioChartView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

RDOStudioChartDoc* RDOStudioChartView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioChartDoc)));
	return (RDOStudioChartDoc*)m_pDocument;
}
#endif

BOOL RDOStudioChartView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void RDOStudioChartView::OnSize(UINT nType, int cx, int cy) 
{
	GetClientRect( &newClientRect );

	if ( newClientRect.Width() > bmpRect.Width() || newClientRect.Height() > bmpRect.Height() )
	{
		CClientDC cldc( this );
		if ( bmp.GetSafeHandle() )
			bmp.DeleteObject();
		bmp.CreateCompatibleBitmap( &cldc, newClientRect.Width(), newClientRect.Height() );
		bmpRect = newClientRect;
	}

	recalcLayout();
	updateScrollBars();
}

void RDOStudioChartView::OnChartTimewrap() 
{
	timeWrap = !timeWrap;
	recalcLayout();
	updateScrollBars();
	Invalidate();
	UpdateWindow();
}

void RDOStudioChartView::OnUpdateChartTimewrap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( !canUnwrapTime() || timeWrap );
	pCmdUI->Enable( canUnwrapTime() );
}

void RDOStudioChartView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	bool lastvisible = maxXVisible();
	recalcLayout();
	updateScrollBars();
	if ( lastvisible  && !maxXVisible() ) {
		/*xPos = xMax;
		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		Invalidate();
		UpdateWindow();*/
		setScrollPos( SB_HORZ, xMax );
	}
	CView::OnUpdate( pSender, lHint, pHint );
}

void RDOStudioChartView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
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
			inc = -tickWidth;
			break;

		case SB_LINEDOWN:
			inc = tickWidth;
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
	if ( inc = max ( -xPos, min ( inc, xMax - xPos ) ) ) {
		xPos += inc;
		
		/*ScrollWindowEx( -charWidth * inc, 0,
			(CONST CRect*)NULL, (CONST CRect*)NULL,
			(CRgn*)NULL, (CRect*)NULL, SW_INVALIDATE );
		
		if ( isVisible( selectedLine) ) {
			CRect rect;
			getLineRect( selectedLine, &rect );
			InvalidateRect( &rect );
		}*/
		
		/*SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		Invalidate();
		UpdateWindow();*/
		setScrollPos( SB_HORZ, xPos );
	}
}

void RDOStudioChartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;
	bool ctrl = ( ( ::GetKeyState( VK_CONTROL ) & 0x80000000 ) != 0 );
	bool side = false;
	int pos = 0;
	
	switch ( nChar ) {
	
		/*case VK_UP:
			selectLine( selectedLine - 1 );
			break;

		case VK_PRIOR:
			selectLine( max ( selectedLine - yPageSize, 0 ) );
			break;

		case VK_NEXT:
			selectLine( min ( selectedLine + yPageSize, stringsCount - 1 ) );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( stringsCount - 1 );
			break;*/

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

	/*if ( side ) {
		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		Invalidate();
		UpdateWindow();
	}*/
	if ( side )
		setScrollPos( SB_HORZ, pos );
}

void RDOStudioChartView::OnEditCopy() 
{
	copyToClipboard();
}

void RDOStudioChartView::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	CView::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOStudioChartView::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	CView::OnContextMenu( pWnd, pos );
	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

void RDOStudioChartView::OnChartZoomZoomin() 
{
	double delta = zoom < 1 ? 0.1 : 0.5;
	setZoom( zoom + delta );
}

void RDOStudioChartView::OnChartZoomZoomout() 
{
	double delta = zoom > 1 ? -0.5 : -0.1;
	double zoom_new = zoom + delta;
	if ( zoom_new < auto_zoom )
		zoom_new = auto_zoom;
	setZoom( zoom_new );
}

void RDOStudioChartView::OnChartZoomResetzoom() 
{
	setZoom( 1 );
}

void RDOStudioChartView::OnUpdateChartZoomZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !zoomAuto && zoom < 5 );
}

void RDOStudioChartView::OnUpdateChartZoomZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !zoomAuto && zoom > 0.2 && zoom != auto_zoom && auto_zoom <= 1 );
}

void RDOStudioChartView::OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( zoom != 1 );
}

void RDOStudioChartView::OnChartZoomZoomauto() 
{
	zoomAuto = !zoomAuto;
	if ( !zoomAuto )
		setZoom( old_zoom );
	else
		old_zoom = zoom;
	recalcLayout();
	updateScrollBars();
	Invalidate();
	UpdateWindow();
}

void RDOStudioChartView::OnUpdateChartZoomZoomauto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( zoomAuto );
}
