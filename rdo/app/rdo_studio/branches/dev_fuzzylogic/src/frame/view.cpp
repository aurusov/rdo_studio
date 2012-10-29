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

void FrameAnimationContent::init(CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

	QSize size;
	rbool imageFound = false;
	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
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

void FrameAnimationContent::mousePressEvent(QMouseEvent* pEvent)
{
	ASSERT(pEvent);
	if (pEvent->button() == Qt::LeftButton)
	{
		PTR(RDOStudioFrameManager) pFrameManager = &model->m_frameManager;
		ruint index = pFrameManager->findFrameIndex(this);
		if (index != ruint(~0))
		{
			pFrameManager->areaDown(index, pEvent->pos());
		}
	}
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
	CPTRC(rdo::animation::Frame)         pFrame,
	 CREF(rdo::gui::BitmapList)          bitmapList,
	  REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	  REF(rdo::gui::animation::AreaList) areaList
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
		case rdo::animation::FrameItem::FIT_ACTIVE : elementActive   (static_cast<PTR(rdo::animation::ActiveElement   )>(pCurrElement), areaList); break;
		}
	}

	m_memDC.dc().end();

	parent_type::update();
}

void FrameAnimationContent::drawBackground(CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

	rbool bgImage = false;
	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(pFrame->m_bgImageName);
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
	CREF(rdo::gui::BitmapList)       bitmapList,
	 REF(rdo::gui::BitmapList)       bitmapGeneratedList)
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
	CREF(rdo::gui::BitmapList)             bitmapList,
	 REF(rdo::gui::BitmapList)             bitmapGeneratedList)
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
	CREF(rdo::gui::BitmapList) bitmapList,
	 REF(rdo::gui::BitmapList) bitmapGeneratedList)
{
	rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(bitmapName);
	if (bmpIt == bitmapList.end())
		return QPixmap();

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
			QPixmap pixmap = rdo::gui::Bitmap::transparent(bmpIt->second, maskIt->second);
			if (!pixmap.isNull())
			{
				std::pair<rdo::gui::BitmapList::const_iterator, rbool> result =
					bitmapGeneratedList.insert(rdo::gui::BitmapList::value_type(maskedBitmapName, pixmap));
				ASSERT(result.second);
				return pixmap;
			}
		}
	}

	return bmpIt->second;
}

void FrameAnimationContent::elementActive(PTR(rdo::animation::ActiveElement) pElement, REF(rdo::gui::animation::AreaList) areaList)
{
	ASSERT(pElement);

	rdo::gui::animation::AreaList::iterator it = areaList.find(pElement->m_opr_name);
	if (it == areaList.end())
	{
		std::pair<rdo::gui::animation::AreaList::iterator, rbool> result =
			areaList.insert(rdo::gui::animation::AreaList::value_type(pElement->m_opr_name, rdo::gui::animation::Area()));
		ASSERT(result.second);
		it = result.first;
	}
	it->second.m_rect = QRect(
		(int)(pElement->m_point.m_x),
		(int)(pElement->m_point.m_y),
		(int)(pElement->m_size.m_width),
		(int)(pElement->m_size.m_height)
	);
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
	ON_WM_CREATE ()
	ON_WM_DESTROY()
	ON_WM_SIZE   ()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP  ()
	ON_WM_PAINT  ()
	ON_COMMAND   (ID_HELP_KEYWORD, OnHelpKeyword)
END_MESSAGE_MAP()

RDOStudioFrameView::RDOStudioFrameView()
	: RDOStudioView       ()
	, m_clientRect        (0, 0, 0, 0)
	, m_pWidget           (NULL )
	, m_pFrameAnimationWnd(NULL )
{}

RDOStudioFrameView::~RDOStudioFrameView()
{}

BOOL RDOStudioFrameView::PreCreateWindow(REF(CREATESTRUCT) cs)
{
	if (!RDOStudioView::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW));

	return TRUE;
}

int RDOStudioFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (RDOStudioView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pWidget = new QWinWidget(this);
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

	updateFont();

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

void RDOStudioFrameView::update(
	CPTRC(rdo::animation::Frame)         pFrame,
	 CREF(rdo::gui::BitmapList)          bitmapList,
	  REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	  REF(rdo::gui::animation::AreaList) areaList
)
{
	static_cast<PTR(FrameAnimationContent)>(m_pFrameAnimationWnd->widget())->update(pFrame, bitmapList, bitmapGeneratedList, areaList);
}

void RDOStudioFrameView::updateFont()
{
	static_cast<PTR(FrameAnimationContent)>(m_pFrameAnimationWnd->widget())->updateFont();
}

PTR(FrameAnimationContent) RDOStudioFrameView::getContent()
{
	PTR(FrameAnimationContent) pContent = static_cast<PTR(FrameAnimationContent)>(m_pFrameAnimationWnd->widget());
	ASSERT(pContent);
	return pContent;
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
	return m_clientRect;
}

void RDOStudioFrameView::OnSize(UINT nType, int cx, int cy)
{
	RDOStudioView::OnSize(nType, cx, cy);

	GetClientRect(&m_clientRect);
	m_pWidget->resize(cx, cy);
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

void RDOStudioFrameView::OnPaint()
{
	CPaintDC dc(this);
	m_pFrameAnimationWnd->update();
}

void RDOStudioFrameView::OnDraw(PTR(CDC) pDC)
{
	RDOStudioView::OnDraw(pDC);
}

void RDOStudioFrameView::OnHelpKeyword()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_frame.htm\n");
	studioApp.callQtAssistant(ba);
}
