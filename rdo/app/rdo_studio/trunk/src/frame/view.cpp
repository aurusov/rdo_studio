#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameView
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameView, RDOStudioView)

BEGIN_MESSAGE_MAP(RDOStudioFrameView, RDOStudioView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
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
	hbmp( NULL ),
	mouseOnHScroll( false )
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
	int index = model->m_frameManager.findFrameIndex( this );
	if ( index != -1 )
	{
		model->m_frameManager.getFrameMutexDraw( index )->Lock();
	}

	if ( hfontCurrent )
	{
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
	lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
	strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)

	hfontCurrent = ::CreateFontIndirect( &lf );
	::SelectObject( hmemdc, hfontCurrent );

	if ( index != -1 )
	{
		model->m_frameManager.getFrameMutexDraw( index )->Unlock();
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
	int index = model->m_frameManager.findFrameIndex( this );
	if ( index != -1 ) {
		model->m_frameManager.getFrameEventClose( index )->SetEvent();
		model->m_frameManager.disconnectFrameDoc( GetDocument() );
		model->m_frameManager.resetCurrentShowingFrame( index );
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
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	if ( nSBCode == SB_ENDSCROLL ) {
		model->setGUIContinue();
	}
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_LEFT:
			xPos = 0;
			break;

		case SB_RIGHT:
			xPos = frameBmpRect.right - newClientRect.right;
			break;

		case SB_PAGELEFT:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			xPos -= si.nPage;
			break; 

		case SB_PAGERIGHT:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			xPos += si.nPage;
			break;

		case SB_LINELEFT:
			xPos--;
			break;

		case SB_LINERIGHT:
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
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	if ( nSBCode == SB_ENDSCROLL ) {
		model->setGUIContinue();
	}
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_TOP:
			yPos = 0;
			break;

		case SB_BOTTOM:
			yPos = frameBmpRect.bottom - newClientRect.bottom;
			break;

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
	RDOStudioFrameManager* frameManager = &model->m_frameManager;
	int index = frameManager->findFrameIndex( this );
	CSingleLock lock_draw( frameManager->getFrameMutexDraw( index ) );
	lock_draw.Lock();

	point.Offset( xPos, yPos );
	std::vector< RDOStudioFrameManager::Area* >* areas_sim = &frameManager->frames[index]->areas_sim;
	std::vector< RDOStudioFrameManager::Area* >::iterator it = areas_sim->begin();
	while ( it != areas_sim->end() ) {
		RDOStudioFrameManager::Area* area = *it;
		if ( CRect( area->x, area->y, area->x + area->w, area->y + area->h ).PtInRect( point ) ) {
			model->sendMessage( kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &area->name );
		}
		it++;
	};

	lock_draw.Unlock();

	RDOStudioView::OnLButtonDown( nFlags, point );
}

void RDOStudioFrameView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	model->sendMessage( kernel->runtime(), RDOThread::RT_RUNTIME_KEY_DOWN, &nChar );
	RDOStudioView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	model->sendMessage( kernel->runtime(), RDOThread::RT_RUNTIME_KEY_UP, &nChar );
	RDOStudioView::OnKeyUp( nChar, nRepCnt, nFlags );
}

void RDOStudioFrameView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if ( bActivate ) {
		int index = model->m_frameManager.findFrameIndex( this );
		model->m_frameManager.setLastShowedFrame( index );
		model->m_frameManager.setCurrentShowingFrame( index );
	}
	RDOStudioView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}

BOOL RDOStudioFrameView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	UNUSED(nFlags);
	UNUSED(pt    );

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_POS;
	if ( mouseOnHScroll ) {
		xPos      -= newClientRect.right / 4 * (zDelta >= 0 ? 1 : -1);
		if ( xPos > frameBmpRect.right - newClientRect.right ) xPos = frameBmpRect.right - newClientRect.right;
		if ( xPos < 0 ) xPos = 0;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );
	} else {
		yPos      -= newClientRect.bottom / 4 * (zDelta >= 0 ? 1 : -1);
		if ( yPos > frameBmpRect.bottom - newClientRect.bottom ) yPos = frameBmpRect.bottom - newClientRect.bottom;
		if ( yPos < 0 ) yPos = 0;
		si.nPos   = yPos;
		SetScrollInfo( SB_VERT, &si, TRUE );
	}
	InvalidateRect( NULL );
	UpdateWindow();
	return TRUE;
}

void RDOStudioFrameView::OnPaint()
{
	int index = model->m_frameManager.findFrameIndex( this );
	CSingleLock lock_draw( model->m_frameManager.getFrameMutexDraw( index ) );
	lock_draw.Lock();

	PAINTSTRUCT ps;
	::BeginPaint( hwnd, &ps );
	onDraw();
	::EndPaint( hwnd, &ps );

	lock_draw.Unlock();
	model->m_frameManager.getFrameEventTimer( index )->SetEvent();
}

void RDOStudioFrameView::OnDraw( CDC* pDC )
{
	RDOStudioView::OnDraw( pDC );
}

void RDOStudioFrameView::onDraw()
{
//	RDOStudioFrameDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

//	GetClientRect( &newClientRect );

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

void RDOStudioFrameView::OnHelpKeyword()
{
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_model_frame.htm";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

void RDOStudioFrameView::OnNcLButtonDown( UINT nHitTest, CPoint point )
{
	model->setGUIPause();
	RDOStudioView::OnNcLButtonDown( nHitTest, point );
}

void RDOStudioFrameView::OnNcRButtonDown( UINT nHitTest, CPoint point )
{
	model->setGUIPause();
	RDOStudioView::OnNcRButtonDown( nHitTest, point );
}

void RDOStudioFrameView::OnNcMouseMove( UINT nHitTest, CPoint point )
{
	mouseOnHScroll = nHitTest == HTHSCROLL;
	RDOStudioView::OnNcMouseMove( nHitTest, point );
}

void RDOStudioFrameView::OnMouseMove( UINT nFlags, CPoint point )
{
	mouseOnHScroll = false;
	RDOStudioView::OnMouseMove( nFlags, point );
}
