/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

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

RDOStudioFrameView::RDOStudioFrameView()
	: RDOStudioView()
	, frameBmpRect( 0, 0, 0, 0 )
	, newClientRect( 0, 0, 0, 0 )
	, mustBeInit( true )
	, m_hwnd( NULL )
	, hfontInit( NULL )
	, hfontCurrent( NULL )
	, mouseOnHScroll( false )
{
	m_bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
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
	if (RDOStudioView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hwnd = GetSafeHwnd();
//	m_pGraphics.reset(new Gdiplus::Graphics(m_hwnd));
//	if (!m_pGraphics.get() || m_pGraphics->GetLastStatus() != Gdiplus::Ok)
//		return -1;

//	hfontInit = static_cast<HFONT>(::GetCurrentObject( hmemdc, OBJ_FONT ));

	updateFont();
	updateScrollBars();

	return 0;
}

void RDOStudioFrameView::updateFont()
{
	if ( hfontCurrent )
	{
//		::SelectObject( hmemdc, hfontInit );
		::DeleteObject( hfontCurrent );
	}

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	RDOStudioFrameStyle* style = &studioApp.m_pMainFrame->style_frame;
//	lf.lfHeight    = -MulDiv( style->font->size, ::GetDeviceCaps( hmemdc, LOGPIXELSY ), 72 );
	lf.lfWeight    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC;
	lf.lfUnderline = style->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
	strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)

	hfontCurrent = ::CreateFontIndirect( &lf );
//	::SelectObject( hmemdc, hfontCurrent );
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
		model->m_frameManager.disconnectFrameDoc( GetDocument() );
		model->m_frameManager.resetCurrentShowingFrame( index );
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

	if ( m_pos.X > frameBmpRect.GetRight() - newClientRect.right ) m_pos.X = frameBmpRect.GetRight() - newClientRect.right;
	if ( m_pos.X < 0 ) m_pos.X = 0;
	si.nMin  = 0;
	si.nMax  = frameBmpRect.GetRight() - 1;
	si.nPos  = m_pos.X;
	si.nPage = newClientRect.right;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	if ( m_pos.Y > frameBmpRect.GetBottom() - newClientRect.bottom ) m_pos.Y = frameBmpRect.GetBottom() - newClientRect.bottom;
	if ( m_pos.Y < 0 ) m_pos.Y = 0;
	si.nMin  = 0;
	si.nMax  = frameBmpRect.GetBottom() - 1;
	si.nPos  = m_pos.Y;
	si.nPage = newClientRect.bottom;
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
			m_pos.X = 0;
			break;

		case SB_RIGHT:
			m_pos.X = frameBmpRect.GetRight() - newClientRect.right;
			break;

		case SB_PAGELEFT:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			m_pos.X -= si.nPage;
			break; 

		case SB_PAGERIGHT:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			m_pos.X += si.nPage;
			break;

		case SB_LINELEFT:
			m_pos.X--;
			break;

		case SB_LINERIGHT:
			m_pos.X++;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			m_pos.X += si.nTrackPos - m_pos.X;
			break;
		}
	}
	if ( m_pos.X > frameBmpRect.GetRight() - newClientRect.right ) m_pos.X = frameBmpRect.GetRight() - newClientRect.right;
	if ( m_pos.X < 0 ) m_pos.X = 0;
	si.fMask = SIF_POS;
	si.nPos  = m_pos.X;
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
			m_pos.Y = 0;
			break;

		case SB_BOTTOM:
			m_pos.Y = frameBmpRect.GetBottom() - newClientRect.bottom;
			break;

		case SB_PAGEUP:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			m_pos.Y -= si.nPage;
			break; 

		case SB_PAGEDOWN:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			m_pos.Y += si.nPage;
			break;

		case SB_LINEUP:
			m_pos.Y--;
			break;

		case SB_LINEDOWN:
			m_pos.Y++;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			m_pos.Y += si.nTrackPos - m_pos.Y;
			break;
		}
	}
	if ( m_pos.Y > frameBmpRect.GetBottom() - newClientRect.bottom ) m_pos.Y = frameBmpRect.GetBottom() - newClientRect.bottom;
	if ( m_pos.Y < 0 ) m_pos.Y = 0;
	si.fMask = SIF_POS;
	si.nPos  = m_pos.Y;
	SetScrollInfo( SB_VERT, &si, TRUE );
	InvalidateRect( NULL );
	UpdateWindow();
}

void RDOStudioFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	RDOStudioFrameManager* frameManager = &model->m_frameManager;
	int index = frameManager->findFrameIndex( this );

	point.Offset( m_pos.X, m_pos.Y );
	std::vector< RDOStudioFrameManager::Area* >* areas_sim = &frameManager->m_frameList[index]->areas_sim;
	std::vector< RDOStudioFrameManager::Area* >::iterator it = areas_sim->begin();
	while ( it != areas_sim->end() ) {
		RDOStudioFrameManager::Area* area = *it;
		if ( CRect( area->x, area->y, area->x + area->w, area->y + area->h ).PtInRect( point ) ) {
			model->sendMessage( kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &area->name );
		}
		it++;
	};

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
		m_pos.X -= newClientRect.right / 4 * (zDelta >= 0 ? 1 : -1);
		if ( m_pos.X > frameBmpRect.GetRight() - newClientRect.right ) m_pos.X = frameBmpRect.GetRight() - newClientRect.right;
		if ( m_pos.X < 0 ) m_pos.X = 0;
		si.nPos = m_pos.X;
		SetScrollInfo( SB_HORZ, &si, TRUE );
	} else {
		m_pos.Y -= newClientRect.bottom / 4 * (zDelta >= 0 ? 1 : -1);
		if ( m_pos.Y > frameBmpRect.GetBottom() - newClientRect.bottom ) m_pos.Y = frameBmpRect.GetBottom() - newClientRect.bottom;
		if ( m_pos.Y < 0 ) m_pos.Y = 0;
		si.nPos = m_pos.Y;
		SetScrollInfo( SB_VERT, &si, TRUE );
	}
	InvalidateRect( NULL );
	UpdateWindow();
	return TRUE;
}

void RDOStudioFrameView::OnPaint()
{
	PAINTSTRUCT ps;
	::BeginPaint( m_hwnd, &ps );
	onDraw();
	::EndPaint( m_hwnd, &ps );
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

	if (!mustBeInit)
	{
		m_pGraphics.reset(new Gdiplus::Graphics(m_hwnd));
		if (!m_pGraphics.get() || m_pGraphics->GetLastStatus() != Gdiplus::Ok)
			return;

		m_pGraphics->DrawImage(&m_memDC.buffer(), -m_pos.X, -m_pos.Y);
/*
		pDC->SetStretchBltMode( HALFTONE );
		double k1 = (double)newClientRect.bottom / frameBmpRect.GetBottom();
		double k2 = (double)newClientRect.right / frameBmpRect.GetRight();
		double k = min( k1, k2 );
		if ( k > 1 ) k = 1;
		CRect rect( 0, 0, frameBmpRect.GetRight() * k, frameBmpRect.GetBottom() * k );
		rect.OffsetRect( (newClientRect.right - rect.right) / 2, (newClientRect.bottom - rect.bottom) / 2 );
		pDC->StretchBlt( rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, &dc, 0, 0, frameBmpRect.GetRight(), frameBmpRect.GetBottom(), SRCCOPY );
		if ( rect.left ) {
			pDC->FillSolidRect( 0, 0, rect.left, newClientRect.bottom, bgColor );
			pDC->FillSolidRect( rect.right, 0, newClientRect.right, newClientRect.bottom, bgColor );
		}
		if ( rect.top ) {
			pDC->FillSolidRect( 0, 0, newClientRect.right, rect.top, bgColor );
			pDC->FillSolidRect( 0, rect.bottom, newClientRect.right, newClientRect.bottom, bgColor );
		}
*/
		if (newClientRect.right - frameBmpRect.GetRight() > 0)
		{
			Gdiplus::SolidBrush brush(m_bgColor);
			m_pGraphics->FillRectangle(&brush, frameBmpRect.GetRight(), 0, newClientRect.right - frameBmpRect.GetRight(), newClientRect.bottom);
		}
		if (newClientRect.bottom - frameBmpRect.GetBottom() > 0)
		{
			Gdiplus::SolidBrush brush(m_bgColor);
			m_pGraphics->FillRectangle(&brush, 0, frameBmpRect.GetBottom(), newClientRect.right, newClientRect.bottom - frameBmpRect.GetBottom());
		}
	}
	else
	{
		Gdiplus::SolidBrush brush(m_bgColor);
		if (m_pGraphics.get())
			m_pGraphics->FillRectangle(&brush, newClientRect.left, newClientRect.top, newClientRect.Width(), newClientRect.Height());
	}
}

void RDOStudioFrameView::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
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
