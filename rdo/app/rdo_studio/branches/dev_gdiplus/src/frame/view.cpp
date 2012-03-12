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
	ON_WM_CREATE       ()
	ON_WM_DESTROY      ()
	ON_WM_SIZE         ()
	ON_WM_HSCROLL      ()
	ON_WM_VSCROLL      ()
	ON_WM_LBUTTONDOWN  ()
	ON_WM_KEYDOWN      ()
	ON_WM_KEYUP        ()
	ON_WM_MOUSEWHEEL   ()
	ON_WM_PAINT        ()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCMOUSEMOVE  ()
	ON_WM_MOUSEMOVE    ()
	ON_COMMAND         (ID_HELP_KEYWORD, OnHelpKeyword)
	ON_COMMAND         (ID_FILE_PRINT, RDOStudioView::OnFilePrint)
	ON_COMMAND         (ID_FILE_PRINT_DIRECT, RDOStudioView::OnFilePrint)
	ON_COMMAND         (ID_FILE_PRINT_PREVIEW, RDOStudioView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioFrameView::RDOStudioFrameView()
	: RDOStudioView   ()
	, m_newClientRect (0, 0, 0, 0)
	, m_hwnd          (NULL )
	, m_hfontInit     (NULL )
	, m_hfontCurrent  (NULL )
	, m_mouseOnHScroll(false)
{
	m_bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
}

RDOStudioFrameView::~RDOStudioFrameView()
{}

rbool RDOStudioFrameView::valid()
{
	return m_memDC.valid();
}

void RDOStudioFrameView::init(CREF(Gdiplus::Size) size)
{
	m_memDC.resize(size.Width, size.Height);

	m_points[0].X = 0;
	m_points[0].Y = 0;
	m_points[1].X = m_memDC.width () - 1;
	m_points[1].Y = 0;
	m_points[2].X = m_memDC.width () - 1;
	m_points[2].Y = m_memDC.height() - 1;
	m_points[3].X = 0;
	m_points[3].Y = m_memDC.height() - 1;
	m_points[4].X = 0;
	m_points[4].Y = 0;

	updateScrollBars();
}

REF(Gdiplus::Graphics) RDOStudioFrameView::dc()
{
	return m_memDC.dc();
}

void RDOStudioFrameView::setBGColor(CREF(Gdiplus::Color) color)
{
	m_bgColor = color;
}

BOOL RDOStudioFrameView::PreCreateWindow(REF(CREATESTRUCT) cs)
{
	if (!RDOStudioView::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW));

	return TRUE;
}

int RDOStudioFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (RDOStudioView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hwnd = GetSafeHwnd();

//	m_hfontInit = static_cast<HFONT>(::GetCurrentObject(hmemdc, OBJ_FONT));

	updateFont      ();
	updateScrollBars();

	return 0;
}

void RDOStudioFrameView::OnDestroy()
{
	ruint index = model->m_frameManager.findFrameIndex(this);
	if (index != ruint(~0))
	{
		model->m_frameManager.disconnectFrameDoc(GetDocument());
		model->m_frameManager.resetCurrentShowingFrame(index);
	}

	RDOStudioView::OnDestroy();
}

void RDOStudioFrameView::updateFont()
{
	if (m_hfontCurrent)
	{
//		::SelectObject(hmemdc, m_hfontInit);
		::DeleteObject(m_hfontCurrent);
	}

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	PTR(RDOStudioFrameStyle) pStyle = &studioApp.m_pMainFrame->style_frame;
//	lf.lfHeight    = -MulDiv(pStyle->font->size, ::GetDeviceCaps(hmemdc, LOGPIXELSY), 72);
	lf.lfWeight    = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC;
	lf.lfUnderline = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = BYTE(pStyle->font->characterSet);
#pragma warning(disable: 4996)
	strcpy(lf.lfFaceName, pStyle->font->name.c_str());
#pragma warning(default: 4996)

	m_hfontCurrent = ::CreateFontIndirect(&lf);
//	::SelectObject(hmemdc, m_hfontCurrent);
}

BOOL RDOStudioFrameView::OnPreparePrinting(PTR(CPrintInfo) pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioFrameView::OnBeginPrinting(PTR(CDC) pDC, PTR(CPrintInfo) pInfo)
{
	UNUSED(pDC  );
	UNUSED(pInfo);
}

void RDOStudioFrameView::OnEndPrinting(PTR(CDC) pDC, PTR(CPrintInfo) pInfo)
{
	UNUSED(pDC  );
	UNUSED(pInfo);
}

#ifdef _DEBUG
void RDOStudioFrameView::AssertValid() const
{
	RDOStudioView::AssertValid();
}

void RDOStudioFrameView::Dump(REF(CDumpContext) dc) const
{
	RDOStudioView::Dump(dc);
}
#endif

PTR(RDOStudioFrameDoc) RDOStudioFrameView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioFrameDoc)));
	return static_cast<PTR(RDOStudioFrameDoc)>(m_pDocument);
}

CREF(CRect) RDOStudioFrameView::getClientRect() const
{
	return m_newClientRect;
}

void RDOStudioFrameView::OnSize(UINT nType, int cx, int cy)
{
	RDOStudioView::OnSize(nType, cx, cy);

	GetClientRect(&m_newClientRect);

	updateScrollBars();
}

void RDOStudioFrameView::updateScrollBars()
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;

	if (m_pos.X > rsint(m_memDC.width()) - m_newClientRect.right)
	{
		m_pos.X = m_memDC.width() - m_newClientRect.right;
	}
	if (m_pos.X < 0)
	{
		m_pos.X = 0;
	}
	si.nMin  = 0;
	si.nMax  = m_memDC.width() - 1;
	si.nPos  = m_pos.X;
	si.nPage = m_newClientRect.right;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	if (m_pos.Y > rsint(m_memDC.height()) - m_newClientRect.bottom)
	{
		m_pos.Y = m_memDC.height() - m_newClientRect.bottom;
	}
	if (m_pos.Y < 0)
	{
		m_pos.Y = 0;
	}
	si.nMin  = 0;
	si.nMax  = m_memDC.height() - 1;
	si.nPos  = m_pos.Y;
	si.nPage = m_newClientRect.bottom;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

void RDOStudioFrameView::OnHScroll(UINT nSBCode, UINT nPos, PTR(CScrollBar) pScrollBar)
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	if (nSBCode == SB_ENDSCROLL)
	{
		model->setGUIContinue();
	}
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	switch (nSBCode)
	{
	case SB_LEFT:
		m_pos.X = 0;
		break;

	case SB_RIGHT:
		m_pos.X = m_memDC.width() - m_newClientRect.right;
		break;

	case SB_PAGELEFT:
		GetScrollInfo(SB_HORZ, &si, SIF_PAGE);
		m_pos.X -= si.nPage;
		break; 

	case SB_PAGERIGHT:
		GetScrollInfo(SB_HORZ, &si, SIF_PAGE);
		m_pos.X += si.nPage;
		break;

	case SB_LINELEFT:
		m_pos.X--;
		break;

	case SB_LINERIGHT:
		m_pos.X++;
		break;

	case SB_THUMBTRACK:
		GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
		m_pos.X += si.nTrackPos - m_pos.X;
		break;
	}
	if (m_pos.X > rsint(m_memDC.width()) - m_newClientRect.right)
	{
		m_pos.X = m_memDC.width() - m_newClientRect.right;
	}
	if (m_pos.X < 0)
	{
		m_pos.X = 0;
	}
	si.fMask = SIF_POS;
	si.nPos  = m_pos.X;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	InvalidateRect(NULL);
	UpdateWindow();
}

void RDOStudioFrameView::OnVScroll(UINT nSBCode, UINT nPos, PTR(CScrollBar) pScrollBar)
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	if (nSBCode == SB_ENDSCROLL)
	{
		model->setGUIContinue();
	}
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	switch(nSBCode)
	{
	case SB_TOP:
		m_pos.Y = 0;
		break;

	case SB_BOTTOM:
		m_pos.Y = m_memDC.height() - m_newClientRect.bottom;
		break;

	case SB_PAGEUP:
		GetScrollInfo(SB_VERT, &si, SIF_PAGE);
		m_pos.Y -= si.nPage;
		break; 

	case SB_PAGEDOWN:
		GetScrollInfo(SB_VERT, &si, SIF_PAGE);
		m_pos.Y += si.nPage;
		break;

	case SB_LINEUP:
		m_pos.Y--;
		break;

	case SB_LINEDOWN:
		m_pos.Y++;
		break;

	case SB_THUMBTRACK:
		GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
		m_pos.Y += si.nTrackPos - m_pos.Y;
		break;
	}
	if (m_pos.Y > rsint(m_memDC.height()) - m_newClientRect.bottom)
	{
		m_pos.Y = m_memDC.height() - m_newClientRect.bottom;
	}
	if (m_pos.Y < 0)
	{
		m_pos.Y = 0;
	}
	si.fMask = SIF_POS;
	si.nPos  = m_pos.Y;
	SetScrollInfo(SB_VERT, &si, TRUE);
	InvalidateRect(NULL);
	UpdateWindow();
}

void RDOStudioFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	PTR(RDOStudioFrameManager) pFrameManager = &model->m_frameManager;
	ruint index = pFrameManager->findFrameIndex(this);
	if (index != ruint(~0))
	{
		pFrameManager->areaDown(index, Gdiplus::Point(point.x + m_pos.X, point.y + m_pos.Y));
	}

	RDOStudioView::OnLButtonDown(nFlags, point);
}

void RDOStudioFrameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	model->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_KEY_DOWN, &nChar);
	RDOStudioView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	model->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_KEY_UP, &nChar);
	RDOStudioView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void RDOStudioFrameView::OnActivateView(BOOL bActivate, PTR(CView) pActivateView, PTR(CView) pDeactiveView)
{
	if (bActivate)
	{
		ruint index = model->m_frameManager.findFrameIndex(this);
		model->m_frameManager.setLastShowedFrame    (index);
		model->m_frameManager.setCurrentShowingFrame(index);
	}
	RDOStudioView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL RDOStudioFrameView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UNUSED(nFlags);
	UNUSED(pt    );

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_POS;
	if (m_mouseOnHScroll)
	{
		m_pos.X -= m_newClientRect.right / 4 * (zDelta >= 0 ? 1 : -1);
		if (m_pos.X > rsint(m_memDC.width()) - m_newClientRect.right)
		{
			m_pos.X = rsint(m_memDC.width()) - m_newClientRect.right;
		}
		if (m_pos.X < 0)
		{
			m_pos.X = 0;
		}
		si.nPos = m_pos.X;
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}
	else
	{
		m_pos.Y -= m_newClientRect.bottom / 4 * (zDelta >= 0 ? 1 : -1);
		if (m_pos.Y > rsint(m_memDC.height()) - m_newClientRect.bottom)
		{
			m_pos.Y = rsint(m_memDC.height()) - m_newClientRect.bottom;
		}
		if (m_pos.Y < 0)
		{
			m_pos.Y = 0;
		}
		si.nPos = m_pos.Y;
		SetScrollInfo(SB_VERT, &si, TRUE);
	}
	InvalidateRect(NULL);
	UpdateWindow  ();
	return TRUE;
}

void RDOStudioFrameView::OnPaint()
{
	Gdiplus::Graphics graphics(m_hwnd);

	PAINTSTRUCT ps;
	::BeginPaint(m_hwnd, &ps);
	onDraw      (graphics);
	::EndPaint  (m_hwnd, &ps);
}

void RDOStudioFrameView::OnDraw(PTR(CDC) pDC)
{
	RDOStudioView::OnDraw(pDC);
}

void RDOStudioFrameView::onDraw(REF(Gdiplus::Graphics) graphics)
{
//	PTR(RDOStudioFrameDoc) pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

//	GetClientRect(&m_newClientRect);

	if (valid())
	{
		graphics.DrawImage(&m_memDC.buffer(), -m_pos.X, -m_pos.Y);

		if (m_newClientRect.right - m_memDC.width() > 0)
		{
			Gdiplus::SolidBrush brush(m_bgColor);
			graphics.FillRectangle(&brush, m_memDC.width(), 0, m_newClientRect.right - m_memDC.width(), m_newClientRect.bottom);
		}
		if (m_newClientRect.bottom - m_memDC.height() > 0)
		{
			Gdiplus::SolidBrush brush(m_bgColor);
			graphics.FillRectangle(&brush, 0, m_memDC.height(), m_newClientRect.right, m_newClientRect.bottom - m_memDC.height());
		}
	}
	else
	{
		Gdiplus::SolidBrush brush(m_bgColor);
		graphics.FillRectangle(&brush, m_newClientRect.left, m_newClientRect.top, m_newClientRect.Width(), m_newClientRect.Height());
	}
}

void RDOStudioFrameView::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
	if (filename.empty())
		return;

	filename += "::/html/work_model_frame.htm";
	::HtmlHelp(::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL);
}

void RDOStudioFrameView::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	model->setGUIPause();
	RDOStudioView::OnNcLButtonDown(nHitTest, point);
}

void RDOStudioFrameView::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	model->setGUIPause();
	RDOStudioView::OnNcRButtonDown(nHitTest, point);
}

void RDOStudioFrameView::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	m_mouseOnHScroll = nHitTest == HTHSCROLL;
	RDOStudioView::OnNcMouseMove(nHitTest, point);
}

void RDOStudioFrameView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mouseOnHScroll = false;
	RDOStudioView::OnMouseMove(nFlags, point);
}
