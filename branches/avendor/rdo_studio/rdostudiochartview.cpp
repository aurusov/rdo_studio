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
	RDOStudioChartDoc* pDoc = GetDocument();
	GetClientRect( &newClientRect );
	CRect rect;
	//GetClientRect( &newrect );
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
	

	CBrush brush( RGB( 255, 255, 255 ) );
	CBrush* pOldBrush = dc.SelectObject( &brush );
	CPen penBlack;
	penBlack.CreatePen( PS_SOLID, 0, RGB( 0, 0, 0 ) );
	CPen* pOldPen = dc.SelectObject( &penBlack );
	
	//dc.Rectangle( newClientRect );
	dc.FillSolidRect( newClientRect, RGB( 255, 255, 255 ) );

	CString str = pDoc->GetTitle();
	//newrect.OffsetRect( 1, 1 );
	dc.DrawText( str, rect, DT_CENTER | DT_CALCRECT );
	dc.DrawText( str, newClientRect, DT_CENTER );
	int height = rect.Height();
	rect.top = height;
	rect.bottom = newClientRect.Height() - height;
	rect.left = 10;
	rect.right = newClientRect.right - 10;
	
	if ( rect.Height() > 0 && rect.Width() > 0 ) {
		dc.Rectangle( rect );
		rect.InflateRect( -1, -1 );

		int count = pDoc->series.size();
		for ( int i = 0; i < count; i++ ) {
			pDoc->series.at( i )->drawSerie( dc, rect );
			/*dc.DrawText( pDoc->series.at( i )->getTitle(), newrect, DT_LEFT | DT_CALCRECT );
			dc.DrawText( pDoc->series.at( i )->getTitle(), newrect, DT_LEFT );
			newrect.OffsetRect( 0, newrect.Height() + 2 );*/
		}
	}

	pDC->BitBlt( 0, 0, newClientRect.Width(), newClientRect.Height(), &dc, 0, 0, SRCCOPY );
	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );
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
