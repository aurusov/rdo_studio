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
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//#define MODEL_DOROGA_HACK

// --------------------------------------------------------------------------------
// -------------------- IRDOFRMItemGetBitmap
// --------------------------------------------------------------------------------
IRDOFRMItemGetBitmap::IRDOFRMItemGetBitmap()
{}

IRDOFRMItemGetBitmap::~IRDOFRMItemGetBitmap()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMColor
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMColor::RDOFRMColor(ColorType type)
	: m_type(type)
{}

RDOFRMSprite::RDOFRMColor::RDOFRMColor(rbyte red, rbyte green, rbyte blue, CREF(RDOSrcInfo) srcInfo)
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

RDOFRMSprite::RDOFRMColor::RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc)
	: m_type      (CT_RGB    )
	, m_pRedCalc  (pRedCalc  )
	, m_pGreenCalc(pGreenCalc)
	, m_pBlueCalc (pBlueCalc )
{
	ASSERT(m_pRedCalc  );
	ASSERT(m_pGreenCalc);
	ASSERT(m_pBlueCalc );
}

RDOFRMSprite::RDOFRMColor::~RDOFRMColor()
{}

rdo::animation::Color RDOFRMSprite::RDOFRMColor::getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const
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
	case CT_LAST_BG     : return pSprite->m_colorLastBg;
	case CT_LAST_FG     : return pSprite->m_colorLastFg;
	case CT_LAST_BG_TEXT: return pSprite->m_colorLastBgText;
	case CT_LAST_FG_TEXT: return pSprite->m_colorLastFgText;
	default             : NEVER_REACH_HERE;
	}
	return rdo::animation::Color();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMSprite(CREF(RDOSrcInfo) srcInfo)
	: RDOSrcInfo  (srcInfo)
	, m_lastX         (0             )
	, m_lastY         (0             )
	, m_lastWidth     (0             )
	, m_lastHeight(0      )
{
	m_colorLastBg     = rdo::animation::Color(50, 200, 50);
	m_colorLastFg     = m_colorLastBg;
	m_colorLastBgText = m_colorLastBg;
	m_colorLastFgText = m_colorLastBg;
}

RDOFRMSprite::~RDOFRMSprite()
{}

RDOValue RDOFRMSprite::prepareFrame(CREF(LPRDORuntime) pRuntime)
{
	return RDOFRMSprite::doCalc(pRuntime);
}

RDOValue RDOFRMSprite::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_lastX      = 0;
	m_lastY      = 0;
	m_lastWidth  = 0;
	m_lastHeight = 0;

	m_pSpriteCalc->calcValue(pRuntime);

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

	return RDOValue();
}

void RDOFRMSprite::getBitmaps(REF(IRDOFRMItemGetBitmap::ImageNameList) list) const
{
	BOOST_FOREACH(const LPIRDOFRMItemGetBitmap& pGetBitmap, m_getBitmapList)
	{
		pGetBitmap->getBitmaps(list);
	}
}

void RDOFRMSprite::setColorLastBG(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBg)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastBg = lastBg;
	}
}

void RDOFRMSprite::setColorLastFG(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFg)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastFg = lastFg;
	}
}

void RDOFRMSprite::setColorLastBGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBgText)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastBgText = lastBgText;
	}
}

void RDOFRMSprite::setColorLastFGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFgText)
{
	if (type == RDOFRMColor::CT_RGB)
	{
		m_colorLastFgText = lastFgText;
	}
}

void RDOFRMSprite::insertItem(CREF(LPRDOCalc) pItem)
{
	LPRDOCalcFunctionCaller pFunctionCaller = pItem.object_dynamic_cast<RDOCalcFunctionCaller>();
	if (pFunctionCaller)
	{
		LPRDOFRMSprite pSprite = pFunctionCaller->function().object_dynamic_cast<RDOFRMSprite>();
		if (pSprite)
		{
			LPIRDOFRMItemGetBitmap pGetBitmap = pSprite.interface_dynamic_cast<IRDOFRMItemGetBitmap>();
			if (pGetBitmap)
			{
				insertGetBitmap(pGetBitmap);
			}
		}
	}

	LPIRDOFRMItemGetBitmap pGetBitmap = pItem.interface_dynamic_cast<IRDOFRMItemGetBitmap>();
	if (pGetBitmap)
	{
		insertGetBitmap(pGetBitmap);
	}

	LPRDOFRMRulet pRulet = pItem.object_dynamic_cast<RDOFRMRulet>();
	if (pRulet)
	{
		insertRulet(pRulet);
	}
}

void RDOFRMSprite::setSpriteCalc(CREF(LPRDOCalc) pSpriteCalc)
{
	ASSERT(pSpriteCalc);
	ASSERT(!m_pSpriteCalc);

	LPRDOCalcReturnCatch pCalcReturnCatch = rdo::Factory<RDOCalcReturnCatch>::create();
	ASSERT(pCalcReturnCatch);
	pCalcReturnCatch->setTryCalc(pSpriteCalc);

	m_pSpriteCalc = pCalcReturnCatch;
}

void RDOFRMSprite::insertGetBitmap(CREF(LPIRDOFRMItemGetBitmap) pGetBitmap)
{
	ASSERT(pGetBitmap);
	m_getBitmapList.push_back(pGetBitmap);
}

void RDOFRMSprite::insertRulet(CREF(LPRDOFRMRulet) pRulet)
{
	ASSERT(pRulet);
	std::pair<RuletList::const_iterator, rbool> result =
		m_ruletList.insert(RuletList::value_type(pRulet->getIndex(), pRulet));
	ASSERT(result.second);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMRulet
// --------------------------------------------------------------------------------
RDOValue RDOFRMSprite::RDOFRMRulet::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return RDOValue();
}

CREF(RDOSrcInfo) RDOFRMSprite::RDOFRMRulet::src_info() const
{
	return srcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMText
// --------------------------------------------------------------------------------
RDOFRMText::RDOFRMText(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pSprite                               )
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

RDOValue RDOFRMText::doCalc(CREF(LPRDORuntime) pRuntime)
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
		try
		{
			t = m_pValue->calcValue(pRuntime).getAsString();
		}
		catch (CREF(RDOUndefinedException))
		{
			t = _T("#");
		}
	}

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::TextElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg),
		t, m_align
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmap
// --------------------------------------------------------------------------------
RDOFRMBitmap::RDOFRMBitmap(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename
	)
	: RDOFRMBitmapBase(pSprite, pictFilename, maskFilename)
	, m_pX            (pX)
	, m_pY            (pY)
{}

RDOFRMBitmap::~RDOFRMBitmap()
{}

RDOValue RDOFRMBitmap::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::BmpElement));

	int x = m_pX->getX(pRuntime, getFrame());
	int y = m_pY->getY(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, 0, 0);

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::BmpElement(
		rdo::animation::Point(x, y),
		m_pictFilename, m_maskFilename
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmapStretch
// --------------------------------------------------------------------------------
RDOFRMBitmapStretch::RDOFRMBitmapStretch(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename
	)
	: RDOFRMBitmapBase  (pSprite, pictFilename, maskFilename)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
{}

RDOFRMBitmapStretch::~RDOFRMBitmapStretch()
{}

RDOValue RDOFRMBitmapStretch::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::ScaledBmpElement));

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::ScaledBmpElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		m_pictFilename, m_maskFilename
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMRect
// --------------------------------------------------------------------------------
RDOFRMRect::RDOFRMRect(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pSprite)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRect::~RDOFRMRect()
{}

RDOValue RDOFRMRect::doCalc(CREF(LPRDORuntime) pRuntime)
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

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::RectElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMRectRound
// --------------------------------------------------------------------------------
RDOFRMRectRound::RDOFRMRectRound(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pSprite)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRectRound::~RDOFRMRectRound()
{}

RDOValue RDOFRMRectRound::doCalc(CREF(LPRDORuntime) pRuntime)
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

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::RoundRectElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMCircle
// --------------------------------------------------------------------------------
RDOFRMCircle::RDOFRMCircle(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pRadius,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem       (pSprite)
	, RDOFRMColoredItem(pBgColor, pFgColor)
	, m_pX             (pX     )
	, m_pY             (pY     )
	, m_pRadius        (pRadius)
{}

RDOFRMCircle::~RDOFRMCircle()
{}

RDOValue RDOFRMCircle::doCalc(CREF(LPRDORuntime) pRuntime)
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

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::CircleElement(
		rdo::animation::Point (x ,y),
		rdo::animation::Radius(radius),
		rdo::animation::ColoredElement(bg, fg)
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMEllipse
// --------------------------------------------------------------------------------
RDOFRMEllipse::RDOFRMEllipse(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pSprite)
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMEllipse::~RDOFRMEllipse()
{}

RDOValue RDOFRMEllipse::doCalc(CREF(LPRDORuntime) pRuntime)
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

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::EllipseElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		rdo::animation::ColoredElement(bg, fg)
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMLine
// --------------------------------------------------------------------------------
RDOFRMLine::RDOFRMLine(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY2,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pColor
	)
	: RDOFRMItem        (pSprite           )
	, RDOFRMBoundingItem(pX1, pY1, pX2, pY2)
	, m_pColor          (pColor            )
{}

RDOFRMLine::~RDOFRMLine()
{}

RDOValue RDOFRMLine::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::LineElement));

	rdo::animation::Color fg = m_pColor->getColor(pRuntime, getFrame());
	getFrame()->setColorLastFG(m_pColor->getType(), fg);
	int x1 = getX        (pRuntime, getFrame());
	int y1 = getY        (pRuntime, getFrame());
	int x2 = getWidthAsX (pRuntime, getFrame());
	int y2 = getHeightAsY(pRuntime, getFrame());
	getFrame()->setLastXYWH(x1, y1, x2 - x1, y2 - y1);

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::LineElement(
		rdo::animation::Point(x1, y1),
		rdo::animation::Point(x2, y2),
		fg
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMTriang
// --------------------------------------------------------------------------------
RDOFRMTriang::RDOFRMTriang(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX3,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY3,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem       (pSprite           )
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

RDOValue RDOFRMTriang::doCalc(CREF(LPRDORuntime) pRuntime)
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

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::TriangElement(
		rdo::animation::Point(x1, y1),
		rdo::animation::Point(x2, y2),
		rdo::animation::Point(x3, y3),
		rdo::animation::ColoredElement(bg, fg)
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMActive
// --------------------------------------------------------------------------------
RDOFRMActive::RDOFRMActive(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       operName
	)
	: RDOFRMItem        (pSprite                )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
	, m_operName        (operName               )
{}

RDOFRMActive::~RDOFRMActive()
{}

RDOValue RDOFRMActive::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdo::animation::ActiveElement));

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	pRuntime->getPreparingFrame()->m_elements.push_back(new rdo::animation::ActiveElement(
		rdo::animation::BoundedElement(rdo::animation::Point(x, y), rdo::animation::Size(width, height)),
		m_operName
	));

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSpace
// --------------------------------------------------------------------------------
RDOFRMSpace::RDOFRMSpace(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight
	)
	: RDOFRMItem        (pSprite                )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
{}

RDOFRMSpace::~RDOFRMSpace()
{}

RDOValue RDOFRMSpace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOSrcInfo) srcInfo)
	: RDOFRMSprite (srcInfo)
	, m_picFileName(_T("") )
	, m_width      (800    )
	, m_height     (600    )
	{}

RDOFRMFrame::~RDOFRMFrame()
{}

RDOValue RDOFRMFrame::doCalc(CREF(LPRDORuntime) pRuntime)
{
	PTR(rdo::animation::Frame) pFrame = pRuntime->getPreparingFrame();
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

	return RDOFRMSprite::prepareFrame(pRuntime);
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

void RDOFRMFrame::prepareFrame(PTR(rdo::animation::Frame) pFrame, CREF(LPRDORuntime) pRuntime)
{
	ASSERT(pFrame);

	pFrame->m_bgImageName   = m_picFileName;
	pFrame->m_size.m_width  = m_width;
	pFrame->m_size.m_height = m_height;

	pRuntime->setPreparingFrame(pFrame);
	calcValue(pRuntime);
	pRuntime->resetPreparingFrame();
}

void RDOFRMFrame::getBitmaps(REF(IRDOFRMItemGetBitmap::ImageNameList) list) const
{
	if (!m_picFileName.empty())
		list.push_back(m_picFileName);

	RDOFRMSprite::getBitmaps(list);
}

CLOSE_RDO_RUNTIME_NAMESPACE
