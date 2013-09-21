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
#include "utils/src/common/warning_disable.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <QLayout>
#include <QPainter>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/frame/frame_view.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::frame;

// --------------------------------------------------------------------------------
// -------------------- Content
// --------------------------------------------------------------------------------
Content::Content(QWidget* pParent)
	: parent_type(pParent)
	, m_size     (QSize (0, 0))
	, m_pos      (QPoint(0, 0))
{
	setAttribute(Qt::WA_NoSystemBackground, true);

	m_bgColor = QColor(g_pApp->getStyle()->style_frame.backgroundColor);

	updateFont();
}

Content::~Content()
{}

bool Content::valid() const
{
	return m_memDC.valid();
}

void Content::init(CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

	QSize size;
	bool imageFound = false;
	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(QString::fromStdString(pFrame->m_bgImageName));
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
		bgColor = QColor(g_pApp->getStyle()->style_frame.backgroundColor);
	}
	else
	{
		bgColor = QColor(pFrame->m_bgColor.m_r, pFrame->m_bgColor.m_g, pFrame->m_bgColor.m_b);
	}
	setBGColor(bgColor);

	init(size);
}

void Content::init(CREF(QSize) size)
{
	m_memDC.resize(size.width(), size.height());
	setMinimumSize(size);
}

void Content::updateFont()
{
	PTR(FrameStyle) pStyle = &g_pApp->getStyle()->style_frame;
	ASSERT(pStyle);

	m_font = QFont(pStyle->font.name.c_str());
	m_font.setBold     (pStyle->defaultStyle & rdo::gui::style::StyleFont::BOLD      ? true : false);
	m_font.setItalic   (pStyle->defaultStyle & rdo::gui::style::StyleFont::ITALIC    ? true : false);
	m_font.setUnderline(pStyle->defaultStyle & rdo::gui::style::StyleFont::UNDERLINE ? true : false);
	m_font.setPointSize(pStyle->font.size);
}

void Content::setBGColor(CREF(QColor) color)
{
	m_bgColor = color;
}

void Content::resizeEvent(QResizeEvent* pEvent)
{
	m_size = pEvent->size();
	parent_type::resizeEvent(pEvent);
}

void Content::paintEvent(QPaintEvent* pEvent)
{
	UNUSED(pEvent);

	QPainter painter(this);
	onDraw(painter);

	parent_type::paintEvent(pEvent);
}

void Content::mousePressEvent(QMouseEvent* pEvent)
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

void Content::onDraw(REF(QPainter) painter)
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

void Content::update(
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

void Content::drawBackground(CPTRC(rdo::animation::Frame) pFrame, CREF(rdo::gui::BitmapList) bitmapList)
{
	ASSERT(pFrame);

	bool bgImage = false;
	if (pFrame->hasBgImage())
	{
		rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(QString::fromStdString(pFrame->m_bgImageName));
		if (bmpIt != bitmapList.end())
		{
			m_memDC.dc().drawPixmap(0, 0, bmpIt->second);
			bgImage = true;
		}
	}

	if (!bgImage)
	{
		m_memDC.dc().setPen(QColor(g_pApp->getStyle()->style_frame.defaultColor));
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
void Content::drawColoredElement(CPTR(rdo::animation::ColoredElement) pColor, F drawMethod)
{
	setColors(pColor);
	drawMethod(m_memDC.dc());
}

void Content::setColors(CPTR(rdo::animation::ColoredElement) pColor)
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

void Content::elementText(PTR(rdo::animation::TextElement) pElement)
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

void Content::elementRect(PTR(rdo::animation::RectElement) pElement)
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

void Content::elementRoundRect(PTR(rdo::animation::RoundRectElement) pElement)
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

void Content::elementLine(PTR(rdo::animation::LineElement) pElement)
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

void Content::elementTriang(PTR(rdo::animation::TriangElement) pElement)
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

void Content::elementCircle(PTR(rdo::animation::CircleElement) pElement)
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

void Content::elementEllipse(PTR(rdo::animation::EllipseElement) pElement)
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

void Content::elementBMP(
	 PTR(rdo::animation::BmpElement) pElement,
	CREF(rdo::gui::BitmapList)       bitmapList,
	 REF(rdo::gui::BitmapList)       bitmapGeneratedList)
{
	ASSERT(pElement);

	QPixmap pixmap = Content::getBitmap(
		QString::fromStdString(pElement->m_bmp_name),
		pElement->hasMask() ? QString::fromStdString(pElement->m_mask_name) : QString(),
		bitmapList,
		bitmapGeneratedList
	);

	if (!pixmap.isNull())
	{
		m_memDC.dc().drawPixmap((int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), pixmap);
	}
}

void Content::elementSBMP(
	 PTR(rdo::animation::ScaledBmpElement) pElement,
	CREF(rdo::gui::BitmapList)             bitmapList,
	 REF(rdo::gui::BitmapList)             bitmapGeneratedList)
{
	ASSERT(pElement);

	QPixmap pixmap = Content::getBitmap(
		QString::fromStdString(pElement->m_bmp_name),
		pElement->hasMask() ? QString::fromStdString(pElement->m_mask_name) : QString(),
		bitmapList,
		bitmapGeneratedList
	);

	if (!pixmap.isNull())
	{
		m_memDC.dc().drawPixmap((int)(pElement->m_point.m_x), (int)(pElement->m_point.m_y), (int)(pElement->m_size.m_width), (int)(pElement->m_size.m_height), pixmap);
	}
}

QPixmap Content::getBitmap(
	CREF(QString)              bitmapName,
	CREF(QString)              maskName,
	CREF(rdo::gui::BitmapList) bitmapList,
	 REF(rdo::gui::BitmapList) bitmapGeneratedList)
{
	rdo::gui::BitmapList::const_iterator bmpIt = bitmapList.find(bitmapName);
	if (bmpIt == bitmapList.end())
		return QPixmap();

	if (!maskName.isEmpty())
	{
		QString maskedBitmapName = QString("%1%2").arg(bitmapName).arg(maskName);

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
				std::pair<rdo::gui::BitmapList::const_iterator, bool> result =
					bitmapGeneratedList.insert(rdo::gui::BitmapList::value_type(maskedBitmapName, pixmap));
				ASSERT(result.second);
				return pixmap;
			}
		}
	}

	return bmpIt->second;
}

void Content::elementActive(PTR(rdo::animation::ActiveElement) pElement, REF(rdo::gui::animation::AreaList) areaList)
{
	ASSERT(pElement);

	QString oprName(QString::fromStdString(pElement->m_opr_name));
	rdo::gui::animation::AreaList::iterator it = areaList.find(oprName);
	if (it == areaList.end())
	{
		std::pair<rdo::gui::animation::AreaList::iterator, bool> result =
			areaList.insert(rdo::gui::animation::AreaList::value_type(oprName, rdo::gui::animation::Area()));
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
// -------------------- View
// --------------------------------------------------------------------------------
View::View(QWidget* pParent)
	: parent_type(pParent)
{
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent,   true);
	setAttribute(Qt::WA_NoSystemBackground, true);

	setWidgetResizable(true);
	setFrameShadow(QFrame::Plain  );
	setFrameShape (QFrame::NoFrame);

	m_pContent = new Content(this);
	ASSERT(m_pContent);
	setWidget(m_pContent);

	updateFont();
}

View::~View()
{
	ruint index = g_pModel->getFrameManager().findFrameIndex(this);
	if (index != ruint(~0))
	{
		g_pModel->getFrameManager().disconnectView(this);
		g_pModel->getFrameManager().resetCurrentShowingFrame(index);
	}
}

PTR(Content) View::getContent()
{
	PTR(Content) pContent = static_cast<PTR(Content)>(widget());
	ASSERT(pContent);
	return pContent;
}

void View::update(
	CPTRC(rdo::animation::Frame)         pFrame,
	 CREF(rdo::gui::BitmapList)          bitmapList,
	  REF(rdo::gui::BitmapList)          bitmapGeneratedList,
	  REF(rdo::gui::animation::AreaList) areaList
)
{
	getContent()->update(pFrame, bitmapList, bitmapGeneratedList, areaList);
}

void View::updateFont()
{
	getContent()->updateFont();
}

bool View::event(QEvent* pEvent)
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
			g_pApp->callQtAssistant(ba);
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
