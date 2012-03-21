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
#include <boost/bind.hpp>
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

void RDOStudioFrameView::init(CPTRC(rdoAnimation::RDOFrame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

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

	init(size);
}

void RDOStudioFrameView::init(CREF(Gdiplus::Size) size)
{
	m_memDC.resize(size.Width, size.Height);
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
	  REF(rdo::gui::BitmapList)   bitmapGeneratedList,
	  REF(AreaList)               areaList
)
{
	ASSERT(pFrame);

	if (!valid())
	{
		init(pFrame, bitmapList);
	}

	drawBackground(pFrame, bitmapList);

	STL_FOR_ALL_CONST(pFrame->m_elements, it)
	{
		PTR(rdoAnimation::FrameItem) pCurrElement = *it;
		ASSERT(pCurrElement);
		switch (pCurrElement->getType())
		{
		case rdoAnimation::FrameItem::FIT_TEXT   : elementText     (static_cast<PTR(rdoAnimation::RDOTextElement   )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_RECT   : elementRect     (static_cast<PTR(rdoAnimation::RDORectElement   )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_R_RECT : elementRoundRect(static_cast<PTR(rdoAnimation::RDORRectElement  )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_LINE   : elementLine     (static_cast<PTR(rdoAnimation::RDOLineElement   )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_TRIANG : elementTriang   (static_cast<PTR(rdoAnimation::RDOTriangElement )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_CIRCLE : elementCircle   (static_cast<PTR(rdoAnimation::RDOCircleElement )>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_ELLIPSE: elementEllipse  (static_cast<PTR(rdoAnimation::RDOEllipseElement)>(pCurrElement)); break;
		case rdoAnimation::FrameItem::FIT_BMP    : elementBMP      (static_cast<PTR(rdoAnimation::RDOBmpElement    )>(pCurrElement), bitmapList, bitmapGeneratedList); break;
		case rdoAnimation::FrameItem::FIT_S_BMP  : elementSBMP     (static_cast<PTR(rdoAnimation::RDOSBmpElement   )>(pCurrElement), bitmapList, bitmapGeneratedList); break;
		case rdoAnimation::FrameItem::FIT_ACTIVE : elementActive   (static_cast<PTR(rdoAnimation::RDOActiveElement )>(pCurrElement), areaList); break;
		}
	}

	InvalidateRect   (NULL);
	SendNotifyMessage(WM_PAINT, 0, 0);
}

void RDOStudioFrameView::drawBackground(CPTRC(rdoAnimation::RDOFrame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

	rbool bgImage = false;
	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
		if (bmpIt != bitmapList.end())
		{
			m_memDC.dc().DrawImage(bmpIt->second, 0, 0, bmpIt->second->GetWidth(), bmpIt->second->GetHeight());
			bgImage = true;
		}
	}
	
	if (!bgImage)
	{
		Gdiplus::SolidBrush brush(m_bgColor);
		m_memDC.dc().FillRectangle(&brush, 0, 0, m_memDC.width(), m_memDC.height());

		Gdiplus::Color penColor;
		penColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->defaultColor);
		Gdiplus::Pen pen(penColor);

		const ruint pountListCount = 4;
		Gdiplus::Point pointList[pountListCount];
		pointList[0].X = 0;
		pointList[0].Y = 0;
		pointList[1].X = m_memDC.width () - 1;
		pointList[1].Y = 0;
		pointList[2].X = m_memDC.width () - 1;
		pointList[2].Y = m_memDC.height() - 1;
		pointList[3].X = 0;
		pointList[3].Y = m_memDC.height() - 1;

		m_memDC.dc().DrawPolygon(&pen, &pointList[0], pountListCount);
	}
}

void RDOStudioFrameView::elementText(PTR(rdoAnimation::RDOTextElement) pElement)
{
	ASSERT(pElement);

	Gdiplus::Color color(0, 0, 0);
	if (!pElement->m_background.m_transparent)
	{
//		::SetBkMode(hdc, OPAQUE);
//		::SetBkColor(hdc, RGB(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
//		color = Gdiplus::Color(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b);
	}
	else
	{
//		::SetBkMode(hdc, TRANSPARENT);
	}

	if(!pElement->m_foreground.m_transparent)
	{
//		::SetTextColor(hdc, RGB(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
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
	//	::DrawText(hdc, pElement->m_text.c_str(), pElement->m_text.length(), CRect((int)pElement->m_point.m_x, (int)pElement->m_point.m_y, (int)(pElement->m_point.m_x + pElement->m_size.m_width), (int)(pElement->m_point.m_y + pElement->m_size.m_height)), nFormat);
	//}
}

template <class F, class D>
void RDOStudioFrameView::drawColoredElement(CREF(rdoAnimation::RDOColoredElement) coloredElement, F fillBinder, D drawBinder)
{
	if (!coloredElement.m_background.m_transparent)
	{
		Gdiplus::SolidBrush brush(
			Gdiplus::Color(
				coloredElement.m_background.m_r,
				coloredElement.m_background.m_g,
				coloredElement.m_background.m_b
			)
		);
		fillBinder(&m_memDC.dc(), &brush);
	}

	if (!coloredElement.m_foreground.m_transparent)
	{
		Gdiplus::Pen pen(
			Gdiplus::Color(
				coloredElement.m_foreground.m_r,
				coloredElement.m_foreground.m_g,
				coloredElement.m_foreground.m_b
			)
		);
		drawBinder(&m_memDC.dc(), &pen);
	}
}

void RDOStudioFrameView::elementRect(PTR(rdoAnimation::RDORectElement) pElement)
{
	ASSERT(pElement);

	Gdiplus::Rect rect(
		(int)pElement->m_point.m_x,
		(int)pElement->m_point.m_y,
		(int)pElement->m_size.m_width,
		(int)pElement->m_size.m_height
	);

	Gdiplus::Status (Gdiplus::Graphics::*pBrush)(CPTR(Gdiplus::Brush), CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::FillRectangle;
	Gdiplus::Status (Gdiplus::Graphics::*pPen  )(CPTR(Gdiplus::Pen),   CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::DrawRectangle;

	drawColoredElement(*pElement,
		boost::bind(pBrush, _1, _2, rect),
		boost::bind(pPen,   _1, _2, rect)
	);
}

void RDOStudioFrameView::elementRoundRect(PTR(rdoAnimation::RDORRectElement) pElement)
{
	ASSERT(pElement);

	int radius = (int)(std::min<double>(pElement->m_size.m_width, pElement->m_size.m_height) / 3.0);

	Gdiplus::Rect rect(
		(int)pElement->m_point.m_x,
		(int)pElement->m_point.m_y,
		(int)pElement->m_size.m_width,
		(int)pElement->m_size.m_height
	);

	Gdiplus::GraphicsPath gpath;
	gpath.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
	gpath.AddArc (rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
	gpath.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
	gpath.AddArc (rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
	gpath.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
	gpath.AddArc (rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
	gpath.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
	gpath.AddArc (rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
	gpath.CloseFigure();

	Gdiplus::Status (Gdiplus::Graphics::*pBrush)(CPTR(Gdiplus::Brush), CPTR(Gdiplus::GraphicsPath)) = &Gdiplus::Graphics::FillPath;
	Gdiplus::Status (Gdiplus::Graphics::*pPen  )(CPTR(Gdiplus::Pen),   CPTR(Gdiplus::GraphicsPath)) = &Gdiplus::Graphics::DrawPath;

	drawColoredElement(*pElement,
		boost::bind(pBrush, _1, _2, &gpath),
		boost::bind(pPen,   _1, _2, &gpath)
	);
}

void RDOStudioFrameView::elementLine(PTR(rdoAnimation::RDOLineElement) pElement)
{
	ASSERT(pElement);

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
}

void RDOStudioFrameView::elementTriang(PTR(rdoAnimation::RDOTriangElement) pElement)
{
	ASSERT(pElement);

	const ruint pountListCount = 3;
	Gdiplus::Point pointList[pountListCount];
	pointList[0].X = (int)(pElement->m_point1.m_x);
	pointList[0].Y = (int)(pElement->m_point1.m_y);
	pointList[1].X = (int)(pElement->m_point2.m_x);
	pointList[1].Y = (int)(pElement->m_point2.m_y);
	pointList[2].X = (int)(pElement->m_point3.m_x);
	pointList[2].Y = (int)(pElement->m_point3.m_y);

	Gdiplus::Status (Gdiplus::Graphics::*pBrush)(CPTR(Gdiplus::Brush), CPTR(Gdiplus::Point), int) = &Gdiplus::Graphics::FillPolygon;
	Gdiplus::Status (Gdiplus::Graphics::*pPen  )(CPTR(Gdiplus::Pen),   CPTR(Gdiplus::Point), int) = &Gdiplus::Graphics::DrawPolygon;

	drawColoredElement(*pElement,
		boost::bind(pBrush, _1, _2, &pointList[0], pountListCount),
		boost::bind(pPen,   _1, _2, &pointList[0], pountListCount)
	);
}

void RDOStudioFrameView::elementCircle(PTR(rdoAnimation::RDOCircleElement) pElement)
{
	ASSERT(pElement);

	Gdiplus::Rect rect(
		(int)(pElement->m_center.m_x - pElement->m_radius.m_radius),
		(int)(pElement->m_center.m_y - pElement->m_radius.m_radius),
		(int)(pElement->m_center.m_x + pElement->m_radius.m_radius),
		(int)(pElement->m_center.m_y + pElement->m_radius.m_radius)
	);

	Gdiplus::Status (Gdiplus::Graphics::*pBrush)(CPTR(Gdiplus::Brush), CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::FillEllipse;
	Gdiplus::Status (Gdiplus::Graphics::*pPen  )(CPTR(Gdiplus::Pen),   CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::DrawEllipse;

	drawColoredElement(*pElement,
		boost::bind(pBrush, _1, _2, rect),
		boost::bind(pPen,   _1, _2, rect)
	);
}

void RDOStudioFrameView::elementEllipse(PTR(rdoAnimation::RDOEllipseElement) pElement)
{
	ASSERT(pElement);

	Gdiplus::Rect rect(
		(int)(pElement->m_point.m_x),
		(int)(pElement->m_point.m_y),
		(int)(pElement->m_point.m_x + pElement->m_size.m_width),
		(int)(pElement->m_point.m_y + pElement->m_size.m_height)
	);

	Gdiplus::Status (Gdiplus::Graphics::*pBrush)(CPTR(Gdiplus::Brush), CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::FillEllipse;
	Gdiplus::Status (Gdiplus::Graphics::*pPen  )(CPTR(Gdiplus::Pen),   CREF(Gdiplus::Rect)) = &Gdiplus::Graphics::DrawEllipse;

	drawColoredElement(*pElement,
		boost::bind(pBrush, _1, _2, rect),
		boost::bind(pPen,   _1, _2, rect)
	);
}

void RDOStudioFrameView::elementBMP(
	 PTR(rdoAnimation::RDOBmpElement) pElement,
	CREF(rdo::gui::BitmapList)        bitmapList,
	 REF(rdo::gui::BitmapList)        bitmapGeneratedList)
{
	ASSERT(pElement);

	PTR(Gdiplus::Bitmap) pBitmap = RDOStudioFrameView::getBitmap(
		pElement->m_bmp_name,
		pElement->hasMask() ? pElement->m_mask_name : tstring(),
		bitmapList,
		bitmapGeneratedList
	);

	if (pBitmap)
	{
		m_memDC.dc().DrawImage(pBitmap, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y));
	}
}

void RDOStudioFrameView::elementSBMP(
	 PTR(rdoAnimation::RDOSBmpElement) pElement,
	CREF(rdo::gui::BitmapList)         bitmapList,
	 REF(rdo::gui::BitmapList)         bitmapGeneratedList)
{
	ASSERT(pElement);

	PTR(Gdiplus::Bitmap) pBitmap = RDOStudioFrameView::getBitmap(
		pElement->m_bmp_name,
		pElement->hasMask() ? pElement->m_mask_name : tstring(),
		bitmapList,
		bitmapGeneratedList
	);

	if (pBitmap)
	{
		m_memDC.dc().DrawImage(pBitmap, (int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height));
	}
}

PTR(Gdiplus::Bitmap) RDOStudioFrameView::getBitmap(
	CREF(tstring)              bitmapName,
	CREF(tstring)              maskName,
	CREF(rdo::gui::BitmapList) bitmapList,
	 REF(rdo::gui::BitmapList) bitmapGeneratedList)
{
	rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(bitmapName);
	if (bmpIt == bitmapList.end())
		return NULL;

	if (!maskName.empty())
	{
		tstring maskedBitmapName(rdo::format(_T("%s%s"), bitmapName.c_str(), maskName.c_str()));

		rdo::gui::BitmapList::const_iterator generatedIt = bitmapList.find(maskedBitmapName);
		if (generatedIt != bitmapList.end())
		{
			return generatedIt->second;
		}

		generatedIt = bitmapGeneratedList.find(maskedBitmapName);
		if (generatedIt != bitmapGeneratedList.end())
		{
			return generatedIt->second;
		}

		rdo::gui::BitmapList::const_iterator maskIt = bitmapList.find(maskName);
		if (maskIt != bitmapList.end())
		{
			PTR(Gdiplus::Bitmap) pGenerated = rdo::gui::Bitmap::transparent(*bmpIt->second, *maskIt->second);
			if (pGenerated)
			{
				std::pair<rdo::gui::BitmapList::const_iterator, rbool> result =
					bitmapGeneratedList.insert(rdo::gui::BitmapList::value_type(maskedBitmapName, pGenerated));
				ASSERT(result.second);
				return pGenerated;
			}
		}
	}

	return bmpIt->second;
}

void RDOStudioFrameView::elementActive(PTR(rdoAnimation::RDOActiveElement) pElement, REF(AreaList) areaList)
{
	ASSERT(pElement);

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
}
