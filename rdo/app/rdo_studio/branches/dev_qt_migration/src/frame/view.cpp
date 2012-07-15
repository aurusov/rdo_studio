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
#include <QtGui/qlayout.h>
#include <QtGui/qpainter.h>
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

FrameAnimationContent::FrameAnimationContent(PTR(QWidget) pParent)
	: parent_type(pParent)
	, m_size     (QSize (0, 0))
	, m_pos      (QPoint(0, 0))
{
//	setAutoFillBackground(false);
//	setAttribute(Qt::WA_OpaquePaintEvent,   true);
	setAttribute(Qt::WA_NoSystemBackground, true);

	m_bgColor = QColor(
		GetRValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor),
		GetGValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor),
		GetBValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor)
	);

	updateFont();
}

FrameAnimationContent::~FrameAnimationContent()
{}

rbool FrameAnimationContent::valid() const
{
	return m_memDC.valid();
}

void FrameAnimationContent::init(CPTRC(rdo::animation::Frame) pFrame, CREF(BitmapList) bitmapList)
{
	ASSERT(pFrame);

	QSize size;
	rbool imageFound = false;
	if (pFrame->hasBgImage())
	{
		BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
		if (bmpIt != bitmapList.end())
		{
			size.setWidth (bmpIt->second.width ());
			size.setHeight(bmpIt->second.height());
			imageFound  = true;
		}
	}
	if (!imageFound)
	{
		size.setWidth ((ruint)pFrame->m_size.m_width );
		size.setHeight((ruint)pFrame->m_size.m_height);
	}

	QColor bgColor;
	if (pFrame->m_bgColor.m_transparent)
	{
		bgColor = QColor(
			GetRValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor),
			GetGValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor),
			GetBValue(studioApp.m_pMainFrame->style_frame.theme->backgroundColor)
		);
	}
	else
	{
		bgColor = QColor(pFrame->m_bgColor.m_r, pFrame->m_bgColor.m_g, pFrame->m_bgColor.m_b);
	}
	setBGColor(bgColor);

	init(size);
}

void FrameAnimationContent::init(CREF(QSize) size)
{
	m_memDC.resize(size.width(), size.height());
	setMinimumSize(size);
}

void FrameAnimationContent::updateFont()
{
	PTR(RDOStudioFrameStyle) pStyle = &studioApp.m_pMainFrame->style_frame;
	ASSERT(pStyle);

	m_font = QFont(pStyle->font->name.c_str());
	m_font.setBold     (pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD     );
	m_font.setItalic   (pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC   );
	m_font.setUnderline(pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE);
	m_font.setPointSize(pStyle->font->size);
}

void FrameAnimationContent::setBGColor(CREF(QColor) color)
{
	m_bgColor = color;
}

void FrameAnimationContent::resizeEvent(QResizeEvent* pEvent)
{
	m_size = pEvent->size();
}

void FrameAnimationContent::paintEvent(QPaintEvent* pEvent)
{
	UNUSED(pEvent);

	QPainter painter(this);
	onDraw(painter);
}

void FrameAnimationContent::onDraw(REF(QPainter) painter)
{
	if (valid())
	{
		painter.drawPixmap(-m_pos.x(), -m_pos.y(), m_memDC.buffer());

		if (m_size.width() - m_memDC.width() > 0)
		{
			painter.fillRect(m_memDC.width(), 0, m_size.width() - m_memDC.width(), m_size.height(), m_bgColor);
		}
		if (m_size.height() - m_memDC.height() > 0)
		{
			painter.fillRect(0, m_memDC.height(), m_size.width(), m_size.height() - m_memDC.height(), m_bgColor);
		}
	}
	else
	{
		painter.fillRect(0, 0, m_size.width(), m_size.height(), m_bgColor);
	}
}

void FrameAnimationContent::update(
	CPTRC(rdo::animation::Frame) pFrame,
	 CREF(BitmapList)  bitmapList,
	  REF(BitmapList)  bitmapGeneratedList
)
{
	ASSERT(pFrame);

	if (!valid())
	{
		init(pFrame, bitmapList);
	}

	m_memDC.dc().begin(&m_memDC.buffer());

	drawBackground(pFrame, bitmapList);

	m_memDC.dc().setFont(m_font);

	STL_FOR_ALL_CONST(pFrame->m_elements, it)
	{
		PTR(rdo::animation::FrameItem) pCurrElement = *it;
		ASSERT(pCurrElement);
		switch (pCurrElement->getType())
		{
		case rdo::animation::FrameItem::FIT_TEXT   : elementText     (static_cast<PTR(rdo::animation::TextElement     )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_RECT   : elementRect     (static_cast<PTR(rdo::animation::RectElement     )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_R_RECT : elementRoundRect(static_cast<PTR(rdo::animation::RoundRectElement)>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_LINE   : elementLine     (static_cast<PTR(rdo::animation::LineElement     )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_TRIANG : elementTriang   (static_cast<PTR(rdo::animation::TriangElement   )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_CIRCLE : elementCircle   (static_cast<PTR(rdo::animation::CircleElement   )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_ELLIPSE: elementEllipse  (static_cast<PTR(rdo::animation::EllipseElement  )>(pCurrElement)); break;
		case rdo::animation::FrameItem::FIT_BMP    : elementBMP      (static_cast<PTR(rdo::animation::BmpElement      )>(pCurrElement), bitmapList, bitmapGeneratedList); break;
		case rdo::animation::FrameItem::FIT_S_BMP  : elementSBMP     (static_cast<PTR(rdo::animation::ScaledBmpElement)>(pCurrElement), bitmapList, bitmapGeneratedList); break;
//		case rdo::animation::FrameItem::FIT_ACTIVE : elementActive   (static_cast<PTR(rdo::animation::ActiveElement   )>(pCurrElement), areaList); break;
		}
	}

	m_memDC.dc().end();

	parent_type::update();
}

void FrameAnimationContent::drawBackground(CPTRC(rdo::animation::Frame) pFrame, CREF(BitmapList) bitmapList)
{
	ASSERT(pFrame);

	rbool bgImage = false;
	if (pFrame->hasBgImage())
	{
		BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
		if (bmpIt != bitmapList.end())
		{
			m_memDC.dc().drawPixmap(0, 0, bmpIt->second);
			bgImage = true;
		}
	}

	if (!bgImage)
	{
		m_memDC.dc().setPen(
			QColor(
				GetRValue(studioApp.m_pMainFrame->style_frame.theme->defaultColor),
				GetGValue(studioApp.m_pMainFrame->style_frame.theme->defaultColor),
				GetBValue(studioApp.m_pMainFrame->style_frame.theme->defaultColor)
			)
		);
		m_memDC.dc().setBrush(m_bgColor);

		const ruint pountListCount = 4;
		QPoint pointList[pountListCount];
		pointList[0].setX(0);
		pointList[0].setY(0);
		pointList[1].setX(m_memDC.width () - 1);
		pointList[1].setY(0);
		pointList[2].setX(m_memDC.width () - 1);
		pointList[2].setY(m_memDC.height() - 1);
		pointList[3].setX(0);
		pointList[3].setY(m_memDC.height() - 1);

		m_memDC.dc().drawPolygon(&pointList[0], pountListCount);
	}
}

template <class F>
void FrameAnimationContent::drawColoredElement(CPTR(rdo::animation::ColoredElement) pColor, F drawMethod)
{
	setColors(pColor);
	drawMethod(m_memDC.dc());
}

void FrameAnimationContent::setColors(CPTR(rdo::animation::ColoredElement) pColor)
{
	ASSERT(pColor);
	if (!pColor->m_foreground.m_transparent)
	{
		m_memDC.dc().setPen(QColor(
			pColor->m_foreground.m_r,
			pColor->m_foreground.m_g,
			pColor->m_foreground.m_b
		));
	}
	else
	{
		m_memDC.dc().setPen(Qt::NoPen);
	}

	if (!pColor->m_background.m_transparent)
	{
		m_memDC.dc().setBrush(QColor(
			pColor->m_background.m_r,
			pColor->m_background.m_g,
			pColor->m_background.m_b
		));
	}
	else
	{
		m_memDC.dc().setBrush(Qt::NoBrush);
	}
}

void FrameAnimationContent::elementText(PTR(rdo::animation::TextElement) pElement)
{
	ASSERT(pElement);

	if (pElement->m_foreground.m_transparent)
		return;

	if (!pElement->m_background.m_transparent)
	{
		m_memDC.dc().setBackgroundMode(Qt::OpaqueMode);
		m_memDC.dc().setBackground(QColor(pElement->m_background.m_r, pElement->m_background.m_g, pElement->m_background.m_b));
	}
	else
	{
		m_memDC.dc().setBackgroundMode(Qt::TransparentMode);
	}

	if (!pElement->m_foreground.m_transparent)
	{
		m_memDC.dc().setPen(QColor(pElement->m_foreground.m_r, pElement->m_foreground.m_g, pElement->m_foreground.m_b));
	}

	int flags = Qt::TextSingleLine | Qt::AlignVCenter;
	switch (pElement->m_align)
	{
	case rdo::animation::TextElement::TETA_LEFT  : flags |= Qt::AlignLeft;    break;
	case rdo::animation::TextElement::TETA_RIGHT : flags |= Qt::AlignRight;   break;
	case rdo::animation::TextElement::TETA_CENTER: flags |= Qt::AlignHCenter; break;
	}

	QRectF rect(
		pElement->m_point.m_x,
		pElement->m_point.m_y,
		pElement->m_size.m_width,
		pElement->m_size.m_height
	);

	m_memDC.dc().drawText(rect, flags, QString::fromStdString(pElement->m_text), &rect);
}

void FrameAnimationContent::elementRect(PTR(rdo::animation::RectElement) pElement)
{
	ASSERT(pElement);

	drawColoredElement(
		pElement,
		boost::bind(&QPainter::drawRect,
			_1,
			(int)pElement->m_point.m_x,
			(int)pElement->m_point.m_y,
			(int)pElement->m_size.m_width - 1,
			(int)pElement->m_size.m_height - 1
		)
	);
}

void FrameAnimationContent::elementRoundRect(PTR(rdo::animation::RoundRectElement) pElement)
{
	ASSERT(pElement);

	double radius = std::min<double>(pElement->m_size.m_width, pElement->m_size.m_height) / 3.0f;

	drawColoredElement(
		pElement,
		boost::bind(&QPainter::drawRoundedRect,
			_1,
			(int)pElement->m_point.m_x,
			(int)pElement->m_point.m_y,
			(int)pElement->m_size.m_width,
			(int)pElement->m_size.m_height,
			radius,
			radius,
			Qt::AbsoluteSize
		)
	);
}

void FrameAnimationContent::elementLine(PTR(rdo::animation::LineElement) pElement)
{
	ASSERT(pElement);

	if (!pElement->m_color.m_transparent)
	{
		m_memDC.dc().setPen(QColor(pElement->m_color.m_r, pElement->m_color.m_g, pElement->m_color.m_b));
		m_memDC.dc().drawLine(
			(int)(pElement->m_point1.m_x), (int)(pElement->m_point1.m_y),
			(int)(pElement->m_point2.m_x), (int)(pElement->m_point2.m_y)
		);
	}
}

void FrameAnimationContent::elementTriang(PTR(rdo::animation::TriangElement) pElement)
{
	ASSERT(pElement);

	const ruint pountListCount = 3;
	QPoint pointList[pountListCount];
	pointList[0].setX((int)(pElement->m_point1.m_x));
	pointList[0].setY((int)(pElement->m_point1.m_y));
	pointList[1].setX((int)(pElement->m_point2.m_x));
	pointList[1].setY((int)(pElement->m_point2.m_y));
	pointList[2].setX((int)(pElement->m_point3.m_x));
	pointList[2].setY((int)(pElement->m_point3.m_y));

	void (QPainter::*pMethod)(CPTR(QPoint), int, Qt::FillRule) = &QPainter::drawPolygon;

	drawColoredElement(
		pElement,
		boost::bind(pMethod,
			_1,
			&pointList[0],
			pountListCount,
			Qt::OddEvenFill
		)
	);
}

void FrameAnimationContent::elementCircle(PTR(rdo::animation::CircleElement) pElement)
{
	ASSERT(pElement);

	QRect rect(
		(int)(pElement->m_center.m_x - pElement->m_radius.m_radius),
		(int)(pElement->m_center.m_y - pElement->m_radius.m_radius),
		(int)(pElement->m_radius.m_radius * 2),
		(int)(pElement->m_radius.m_radius * 2)
	);

	void (QPainter::*pMethod)(CREF(QRect)) = &QPainter::drawEllipse;

	drawColoredElement(
		pElement,
		boost::bind(pMethod,
			_1,
			rect
		)
	);
}

void FrameAnimationContent::elementEllipse(PTR(rdo::animation::EllipseElement) pElement)
{
	ASSERT(pElement);

	QRect rect(
		(int)(pElement->m_point.m_x),
		(int)(pElement->m_point.m_y),
		(int)(pElement->m_size.m_width),
		(int)(pElement->m_size.m_height)
	);

	void (QPainter::*pMethod)(CREF(QRect)) = &QPainter::drawEllipse;

	drawColoredElement(
		pElement,
		boost::bind(pMethod,
			_1,
			rect
		)
	);
}

void FrameAnimationContent::elementBMP(
	 PTR(rdo::animation::BmpElement) pElement,
	CREF(BitmapList)       bitmapList,
	 REF(BitmapList)       bitmapGeneratedList)
{
	ASSERT(pElement);

	QPixmap pixmap = FrameAnimationContent::getBitmap(
		pElement->m_bmp_name,
		pElement->hasMask() ? pElement->m_mask_name : tstring(),
		bitmapList,
		bitmapGeneratedList
	);

	if (!pixmap.isNull())
	{
		m_memDC.dc().drawPixmap((int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), pixmap);
	}
}

void FrameAnimationContent::elementSBMP(
	 PTR(rdo::animation::ScaledBmpElement) pElement,
	CREF(BitmapList)             bitmapList,
	 REF(BitmapList)             bitmapGeneratedList)
{
	ASSERT(pElement);

	QPixmap pixmap = FrameAnimationContent::getBitmap(
		pElement->m_bmp_name,
		pElement->hasMask() ? pElement->m_mask_name : tstring(),
		bitmapList,
		bitmapGeneratedList
	);

	if (!pixmap.isNull())
	{
		m_memDC.dc().drawPixmap((int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height), pixmap);
	}
}

QPixmap FrameAnimationContent::getBitmap(
	CREF(tstring)              bitmapName,
	CREF(tstring)              maskName,
	CREF(BitmapList) bitmapList,
	 REF(BitmapList) bitmapGeneratedList)
{
	BitmapList::const_iterator bmpIt = bitmapList.find(bitmapName);
	if (bmpIt == bitmapList.end())
		return QPixmap();

	if (!maskName.empty())
	{
		tstring maskedBitmapName(rdo::format(_T("%s%s"), bitmapName.c_str(), maskName.c_str()));

		BitmapList::const_iterator generatedIt = bitmapList.find(maskedBitmapName);
		if (generatedIt != bitmapList.end())
		{
			return generatedIt->second;
		}

		generatedIt = bitmapGeneratedList.find(maskedBitmapName);
		if (generatedIt != bitmapGeneratedList.end())
		{
			return generatedIt->second;
		}

		BitmapList::const_iterator maskIt = bitmapList.find(maskName);
		if (maskIt != bitmapList.end())
		{
			QImage  generated = bmpIt->second.copy().toImage().convertToFormat(QImage::Format_ARGB32);
			QImage  mask      = maskIt->second.toImage();
			if (!generated.isNull() && !mask.isNull() && generated.size() == mask.size())
			{
				QRgb white       = QColor(Qt::white).rgb();
				QRgb transparent = QColor(Qt::transparent).rgba();
				for (int x = 0; x < mask.width(); ++x)
				{
					for (int y = 0; y < mask.height(); ++y)
					{
						if (mask.pixel(x, y) == white)
						{
							generated.setPixel(x, y, transparent);
						}
					}
				}

				QPixmap pixmap = QPixmap::fromImage(generated);
				if (!pixmap.isNull())
				{
					std::pair<BitmapList::const_iterator, rbool> result =
						bitmapGeneratedList.insert(BitmapList::value_type(maskedBitmapName, pixmap));
					ASSERT(result.second);
					return pixmap;
				}
			}
		}
	}

	return bmpIt->second;
}

// --------------------------------------------------------------------------------
// -------------------- FrameAnimationWnd
// --------------------------------------------------------------------------------
FrameAnimationWnd::FrameAnimationWnd(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent,   true);
	setAttribute(Qt::WA_NoSystemBackground, true);

	setWidgetResizable(true);
	setFrameShadow(QFrame::Plain  );
	setFrameShape (QFrame::NoFrame);

	m_pContent = new FrameAnimationContent(this);
	ASSERT(m_pContent);
	setWidget(m_pContent);
}

FrameAnimationWnd::~FrameAnimationWnd()
{}

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
	: RDOStudioView       ()
	, m_newClientRect     (0, 0, 0, 0)
	, m_hwnd              (NULL )
	, m_mouseOnHScroll    (false)
	, m_pWidget           (NULL )
	, m_pFrameAnimationWnd(NULL )
{
	m_bgColor.SetFromCOLORREF(studioApp.m_pMainFrame->style_frame.theme->backgroundColor);
}

RDOStudioFrameView::~RDOStudioFrameView()
{}

rbool RDOStudioFrameView::valid()
{
	return m_memDC.valid();
}

void RDOStudioFrameView::init(CPTRC(rdo::animation::Frame) pFrame, CREF(FrameAnimationContent::BitmapList) bitmapList)
{
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

	m_pWidget = new QWinWidget(this);
//	m_pWidget->setAutoFillBackground(false);
//	m_pWidget->setAttribute(Qt::WA_OpaquePaintEvent,   true);
	m_pWidget->setAttribute(Qt::WA_NoSystemBackground, true);

	ASSERT(m_pWidget);

	PTR(QVBoxLayout) pVBoxLayout = new QVBoxLayout(m_pWidget);
	ASSERT(pVBoxLayout);
	pVBoxLayout->setSpacing(0);
	pVBoxLayout->setContentsMargins(0, 0, 0, 0);

	m_pFrameAnimationWnd = new FrameAnimationWnd(m_pWidget);
	ASSERT(m_pFrameAnimationWnd);

	pVBoxLayout->addWidget(m_pFrameAnimationWnd);

	m_pWidget->move(0, 0);
	m_pWidget->show();

	m_hwnd = GetSafeHwnd();

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

	delete m_pWidget;
	m_pWidget = NULL;

	m_pFrameAnimationWnd = NULL;

	RDOStudioView::OnDestroy();
}

void RDOStudioFrameView::updateFont()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	PTR(RDOStudioFrameStyle) pStyle = &studioApp.m_pMainFrame->style_frame;
//	lf.lfHeight    = -MulDiv(pStyle->font->size, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight    = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC;
	lf.lfUnderline = pStyle->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = BYTE(pStyle->font->characterSet);
#pragma warning(disable: 4996)
	strcpy(lf.lfFaceName, pStyle->font->name.c_str());
#pragma warning(default: 4996)

	Gdiplus::FontStyle style;
	switch (pStyle->theme->defaultStyle)
	{
	case rdoStyle::RDOStyleFont::BOLD     : style = Gdiplus::FontStyleBold;      break;
	case rdoStyle::RDOStyleFont::ITALIC   : style = Gdiplus::FontStyleItalic;    break;
	case rdoStyle::RDOStyleFont::UNDERLINE: style = Gdiplus::FontStyleUnderline; break;
	default                               : style = Gdiplus::FontStyleRegular;   break;
	}

	std::wstring fontName = rdo::toUnicode(pStyle->font->name);
	m_pFont.reset(new Gdiplus::Font(fontName.c_str(), Gdiplus::REAL(pStyle->font->size), style));

	static_cast<PTR(FrameAnimationContent)>(m_pFrameAnimationWnd->widget())->updateFont();
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
	m_pWidget->resize(cx, cy);

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
	PAINTSTRUCT ps;
	::BeginPaint(m_hwnd, &ps);

//	Gdiplus::Graphics graphics(m_hwnd);
//	onDraw      (graphics);

	::EndPaint  (m_hwnd, &ps);

	m_pFrameAnimationWnd->update();
}

void RDOStudioFrameView::OnDraw(PTR(CDC) pDC)
{
	RDOStudioView::OnDraw(pDC);
}

void RDOStudioFrameView::onDraw(REF(Gdiplus::Graphics) graphics)
{
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
	CPTRC(rdo::animation::Frame) pFrame,
	 CREF(FrameAnimationContent::BitmapList)  bitmapList,
	  REF(FrameAnimationContent::BitmapList)  bitmapGeneratedList,
	  REF(AreaList)              areaList
)
{
	static_cast<PTR(FrameAnimationContent)>(m_pFrameAnimationWnd->widget())->update(pFrame, bitmapList, bitmapGeneratedList);

	//ASSERT(pFrame);

	//if (!valid())
	//{
	//	init(pFrame, bitmapList);
	//}

	//drawBackground(pFrame, bitmapList);

	//STL_FOR_ALL_CONST(pFrame->m_elements, it)
	//{
	//	PTR(rdo::animation::FrameItem) pCurrElement = *it;
	//	ASSERT(pCurrElement);
	//	switch (pCurrElement->getType())
	//	{
	//	case rdo::animation::FrameItem::FIT_ACTIVE : elementActive   (static_cast<PTR(rdo::animation::ActiveElement   )>(pCurrElement), areaList); break;
	//	}
	//}

	//InvalidateRect   (NULL);
	//SendNotifyMessage(WM_PAINT, 0, 0);
}

void RDOStudioFrameView::elementActive(PTR(rdo::animation::ActiveElement) pElement, REF(AreaList) areaList)
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
