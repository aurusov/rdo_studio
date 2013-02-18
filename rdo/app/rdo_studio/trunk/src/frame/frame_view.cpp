/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <QtWidgets/qlayout.h>
#include <QtGui/qpainter.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/frame/frame_view.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_windows_base.h"
// --------------------------------------------------------------------------------

FrameAnimationContent::FrameAnimationContent(PTR(QWidget) pParent)
	: parent_type(pParent)
	, m_size     (QSize (0, 0))
	, m_pos      (QPoint(0, 0))
{
	setAttribute(Qt::WA_NoSystemBackground, true);

	m_bgColor = QColor(studioApp.getStyle()->style_frame.theme->backgroundColor);

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
		bgColor = QColor(studioApp.getStyle()->style_frame.theme->backgroundColor);
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
	PTR(RDOStudioFrameStyle) pStyle = &studioApp.getStyle()->style_frame;
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
	parent_type::resizeEvent(pEvent);
}

void FrameAnimationContent::paintEvent(QPaintEvent* pEvent)
{
	UNUSED(pEvent);

	QPainter painter(this);
	onDraw(painter);

	parent_type::paintEvent(pEvent);
}

void FrameAnimationContent::mousePressEvent(QMouseEvent* pEvent)
{
	ASSERT(pEvent);
	if (pEvent->button() == Qt::LeftButton)
	{
		ruint index = g_pModel->getFrameManager().findFrameIndex(this);
		if (index != ruint(~0))
		{
			g_pModel->getFrameManager().areaDown(index, pEvent->pos());
		}
	}

	parent_type::mousePressEvent(pEvent);
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

	BOOST_FOREACH(PTR(rdo::animation::FrameItem) pCurrElement, pFrame->m_elements)
	{
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
		m_memDC.dc().setPen(QColor(studioApp.getStyle()->style_frame.theme->defaultColor));
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

	m_memDC.dc().drawText(rect, flags, QString::fromLocal8Bit(pElement->m_text.c_str()), &rect);
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

	updateFont();
}

FrameAnimationWnd::~FrameAnimationWnd()
{
	ruint index = g_pModel->getFrameManager().findFrameIndex(this);
	if (index != ruint(~0))
	{
		g_pModel->getFrameManager().disconnectView(this);
		g_pModel->getFrameManager().resetCurrentShowingFrame(index);
	}
}

PTR(FrameAnimationContent) FrameAnimationWnd::getContent()
{
	PTR(FrameAnimationContent) pContent = static_cast<PTR(FrameAnimationContent)>(widget());
	ASSERT(pContent);
	return pContent;
}

void FrameAnimationWnd::update(
	CPTRC(rdo::animation::Frame)         pFrame,
	 CREF(rdo::gui::BitmapList)          bitmapList,
	  REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	  REF(rdo::gui::animation::AreaList) areaList
)
{
	getContent()->update(pFrame, bitmapList, bitmapGeneratedList, areaList);
}

void FrameAnimationWnd::updateFont()
{
	getContent()->updateFont();
}

rbool FrameAnimationWnd::event(QEvent* pEvent)
{
	if (pEvent->type() == QEvent::KeyPress || pEvent->type() == QEvent::ShortcutOverride)
	{
		QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
		ruint scanCode = pKeyEvent->nativeScanCode();
		g_pModel->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_KEY_DOWN, &scanCode);

		if (pKeyEvent->key() == Qt::Key_F1)
		{
			QByteArray ba;
			ba.append("setSource qthelp://language/doc/rdo_studio_rus/html/work_model/work_model_frame.htm\n");
			studioApp.callQtAssistant(ba);
		}

		return true;
	}
	else if (pEvent->type() == QEvent::KeyRelease)
	{
		QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
		ruint scanCode = pKeyEvent->nativeScanCode();
		g_pModel->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_KEY_UP, &scanCode);
		return true;
	}

	return parent_type::event(pEvent);
}
