#include "stdafx.h"
#include "rdostudiochartview.h"
#include "rdo_tracer/rdotracertrace.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"
#include "rdostudiochartviewstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

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
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartView::RDOStudioChartView( const bool preview )
	: CView(),
	bmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	dragedSerie( NULL ),
	valueCountX( 5 ),
	valueCountY( 5 ),
	yaxis( NULL ),
	tickWidth( 5 ),
	timeWrap( true ),
	chartRect( 0, 0, 0, 0 ),
	xMax( 0 ),
	xPos( 0 ),
	timeScale( 0 ),
	drawFromEventIndex( 0 ),
	drawToEventCount( 0 ),
	zoom( 1 ),
	old_zoom( 1 ),
	auto_zoom( 1 ),
	zoomAuto( false ),
	scale_koeff( 1 ),
	style( NULL ),
	previewMode( preview ),
	font_sizeTitle( 1.2 ),
	font_sizeLegend( 0.8 )
{
	if ( previewMode )
		timeWrap = false;
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

	CFont* oldFont = dc.SelectObject( &fontTitle );

	RDOStudioChartDoc* doc = GetDocument();
	
	string str = doc->GetTitle();

	chartRect.top = dc.GetTextExtent( str.c_str() ).cy;

	dc.SelectObject( &fontAxis );

	double val = 0;

	chartRect.left = dc.GetTextExtent( format( "%.3f", val ).c_str() ).cx + 5;

	if ( !doc->docTimes.empty() )
		val = doc->docTimes.back()->time;
	else
		val = 0;
	chartRect.right = newClientRect.right - dc.GetTextExtent( format( "%.3f", val ).c_str() ).cx - 5;

	dc.SelectObject( oldFont );

	chartRect.bottom = newClientRect.bottom - chartRect.top - 2 - 3;
	
	if ( !doc->docTimes.empty() ) {
		double timeRange = doc->docTimes.back()->time - doc->docTimes.front()->time;
		if ( timeRange > 0 ) {
			long double timeScaleAuto = doUnwrapTime() ? (double)( chartRect.Width() - tickWidth * doc->ticksCount ) / timeRange : (double)chartRect.Width() / timeRange;
			timeScale = (double)tickWidth / doc->minTimeOffset;
			auto_zoom = timeScaleAuto / timeScale;
			if ( doUnwrapTime() && auto_zoom < 0 ) {
				auto_zoom = 1;
			}
			if ( zoomAuto || ( auto_zoom > 1 && scale_koeff < auto_zoom ) ) {
				scale_koeff = auto_zoom;
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
}

void RDOStudioChartView::setScrollPos( UINT nSBCode, UINT nPos, const bool need_update )
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
		UpdateWindow();
	}
}

void RDOStudioChartView::updateScrollBars( const bool need_update )
{
	RDOStudioChartDoc* doc = GetDocument();
	int size;
	if ( !doc->docTimes.empty() ) {
		size = roundDouble( ( doc->docTimes.back()->time - doc->docTimes.front()->time ) * timeScale );
		if ( doUnwrapTime() )
			size += tickWidth * doc->ticksCount;
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
}

bool RDOStudioChartView::setTo( const int from_max_pos )
{
	bool res = true;
	int delta = ( from_max_pos - xPos - chartRect.Width() ) / tickWidth;
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
		int ticks = 0;
		for( timesList::iterator it = doc->docTimes.begin(); it != doc->docTimes.end(); it++ ) {
			it_pos = roundDouble( ( (*it)->time - doc->docTimes.front()->time ) * timeScale ) + ticks * tickWidth;
			it_max_pos = it_pos + tickWidth * (*it)->eventCount;
			if ( it_pos == xPos ) {
				drawFromX = *(*it);
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos < xPos && ( it_max_pos >= xPos ) ) {
				drawFromX = *(*it);
				drawFromEventIndex = roundDouble( (double)( xPos - it_pos ) / (double)tickWidth );
				need_search_to = setTo( it_max_pos );
				break;
			}
			if ( it_pos > xPos ) {
				drawFromX.time = (*it)->time - ( it_pos - xPos ) / timeScale;
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
				int delta = drawToX.eventCount * tickWidth - chartRect.Width();
				drawToEventCount = delta >= 0 ? roundDouble( (double)delta / (double)tickWidth ) : drawToX.eventCount;

			}
			int pos = xPos + chartRect.Width();
			for( ; it != doc->docTimes.end(); it++ ) {
				it_pos = roundDouble( ( (*it)->time - doc->docTimes.front()->time ) * timeScale ) + ticks * tickWidth;
				it_max_pos = it_pos + tickWidth * (*it)->eventCount;
				if ( it_pos == pos ) {
					drawToX = *(*it);
					break;
				}
				if ( it_pos < pos && it_max_pos >= pos ) {
					drawToX = *(*it);
					drawToEventCount = roundDouble( (double)( pos - it_pos ) / (double)tickWidth );
					break;
				}
				if ( it_pos > pos ) {
					drawToX.time = (*it)->time - ( it_pos - pos ) / timeScale;
					break;
				}
				unwrapTimesList.push_back( (*it) );
				ticks += (*it)->eventCount;
			}
		}
		unwrapTimesList.push_back( &drawToX );
	}
}

void RDOStudioChartView::drawTitle( CDC &dc, CRect& chartRect )
{
	//CRect tmprect;
	//tmprect.CopyRect( &newClientRect );
	int old_bk = dc.SetBkMode( TRANSPARENT );
	CFont*  oldFont = dc.SelectObject( &fontTitle );
	COLORREF old_color = dc.SetTextColor( style->getTheme()->titleFGColor );
	string str = GetDocument()->GetTitle();
	//dc.DrawText( str.c_str(), rect, DT_CENTER );
	dc.DrawText( str.c_str(), newClientRect, DT_CENTER );
	dc.SetTextColor( old_color );
	dc.SelectObject( oldFont );
	dc.SetBkMode( old_bk );
}

void RDOStudioChartView::drawYAxis( CDC &dc, CRect& chartRect, const RDOTracerSerie* axisValues)
{
	CRect tmprect;
	CSize size;

	tmprect.CopyRect( &newClientRect );
	tmprect.left = 5;
	tmprect.right = chartRect.left;
	if ( axisValues && axisValues->getValueCount() ) {
		int old_bk = dc.SetBkMode( TRANSPARENT );
		CFont*  oldFont = dc.SelectObject( &fontAxis );
		COLORREF old_color = dc.SetTextColor( style->getTheme()->axisFgColor );
		string formatstr = "%.3f";
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
		tmprect.top = y /*- size.cy / 2*/;
		dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		valo += valoffset;
		y -= heightoffset;
		if ( valoffset ) {
			for ( int i = 1; i < valueCountY; i++ ) {
				str = format( formatstr.c_str(), valo );
				tmprect.top = y /*- size.cy / 2*/;
				dc.DrawText( str.c_str(), tmprect, DT_LEFT );
				dc.MoveTo( chartRect.left, y );
				dc.LineTo( chartRect.left - 3, y );
				valo += valoffset;
				y -= heightoffset;
			}
		}
		dc.SetTextColor( old_color );
		dc.SelectObject( oldFont );
		dc.SetBkMode( old_bk );
	}

}

void RDOStudioChartView::drawXAxis( CDC &dc, CRect& chartRect )
{	
	CRect tmprect;
	CSize size;
	tmprect.CopyRect( &chartRect );
	
	tmprect.top = chartRect.bottom;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = newClientRect.right - 5;
	
	RDOStudioChartDoc* doc = GetDocument();
	if ( !doc->docTimes.empty() ) {
		string formatstr = "%.3f";
		int old_bk = dc.SetBkMode( TRANSPARENT );
		CFont*  oldFont = dc.SelectObject( &fontAxis );
		COLORREF old_color = dc.SetTextColor( style->getTheme()->axisFgColor );
		if( !doUnwrapTime() ) {
			double valoffset = 0;
			int widthoffset = chartRect.Width() / ( valueCountX - 1 );
			if ( drawToX != drawFromX ) {
				valoffset = ( drawToX.time - drawFromX.time ) / ( valueCountX - 1 );
			}
			double valo = drawFromX.time;
			int x = chartRect.left;
			string str = format( formatstr.c_str(), valo );
			size = dc.GetTextExtent( str.c_str() );
			tmprect.left = x /*- size.cy / 2*/;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
			valo += valoffset;
			x += widthoffset;
			if ( valoffset ) {
				for ( int i = 1; i < valueCountX; i++ ) {
					str = format( formatstr.c_str(), valo );
					tmprect.left = x /*- size.cy / 2*/;
					dc.DrawText( str.c_str(), tmprect, DT_LEFT );
					dc.MoveTo( x, chartRect.bottom );
					dc.LineTo( x, chartRect.bottom - 3 );
					valo += valoffset;
					x += widthoffset;
				}
			}
		} else {
			int ticks = 0;
			string str;
			for( timesList::iterator it = unwrapTimesList.begin(); it != unwrapTimesList.end(); it++ ) {
				int width = (*it)->eventCount * tickWidth;
				RDOTracerTimeNow* timenow = (*it);
				tmprect.left = chartRect.left + ( (*it)->time - unwrapTimesList.front()->time ) * timeScale + ticks * tickWidth;
				tmprect.left = min( tmprect.left, chartRect.right - 1 );
				str = format( formatstr.c_str(), (*it)->time );
				dc.DrawText( str.c_str(), tmprect, DT_LEFT );
				dc.MoveTo( tmprect.left, chartRect.bottom );
				dc.LineTo( tmprect.left, chartRect.bottom - 3 );

				ticks += (*it)->eventCount;
				if ( *(*it) == drawFromX ) {
					ticks -= drawFromEventIndex;
				}
			}
		}
		dc.SetTextColor( old_color );
		dc.SelectObject( oldFont );
		dc.SetBkMode( old_bk );
	}
}

void RDOStudioChartView::drawGrid(	CDC &dc, CRect& chartRect )
{
	CBrush brush_chart( style->getTheme()->chartBgColor );
	CBrush* old_brush = dc.SelectObject( &brush_chart );
	dc.Rectangle( chartRect );
	dc.SelectObject( old_brush );

	if ( doUnwrapTime() ) {
		CRect rect;
		rect.CopyRect( chartRect );
		rect.InflateRect( -1, -1 );
		CRect tmprect;
		tmprect.CopyRect( &rect );
		
		RDOStudioChartDoc* doc = GetDocument();
		dc.IntersectClipRect( &rect );
			int ticks = 0;
			timesList::iterator it = unwrapTimesList.begin();
			if ( drawFromX == drawToX ) {
				it ++;
			}
			for( ; it != unwrapTimesList.end(); it++ ) {
				int width = (*it)->eventCount * tickWidth;
				RDOTracerTimeNow* timenow = (*it);
				tmprect.left = rect.left + ( (*it)->time - unwrapTimesList.front()->time ) * timeScale + ticks * tickWidth;
				if ( *(*it) == drawFromX ) {
					width -= drawFromEventIndex * tickWidth;
				}
				if ( *(*it) == drawToX ) {
					width = drawToEventCount * tickWidth;
				}
				tmprect.right = tmprect.left + width;
				dc.FillSolidRect( &tmprect, style->getTheme()->timeBgColor );
				ticks += (*it)->eventCount;
				if ( *(*it) == drawFromX ) {
					ticks -= drawFromEventIndex;
				}
			}
		dc.SelectClipRgn( NULL );
	}
}

void RDOStudioChartView::copyToClipboard()
{
	if ( !OpenClipboard() || !::EmptyClipboard() )
		return;
	
	CClientDC cldc( this );
	CDC dc;
	if ( dc.CreateCompatibleDC( &cldc ) ) {
		CBitmap* oldBitmap = dc.SelectObject( &bmp );
		HBITMAP hbm = ::CreateCompatibleBitmap( dc.GetSafeHdc(), newClientRect.Width(), newClientRect.Height() );
		if ( hbm ) {
			HDC hdcDst = ::CreateCompatibleDC( dc.GetSafeHdc() );
			if ( hdcDst ) {
				HGDIOBJ oldObj = ::SelectObject( hdcDst, hbm );

				::BitBlt( hdcDst, 0, 0, newClientRect.Width(), newClientRect.Height(),
					dc.GetSafeHdc(), newClientRect.left, newClientRect.top, SRCCOPY );
				
				::SelectObject( hdcDst, oldObj );
				::DeleteDC( hdcDst );
				
				::SetClipboardData( CF_BITMAP, hbm );
			} else {
				::DeleteObject( hbm );
			}
			dc.SelectObject( oldBitmap );
		}

	}

	CloseClipboard();
}

void RDOStudioChartView::setZoom( double new_zoom, const bool force_update )
{
	scale_koeff = new_zoom;
	if ( scale_koeff < auto_zoom ) {
		scale_koeff = auto_zoom;
	}
	if ( zoom != new_zoom || scale_koeff != new_zoom || force_update ) {
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

	CBrush brush_background( style->theme->backgroundColor );
	CBrush* pOldBrush = dc.SelectObject( &brush_background );
	CPen pen_chart;
	pen_chart.CreatePen( PS_SOLID, 0, style->theme->defaultColor );
	CPen* pOldPen = dc.SelectObject( &pen_chart );
	
	dc.FillSolidRect( newClientRect, style->theme->backgroundColor );

	//int height = rect.Height();
	drawTitle( dc, chartRect );
	
	if ( chartRect.Height() > 0 && chartRect.Width() > 0 ) {

		drawYAxis( dc, chartRect, yaxis );

		setFromTo();
		
		drawXAxis( dc, chartRect );

		drawGrid( dc, chartRect );

		for ( vector< RDOStudioDocSerie >::iterator it = pDoc->series.begin(); it != pDoc->series.end(); it++ )
			it->getSerie()->drawSerie( this, dc, chartRect, it->getColor() );
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
	
	setStyle( style = &studioApp.mainFrame->style_chart, false );

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
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 4, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 6, &popupMenu );

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
	if ( !m_pDocument )
		return NULL;
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

	if ( GetDocument() ) {
		recalcLayout();
		updateScrollBars( false );
		setZoom( zoom );
	}
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
	updateScrollBars( false );
	if ( lastvisible  && !maxXVisible() ) {
		setScrollPos( SB_HORZ, xMax, false );
	}
	CView::OnUpdate( pSender, lHint, pHint );
	updateScrollBars( true );
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
	CView::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOStudioChartView::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	if ( previewMode ) return;
	CView::OnContextMenu( pWnd, pos );
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
	pCmdUI->Enable( !zoomAuto && zoom > auto_zoom );
}

void RDOStudioChartView::OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( zoom != 1 );
}

void RDOStudioChartView::OnChartZoomZoomauto() 
{
	zoomAuto = !zoomAuto;
	if ( !zoomAuto )
		setZoom( old_zoom, true );
	else {
		old_zoom = zoom;
		setZoom( auto_zoom, true );
	}
}

void RDOStudioChartView::OnUpdateChartZoomZoomauto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( zoomAuto );
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
		return CView::OnMouseActivate( pDesktopWnd, nHitTest, message );
	}
}

void RDOStudioChartView::setFonts( const bool needRedraw )
{
	if ( !style ) return;
	
	CClientDC dc( this );
	LOGFONT lf;
	RDOStudioChartViewTheme* chart_theme = static_cast<RDOStudioChartViewTheme*>(style->theme);

	if ( !fontAxis.m_hObject || fontAxis.DeleteObject() ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		lf.lfHeight    = -MulDiv( style->font->size, ::GetDeviceCaps( dc.GetSafeHdc(), LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->defaultStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->defaultStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->defaultStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = style->font->characterSet;
		strcpy( lf.lfFaceName, style->font->name.c_str() );

		fontAxis.CreateFontIndirect( &lf );
	}

	if ( !fontTitle.m_hObject || fontTitle.DeleteObject() ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		bool b = chart_theme->titleStyle & RDOStyleFont::BOLD;
		lf.lfHeight    = -MulDiv( roundDouble( style->font->size * font_sizeTitle ), ::GetDeviceCaps( dc.GetSafeHdc(), LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->titleStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->titleStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->titleStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = style->font->characterSet;
		strcpy( lf.lfFaceName, style->font->name.c_str() );

		fontTitle.CreateFontIndirect( &lf );
	}
	if ( !fontLegend.m_hObject || fontLegend.DeleteObject() ) {

		memset( &lf, 0, sizeof(lf) );
		// The negative is to allow for leading
		lf.lfHeight    = -MulDiv( roundDouble( style->font->size * font_sizeLegend ), ::GetDeviceCaps( dc.GetSafeHdc(), LOGPIXELSY ), 72 );
		lf.lfWeight    = chart_theme->legendStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic    = chart_theme->legendStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->legendStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet   = style->font->characterSet;
		strcpy( lf.lfFaceName, style->font->name.c_str() );

		fontLegend.CreateFontIndirect( &lf );
	}
}

void RDOStudioChartView::setStyle( RDOStudioChartViewStyle* _style, const bool needRedraw )
{
	style = _style;

	setFonts( false );
	
	if ( needRedraw ) {
		recalcLayout();
		updateScrollBars();
		Invalidate();
		UpdateWindow();
	}
}
