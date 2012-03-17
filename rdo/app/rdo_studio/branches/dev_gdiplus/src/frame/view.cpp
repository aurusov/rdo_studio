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

void RDOStudioFrameView::update(
	CPTRC(rdoAnimation::RDOFrame) pFrame,
	 CREF(rdo::gui::BitmapList)   bitmapList,
	  REF(rdo::gui::BitmapList)   bitmapMaskInvertList,
	  REF(AreaList)               areaList
)
{
	if (!valid())
	{
		Gdiplus::Size size;
		rbool imageFound = false;
		if (pFrame->hasBgImage())
		{
			rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
			if (bmpIt != bitmapList.end())
			{
				size.Width  = bmpIt->second->GetWidth ();
				size.Height = bmpIt->second->GetHeight();
				imageFound  = true;
			}
		}
		if (!imageFound)
		{
			size.Width  = (ruint)pFrame->m_size.m_width;
			size.Height = (ruint)pFrame->m_size.m_height;
		}
		init(size);
	}

	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
		if (bmpIt != bitmapList.end())
		{
			m_memDC.dc().DrawImage(bmpIt->second, 0, 0, bmpIt->second->GetWidth(), bmpIt->second->GetHeight());
		}
	}
	Gdiplus::Color bgColor;
	if (pFrame->m_bgColor.m_transparent)
	{
		bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
	}
	else
	{
		bgColor.SetFromCOLORREF(RGB(pFrame->m_bgColor.m_r, pFrame->m_bgColor.m_g, pFrame->m_bgColor.m_b));
	}
	setBGColor(bgColor);

	ruint size = pFrame->m_elements.size();
	for (ruint i = 0; i < size; i++)
	{
		PTR(rdoAnimation::FrameItem) pCurrElement = pFrame->m_elements[i];
		ASSERT(pCurrElement);
		switch (pCurrElement->getType())
		{
			case rdoAnimation::FrameItem::FIT_TEXT:
			{
				Gdiplus::Color color(0, 0, 0);
				PTR(rdoAnimation::RDOTextElement) pElement = static_cast<PTR(rdoAnimation::RDOTextElement)>(pCurrElement);
				if (!pElement->m_background.m_transparent)
				{
//							::SetBkMode(hdc, OPAQUE);
//							::SetBkColor(hdc, RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
//							color = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
				}
				else
				{
//							::SetBkMode(hdc, TRANSPARENT);
				}

				if(!pElement->m_foreground.m_transparent)
				{
//							::SetTextColor(hdc, RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
				}

				Gdiplus::StringFormat sformat;
				Gdiplus::SolidBrush   brush(color);

				UINT nFormat = DT_SINGLELINE | DT_VCENTER;
				switch (pElement->m_align)
				{
				case rdoAnimation::RDOTextElement::TETA_LEFT  : nFormat |= DT_LEFT;   sformat.SetAlignment(Gdiplus::StringAlignmentNear  ); break;
				case rdoAnimation::RDOTextElement::TETA_RIGHT : nFormat |= DT_RIGHT;  sformat.SetAlignment(Gdiplus::StringAlignmentFar   ); break;
				case rdoAnimation::RDOTextElement::TETA_CENTER: nFormat |= DT_CENTER; sformat.SetAlignment(Gdiplus::StringAlignmentCenter); break;
				}

				std::wstring wtext = rdo::toUnicode(pElement->m_text);

				//HDC hDC = m_memDC.dc().GetHDC();
				//Gdiplus::Font  font(hDC, hfontCurrent);
				//m_memDC.dc().ReleaseHDC(hDC);

				//if (font.GetLastStatus() == Gdiplus::Ok)
				//{
				//	Gdiplus::RectF rect(
				//		Gdiplus::REAL(pElement->m_point.m_x),
				//		Gdiplus::REAL(pElement->m_point.m_y),
				//		Gdiplus::REAL(pElement->m_size.m_width),
				//		Gdiplus::REAL(pElement->m_size.m_height)
				//	);

				//	wtext = L"text";

				//	status = m_memDC.dc().DrawString(wtext.c_str(), wtext.length(), &font, rect, &sformat, &brush);
//							::DrawText(hdc, pElement->m_text.c_str(), pElement->m_text.length(), CRect((int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)(pElement->m_point.m_x + pElement->m_size.m_width), (int)(pElement->m_point.m_y + pElement->m_size.m_height)), nFormat);
				//}

				break;
			}

			case rdoAnimation::FrameItem::FIT_RECT:
			{
				PTR(rdoAnimation::RDORectElement) pElement = static_cast<PTR(rdoAnimation::RDORectElement)>(pCurrElement);
				Gdiplus::Color bgColor;
				if (!pElement->m_background.m_transparent)
				{
					bgColor = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
				}
				else
				{
					bgColor.SetValue(0);
				}
				Gdiplus::SolidBrush brush(bgColor);

				m_memDC.dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
				//! @todo добавить вывод линии вокруг прямоугольника

				break;
			}

			case rdoAnimation::FrameItem::FIT_R_RECT:
			{
				PTR(rdoAnimation::RDORRectElement) pElement = static_cast<PTR(rdoAnimation::RDORRectElement)>(pCurrElement);
				Gdiplus::Color bgColor;
				if (!pElement->m_background.m_transparent)
				{
					bgColor = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
				}
				else
				{
					bgColor.SetValue(0);
				}
				Gdiplus::SolidBrush brush(bgColor);

				//! @todo gdi+ не умеет выводить roundrect :(
				m_memDC.dc().FillRectangle(&brush, (int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)pElement->m_size.m_width, (int)pElement->m_size.m_height);
				//! @todo добавить вывод линии вокруг прямоугольника

				break;
			}

			case rdoAnimation::FrameItem::FIT_LINE:
			{
				PTR(rdoAnimation::RDOLineElement) pElement = static_cast<PTR(rdoAnimation::RDOLineElement)>(pCurrElement);
				if (!pElement->m_color.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_color.m_r, pElement->m_color.m_g, pElement->m_color.m_b));
					Gdiplus::Pen pen(color);
					m_memDC.dc().DrawLine(
						&pen,
						(int)(pElement->m_point1.m_x), (int)(pElement->m_point1.m_y),
						(int)(pElement->m_point2.m_x), (int)(pElement->m_point2.m_y)
					);
				}

				break;
			}

			case rdoAnimation::FrameItem::FIT_TRIANG:
			{
				PTR(rdoAnimation::RDOTriangElement) pElement = static_cast<PTR(rdoAnimation::RDOTriangElement)>(pCurrElement);

				const ruint pountListCount = 3;
				Gdiplus::Point pointList[pountListCount];
				pointList[0].X = (int)(pElement->m_point1.m_x);
				pointList[0].Y = (int)(pElement->m_point1.m_y);
				pointList[1].X = (int)(pElement->m_point2.m_x);
				pointList[1].Y = (int)(pElement->m_point2.m_y);
				pointList[2].X = (int)(pElement->m_point3.m_x);
				pointList[2].Y = (int)(pElement->m_point3.m_y);

				if (!pElement->m_background.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
					Gdiplus::SolidBrush brush(color);
					m_memDC.dc().FillPolygon(&brush, &pointList[0], pountListCount);
				}

				if (!pElement->m_foreground.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
					Gdiplus::Pen pen(color);
					m_memDC.dc().DrawPolygon(&pen, &pointList[0], pountListCount);
				}

				break;
			}

			case rdoAnimation::FrameItem::FIT_CIRCLE:
			{
				PTR(rdoAnimation::RDOCircleElement) pElement = static_cast<PTR(rdoAnimation::RDOCircleElement)>(pCurrElement);

				Gdiplus::Rect rect(
					(int)(pElement->m_center.m_x - pElement->m_radius.m_radius),
					(int)(pElement->m_center.m_y - pElement->m_radius.m_radius),
					(int)(pElement->m_center.m_x + pElement->m_radius.m_radius),
					(int)(pElement->m_center.m_y + pElement->m_radius.m_radius)
				);

				if (!pElement->m_background.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
					Gdiplus::SolidBrush brush(color);
					m_memDC.dc().FillEllipse(&brush, rect);
				}

				if (!pElement->m_foreground.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
					Gdiplus::Pen pen(color);
					m_memDC.dc().DrawEllipse(&pen, rect);
				}

				break;
			}

			case rdoAnimation::FrameItem::FIT_ELLIPSE:
			{
				PTR(rdoAnimation::RDOEllipseElement) pElement = static_cast<PTR(rdoAnimation::RDOEllipseElement)>(pCurrElement);

				Gdiplus::Rect rect(
					(int)(pElement->m_point.m_x),
					(int)(pElement->m_point.m_y),
					(int)(pElement->m_point.m_x + pElement->m_size.m_width),
					(int)(pElement->m_point.m_y + pElement->m_size.m_height)
				);

				if (!pElement->m_background.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
					Gdiplus::SolidBrush brush(color);
					m_memDC.dc().FillEllipse(&brush, rect);
				}

				if (!pElement->m_foreground.m_transparent)
				{
					Gdiplus::Color color;
					color.SetFromCOLORREF(RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
					Gdiplus::Pen pen(color);
					m_memDC.dc().DrawEllipse(&pen, rect);
				}

				break;
			}

			case rdoAnimation::FrameItem::FIT_BMP:
			{
				PTR(rdoAnimation::RDOBmpElement) pElement = static_cast<PTR(rdoAnimation::RDOBmpElement)>(pCurrElement);
				rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pElement->m_bmp_name);
				if (bmpIt != bitmapList.end())
				{
					rbool maskDraw = false;
					if (pElement->hasMask())
					{
						rdo::gui::BitmapList::const_iterator maskIt = bitmapList.find(pElement->m_mask_name);
						if (maskIt != bitmapList.end())
						{
							rdo::gui::BitmapList::const_iterator maskInvertIt = bitmapMaskInvertList.find(pElement->m_mask_name);
							if (maskInvertIt == bitmapMaskInvertList.end())
							{
								PTR(Gdiplus::Bitmap) pMaskInvert = NULL;
								{
									Gdiplus::Graphics graphics(maskIt->second);
									pMaskInvert = new Gdiplus::Bitmap(
										maskIt->second->GetWidth (),
										maskIt->second->GetHeight(),
										&graphics
									);
								}
								if (pMaskInvert)
								{
									if (pMaskInvert->GetLastStatus() == Gdiplus::Ok)
									{
										std::pair<rdo::gui::BitmapList::const_iterator, rbool> result = bitmapMaskInvertList.insert(rdo::gui::BitmapList::value_type(pElement->m_mask_name, pMaskInvert));
										if (result.second)
										{
											maskInvertIt = result.first;

											Gdiplus::ColorMatrix colorMatrix = {
												-1,  0,  0, 0, 0,
												 0, -1,  0, 0, 0,
												 0,  0, -1, 0, 0,
												 0,  0,  0, 1, 0,
												 1,  1,  1, 0, 1
											};
											Gdiplus::ImageAttributes imageAttributes;
											imageAttributes.SetColorMatrix(&colorMatrix);

											Gdiplus::Graphics graphics(maskInvertIt->second);
											graphics.DrawImage(
												maskIt->second,
												Gdiplus::RectF(
													0.0,
													0.0,
													Gdiplus::REAL(maskIt->second->GetWidth()),
													Gdiplus::REAL(maskIt->second->GetHeight())
												),
												0.0,
												0.0,
												Gdiplus::REAL(maskIt->second->GetWidth()),
												Gdiplus::REAL(maskIt->second->GetHeight()),
												Gdiplus::UnitPixel,
												&imageAttributes
											);
										}
									}
									else
									{
										delete pMaskInvert;
									}
								}
							}

							if (maskInvertIt != bitmapMaskInvertList.end())
							{
								//Gdiplus::Graphics graphics(maskInvertIt->second);
								m_memDC.dc().DrawImage(maskInvertIt->second, 0, 0);
								break;
							}

							Gdiplus::ImageAttributes imageAttributes;
							imageAttributes.SetColorKey(
								Gdiplus::ARGB(Gdiplus::Color::Black),
								Gdiplus::ARGB(Gdiplus::Color::Black)
							);
							m_memDC.dc().DrawImage(
								maskIt->second,
								Gdiplus::RectF(
									Gdiplus::REAL(pElement->m_point.m_x),
									Gdiplus::REAL(pElement->m_point.m_y),
									Gdiplus::REAL(maskIt->second->GetWidth()),
									Gdiplus::REAL(maskIt->second->GetHeight())
								),
								0.0,
								0.0,
								Gdiplus::REAL(maskIt->second->GetWidth()),
								Gdiplus::REAL(maskIt->second->GetHeight()),
								Gdiplus::UnitPixel,
								&imageAttributes
							);
							maskDraw = true;

							//MemDC bg;
							//if (bg.create(maskIt->second->GetWidth(), maskIt->second->GetHeight(), dc()))
							//{
							//	Gdiplus::RectF bgRect    (0.0, 0.0, Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
							//	Gdiplus::RectF bufferRect(Gdiplus::REAL(pElement->m_point.m_x), Gdiplus::REAL(pElement->m_point.m_y), Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
							//	Gdiplus::ImageAttributes imageAttributes;
							//	bg.m_memDC.dc().DrawImage(&buffer(), 0, 0, Gdiplus::REAL(pElement->m_point.m_x), Gdiplus::REAL(pElement->m_point.m_y), Gdiplus::REAL(bg.width()), Gdiplus::REAL(bg.height()));
							//}
							//CBitmap* pOldMask = dcMask.SelectObject(&mask->bmp);
							//::BitBlt(hdc, (int)(element->m_point.m_x), (int)(element->m_point.m_y), mask->w, mask->h, dcMask.m_hDC, 0, 0, SRCAND);
							//::BitBlt(hdc, (int)(element->m_point.m_x), (int)(element->m_point.m_y), bmp->w, bmp->h, dcBmp.m_hDC, 0, 0, SRCPAINT);
							//dcMask.SelectObject(pOldMask);

//									NEVER_REACH_HERE;
//									maskDraw = true;
						}
					}
					if (!maskDraw)
					{
						m_memDC.dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y));
					}
				}
				break;
			}

			case rdoAnimation::FrameItem::FIT_S_BMP:
			{
				PTR(rdoAnimation::RDOSBmpElement) pElement = static_cast<PTR(rdoAnimation::RDOSBmpElement)>(pCurrElement);
				rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pElement->m_bmp_name);
				if (bmpIt != bitmapList.end())
				{
					rbool maskDraw = false;
					if (pElement->hasMask())
					{
						rdo::gui::BitmapList::const_iterator maskIt = bitmapList.find(pElement->m_mask_name);
						if (maskIt != bitmapList.end())
						{
//									NEVER_REACH_HERE;
//									maskDraw = true;
						}
					}
					if (!maskDraw)
					{
						m_memDC.dc().DrawImage(bmpIt->second, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height));
					}
				}
				break;
			}

			case rdoAnimation::FrameItem::FIT_ACTIVE:
			{
				PTR(rdoAnimation::RDOActiveElement) pElement = static_cast<PTR(rdoAnimation::RDOActiveElement)>(pCurrElement);
				AreaList::iterator it = areaList.find(pElement->m_opr_name);
				if (it == areaList.end())
				{
					std::pair<AreaList::iterator, rbool> result =
						areaList.insert(AreaList::value_type(pElement->m_opr_name, Area()));
					ASSERT(result.second);
					it = result.first;
				}
				it->second.m_rect.X      = (int)(pElement->m_point.m_x);
				it->second.m_rect.Y      = (int)(pElement->m_point.m_y);
				it->second.m_rect.Width  = (int)(pElement->m_size.m_width);
				it->second.m_rect.Height = (int)(pElement->m_size.m_height);
				break;
			}
		}
	}

	InvalidateRect   (NULL);
	SendNotifyMessage(WM_PAINT, 0, 0);
}
