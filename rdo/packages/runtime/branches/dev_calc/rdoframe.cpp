/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoframe.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      07.12.2008
  \brief     Кадры РДО модели
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//#define MODEL_DOROGA_HACK

// --------------------------------------------------------------------------------
// -------------------- RDOFRMColor
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMColor::RDOFRMColor(ColorType type)
	: m_type(type)
{}

RDOFRMFrame::RDOFRMColor::RDOFRMColor(rbyte red, rbyte green, rbyte blue, CREF(RDOSrcInfo) srcInfo)
	: m_type(CT_RGB)
{
	m_pRedCalc   = rdo::Factory<RDOCalcConst>::create((rsint)red  );
	m_pGreenCalc = rdo::Factory<RDOCalcConst>::create((rsint)green);
	m_pBlueCalc  = rdo::Factory<RDOCalcConst>::create((rsint)blue );
	ASSERT(m_pRedCalc  );
	ASSERT(m_pGreenCalc);
	ASSERT(m_pBlueCalc );
	m_pRedCalc  ->setSrcInfo(srcInfo);
	m_pGreenCalc->setSrcInfo(srcInfo);
	m_pBlueCalc ->setSrcInfo(srcInfo);
}

RDOFRMFrame::RDOFRMColor::RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc)
	: m_type      (CT_RGB    )
	, m_pRedCalc  (pRedCalc  )
	, m_pGreenCalc(pGreenCalc)
	, m_pBlueCalc (pBlueCalc )
{
	ASSERT(m_pRedCalc  );
	ASSERT(m_pGreenCalc);
	ASSERT(m_pBlueCalc );
}

RDOFRMFrame::RDOFRMColor::~RDOFRMColor()
{}

rdo::animation::Color RDOFRMFrame::RDOFRMColor::getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	switch (m_type)
	{
	case CT_NONE        : return rdo::animation::Color(50, 200, 50);
	case CT_RGB         : return rdo::animation::Color
						  (
							(rbyte)m_pRedCalc  ->calcValue(pRuntime).getUInt(),
							(rbyte)m_pGreenCalc->calcValue(pRuntime).getUInt(),
							(rbyte)m_pBlueCalc ->calcValue(pRuntime).getUInt()
						  );
	case CT_TRANSPARENT : return rdo::animation::Color();
	case CT_LAST_BG     : return pFrame->m_colorLastBg;
	case CT_LAST_FG     : return pFrame->m_colorLastFg;
	case CT_LAST_BG_TEXT: return pFrame->m_colorLastBgText;
	case CT_LAST_FG_TEXT: return pFrame->m_colorLastFgText;
	default             : NEVER_REACH_HERE;
	}
	return rdo::animation::Color();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOSrcInfo) src_info, CREF(LPRDOCalc) pConditionCalc)
	: RDOSrcInfo      (src_info      )
	, m_pConditionCalc(pConditionCalc)
	, m_picFileName   (_T("")        )
	, m_width         (800           )
	, m_height        (600           )
	, m_lastX         (0             )
	, m_lastY         (0             )
	, m_lastWidth     (0             )
	, m_lastHeight    (0             )
{
	m_colorLastBg     = rdo::animation::Color(50, 200, 50);
	m_colorLastFg     = m_colorLastBg;
	m_colorLastBgText = m_colorLastBg;
	m_colorLastFgText = m_colorLastBg;
}

RDOFRMFrame::~RDOFRMFrame()
{}

void RDOFRMFrame::setColorLastBG(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBg)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastBg = lastBg;
	}
}

void RDOFRMFrame::setColorLastFG(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFg)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastFg = lastFg;
	}
}

void RDOFRMFrame::setColorLastBGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBgText)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastBgText = lastBgText;
	}
}

void RDOFRMFrame::setColorLastFGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFgText)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastFgText = lastFgText;
	}
}

void RDOFRMFrame::setBackgroundColor(CREF(LPRDOFRMColor) pBgColor)
{
	m_pBgColor = pBgColor;
}

void RDOFRMFrame::setBackPicture(CREF(tstring) picFileName)
{
	m_picFileName = picFileName;
}

void RDOFRMFrame::setBackPicture(int width, int height)
{
	m_picFileName = _T("");
	m_width       = width;
	m_height      = height;
}

void RDOFRMFrame::startShow(CREF(LPRDOCalc) pCalc)
{
	m_showList.push_back(rdo::Factory<RDOFRMShow>::create(pCalc));
}

void RDOFRMFrame::addItem(CREF(LPRDOFRMItem) pItem)
{
	ASSERT(pItem);

	if (m_showList.empty())
	{
		startShow();
	}

	m_showList.back()->insertItem(pItem);
}

void RDOFRMFrame::addRulet(CREF(LPRDOFRMRulet) pRulet)
{
	ASSERT(pRulet);
	std::pair<RuletList::const_iterator, rbool> result =
		m_ruletList.insert(RuletList::value_type(pRulet->getIndex(), pRulet));
	ASSERT(result.second);
}

rbool RDOFRMFrame::checkCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!m_pConditionCalc)
		return true;

	return m_pConditionCalc->calcValue(pRuntime).getAsBool();
}

PTR(rdo::animation::Frame) RDOFRMFrame::prepareFrame(PTR(rdo::animation::Frame) pFrame, CREF(LPRDORuntime) pRuntime)
{
	ASSERT(pFrame);

	if (m_pBgColor)
	{
		if (m_pBgColor->getType() == RDOFRMColor::CT_RGB)
		{
			rdo::animation::Color bgColor = m_pBgColor->getColor(pRuntime, this);
			pFrame->m_bgColor = bgColor;
		}
		else
		{
			pFrame->m_bgColor = rdo::animation::Color();
		}
	}
	else
	{
		pFrame->m_bgColor = rdo::animation::Color();
	}
	pFrame->m_bgImageName   = m_picFileName;
	pFrame->m_size.m_width  = m_width;
	pFrame->m_size.m_height = m_height;

	m_lastX      = 0;
	m_lastY      = 0;
	m_lastWidth  = 0;
	m_lastHeight = 0;

	if (checkCondition(pRuntime))
	{
		STL_FOR_ALL(m_showList, showIt)
		{
			if ((*showIt)->checkCondition(pRuntime))
			{
				STL_FOR_ALL((*showIt)->getItemList(), itemIt)
				{
					PTR(rdo::animation::FrameItem) pElement = (*itemIt)->createElement(pRuntime);
					if (pElement)
					{
						pFrame->m_elements.push_back(pElement);
					}
				}
			}
		}

#ifdef MODEL_DOROGA_HACK
		RDORuntime::ResCIterator end = pRuntime->res_end();
		for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; ++it)
		{
			if (*it && (*it)->checkType(1))
			{
				rdo::animation::Point point((*it)->getParam(0).getInt(), (*it)->getParam(1).getInt());
				rdo::animation::Size  size;
				if ((*it)->getParam(5).getInt() == 1 || (*it)->getParam(5).getInt() == 3)
				{
					size.m_width  = (*it)->getParam(8).getInt();
					size.m_height = (*it)->getParam(9).getInt();
				}
				else
				{
					size.m_width  = (*it)->getParam(9).getInt();
					size.m_height = (*it)->getParam(8).getInt();
				}
				point.m_x -= size.m_width  / 2;
				point.m_y -= size.m_height / 2;

				ruint colorRuint = (*it)->getParam(7).getUInt();
				rdo::animation::Color color(GetBValue(colorRuint), GetGValue(colorRuint), GetRValue(colorRuint));

				PTR(rdo::animation::FrameItem) pRect = new rdo::animation::RectElement(
					rdo::animation::BoundedElement(point, size),
					rdo::animation::ColoredElement(color, color)
				);
				pFrame->m_elements.push_back(pRect);
			}
		}
#endif // MODEL_DOROGA_HACK
	}

	return pFrame;
}

void RDOFRMFrame::getBitmaps(REF(ImageNameList) list) const
{
	if (!m_picFileName.empty())
		list.push_back(m_picFileName);

	STL_FOR_ALL_CONST(m_showList, it)
	{
		(*it)->getBitmaps(list);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMText
// --------------------------------------------------------------------------------
RDOFRMText::RDOFRMText(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pFrame                                )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight               )
	, RDOFRMColoredItem (pBgColor, pFgColor                    )
	, m_align           (rdo::animation::TextElement::TETA_LEFT)
	, m_isTextString    (true                                  )
{}

RDOFRMText::~RDOFRMText()
{}

void RDOFRMText::setText(Align align, CREF(LPRDOCalc) pValue)
{
	m_align        = align;
	m_pValue       = pValue;
	m_isTextString = false;
}

void RDOFRMText::setText(Align align, CREF(tstring) text)
{
	m_align        = align;
	m_text         = text;
	m_isTextString = true;
}

PTR(rdo::animation::FrameItem) RDOFRMText::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::TextElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBGText(getBgColor()->getType(), bg);
	getFrame()->setColorLastFGText(getFgColor()->getType(), fg);

	tstring t;
	if (m_isTextString)
	{
		t = m_text;
	}
	else
	{
		RDOValue val = m_pValue->calcValue(pRuntime);
		t = val.getAsString();
	}

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::TextElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg),
		t, m_align
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmap
// --------------------------------------------------------------------------------
RDOFRMBitmap::RDOFRMBitmap(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename
	)
	: RDOFRMBitmapBase(pFrame, pictFilename, maskFilename)
	, m_pX            (pX)
	, m_pY            (pY)
{}

RDOFRMBitmap::~RDOFRMBitmap()
{}

PTR(rdo::animation::FrameItem) RDOFRMBitmap::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::BmpElement));

	int x = m_pX->getX(pRuntime, getFrame());
	int y = m_pY->getY(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, 0, 0);

	return new rdo::animation::BmpElement(
		rdo::animation::Point(x, y),
		m_pictFilename, m_maskFilename
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmapStretch
// --------------------------------------------------------------------------------
RDOFRMBitmapStretch::RDOFRMBitmapStretch(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename
	)
	: RDOFRMBitmapBase  (pFrame, pictFilename, maskFilename)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight           )
{}

RDOFRMBitmapStretch::~RDOFRMBitmapStretch()
{}

PTR(rdo::animation::FrameItem) RDOFRMBitmapStretch::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::ScaledBmpElement));

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::ScaledBmpElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		m_pictFilename, m_maskFilename
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMRect
// --------------------------------------------------------------------------------
RDOFRMRect::RDOFRMRect(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pFrame)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRect::~RDOFRMRect()
{}

PTR(rdo::animation::FrameItem) RDOFRMRect::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::RectElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::RectElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMRectRound
// --------------------------------------------------------------------------------
RDOFRMRectRound::RDOFRMRectRound(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pFrame)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRectRound::~RDOFRMRectRound()
{}

PTR(rdo::animation::FrameItem) RDOFRMRectRound::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::RoundRectElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::RoundRectElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMCircle
// --------------------------------------------------------------------------------
RDOFRMCircle::RDOFRMCircle(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pRadius,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem       (pFrame )
	, m_pX             (pX     )
	, m_pY             (pY     )
	, m_pRadius        (pRadius)
	, RDOFRMColoredItem(pBgColor, pFgColor)
{}

RDOFRMCircle::~RDOFRMCircle()
{}

PTR(rdo::animation::FrameItem) RDOFRMCircle::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::CircleElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getType(), fg);

	int x      = m_pX     ->getX(pRuntime, getFrame());
	int y      = m_pY     ->getY(pRuntime, getFrame());
	int radius = m_pRadius->getX(pRuntime, getFrame());
	getFrame()->setLastXY(x, y);

	return new rdo::animation::CircleElement(
		rdo::animation::Point (x ,y),
		rdo::animation::Radius(radius),
		rdo::animation::ColoredElement(bg, fg)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMEllipse
// --------------------------------------------------------------------------------
RDOFRMEllipse::RDOFRMEllipse(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pFrame)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMEllipse::~RDOFRMEllipse()
{}

PTR(rdo::animation::FrameItem) RDOFRMEllipse::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::EllipseElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::EllipseElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMLine
// --------------------------------------------------------------------------------
RDOFRMLine::RDOFRMLine(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY2,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pColor
	)
	: RDOFRMItem        (pFrame            )
	, RDOFRMBoundingItem(pX1, pY1, pX2, pY2)
	, m_pColor          (pColor            )
{}

RDOFRMLine::~RDOFRMLine()
{}

PTR(rdo::animation::FrameItem) RDOFRMLine::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::LineElement));

	rdo::animation::Color fg = m_pColor->getColor(pRuntime, getFrame());
	getFrame()->setColorLastFG(m_pColor->getType(), fg);
	int x1 = getX        (pRuntime, getFrame());
	int y1 = getY        (pRuntime, getFrame());
	int x2 = getWidthAsX (pRuntime, getFrame());
	int y2 = getHeightAsY(pRuntime, getFrame());
	getFrame()->setLastXYWH(x1, y1, x2 - x1, y2 - y1);

	return new rdo::animation::LineElement(
		rdo::animation::Point(x1, y1),
		rdo::animation::Point(x2, y2),
		fg
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMTriang
// --------------------------------------------------------------------------------
RDOFRMTriang::RDOFRMTriang(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX3,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY3,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem       (pFrame            )
	, RDOFRMColoredItem(pBgColor, pFgColor)
	, m_pX1            (pX1               )
	, m_pY1            (pY1               )
	, m_pX2            (pX2               )
	, m_pY2            (pY2               )
	, m_pX3            (pX3               )
	, m_pY3            (pY3               )
{}

RDOFRMTriang::~RDOFRMTriang()
{}

PTR(rdo::animation::FrameItem) RDOFRMTriang::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::TriangElement));

	rdo::animation::Color bg = getBg(pRuntime, getFrame());
	rdo::animation::Color fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getType(), fg);
	int x1 = m_pX1->getX(pRuntime, getFrame());
	int y1 = m_pY1->getY(pRuntime, getFrame());
	int minX = x1;
	int maxX = x1;
	int minY = y1;
	int maxY = y1;
	getFrame()->setLastXYWH(x1, y1, x1, y1);
	int x2 = m_pX2->getX(pRuntime, getFrame());
	int y2 = m_pY2->getY(pRuntime, getFrame());
	if (minX > x2) minX = x2;
	if (maxX < x2) maxX = x2;
	if (minY > y2) minY = y2;
	if (maxY < x2) maxY = y2;
	getFrame()->setLastXYWH(x2, y2, x2 - x1, y2 - y1);
	int x3 = m_pX3->getX(pRuntime, getFrame());
	int y3 = m_pY3->getY(pRuntime, getFrame());
	if (minX > x3) minX = x3;
	if (maxX < x3) maxX = x3;
	if (minY > y3) minY = y3;
	if (maxY < x3) maxY = y3;
	getFrame()->setLastXYWH(minX, minY, maxX - minX, maxY - minY);

	return new rdo::animation::TriangElement(
		rdo::animation::Point(x1, y1),
		rdo::animation::Point(x2, y2),
		rdo::animation::Point(x3, y3),
		rdo::animation::ColoredElement(bg, fg)
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMActive
// --------------------------------------------------------------------------------
RDOFRMActive::RDOFRMActive(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       operName
	)
	: RDOFRMItem        (pFrame                 )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, m_operName        (operName               )
{}

RDOFRMActive::~RDOFRMActive()
{}

PTR(rdo::animation::FrameItem) RDOFRMActive::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::ActiveElement));

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdo::animation::ActiveElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		m_operName
	);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSpace
// --------------------------------------------------------------------------------
RDOFRMSpace::RDOFRMSpace(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight
	)
	: RDOFRMItem        (pFrame                 )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
{}

RDOFRMSpace::~RDOFRMSpace()
{}

PTR(rdo::animation::FrameItem) RDOFRMSpace::createElement(CREF(LPRDORuntime) pRuntime)
{
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMShow
// --------------------------------------------------------------------------------
RDOFRMShow::RDOFRMShow(CREF(LPRDOCalc) pConditionCalc)
	: m_pConditionCalc(pConditionCalc)
{}

RDOFRMShow::~RDOFRMShow()
{}

void RDOFRMShow::insertItem(CREF(LPRDOFRMItem) pItem)
{
	ASSERT(pItem);
	m_itemList.push_back(pItem);
}

REF(RDOFRMShow::ItemList) RDOFRMShow::getItemList()
{
	return m_itemList;
}

void RDOFRMShow::getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
{
	STL_FOR_ALL(m_itemList, it)
	{
		(*it)->getBitmaps(list);
	}
}

rbool RDOFRMShow::checkCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!m_pConditionCalc)
		return true;

	return m_pConditionCalc->calcValue(pRuntime).getAsBool();
}

CLOSE_RDO_RUNTIME_NAMESPACE
