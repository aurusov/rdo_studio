/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoframe.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      31.07.2011
  \brief     Кадры РДО модели
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame::RDOFRMPosition
// --------------------------------------------------------------------------------
inline RDOFRMFrame::RDOFRMPosition::RDOFRMPosition()
	: RDORuntimeObject()
	, m_type   (PT_ABSOLUTE)
	, m_ruletID(0          )
{}

inline RDOFRMFrame::RDOFRMPosition::RDOFRMPosition(CREF(LPRDOCalc) pCalc, PositionType type, int ruletID)
	: RDORuntimeObject()
	, m_pCalc  (pCalc  )
	, m_type   (type   )
	, m_ruletID(ruletID)
{}

inline RDOFRMFrame::RDOFRMPosition::~RDOFRMPosition()
{}

inline RDOFRMFrame::RDOFRMPosition::PositionType RDOFRMFrame::RDOFRMPosition::getType() const
{
	return m_type;
}

inline CREF(LPRDOCalc) RDOFRMFrame::RDOFRMPosition::getCalc() const
{
	return m_pCalc;
}

inline int RDOFRMFrame::RDOFRMPosition::getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA  : res += pFrame->m_lastX;                        break;
	case RDOFRMPosition::PT_GABARIT: res += pFrame->m_lastX + pFrame->m_lastWidth;  break;
	case RDOFRMPosition::PT_MULT   : res *= pFrame->m_lastX;                        break;
	case RDOFRMPosition::PT_RULET  : res += pFrame->getRuletX(pRuntime, m_ruletID); break;
	}
	return res.getInt();
}

inline int RDOFRMFrame::RDOFRMPosition::getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA  : res += pFrame->m_lastY;                        break;
	case RDOFRMPosition::PT_GABARIT: res += pFrame->m_lastY + pFrame->m_lastHeight; break;
	case RDOFRMPosition::PT_MULT   : res *= pFrame->m_lastY;                        break;
	case RDOFRMPosition::PT_RULET  : res += pFrame->getRuletY(pRuntime, m_ruletID); break;
	}
	return res.getInt();
}

inline int RDOFRMFrame::RDOFRMPosition::getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA: res += pFrame->m_lastWidth; break;
	case RDOFRMPosition::PT_MULT : res *= pFrame->m_lastWidth; break;
	case RDOFRMPosition::PT_RULET: res += pFrame->getRuletX(pRuntime, m_ruletID); break;
	}
	return res.getInt();
}

inline int RDOFRMFrame::RDOFRMPosition::getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA: res += pFrame->m_lastHeight; break;
	case RDOFRMPosition::PT_MULT : res *= pFrame->m_lastHeight; break;
	case RDOFRMPosition::PT_RULET: res += pFrame->getRuletY(pRuntime, m_ruletID); break;
	}
	return res.getInt();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame::RDOFRMColor
// --------------------------------------------------------------------------------
inline RDOFRMFrame::RDOFRMColor::ColorType RDOFRMFrame::RDOFRMColor::getType() const
{
	return m_type;
}

inline void RDOFRMFrame::RDOFRMColor::setType(ColorType type)
{
	if (m_type == CT_NONE)
	{
		m_type = type;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame::RDOFRMRulet
// --------------------------------------------------------------------------------
inline ruint RDOFRMFrame::RDOFRMRulet::getIndex() const
{
	return m_index;
}

inline CREF(RDOFRMFrame::LPRDOFRMPosition) RDOFRMFrame::RDOFRMRulet::getX() const
{
	return m_pX;
}

inline CREF(RDOFRMFrame::LPRDOFRMPosition) RDOFRMFrame::RDOFRMRulet::getY() const
{
	return m_pY;
}

inline RDOFRMFrame::RDOFRMRulet::RDOFRMRulet(CREF(RDOSrcInfo) src_info, ruint index, CREF(LPRDOFRMPosition) pX, CREF(LPRDOFRMPosition) pY)
	: RDOSrcInfo(src_info)
	, m_index   (index   )
	, m_pX      (pX      )
	, m_pY      (pY      )
{}

inline RDOFRMFrame::RDOFRMRulet::~RDOFRMRulet()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
inline LPRDOFRMShow RDOFRMFrame::getLastShow() const
{
	return !m_showList.empty() ? m_showList.back() : LPRDOFRMShow(NULL);
}

inline CREF(tstring) RDOFRMFrame::name() const
{
	return src_text();
}

inline PTR(rdo::animation::RDOFrame) RDOFRMFrame::createFrame(CREF(LPRDORuntime) pRuntime)
{
	PTR(rdo::animation::RDOFrame) pFrame = new rdo::animation::RDOFrame();
	return prepareFrame(pFrame, pRuntime);
}

inline void RDOFRMFrame::setLastXYWH(double x, double y, double width, double height)
{
	setLastXY(x ,y);
	m_lastWidth  = width;
	m_lastHeight = height;
}

inline void RDOFRMFrame::setLastXY(double x, double y)
{
	m_lastX = x;
	m_lastY = y;
}

inline int RDOFRMFrame::getRuletX(CREF(LPRDORuntime) pRuntime, ruint ruletID) const
{
	LPRDOFRMRulet pRulet = findRulet(ruletID);
	return pRulet ? pRulet->getX()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

inline int RDOFRMFrame::getRuletY(CREF(LPRDORuntime) pRuntime, ruint ruletID) const
{
	LPRDOFRMRulet pRulet = findRulet(ruletID);
	return pRulet ? pRulet->getY()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

inline RDOFRMFrame::LPRDOFRMRulet RDOFRMFrame::findRulet(ruint ruletID) const
{
	RuletList::const_iterator it = m_ruletList.find(ruletID);
	return it != m_ruletList.end() ? it->second : LPRDOFRMRulet(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBoundingItem
// --------------------------------------------------------------------------------
inline RDOFRMBoundingItem::RDOFRMBoundingItem(CREF(RDOFRMFrame::LPRDOFRMPosition) pX, CREF(RDOFRMFrame::LPRDOFRMPosition) pY, CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth, CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight)
	: m_pX     (pX     )
	, m_pY     (pY     )
	, m_pWidth (pWidth )
	, m_pHeight(pHeight)
{}

inline RDOFRMBoundingItem::~RDOFRMBoundingItem()
{}

inline int RDOFRMBoundingItem::getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pX->getX(pRuntime, pFrame);
}

inline int RDOFRMBoundingItem::getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pY->getY(pRuntime, pFrame);
}

inline int RDOFRMBoundingItem::getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pWidth->getWidth(pRuntime, pFrame);
}

inline int RDOFRMBoundingItem::getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pHeight->getHeight(pRuntime, pFrame);
}

inline int RDOFRMBoundingItem::getWidthAsX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pWidth->getX(pRuntime, pFrame);
}

inline int RDOFRMBoundingItem::getHeightAsY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pHeight->getY(pRuntime, pFrame);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMColoredItem
// --------------------------------------------------------------------------------
inline RDOFRMColoredItem::RDOFRMColoredItem(CREF(RDOFRMFrame::LPRDOFRMColor) pBgColor, CREF(RDOFRMFrame::LPRDOFRMColor) pFgColor)
	: m_pBgColor(pBgColor)
	, m_pFgColor(pFgColor)
{}

inline RDOFRMColoredItem::~RDOFRMColoredItem()
{}

inline CREF(RDOFRMFrame::LPRDOFRMColor) RDOFRMColoredItem::getBgColor() const
{
	return m_pBgColor;
}

inline CREF(RDOFRMFrame::LPRDOFRMColor) RDOFRMColoredItem::getFgColor() const
{
	return m_pFgColor;
}

inline rdo::animation::RDOColor RDOFRMColoredItem::getBg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pBgColor->getColor(pRuntime, pFrame);
}

inline rdo::animation::RDOColor RDOFRMColoredItem::getFg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	return m_pFgColor->getColor(pRuntime, pFrame);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMItem
// --------------------------------------------------------------------------------
inline RDOFRMItem::RDOFRMItem(CREF(LPRDOFRMFrame) pFrame)
	: m_pFrame(pFrame)
{}

inline RDOFRMItem::~RDOFRMItem()
{}

inline void RDOFRMItem::getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
{
	UNUSED(list);
}

inline CREF(LPRDOFRMFrame) RDOFRMItem::getFrame() const
{
	return m_pFrame;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmapBase
// --------------------------------------------------------------------------------
inline RDOFRMBitmapBase::RDOFRMBitmapBase(CREF(LPRDOFRMFrame) pFrame, CREF(tstring) pictFilename, CREF(tstring) maskFilename)
	: RDOFRMItem    (pFrame      )
	, m_pictFilename(pictFilename)
	, m_maskFilename(maskFilename)
{}

inline RDOFRMBitmapBase::~RDOFRMBitmapBase()
{}

inline void RDOFRMBitmapBase::getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
{
	list.push_back(m_pictFilename);
	if (!m_maskFilename.empty())
	{
		list.push_back(m_maskFilename);
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMShow
// --------------------------------------------------------------------------------
inline rbool RDOFRMShow::isShowIf() const
{
	return m_pConditionCalc != LPRDOCalc(NULL);
}

CLOSE_RDO_RUNTIME_NAMESPACE
