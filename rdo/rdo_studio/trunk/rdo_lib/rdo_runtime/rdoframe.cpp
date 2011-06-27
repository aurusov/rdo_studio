#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"

namespace rdoRuntime 
{

// ----------------------------------------------------------------------------
// ---------- RDOFRMColor - объект-цвет
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMColor::RDOFRMColor(ColorType _type )
	: color_type( _type )
{}

RDOFRMFrame::RDOFRMColor::RDOFRMColor(int _red, int _green, int _blue )
	: color_type( color_rgb )
{
	m_pRedCalc   = rdo::Factory<RDOCalcConst>::create(_red  );
	m_pGreenCalc = rdo::Factory<RDOCalcConst>::create(_green);
	m_pBlueCalc  = rdo::Factory<RDOCalcConst>::create(_blue );
	m_pRedCalc->setSrcText( rdo::format("%d, _red") );
	m_pGreenCalc->setSrcText( rdo::format("%d, _green") );
	m_pBlueCalc->setSrcText( rdo::format("%d, _blue") );
}

RDOFRMFrame::RDOFRMColor::RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc)
	: color_type  (color_rgb )
	, m_pRedCalc  (pRedCalc  )
	, m_pGreenCalc(pGreenCalc)
	, m_pBlueCalc (pBlueCalc )
{}

RDOFRMFrame::RDOFRMColor::~RDOFRMColor()
{}

rdoAnimation::RDOColor RDOFRMFrame::RDOFRMColor::getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
{
	switch (color_type)
	{
		case color_none        : return rdoAnimation::RDOColor(50, 200, 50);
		case color_rgb         : return rdoAnimation::RDOColor(
									 const_cast<PTR(RDOFRMFrame::RDOFRMColor)>(this)->m_pRedCalc->calcValue(pRuntime).getInt(),
									 const_cast<PTR(RDOFRMFrame::RDOFRMColor)>(this)->m_pGreenCalc->calcValue(pRuntime).getInt(),
									 const_cast<PTR(RDOFRMFrame::RDOFRMColor)>(this)->m_pBlueCalc->calcValue(pRuntime).getInt());
		case color_transparent : return rdoAnimation::RDOColor();
		case color_last_bg     : return pFrame->color_last_bg;
		case color_last_fg     : return pFrame->color_last_fg;
		case color_last_bg_text: return pFrame->color_last_bg_text;
		case color_last_fg_text: return pFrame->color_last_fg_text;
		default                : NEVER_REACH_HERE;
	}
	return rdoAnimation::RDOColor();
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOSrcInfo) _src_info, CREF(LPRDOCalc) _pConditionCalc)
	: RDOSrcInfo( _src_info )
	, pConditionCalc( _pConditionCalc )
	, background_color( NULL )
	, picFileName(_T(""))
	, width( 800 )
	, height( 600 )
	, last_x( 0 )
	, last_y( 0 )
	, last_width( 0 )
	, last_height( 0 )
{
	color_last_bg      = rdoAnimation::RDOColor(50, 200, 50);
	color_last_fg      = color_last_bg;
	color_last_bg_text = color_last_bg;
	color_last_fg_text = color_last_bg;
}

RDOFRMFrame::~RDOFRMFrame()
{
	rulets.clear();
}

void RDOFRMFrame::setColorLastBG( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_bg )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_bg = _last_bg;
	}
}

void RDOFRMFrame::setColorLastFG( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_fg )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_fg = _last_fg;
	}
}

void RDOFRMFrame::setColorLastBGText( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_bg_text )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_bg_text = _last_bg_text;
	}
}

void RDOFRMFrame::setColorLastFGText( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_fg_text )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_fg_text = _last_fg_text;
	}
}

void RDOFRMFrame::setBackgroundColor(CREF(LPRDOFRMColor) _background_color )
{
	background_color = _background_color;
}

void RDOFRMFrame::setBackPicture( CREF(tstring) _picFileName )
{
	picFileName = _picFileName;
}

void RDOFRMFrame::setBackPicture( int _width, int _height )
{
	picFileName = _T("");
	width = _width;
	height = _height;
}

void RDOFRMFrame::startShow(CREF(LPRDOCalc) pCalc)
{
	shows.push_back(rdo::Factory<RDOFRMShow>::create(pCalc));
}

void RDOFRMFrame::addItem(CREF(LPRDOFRMItem) pItem)
{
	ASSERT(pItem);

	if (shows.empty())
	{
		startShow();
	}

	shows.back()->insertItem(pItem);
}

void RDOFRMFrame::addRulet(CREF(LPRDOFRMRulet) pRulet)
{
	ASSERT(pRulet);
	rulets.push_back(pRulet);
}

bool RDOFRMFrame::checkCondition(CREF(LPRDORuntime) pRuntime)
{
	if ( !pConditionCalc ) return true;
	return pConditionCalc->calcValue( pRuntime ).getAsBool();
}

rdoAnimation::RDOFrame* RDOFRMFrame::prepareFrame( rdoAnimation::RDOFrame* frame, CREF(LPRDORuntime) pRuntime )
{
	if (background_color)
	{
		if (background_color->getColorType() == RDOFRMColor::color_rgb)
		{
			rdoAnimation::RDOColor bg_color = background_color->getColor(pRuntime, this);
			frame->m_bgColor = bg_color;
		}
		else
		{
			frame->m_bgColor = rdoAnimation::RDOColor();
		}
	}
	else
	{
		frame->m_bgColor = rdoAnimation::RDOColor();
	}
	frame->m_bgImageName   = picFileName;
	frame->m_size.m_width  = width;
	frame->m_size.m_height = height;

	last_x      = 0;
	last_y      = 0;
	last_width  = 0;
	last_height = 0;

	if ( checkCondition( pRuntime ) )
	{
		STL_FOR_ALL(shows, it_show)
		{
			if ((*it_show)->checkCondition(pRuntime))
			{
				STL_FOR_ALL((*it_show)->getItemList(), it_obj)
				{
					PTR(rdoAnimation::FrameItem) element = (*it_obj)->createElement(pRuntime);
					if ( element )
					{
						frame->m_elements.push_back( element );
					}
				}
			}
		}
	}

	return frame;
}

void RDOFRMFrame::getBitmaps( std::list< tstring >& list ) const
{
	if (!picFileName.empty())
		list.push_back(picFileName);

	STL_FOR_ALL_CONST(shows, it)
	{
		(*it)->getBitmaps(list);
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
RDOFRMText::RDOFRMText(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	)
	: RDOFRMItem        (pFrame                                 )
	, RDOFRMBoundingItem(pX, pY, pWidth, pHeight                )
	, RDOFRMColoredItem (pBgColor, pFgColor                     )
	, m_align           (rdoAnimation::RDOTextElement::TETA_LEFT)
	, m_isTextString    (true                                   )
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

PTR(rdoAnimation::FrameItem) RDOFRMText::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOTextElement));

	rdoAnimation::RDOColor bg = getBg(pRuntime, getFrame());
	rdoAnimation::RDOColor fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBGText(getBgColor()->getColorType(), bg);
	getFrame()->setColorLastFGText(getFgColor()->getColorType(), fg);

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

	return new rdoAnimation::RDOTextElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(x, y), rdoAnimation::RDOSize(width, height)),
		rdoAnimation::RDOColoredElement(bg, fg),
		t, m_align
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
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

PTR(rdoAnimation::FrameItem) RDOFRMBitmap::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOBmpElement));

	int x = m_pX->getX(pRuntime, getFrame());
	int y = m_pY->getY(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, 0, 0);

	return new rdoAnimation::RDOBmpElement(
		rdoAnimation::RDOPoint(x, y),
		m_pictFilename, m_maskFilename
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
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

PTR(rdoAnimation::FrameItem) RDOFRMBitmapStretch::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOSBmpElement));

	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdoAnimation::RDOSBmpElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(x, y), rdoAnimation::RDOSize(width, height)),
		m_pictFilename, m_maskFilename
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
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

PTR(rdoAnimation::FrameItem) RDOFRMRect::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDORectElement));

	rdoAnimation::RDOColor bg = getBg(pRuntime, getFrame());
	rdoAnimation::RDOColor fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getColorType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getColorType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdoAnimation::RDORectElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(x, y), rdoAnimation::RDOSize(width, height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
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

PTR(rdoAnimation::FrameItem) RDOFRMRectRound::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDORRectElement));

	rdoAnimation::RDOColor bg = getBg(pRuntime, getFrame());
	rdoAnimation::RDOColor fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getColorType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getColorType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdoAnimation::RDORRectElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(x, y), rdoAnimation::RDOSize(width, height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
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

PTR(rdoAnimation::FrameItem) RDOFRMEllipse::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOEllipseElement));

	rdoAnimation::RDOColor bg = getBg(pRuntime, getFrame());
	rdoAnimation::RDOColor fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getColorType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getColorType(), fg);
	int x      = getX     (pRuntime, getFrame());
	int y      = getY     (pRuntime, getFrame());
	int width  = getWidth (pRuntime, getFrame());
	int height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(x, y, width, height);

	return new rdoAnimation::RDOEllipseElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(x, y), rdoAnimation::RDOSize(width, height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
RDOFRMLine::RDOFRMLine(CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x1, CREF(RDOFRMFrame::LPRDOFRMPosition) _y1, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMColor) _color):
	RDOFRMItem(pFrame),
	RDOFRMBoundingItem(_x1, _y1, _x2, _y2),
	color(_color)
{}

PTR(rdoAnimation::FrameItem) RDOFRMLine::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOLineElement));

	rdoAnimation::RDOColor fg = color->getColor(pRuntime, getFrame());
	getFrame()->setColorLastFG(color->getColorType(), fg);
	int _x1 = getX(pRuntime, getFrame());
	int _y1 = getY(pRuntime, getFrame());
	int _x2 = getWidthAsX(pRuntime, getFrame());
	int _y2 = getHeightAsY(pRuntime, getFrame());
	getFrame()->setLastXYWH(_x1, _y1, _x2 - _x1, _y2 - _y1);

	return new rdoAnimation::RDOLineElement(
		rdoAnimation::RDOPoint(_x1, _y1),
		rdoAnimation::RDOPoint(_x2, _y2),
		fg
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
RDOFRMTriang::RDOFRMTriang(CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x1, CREF(RDOFRMFrame::LPRDOFRMPosition) _y1, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMPosition) _x3, CREF(RDOFRMFrame::LPRDOFRMPosition) _y3, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor):
	RDOFRMItem(pFrame),
	RDOFRMColoredItem(bgColor, fgColor),
	x1(_x1),
	y1(_y1),
	x2(_x2),
	y2(_y2),
	x3(_x3),
	y3(_y3)
{}

PTR(rdoAnimation::FrameItem) RDOFRMTriang::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOTriangElement));

	rdoAnimation::RDOColor bg = getBg(pRuntime, getFrame());
	rdoAnimation::RDOColor fg = getFg(pRuntime, getFrame());
	getFrame()->setColorLastBG(getBgColor()->getColorType(), bg);
	getFrame()->setColorLastFG(getFgColor()->getColorType(), fg);
	int _x1 = x1->getX(pRuntime, getFrame());
	int _y1 = y1->getY(pRuntime, getFrame());
	int min_x = _x1;
	int max_x = _x1;
	int min_y = _y1;
	int max_y = _y1;
	getFrame()->setLastXYWH(_x1, _y1, _x1, _y1);
	int _x2 = x2->getX(pRuntime, getFrame());
	int _y2 = y2->getY(pRuntime, getFrame());
	if (min_x > _x2) min_x = _x2;
	if (max_x < _x2) max_x = _x2;
	if (min_y > _y2) min_y = _y2;
	if (max_y < _x2) max_y = _y2;
	getFrame()->setLastXYWH(_x2, _y2, _x2 - _x1, _y2 - _y1);
	int _x3 = x3->getX(pRuntime, getFrame());
	int _y3 = y3->getY(pRuntime, getFrame());
	if (min_x > _x3) min_x = _x3;
	if (max_x < _x3) max_x = _x3;
	if (min_y > _y3) min_y = _y3;
	if (max_y < _x3) max_y = _y3;
	getFrame()->setLastXYWH(min_x, min_y, max_x - min_x, max_y - min_y);

	return new rdoAnimation::RDOTriangElement(
		rdoAnimation::RDOPoint(_x1, _y1),
		rdoAnimation::RDOPoint(_x2, _y2),
		rdoAnimation::RDOPoint(_x3, _y3),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
RDOFRMActive::RDOFRMActive(CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(tstring) _operName):
	RDOFRMItem(pFrame),
	RDOFRMBoundingItem(_x, _y, _width, _height),
	operName(_operName)
{
}

PTR(rdoAnimation::FrameItem) RDOFRMActive::createElement(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->memory_insert(sizeof(rdoAnimation::RDOActiveElement));

	int _x      = getX(pRuntime, getFrame());
	int _y      = getY(pRuntime, getFrame());
	int _width  = getWidth(pRuntime, getFrame());
	int _height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(_x, _y, _width, _height);

	return new rdoAnimation::RDOActiveElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		operName
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
RDOFRMSpace::RDOFRMSpace(CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height):
	RDOFRMItem(pFrame),
	RDOFRMBoundingItem(_x, _y, _width, _height)
{
}

PTR(rdoAnimation::FrameItem) RDOFRMSpace::createElement(CREF(LPRDORuntime) pRuntime)
{
	int _x      = getX(pRuntime, getFrame());
	int _y      = getY(pRuntime, getFrame());
	int _width  = getWidth(pRuntime, getFrame());
	int _height = getHeight(pRuntime, getFrame());
	getFrame()->setLastXYWH(_x, _y, _width, _height);
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
RDOFRMShow::RDOFRMShow(CREF(LPRDOCalc) _pConditionCalc)
	: pConditionCalc(_pConditionCalc)
{}

RDOFRMShow::~RDOFRMShow()
{
}

void RDOFRMShow::insertItem(CREF(LPRDOFRMItem) pItem)
{
	ASSERT(pItem);
	m_itemList.push_back(pItem);
}

REF(RDOFRMShow::ItemList) RDOFRMShow::getItemList()
{
	return m_itemList;
}

void RDOFRMShow::getBitmaps(REF(std::list<tstring>) list)
{
	STL_FOR_ALL(m_itemList, it)
	{
		(*it)->getBitmaps(list);
	}
}

bool RDOFRMShow::checkCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!pConditionCalc) return true;
	return pConditionCalc->calcValue(pRuntime).getAsBool();
}

} // namespace rdoRuntime
