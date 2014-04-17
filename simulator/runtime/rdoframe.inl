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
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMPosition
// --------------------------------------------------------------------------------
inline RDOFRMSprite::RDOFRMPosition::RDOFRMPosition()
	: RDORuntimeObject()
	, m_type   (PT_ABSOLUTE)
	, m_ruletID(0          )
{}

inline RDOFRMSprite::RDOFRMPosition::RDOFRMPosition(const LPRDOCalc& pCalc, PositionType type, int ruletID)
	: RDORuntimeObject()
	, m_pCalc  (pCalc  )
	, m_type   (type   )
	, m_ruletID(ruletID)
{}

inline RDOFRMSprite::RDOFRMPosition::~RDOFRMPosition()
{}

inline RDOFRMSprite::RDOFRMPosition::PositionType RDOFRMSprite::RDOFRMPosition::getType() const
{
	return m_type;
}

inline const LPRDOCalc& RDOFRMSprite::RDOFRMPosition::getCalc() const
{
	return m_pCalc;
}

inline int RDOFRMSprite::RDOFRMPosition::getX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA  : res += pSprite->m_lastX;                        break;
	case RDOFRMPosition::PT_GABARIT: res += pSprite->m_lastX + pSprite->m_lastWidth; break;
	case RDOFRMPosition::PT_MULT   : res *= pSprite->m_lastX;                        break;
	case RDOFRMPosition::PT_RULET  : res += pSprite->getRuletX(pRuntime, m_ruletID); break;
	default                        : break;
	}
	return res.getInt();
}

inline int RDOFRMSprite::RDOFRMPosition::getY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA  : res += pSprite->m_lastY;                         break;
	case RDOFRMPosition::PT_GABARIT: res += pSprite->m_lastY + pSprite->m_lastHeight; break;
	case RDOFRMPosition::PT_MULT   : res *= pSprite->m_lastY;                         break;
	case RDOFRMPosition::PT_RULET  : res += pSprite->getRuletY(pRuntime, m_ruletID);  break;
	default                        : break;
	}
	return res.getInt();
}

inline int RDOFRMSprite::RDOFRMPosition::getWidth(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA: res += pSprite->m_lastWidth; break;
	case RDOFRMPosition::PT_MULT : res *= pSprite->m_lastWidth; break;
	case RDOFRMPosition::PT_RULET: res += pSprite->getRuletX(pRuntime, m_ruletID); break;
	default                      : break;
	}
	return res.getInt();
}

inline int RDOFRMSprite::RDOFRMPosition::getHeight(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
	RDOValue res = m_pCalc->calcValue(pRuntime);
	switch (m_type)
	{
	case RDOFRMPosition::PT_DELTA: res += pSprite->m_lastHeight; break;
	case RDOFRMPosition::PT_MULT : res *= pSprite->m_lastHeight; break;
	case RDOFRMPosition::PT_RULET: res += pSprite->getRuletY(pRuntime, m_ruletID); break;
	default                      : break;
	}
	return res.getInt();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMColor
// --------------------------------------------------------------------------------
inline RDOFRMSprite::RDOFRMColor::ColorType RDOFRMSprite::RDOFRMColor::getType() const
{
	return m_type;
}

inline void RDOFRMSprite::RDOFRMColor::setType(ColorType type)
{
	if (m_type == CT_NONE)
	{
		m_type = type;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMRulet
// --------------------------------------------------------------------------------
inline std::size_t RDOFRMSprite::RDOFRMRulet::getIndex() const
{
	return m_index;
}

inline const RDOFRMSprite::LPRDOFRMPosition& RDOFRMSprite::RDOFRMRulet::getX() const
{
	return m_pX;
}

inline const RDOFRMSprite::LPRDOFRMPosition& RDOFRMSprite::RDOFRMRulet::getY() const
{
	return m_pY;
}

inline RDOFRMSprite::RDOFRMRulet::RDOFRMRulet(const RDOSrcInfo& src_info, std::size_t index, const LPRDOFRMPosition& pX, const LPRDOFRMPosition& pY)
	: m_index(index)
	, m_pX   (pX   )
	, m_pY   (pY   )
{
	setSrcInfo(src_info);
}

inline RDOFRMSprite::RDOFRMRulet::~RDOFRMRulet()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
inline const std::string& RDOFRMSprite::name() const
{
	return src_text();
}

inline void RDOFRMSprite::setLastXYWH(double x, double y, double width, double height)
{
	setLastXY(x ,y);
	m_lastWidth  = width;
	m_lastHeight = height;
}

inline void RDOFRMSprite::setLastXY(double x, double y)
{
	m_lastX = x;
	m_lastY = y;
}

inline int RDOFRMSprite::getRuletX(const LPRDORuntime& pRuntime, std::size_t ruletID) const
{
	LPRDOFRMRulet pRulet = findRulet(ruletID);
	return pRulet ? pRulet->getX()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

inline int RDOFRMSprite::getRuletY(const LPRDORuntime& pRuntime, std::size_t ruletID) const
{
	LPRDOFRMRulet pRulet = findRulet(ruletID);
	return pRulet ? pRulet->getY()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

inline RDOFRMSprite::LPRDOFRMRulet RDOFRMSprite::findRulet(std::size_t ruletID) const
{
	RuletList::const_iterator it = m_ruletList.find(ruletID);
	return it != m_ruletList.end() ? it->second : LPRDOFRMRulet(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBoundingItem
// --------------------------------------------------------------------------------
inline RDOFRMBoundingItem::RDOFRMBoundingItem(const RDOFRMSprite::LPRDOFRMPosition& pX, const RDOFRMSprite::LPRDOFRMPosition& pY, const RDOFRMSprite::LPRDOFRMPosition& pWidth, const RDOFRMSprite::LPRDOFRMPosition& pHeight)
	: m_pX     (pX     )
	, m_pY     (pY     )
	, m_pWidth (pWidth )
	, m_pHeight(pHeight)
{}

inline RDOFRMBoundingItem::~RDOFRMBoundingItem()
{}

inline int RDOFRMBoundingItem::getX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pX->getX(pRuntime, pSprite);
}

inline int RDOFRMBoundingItem::getY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pY->getY(pRuntime, pSprite);
}

inline int RDOFRMBoundingItem::getWidth(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pWidth->getWidth(pRuntime, pSprite);
}

inline int RDOFRMBoundingItem::getHeight(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pHeight->getHeight(pRuntime, pSprite);
}

inline int RDOFRMBoundingItem::getWidthAsX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pWidth->getX(pRuntime, pSprite);
}

inline int RDOFRMBoundingItem::getHeightAsY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pHeight->getY(pRuntime, pSprite);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMColoredItem
// --------------------------------------------------------------------------------
inline RDOFRMColoredItem::RDOFRMColoredItem(const RDOFRMSprite::LPRDOFRMColor& pBgColor, const RDOFRMSprite::LPRDOFRMColor& pFgColor)
	: m_pBgColor(pBgColor)
	, m_pFgColor(pFgColor)
{}

inline RDOFRMColoredItem::~RDOFRMColoredItem()
{}

inline const RDOFRMSprite::LPRDOFRMColor& RDOFRMColoredItem::getBgColor() const
{
	return m_pBgColor;
}

inline const RDOFRMSprite::LPRDOFRMColor& RDOFRMColoredItem::getFgColor() const
{
	return m_pFgColor;
}

inline rdo::animation::Color RDOFRMColoredItem::getBg(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pBgColor->getColor(pRuntime, pSprite);
}

inline rdo::animation::Color RDOFRMColoredItem::getFg(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
	return m_pFgColor->getColor(pRuntime, pSprite);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMItem
// --------------------------------------------------------------------------------
inline RDOFRMItem::RDOFRMItem(const LPRDOFRMSprite& pSprite)
	: m_pFrame(pSprite)
{}

inline RDOFRMItem::~RDOFRMItem()
{}

inline const LPRDOFRMSprite& RDOFRMItem::getFrame() const
{
	return m_pFrame;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmapBase
// --------------------------------------------------------------------------------
inline RDOFRMBitmapBase::RDOFRMBitmapBase(const LPRDOFRMSprite& pSprite, const std::string& pictFilename, const std::string& maskFilename)
	: RDOFRMItem    (pSprite     )
	, m_pictFilename(pictFilename)
	, m_maskFilename(maskFilename)
{}

inline RDOFRMBitmapBase::~RDOFRMBitmapBase()
{}

inline void RDOFRMBitmapBase::getBitmaps(IRDOFRMItemGetBitmap::ImageNameList& list) const
{
	list.push_back(m_pictFilename);
	if (!m_maskFilename.empty())
	{
		list.push_back(m_maskFilename);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
