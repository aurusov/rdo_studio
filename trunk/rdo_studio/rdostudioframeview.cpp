#include "stdafx.h"
#include "rdostudioframeview.h"
#include "rdostudioframedoc.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameView, CView)

BEGIN_MESSAGE_MAP(RDOStudioFrameView, CView)
	//{{AFX_MSG_MAP(RDOStudioFrameView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioFrameView::RDOStudioFrameView():
	CView(),
	frameBmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	xPos( 0 ),
	yPos( 0 ),
	bgColor( studioApp.mainFrame->style_frame.theme->backgroundColor ),
	mustBeInit( true )
{
	dc.CreateCompatibleDC( NULL );
}

RDOStudioFrameView::~RDOStudioFrameView()
{
}

BOOL RDOStudioFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

int RDOStudioFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CView::OnCreate(lpCreateStruct) == -1 ) return -1;

	updateScrollBars();

	return 0;
}

void RDOStudioFrameView::OnDraw(CDC* pDC)
{
//	RDOStudioFrameDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

	int index = model->frameManager.findFrameIndex( this );
	CSingleLock lock_draw( model->frameManager.getFrameMutexDraw( index ) );
	lock_draw.Lock();

	GetClientRect( &newClientRect );

	if ( !mustBeInit ) {
		CBitmap* pOldBitmap = dc.SelectObject( &frameBmp );
/*
		pDC->SetStretchBltMode( HALFTONE );
		double k1 = (double)newClientRect.bottom / frameBmpRect.bottom;
		double k2 = (double)newClientRect.right / frameBmpRect.right;
		double k = min( k1, k2 );
		if ( k > 1 ) k = 1;
		CRect rect( 0, 0, frameBmpRect.right * k, frameBmpRect.bottom * k );
		rect.OffsetRect( (newClientRect.right - rect.right) / 2, (newClientRect.bottom - rect.bottom) / 2 );
		pDC->StretchBlt( rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, &dc, 0, 0, frameBmpRect.right, frameBmpRect.bottom, SRCCOPY );
		if ( rect.left ) {
			pDC->FillSolidRect( 0, 0, rect.left, newClientRect.bottom, bgColor );
			pDC->FillSolidRect( rect.right, 0, newClientRect.right, newClientRect.bottom, bgColor );
		}
		if ( rect.top ) {
			pDC->FillSolidRect( 0, 0, newClientRect.right, rect.top, bgColor );
			pDC->FillSolidRect( 0, rect.bottom, newClientRect.right, newClientRect.bottom, bgColor );
		}
*/

		pDC->BitBlt( 0, 0, newClientRect.right, newClientRect.bottom, &dc, xPos, yPos, SRCCOPY );
		dc.SelectObject( pOldBitmap );
	}

	if ( newClientRect.right - frameBmpRect.right > 0 ) {
		pDC->FillSolidRect( frameBmpRect.right, 0, newClientRect.right - frameBmpRect.right, newClientRect.bottom, bgColor );
	}
	if ( newClientRect.bottom - frameBmpRect.bottom > 0 ) {
		pDC->FillSolidRect( 0, frameBmpRect.bottom, newClientRect.right, newClientRect.bottom - frameBmpRect.bottom, bgColor );
	}

	lock_draw.Unlock();

	model->frameManager.getFrameEventTimer( index )->SetEvent();
}

BOOL RDOStudioFrameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioFrameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RDOStudioFrameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RDOStudioFrameView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

RDOStudioFrameDoc* RDOStudioFrameView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioFrameDoc)));
	return (RDOStudioFrameDoc*)m_pDocument;
}
#endif

void RDOStudioFrameView::OnDestroy() 
{
	int index = model->frameManager.findFrameIndex( this );
	if ( index != -1 ) {
		model->frameManager.getFrameEventClose( index )->SetEvent();
		model->frameManager.disconnectFrameDoc( GetDocument() );
		model->frameManager.resetCurrentShowingFrame( index );
	}
	CView::OnDestroy();
}

void RDOStudioFrameView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GetClientRect( &newClientRect );

	updateScrollBars();
}

void RDOStudioFrameView::updateScrollBars()
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;

	if ( xPos > frameBmpRect.right - newClientRect.right ) xPos = frameBmpRect.right - newClientRect.right;
	if ( xPos < 0 ) xPos = 0;
	si.nMin   = 0;
	si.nMax   = frameBmpRect.right;
	si.nPos   = xPos;
	si.nPage  = newClientRect.right;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	if ( yPos > frameBmpRect.bottom - newClientRect.bottom ) yPos = frameBmpRect.bottom - newClientRect.bottom;
	if ( yPos < 0 ) yPos = 0;
	si.nMin   = 0;
	si.nMax   = frameBmpRect.bottom;
	si.nPos   = yPos;
	si.nPage  = newClientRect.bottom;
	SetScrollInfo( SB_VERT, &si, TRUE );
}

void RDOStudioFrameView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_PAGEUP:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			xPos -= si.nPage;
			break; 

		case SB_PAGEDOWN:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			xPos += si.nPage;
			break;

		case SB_LINEUP:
			xPos--;
			break;

		case SB_LINEDOWN:
			xPos++;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			xPos += si.nTrackPos - xPos;
			break;
		}
	}
	if ( xPos > frameBmpRect.right - newClientRect.right ) xPos = frameBmpRect.right - newClientRect.right;
	if ( xPos < 0 ) xPos = 0;
	si.fMask = SIF_POS;
	si.nPos  = xPos;
	SetScrollInfo( SB_HORZ, &si, TRUE );
	InvalidateRect( NULL );
	UpdateWindow();
}

void RDOStudioFrameView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_PAGEUP:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			yPos -= si.nPage;
			break; 

		case SB_PAGEDOWN:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			yPos += si.nPage;
			break;

		case SB_LINEUP:
			yPos--;
			break;

		case SB_LINEDOWN:
			yPos++;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			yPos += si.nTrackPos - yPos;
			break;
		}
	}
	if ( yPos > frameBmpRect.bottom - newClientRect.bottom ) yPos = frameBmpRect.bottom - newClientRect.bottom;
	if ( yPos < 0 ) yPos = 0;
	si.fMask = SIF_POS;
	si.nPos  = yPos;
	SetScrollInfo( SB_VERT, &si, TRUE );
	InvalidateRect( NULL );
	UpdateWindow();
}

void RDOStudioFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	RDOStudioFrameManager* frameManager = &model->frameManager;
	int index = frameManager->findFrameIndex( this );
	CSingleLock lock_draw( frameManager->getFrameMutexDraw( index ) );
	lock_draw.Lock();

	RDOStudioFrameManager::Frame* frame = frameManager->frames[index];
	vector< RDOStudioFrameManager::Area* >* areas_sim = &frame->areas_sim;
	vector< RDOStudioFrameManager::Area* >::iterator it = areas_sim->begin();
	while ( it != areas_sim->end() ) {
		RDOStudioFrameManager::Area* area = *it++;
		CRect rect( area->x, area->y, area->x + area->w, area->y + area->h );
		if ( rect.PtInRect( point ) ) {
			frame->areas_clicked.push_back( area->name );
		}
	};

	lock_draw.Unlock();

	CView::OnLButtonDown( nFlags, point );
}

void RDOStudioFrameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	RDOStudioFrameManager* frameManager = &model->frameManager;
	int index = frameManager->findFrameIndex( this );
	CSingleLock lock_draw( frameManager->getFrameMutexDraw( index ) );
	lock_draw.Lock();

	RDOStudioFrameManager::Frame* frame = frameManager->frames[index];
	frame->keys_pressed.push_back( nChar );

	lock_draw.Unlock();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if ( bActivate ) {
		int index = model->frameManager.findFrameIndex( this );
		model->frameManager.setLastShowedFrame( index );
		model->frameManager.setCurrentShowingFrame( index );
	}
	CView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}
