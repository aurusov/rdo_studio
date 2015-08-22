// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_exception.h"
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
RDOFRMSprite::RDOFRMColor::RDOFRMColor(Type type)
    : m_type(type)
{}

RDOFRMSprite::RDOFRMColor::RDOFRMColor(unsigned char red, unsigned char green, unsigned char blue, const RDOSrcInfo& srcInfo)
    : m_type(Type::RGB)
{
    m_pRedCalc = rdo::Factory<RDOCalcConst>::create((int)red);
    m_pGreenCalc = rdo::Factory<RDOCalcConst>::create((int)green);
    m_pBlueCalc = rdo::Factory<RDOCalcConst>::create((int)blue);
    ASSERT(m_pRedCalc  );
    ASSERT(m_pGreenCalc);
    ASSERT(m_pBlueCalc );
    m_pRedCalc  ->setSrcInfo(srcInfo);
    m_pGreenCalc->setSrcInfo(srcInfo);
    m_pBlueCalc ->setSrcInfo(srcInfo);
}

RDOFRMSprite::RDOFRMColor::RDOFRMColor(const LPRDOCalc& pRedCalc, const LPRDOCalc& pGreenCalc, const LPRDOCalc& pBlueCalc)
    : m_type      (Type::RGB    )
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

rdo::animation::Color RDOFRMSprite::RDOFRMColor::getColor(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    switch (m_type)
    {
    case Type::NONE        : return rdo::animation::Color(50, 200, 50);
    case Type::RGB         : return rdo::animation::Color
                          (
                            (unsigned char)m_pRedCalc  ->calcValue(pRuntime).getUInt(),
                            (unsigned char)m_pGreenCalc->calcValue(pRuntime).getUInt(),
                            (unsigned char)m_pBlueCalc ->calcValue(pRuntime).getUInt()
                          );
    case Type::TRANSPARENT_COLOR : return rdo::animation::Color();
    case Type::LAST_BG     : return pSprite->m_colorLastBg;
    case Type::LAST_FG     : return pSprite->m_colorLastFg;
    case Type::LAST_BG_TEXT: return pSprite->m_colorLastBgText;
    case Type::LAST_FG_TEXT: return pSprite->m_colorLastFgText;
    default             : NEVER_REACH_HERE;
    }
    return rdo::animation::Color();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMSprite(const RDOSrcInfo& srcInfo)
    : RDOSrcInfo  (srcInfo)
    , m_lastX     (0      )
    , m_lastY     (0      )
    , m_lastWidth (0      )
    , m_lastHeight(0      )
{
    m_colorLastBg     = rdo::animation::Color(50, 200, 50);
    m_colorLastFg     = m_colorLastBg;
    m_colorLastBgText = m_colorLastBg;
    m_colorLastFgText = m_colorLastBg;
}

RDOFRMSprite::~RDOFRMSprite()
{}

const std::string& RDOFRMSprite::name() const
{
    return src_text();
}

void RDOFRMSprite::setLastXYWH(double x, double y, double width, double height)
{
    setLastXY(x ,y);
    m_lastWidth  = width;
    m_lastHeight = height;
}

void RDOFRMSprite::setLastXY(double x, double y)
{
    m_lastX = x;
    m_lastY = y;
}

int RDOFRMSprite::getRuletX(const LPRDORuntime& pRuntime, std::size_t ruletID) const
{
    LPRDOFRMRulet pRulet = findRulet(ruletID);
    return pRulet ? pRulet->getX()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

int RDOFRMSprite::getRuletY(const LPRDORuntime& pRuntime, std::size_t ruletID) const
{
    LPRDOFRMRulet pRulet = findRulet(ruletID);
    return pRulet ? pRulet->getY()->getCalc()->calcValue(pRuntime).getInt() : 0;
}

RDOFRMSprite::LPRDOFRMRulet RDOFRMSprite::findRulet(std::size_t ruletID) const
{
    RuletList::const_iterator it = m_ruletList.find(ruletID);
    return it != m_ruletList.end() ? it->second : LPRDOFRMRulet(NULL);
}

RDOValue RDOFRMSprite::prepareFrame(const LPRDORuntime& pRuntime)
{
    return RDOFRMSprite::doCalc(pRuntime);
}

RDOValue RDOFRMSprite::doCalc(const LPRDORuntime& pRuntime)
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

            const std::size_t colorRuint = (*it)->getParam(7).getUInt();
            rdo::animation::Color color(GetBValue(colorRuint), GetGValue(colorRuint), GetRValue(colorRuint));

            rdo::animation::FrameItem* pRect = new rdo::animation::RectElement(
                rdo::animation::BoundedElement(point, size),
                rdo::animation::ColoredElement(color, color)
            );
            pFrame->m_elements.push_back(pRect);
        }
    }
#endif // MODEL_DOROGA_HACK

    return RDOValue();
}

void RDOFRMSprite::getBitmaps(IRDOFRMItemGetBitmap::ImageNameList& list) const
{
    for (const LPIRDOFRMItemGetBitmap& pGetBitmap: m_getBitmapList)
    {
        pGetBitmap->getBitmaps(list);
    }
}

void RDOFRMSprite::setColorLastBG(RDOFRMColor::Type type, const rdo::animation::Color& lastBg)
{
    if (type == RDOFRMColor::Type::RGB)
    {
        m_colorLastBg = lastBg;
    }
}

void RDOFRMSprite::setColorLastFG(RDOFRMColor::Type type, const rdo::animation::Color& lastFg)
{
    if (type == RDOFRMColor::Type::RGB)
    {
        m_colorLastFg = lastFg;
    }
}

void RDOFRMSprite::setColorLastBGText(RDOFRMColor::Type type, const rdo::animation::Color& lastBgText)
{
    if (type == RDOFRMColor::Type::RGB)
    {
        m_colorLastBgText = lastBgText;
    }
}

void RDOFRMSprite::setColorLastFGText(RDOFRMColor::Type type, const rdo::animation::Color& lastFgText)
{
    if (type == RDOFRMColor::Type::RGB)
    {
        m_colorLastFgText = lastFgText;
    }
}

void RDOFRMSprite::insertItem(const LPRDOCalc& pItem)
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

void RDOFRMSprite::setSpriteCalc(const LPRDOCalc& pSpriteCalc)
{
    ASSERT(pSpriteCalc);
    ASSERT(!m_pSpriteCalc);

    LPRDOCalcReturnCatch pCalcReturnCatch = rdo::Factory<RDOCalcReturnCatch>::create();
    ASSERT(pCalcReturnCatch);
    pCalcReturnCatch->setTryCalc(pSpriteCalc);

    m_pSpriteCalc = pCalcReturnCatch;
}

void RDOFRMSprite::insertGetBitmap(const LPIRDOFRMItemGetBitmap& pGetBitmap)
{
    ASSERT(pGetBitmap);
    m_getBitmapList.push_back(pGetBitmap);
}

void RDOFRMSprite::insertRulet(const LPRDOFRMRulet& pRulet)
{
    ASSERT(pRulet);
    std::pair<RuletList::const_iterator, bool> result =
        m_ruletList.insert(RuletList::value_type(pRulet->getIndex(), pRulet));
    ASSERT(result.second);
    (void)result;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMPosition
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMPosition::RDOFRMPosition()
    : RDORuntimeObject()
    , m_type   (Type::ABSOLUTE_POSITION)
    , m_ruletID(0          )
{}

RDOFRMSprite::RDOFRMPosition::RDOFRMPosition(const LPRDOCalc& pCalc, Type type, int ruletID)
    : RDORuntimeObject()
    , m_pCalc  (pCalc  )
    , m_type   (type   )
    , m_ruletID(ruletID)
{}

RDOFRMSprite::RDOFRMPosition::~RDOFRMPosition()
{}

RDOFRMSprite::RDOFRMPosition::Type RDOFRMSprite::RDOFRMPosition::getType() const
{
    return m_type;
}

const LPRDOCalc& RDOFRMSprite::RDOFRMPosition::getCalc() const
{
    return m_pCalc;
}

int RDOFRMSprite::RDOFRMPosition::getX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
    RDOValue res = m_pCalc->calcValue(pRuntime);
    switch (m_type)
    {
    case RDOFRMPosition::Type::DELTA  : res += pSprite->m_lastX;                        break;
    case RDOFRMPosition::Type::GABARIT: res += pSprite->m_lastX + pSprite->m_lastWidth; break;
    case RDOFRMPosition::Type::MULT   : res *= pSprite->m_lastX;                        break;
    case RDOFRMPosition::Type::RULET  : res += pSprite->getRuletX(pRuntime, m_ruletID); break;
    default                        : break;
    }
    return res.getInt();
}

int RDOFRMSprite::RDOFRMPosition::getY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
    RDOValue res = m_pCalc->calcValue(pRuntime);
    switch (m_type)
    {
    case RDOFRMPosition::Type::DELTA  : res += pSprite->m_lastY;                         break;
    case RDOFRMPosition::Type::GABARIT: res += pSprite->m_lastY + pSprite->m_lastHeight; break;
    case RDOFRMPosition::Type::MULT   : res *= pSprite->m_lastY;                         break;
    case RDOFRMPosition::Type::RULET  : res += pSprite->getRuletY(pRuntime, m_ruletID);  break;
    default                        : break;
    }
    return res.getInt();
}

int RDOFRMSprite::RDOFRMPosition::getWidth(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
    RDOValue res = m_pCalc->calcValue(pRuntime);
    switch (m_type)
    {
    case RDOFRMPosition::Type::DELTA: res += pSprite->m_lastWidth; break;
    case RDOFRMPosition::Type::MULT : res *= pSprite->m_lastWidth; break;
    case RDOFRMPosition::Type::RULET: res += pSprite->getRuletX(pRuntime, m_ruletID); break;
    default                      : break;
    }
    return res.getInt();
}

int RDOFRMSprite::RDOFRMPosition::getHeight(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite)
{
    RDOValue res = m_pCalc->calcValue(pRuntime);
    switch (m_type)
    {
    case RDOFRMPosition::Type::DELTA: res += pSprite->m_lastHeight; break;
    case RDOFRMPosition::Type::MULT : res *= pSprite->m_lastHeight; break;
    case RDOFRMPosition::Type::RULET: res += pSprite->getRuletY(pRuntime, m_ruletID); break;
    default                      : break;
    }
    return res.getInt();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMColor
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMColor::Type RDOFRMSprite::RDOFRMColor::getType() const
{
    return m_type;
}

void RDOFRMSprite::RDOFRMColor::setType(Type type)
{
    if (m_type == Type::NONE)
    {
        m_type = type;
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite::RDOFRMRulet
// --------------------------------------------------------------------------------
std::size_t RDOFRMSprite::RDOFRMRulet::getIndex() const
{
    return m_index;
}

const RDOFRMSprite::LPRDOFRMPosition& RDOFRMSprite::RDOFRMRulet::getX() const
{
    return m_pX;
}

const RDOFRMSprite::LPRDOFRMPosition& RDOFRMSprite::RDOFRMRulet::getY() const
{
    return m_pY;
}

RDOFRMSprite::RDOFRMRulet::RDOFRMRulet(const RDOSrcInfo& src_info, std::size_t index, const LPRDOFRMPosition& pX, const LPRDOFRMPosition& pY)
    : m_index(index)
    , m_pX   (pX   )
    , m_pY   (pY   )
{
    setSrcInfo(src_info);
}

RDOFRMSprite::RDOFRMRulet::~RDOFRMRulet()
{}

RDOValue RDOFRMSprite::RDOFRMRulet::doCalc(const LPRDORuntime& /*pRuntime*/)
{
    return RDOValue();
}

const RDOSrcInfo& RDOFRMSprite::RDOFRMRulet::src_info() const
{
    return srcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBoundingItem
// --------------------------------------------------------------------------------
RDOFRMBoundingItem::RDOFRMBoundingItem(const RDOFRMSprite::LPRDOFRMPosition& pX, const RDOFRMSprite::LPRDOFRMPosition& pY, const RDOFRMSprite::LPRDOFRMPosition& pWidth, const RDOFRMSprite::LPRDOFRMPosition& pHeight)
    : m_pX     (pX     )
    , m_pY     (pY     )
    , m_pWidth (pWidth )
    , m_pHeight(pHeight)
{}

RDOFRMBoundingItem::~RDOFRMBoundingItem()
{}

int RDOFRMBoundingItem::getX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pX->getX(pRuntime, pSprite);
}

int RDOFRMBoundingItem::getY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pY->getY(pRuntime, pSprite);
}

int RDOFRMBoundingItem::getWidth(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pWidth->getWidth(pRuntime, pSprite);
}

int RDOFRMBoundingItem::getHeight(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pHeight->getHeight(pRuntime, pSprite);
}

int RDOFRMBoundingItem::getWidthAsX(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pWidth->getX(pRuntime, pSprite);
}

int RDOFRMBoundingItem::getHeightAsY(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pHeight->getY(pRuntime, pSprite);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMColoredItem
// --------------------------------------------------------------------------------
RDOFRMColoredItem::RDOFRMColoredItem(const RDOFRMSprite::LPRDOFRMColor& pBgColor, const RDOFRMSprite::LPRDOFRMColor& pFgColor)
    : m_pBgColor(pBgColor)
    , m_pFgColor(pFgColor)
{}

RDOFRMColoredItem::~RDOFRMColoredItem()
{}

const RDOFRMSprite::LPRDOFRMColor& RDOFRMColoredItem::getBgColor() const
{
    return m_pBgColor;
}

const RDOFRMSprite::LPRDOFRMColor& RDOFRMColoredItem::getFgColor() const
{
    return m_pFgColor;
}

rdo::animation::Color RDOFRMColoredItem::getBg(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pBgColor->getColor(pRuntime, pSprite);
}

rdo::animation::Color RDOFRMColoredItem::getFg(const LPRDORuntime& pRuntime, const LPRDOFRMSprite& pSprite) const
{
    return m_pFgColor->getColor(pRuntime, pSprite);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMItem
// --------------------------------------------------------------------------------
RDOFRMItem::RDOFRMItem(const LPRDOFRMSprite& pSprite)
    : m_pFrame(pSprite)
{}

RDOFRMItem::~RDOFRMItem()
{}

const LPRDOFRMSprite& RDOFRMItem::getFrame() const
{
    return m_pFrame;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMText
// --------------------------------------------------------------------------------
RDOFRMText::RDOFRMText(
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
    )
    : RDOFRMItem        (pSprite)
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
    , RDOFRMColoredItem (pBgColor, pFgColor)
    , m_align           (rdo::animation::TextElement::Align::LEFT)
    , m_isTextString    (true)
{}

RDOFRMText::~RDOFRMText()
{}

void RDOFRMText::setText(Align align, const LPRDOCalc& pValue)
{
    m_align        = align;
    m_pValue       = pValue;
    m_isTextString = false;
}

void RDOFRMText::setText(Align align, const std::string& text)
{
    m_align        = align;
    m_text         = text;
    m_isTextString = true;
}

RDOValue RDOFRMText::doCalc(const LPRDORuntime& pRuntime)
{
    pRuntime->memory_insert(sizeof(rdo::animation::TextElement));

    rdo::animation::Color bg = getBg(pRuntime, getFrame());
    rdo::animation::Color fg = getFg(pRuntime, getFrame());
    getFrame()->setColorLastBGText(getBgColor()->getType(), bg);
    getFrame()->setColorLastFGText(getFgColor()->getType(), fg);

    std::string t;
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
        catch (const RDOUndefinedException&)
        {
            t = "#";
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
// -------------------- RDOFRMBitmapBase
// --------------------------------------------------------------------------------
RDOFRMBitmapBase::RDOFRMBitmapBase(const LPRDOFRMSprite& pSprite, const std::string& pictFilename, const std::string& maskFilename)
    : RDOFRMItem    (pSprite     )
    , m_pictFilename(pictFilename)
    , m_maskFilename(maskFilename)
{}

RDOFRMBitmapBase::~RDOFRMBitmapBase()
{}

void RDOFRMBitmapBase::getBitmaps(IRDOFRMItemGetBitmap::ImageNameList& list) const
{
    list.push_back(m_pictFilename);
    if (!m_maskFilename.empty())
    {
        list.push_back(m_maskFilename);
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMBitmap
// --------------------------------------------------------------------------------
RDOFRMBitmap::RDOFRMBitmap(
        const LPRDOFRMSprite& pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const std::string& pictFilename,
        const std::string& maskFilename
    )
    : RDOFRMBitmapBase(pSprite, pictFilename, maskFilename)
    , m_pX            (pX)
    , m_pY            (pY)
{}

RDOFRMBitmap::~RDOFRMBitmap()
{}

RDOValue RDOFRMBitmap::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite& pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const std::string& pictFilename,
        const std::string& maskFilename
    )
    : RDOFRMBitmapBase  (pSprite, pictFilename, maskFilename)
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
{}

RDOFRMBitmapStretch::~RDOFRMBitmapStretch()
{}

RDOValue RDOFRMBitmapStretch::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
    )
    : RDOFRMItem        (pSprite)
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
    , RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRect::~RDOFRMRect()
{}

RDOValue RDOFRMRect::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
    )
    : RDOFRMItem        (pSprite)
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
    , RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMRectRound::~RDOFRMRectRound()
{}

RDOValue RDOFRMRectRound::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pRadius,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
    )
    : RDOFRMItem       (pSprite)
    , RDOFRMColoredItem(pBgColor, pFgColor)
    , m_pX             (pX     )
    , m_pY             (pY     )
    , m_pRadius        (pRadius)
{}

RDOFRMCircle::~RDOFRMCircle()
{}

RDOValue RDOFRMCircle::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
    )
    : RDOFRMItem        (pSprite)
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
    , RDOFRMColoredItem (pBgColor, pFgColor     )
{}

RDOFRMEllipse::~RDOFRMEllipse()
{}

RDOValue RDOFRMEllipse::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX1,
        const RDOFRMSprite::LPRDOFRMPosition& pY1,
        const RDOFRMSprite::LPRDOFRMPosition& pX2,
        const RDOFRMSprite::LPRDOFRMPosition& pY2,
        const RDOFRMSprite::LPRDOFRMColor&    pColor
    )
    : RDOFRMItem        (pSprite           )
    , RDOFRMBoundingItem(pX1, pY1, pX2, pY2)
    , m_pColor          (pColor            )
{}

RDOFRMLine::~RDOFRMLine()
{}

RDOValue RDOFRMLine::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX1,
        const RDOFRMSprite::LPRDOFRMPosition& pY1,
        const RDOFRMSprite::LPRDOFRMPosition& pX2,
        const RDOFRMSprite::LPRDOFRMPosition& pY2,
        const RDOFRMSprite::LPRDOFRMPosition& pX3,
        const RDOFRMSprite::LPRDOFRMPosition& pY3,
        const RDOFRMSprite::LPRDOFRMColor&    pBgColor,
        const RDOFRMSprite::LPRDOFRMColor&    pFgColor
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

RDOValue RDOFRMTriang::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite& pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight,
        const std::string& operName
    )
    : RDOFRMItem        (pSprite                )
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
    , m_operName        (operName               )
{}

RDOFRMActive::~RDOFRMActive()
{}

RDOValue RDOFRMActive::doCalc(const LPRDORuntime& pRuntime)
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
        const LPRDOFRMSprite&                 pSprite,
        const RDOFRMSprite::LPRDOFRMPosition& pX,
        const RDOFRMSprite::LPRDOFRMPosition& pY,
        const RDOFRMSprite::LPRDOFRMPosition& pWidth,
        const RDOFRMSprite::LPRDOFRMPosition& pHeight
    )
    : RDOFRMItem        (pSprite                )
    , RDOFRMBoundingItem(pX, pY, pWidth, pHeight)
{}

RDOFRMSpace::~RDOFRMSpace()
{}

RDOValue RDOFRMSpace::doCalc(const LPRDORuntime& pRuntime)
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
RDOFRMFrame::RDOFRMFrame(const RDOSrcInfo& srcInfo)
    : RDOFRMSprite (srcInfo)
    , m_picFileName("" )
    , m_width      (800    )
    , m_height     (600    )
    {}

RDOFRMFrame::~RDOFRMFrame()
{}

RDOValue RDOFRMFrame::doCalc(const LPRDORuntime& pRuntime)
{
    rdo::animation::Frame* pFrame = pRuntime->getPreparingFrame();
    ASSERT(pFrame);

    if (m_pBgColor)
    {
        if (m_pBgColor->getType() == RDOFRMColor::Type::RGB)
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

void RDOFRMFrame::setBackgroundColor(const LPRDOFRMColor& pBgColor)
{
    m_pBgColor = pBgColor;
}

void RDOFRMFrame::setBackPicture(const std::string& picFileName)
{
    m_picFileName = picFileName;
}

void RDOFRMFrame::setBackPicture(int width, int height)
{
    m_picFileName = "";
    m_width       = width;
    m_height      = height;
}

void RDOFRMFrame::prepareFrame(rdo::animation::Frame* pFrame, const LPRDORuntime& pRuntime)
{
    ASSERT(pFrame);

    pFrame->m_bgImageName   = m_picFileName;
    pFrame->m_size.m_width  = m_width;
    pFrame->m_size.m_height = m_height;

    pRuntime->setPreparingFrame(pFrame);
    calcValue(pRuntime);
    pRuntime->resetPreparingFrame();
}

void RDOFRMFrame::getBitmaps(IRDOFRMItemGetBitmap::ImageNameList& list) const
{
    if (!m_picFileName.empty())
        list.push_back(m_picFileName);

    RDOFRMSprite::getBitmaps(list);
}

CLOSE_RDO_RUNTIME_NAMESPACE
