#include "stdafx.h"
#include "rdostudiochartview.h"
#include "rdostudiochartdoc.h"
#include "./rdo_tracer/rdotracertrace.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartView::RDOStudioChartView()
	: CView(),
	bmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	dragedSerie( NULL )
{
}

RDOStudioChartView::~RDOStudioChartView()
{
}

BOOL RDOStudioChartView::PreCreateWindow(CREATESTRUCT& cs) 
{
//	return CView::PreCreateWindow(cs);
	if ( !CView::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE /*| WS_HSCROLL | WS_VSCROLL*/ | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

void RDOStudioChartView::OnDraw(CDC* pDC)
{
	int valcountx = 5;
	int valcounty = 5;
	RDOTracerSerie* yaxis = NULL;
	
	
	RDOStudioChartDoc* pDoc = GetDocument();

	if ( pDoc->series.size() )
		yaxis = pDoc->series.at( 0 );

	GetClientRect( &newClientRect );
	CRect rect;
	rect.CopyRect( &newClientRect );

	if ( newClientRect.Width() > bmpRect.Width() || newClientRect.Height() > bmpRect.Height() )
	{
		if ( bmp.GetSafeHandle() )
			bmp.DeleteObject();
		bmp.CreateCompatibleBitmap( pDC, newClientRect.Width(), newClientRect.Height() );
		bmpRect = newClientRect;
	}

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
	rect.top = height;
	rect.bottom = newClientRect.Height() - height;
	rect.left = 5;
	rect.right = newClientRect.right - 5;
	
	if ( rect.Height() > 0 && rect.Width() > 0 ) {
		
		CRect tmprect;
		CSize size;
		//draw y aixs

		tmprect.CopyRect( &rect );
		if ( yaxis ) {
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
			double valoffset = ( yaxis->getMaxValue() - yaxis->getMinValue() ) / ( valcounty - 1 );
			int heightoffset = rect.Height() / ( valcounty - 1 );
			double valo = yaxis->getMinValue();
			int y = rect.bottom;
			int maxx = 0;
			for ( int j = 0; j < valcounty; j++ ) {
				//tmprect.left += widthoffset;
				str = format( formatstr.c_str(), valo );
				size = dc.GetTextExtent( str.c_str() );
				if ( size.cx > maxx )
					maxx = size.cx;
				tmprect.top = y - size.cy / 2;
				tmprect.bottom += size.cy / 2;
				dc.DrawText( str.c_str(), tmprect, DT_LEFT );
				valo += valoffset;
				y -= heightoffset;
			}

			rect.left += ( maxx + 2 );
			
			y = rect.bottom;
			for ( j = 1; j < valcounty - 1; j++ ) {
				y -= heightoffset;
				dc.MoveTo( rect.left, y );
				dc.LineTo( rect.left + 3, y );
			}
			rect.left += 3;
		}
		
		//draw time aixs
		double timerange = trace.getMaxTime()->time;
		tmprect.top = rect.bottom;
		tmprect.left = rect.left;
		tmprect.bottom = newClientRect.bottom;
		tmprect.right = rect.right;
		str = format( "%.3f", 0 );
		dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		//CSize size = dc.GetTextExtent( str.c_str() );
		//rect.left += size.cx / 2;
		str = format( "%.3f", timerange );
		dc.DrawText( str.c_str(), tmprect, DT_RIGHT );
		//size = dc.GetTextExtent( str.c_str() );
		//rect.right -= size.cx / 2;
		rect.right -= size.cx;
		int x = rect.left;
		int widthoffset = rect.Width() / (valcountx - 1);
		double valoffset = timerange / (valcountx - 1);
		double tempo = 0;
		for ( int j = 1; j < valcountx - 1; j++ ) {
			tempo += valoffset;
			x += widthoffset;
			dc.MoveTo( x, rect.bottom );
			dc.LineTo( x, rect.bottom - 3 );
			str = format( "%.3f", tempo );
			//size = dc.GetTextExtent( str.c_str() );
			//tmprect.left = x - size.cx / 2;
			tmprect.left = x;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		}

		rect.bottom -= 2;

		dc.Rectangle( rect );
		rect.InflateRect( -1, -1 );

		int count = pDoc->series.size();
		for ( int i = 0; i < count; i++ ) {
			pDoc->series.at( i )->drawSerie( dc, rect );
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
	UINT format = trace.getClipboardFormat();
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
	if ( pDataObject->IsDataAvailable( trace.getClipboardFormat() ) && dragedSerie )
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
	return CView::OnCreate( lpCreateStruct );
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

}
