#include "stdafx.h"
#include "rdostudioframeview.h"
#include "rdostudioframedoc.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameView, RDOStudioView)

BEGIN_MESSAGE_MAP(RDOStudioFrameView, RDOStudioView)
	//{{AFX_MSG_MAP(RDOStudioFrameView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, RDOStudioView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioFrameView::RDOStudioFrameView():
	RDOStudioView(),
	frameBmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	xPos( 0 ),
	yPos( 0 ),
	bgColor( studioApp.mainFrame->style_frame.theme->backgroundColor ),
	mustBeInit( true ),
	hwnd( NULL ),
	hdc( NULL ),
	hmemdc( NULL ),
	saved_hdc( 0 ),
	saved_hmemdc( 0 ),
	hfontInit( NULL ),
	hfontCurrent( NULL ),
//	hbmpInit( NULL ),
	hbmp( NULL )
{
}

RDOStudioFrameView::~RDOStudioFrameView()
{
}

BOOL RDOStudioFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !RDOStudioView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

int RDOStudioFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOStudioView::OnCreate(lpCreateStruct) == -1 ) return -1;

	// Remembering handle to the window in hwnd member
	hwnd = GetSafeHwnd();
	// Remembering handle to the private device context in hdc member
	if ( hwnd ) hdc = ::GetDC( hwnd );
	if ( !hdc ) return -1;

	// Creating memory device context to draw on bitmap
	hmemdc = ::CreateCompatibleDC( hdc );
	if ( !hmemdc ) return -1;

	// Saving the own DC and memory DC states to restore them
	// before deleting the memory DC or destroying window
	saved_hdc    = ::SaveDC( hdc );
	saved_hmemdc = ::SaveDC( hmemdc );
	if ( !saved_hdc || !saved_hmemdc ) return -1;
	// Setting background mode one time in initialization.
	// We have private DC, so we needn't to reset it each time
	// we paint
	::SetBkMode( hmemdc, TRANSPARENT );
	// Remembering default font to select it into DC
	// when destroying window or setting new font
	hfontInit = static_cast<HFONT>(::GetCurrentObject( hmemdc, OBJ_FONT ));
	// Remembering default bmp to select it into DC
	// when resizing window
//	hbmpInit  = static_cast<HBITMAP>(::GetCurrentObject( hmemdc, OBJ_BITMAP ));

	updateFont();
	updateScrollBars();

	return 0;
}

void RDOStudioFrameView::updateFont()
{
	int index = model->frameManager.findFrameIndex( this );
	if ( index != -1 ) {
		model->frameManager.getFrameMutexDraw( index )->Lock();
	}

	if ( hfontCurrent ) {
		::SelectObject( hmemdc, hfontInit );
		::DeleteObject( hfontCurrent );
	}

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	RDOStudioFrameStyle* style = &studioApp.mainFrame->style_frame;
	lf.lfHeight    = -MulDiv( style->font->size, ::GetDeviceCaps( hmemdc, LOGPIXELSY ), 72 );
	lf.lfWeight    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC;
	lf.lfUnderline = style->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = style->font->characterSet;
	strcpy( lf.lfFaceName, style->font->name.c_str() );

	hfontCurrent = ::CreateFontIndirect( &lf );
	::SelectObject( hmemdc, hfontCurrent );

	if ( index != -1 ) {
		model->frameManager.getFrameMutexDraw( index )->Unlock();
	}
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
	RDOStudioView::AssertValid();
}

void RDOStudioFrameView::Dump(CDumpContext& dc) const
{
	RDOStudioView::Dump(dc);
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
	if ( hdc ) {
		::RestoreDC( hdc, saved_hdc );
	}
	if ( hmemdc ) {
		::RestoreDC( hmemdc, saved_hmemdc );
		::DeleteDC( hmemdc );
	}
	if ( hbmp ) {
		::DeleteObject( hbmp );
	}

	RDOStudioView::OnDestroy();
}

void RDOStudioFrameView::OnSize(UINT nType, int cx, int cy)
{
	RDOStudioView::OnSize(nType, cx, cy);

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
	si.nMax   = frameBmpRect.right - 1;
	si.nPos   = xPos;
	si.nPage  = newClientRect.right;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	if ( yPos > frameBmpRect.bottom - newClientRect.bottom ) yPos = frameBmpRect.bottom - newClientRect.bottom;
	if ( yPos < 0 ) yPos = 0;
	si.nMin   = 0;
	si.nMax   = frameBmpRect.bottom - 1;
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

	point.Offset( xPos, yPos );
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

	RDOStudioView::OnLButtonDown( nFlags, point );
}

void RDOStudioFrameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
/*
	RDOStudioFrameManager* frameManager = &model->frameManager;
	int index = frameManager->findFrameIndex( this );
	CSingleLock lock_draw( frameManager->getFrameMutexDraw( index ) );
	lock_draw.Lock();

	RDOStudioFrameManager::Frame* frame = frameManager->frames[index];
	frame->keys_pressed.push_back( nChar );

	lock_draw.Unlock();
*/

	if(!(nFlags & (1 << 14)))
		kernel.getSimulator()->keyDown( nChar );

	RDOStudioView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	kernel.getSimulator()->keyUp( nChar );

	RDOStudioView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if ( bActivate ) {
		int index = model->frameManager.findFrameIndex( this );
		model->frameManager.setLastShowedFrame( index );
		model->frameManager.setCurrentShowingFrame( index );
	}
	RDOStudioView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}

BOOL RDOStudioFrameView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_POS;
	yPos      -= newClientRect.bottom / 4 * (zDelta >= 0 ? 1 : -1);
	if ( yPos > frameBmpRect.bottom - newClientRect.bottom ) yPos = frameBmpRect.bottom - newClientRect.bottom;
	if ( yPos < 0 ) yPos = 0;
	si.nPos   = yPos;
	SetScrollInfo( SB_VERT, &si, TRUE );
	InvalidateRect( NULL );
	UpdateWindow();

	return TRUE;
}

void RDOStudioFrameView::OnPaint()
{
	int index = model->frameManager.findFrameIndex( this );
	CSingleLock lock_draw( model->frameManager.getFrameMutexDraw( index ) );
	lock_draw.Lock();

	PAINTSTRUCT ps;
	::BeginPaint( hwnd, &ps );
	onDraw();
	::EndPaint( hwnd, &ps );

	lock_draw.Unlock();
	model->frameManager.getFrameEventTimer( index )->SetEvent();
}

void RDOStudioFrameView::OnDraw( CDC* pDC )
{
	RDOStudioView::OnDraw( pDC );
}

void RDOStudioFrameView::onDraw()
{
//	RDOStudioFrameDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

	GetClientRect( &newClientRect );

	if ( !mustBeInit ) {
		::BitBlt( hdc, 0, 0, newClientRect.right, newClientRect.bottom, hmemdc, xPos, yPos, SRCCOPY );
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
		if ( newClientRect.right - frameBmpRect.right > 0 ) {
//			dc.FillSolidRect( frameBmpRect.right, 0, newClientRect.right - frameBmpRect.right, newClientRect.bottom, bgColor );
			HBRUSH brush = ::CreateSolidBrush( bgColor );
			RECT r = { frameBmpRect.right, 0, newClientRect.right, newClientRect.bottom };
			::FillRect( hdc, &r, brush );
			::DeleteObject( brush );
		}
		if ( newClientRect.bottom - frameBmpRect.bottom > 0 ) {
//			dc.FillSolidRect( 0, frameBmpRect.bottom, newClientRect.right, newClientRect.bottom - frameBmpRect.bottom, bgColor );
			HBRUSH brush = ::CreateSolidBrush( bgColor );
			RECT r = { 0, frameBmpRect.bottom, frameBmpRect.right, newClientRect.bottom };
			::FillRect( hdc, &r, brush );
			::DeleteObject( brush );
		}
	} else {
		HBRUSH brush = ::CreateSolidBrush( bgColor );
		::FillRect( hdc, &newClientRect, brush );
		::DeleteObject( brush );
	}
}
