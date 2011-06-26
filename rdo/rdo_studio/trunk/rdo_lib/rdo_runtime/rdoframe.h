/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoframe.h
 * author   : Урусов Андрей
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOFRAME_H_
#define _RDOFRAME_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdoanimation.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFRMShow);
PREDECLARE_POINTER(RDOFRMItem);

OBJECT(RDOFRMFrame)
	IS  INSTANCE_OF(RDORuntimeObject)
	AND INSTANCE_OF(RDOSrcInfo      )
{
DECLARE_FACTORY(RDOFRMFrame)
public:
	typedef std::list<tstring> ImageNameList;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMPosition
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMPosition) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMPosition)
	public:
		enum PositionType {
			absolute,
			delta,
			gabarit,
			mult,
			rulet
		};
		LPRDOCalc    pCalc;
		PositionType type;
		int          rulet_id;

		int getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += pFrame->last_x;                          break;
				case RDOFRMPosition::gabarit: res += pFrame->last_x + pFrame->last_width;     break;
				case RDOFRMPosition::mult   : res *= pFrame->last_x;                          break;
				case RDOFRMPosition::rulet  : res += pFrame->getRuletX( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
		int getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += pFrame->last_y;                          break;
				case RDOFRMPosition::gabarit: res += pFrame->last_y + pFrame->last_height;    break;
				case RDOFRMPosition::mult   : res *= pFrame->last_y;                          break;
				case RDOFRMPosition::rulet  : res += pFrame->getRuletY( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
		int getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += pFrame->last_width; break;
				case RDOFRMPosition::mult   : res *= pFrame->last_width; break;
				case RDOFRMPosition::rulet  : res += pFrame->getRuletX( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
		int getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += pFrame->last_height; break;
				case RDOFRMPosition::mult   : res *= pFrame->last_height; break;
				case RDOFRMPosition::rulet  : res += pFrame->getRuletY( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
	private:
		RDOFRMPosition():
			RDORuntimeObject(),
			pCalc( NULL ),
			type( absolute ),
			rulet_id( 0 )
		{}

		RDOFRMPosition(CREF(LPRDOCalc) _pCalc, PositionType _type = absolute, int _rulet_id = 0 ):
			RDORuntimeObject(),
			pCalc( _pCalc ),
			type( _type ),
			rulet_id( _rulet_id )
		{}

		virtual ~RDOFRMPosition()
		{}
	};
	friend class RDOFRMPosition;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMColor - объект-цвет
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMColor) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMColor)
	public:
		enum ColorType {
			color_none,
			color_rgb,
			color_transparent,
			color_last_bg,
			color_last_fg,
			color_last_bg_text,
			color_last_fg_text
		};

		rdoAnimation::RDOColor getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const;

		ColorType getColorType() const {
			return color_type;
		};
		void setColorType( ColorType _type ) {
			if ( color_type == color_none ) color_type = _type;
		};

	private:
		RDOFRMColor(ColorType _type = color_none);
		RDOFRMColor(int _red, int _green, int _blue);
		RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc);
		virtual ~RDOFRMColor();

		ColorType color_type;
		LPRDOCalc m_pRedCalc;
		LPRDOCalc m_pGreenCalc;
		LPRDOCalc m_pBlueCalc;
	};
	friend class RDOFRMColor;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMRulet
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMRulet)
		 IS  INSTANCE_OF(RDORuntimeObject)
		 AND INSTANCE_OF(RDOSrcInfo      )
	{
	DECLARE_FACTORY(RDOFRMRulet)
	public:
		int index;
		LPRDOFRMPosition x;
		LPRDOFRMPosition y;

	private:
		RDOFRMRulet(CREF(RDOSrcInfo) _src_info, int _index, CREF(LPRDOFRMPosition) _x, CREF(LPRDOFRMPosition) _y ):
			RDOSrcInfo( _src_info ),
			index( _index ),
			x(_x),
			y(_y)
		{}
		virtual ~RDOFRMRulet()
		{}
	};

public:
	void setBackgroundColor(CREF(LPRDOFRMColor) _background_color );
	void setBackPicture( CREF(tstring) _picFileName );
	void setBackPicture( int _width, int _height );
	void startShow(CREF(LPRDOCalc) pCalc = NULL);
	LPRDOFRMShow getLastShow() const { return !shows.empty() ? shows.back() : NULL; }
	void addItem (CREF(LPRDOFRMItem)  pItem );
	void addRulet(CREF(LPRDOFRMRulet) pRulet);
	bool checkCondition(CREF(LPRDORuntime) pRuntime);
	rdoAnimation::RDOFrame* createFrame(CREF(LPRDORuntime) pRuntime) {
		rdoAnimation::RDOFrame* frame = new rdoAnimation::RDOFrame();
		return prepareFrame( frame, pRuntime );
	}
	rdoAnimation::RDOFrame* prepareFrame( rdoAnimation::RDOFrame* frame, CREF(LPRDORuntime) pRuntime );
	CREF(tstring) name() const { return src_text(); }
	void getBitmaps(REF(ImageNameList) list) const;

	void setColorLastBG( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_bg );
	void setColorLastFG( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_fg );
	void setColorLastBGText( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_bg_text );
	void setColorLastFGText( RDOFRMColor::ColorType type, const rdoAnimation::RDOColor& _last_fg_text );
	void setLastXYWH( double _x, double _y, double _width, double _height ) {
		last_x      = _x;
		last_y      = _y;
		last_width  = _width;
		last_height = _height;
	}

	int getRuletX( CREF(LPRDORuntime) pRuntime, int rulet_id ) {
		RuletList::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it)->x->pCalc->calcValue( pRuntime ).getInt();
			it++;
		}
		return 0;
	}
	int getRuletY( CREF(LPRDORuntime) pRuntime, int rulet_id ) {
		RuletList::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it)->y->pCalc->calcValue( pRuntime ).getInt();
			it++;
		}
		return 0;
	}
	LPRDOFRMRulet findRulet( int rulet_id ) {
		RuletList::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it);
			it++;
		}
		return NULL;
	}

private:
	RDOFRMFrame(CREF(LPRDORuntime) pRuntime, CREF(RDOSrcInfo) _src_info, CREF(LPRDOCalc) _pConditionCalc = NULL );
	virtual ~RDOFRMFrame();

	typedef std::list  <LPRDOFRMShow > ShowList;
	typedef std::vector<LPRDOFRMRulet> RuletList;

	LPRDOCalc     pConditionCalc;
	LPRDOFRMColor background_color;
	tstring       picFileName;
	ruint         width;
	ruint         height;
	ShowList      shows;
	rdoAnimation::RDOColor color_last_bg;
	rdoAnimation::RDOColor color_last_fg;
	rdoAnimation::RDOColor color_last_bg_text;
	rdoAnimation::RDOColor color_last_fg_text;
	double       last_x;
	double       last_y;
	double       last_width;
	double       last_height;
	RuletList    rulets;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBoundingItem - объект-четырехугольник
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMBoundingItem
{
protected:
	RDOFRMBoundingItem(CREF(RDOFRMFrame::LPRDOFRMPosition) pX, CREF(RDOFRMFrame::LPRDOFRMPosition) pY, CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth, CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight)
		: m_pX     (pX     )
		, m_pY     (pY     )
		, m_pWidth (pWidth )
		, m_pHeight(pHeight)
	{}
	int getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pX->getX(pRuntime, pFrame);
	}
	int getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pY->getY(pRuntime, pFrame);
	}
	int getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pWidth->getWidth(pRuntime, pFrame);
	}
	int getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pHeight->getHeight(pRuntime, pFrame);
	}
	int getWidthAsX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pWidth->getX(pRuntime, pFrame);
	}
	int getHeightAsY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pHeight->getY(pRuntime, pFrame);
	}

private:
	RDOFRMFrame::LPRDOFRMPosition m_pX;
	RDOFRMFrame::LPRDOFRMPosition m_pY;
	RDOFRMFrame::LPRDOFRMPosition m_pWidth;
	RDOFRMFrame::LPRDOFRMPosition m_pHeight;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMColoredItem - цветной объект
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMColoredItem
{
public:
	CREF(RDOFRMFrame::LPRDOFRMColor) getBgColor() const { return m_pBgColor; }
	CREF(RDOFRMFrame::LPRDOFRMColor) getFgColor() const { return m_pFgColor; }

	rdoAnimation::RDOColor getBg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pBgColor->getColor(pRuntime, pFrame);
	}
	rdoAnimation::RDOColor getFg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pFgColor->getColor(pRuntime, pFrame);
	}

protected:
	RDOFRMColoredItem(CREF(RDOFRMFrame::LPRDOFRMColor) pBgColor, CREF(RDOFRMFrame::LPRDOFRMColor) pFgColor)
		: m_pBgColor(pBgColor)
		, m_pFgColor(pFgColor)
	{}
	virtual ~RDOFRMColoredItem()
	{}

private:
	RDOFRMFrame::LPRDOFRMColor m_pBgColor;
	RDOFRMFrame::LPRDOFRMColor m_pFgColor;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMItem - базовый для всех элементов
// ----------------------------------------------------------------------------
OBJECT(RDOFRMItem) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMItem)
public:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime) = 0;

	virtual void getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
	{}

protected:
	RDOFRMItem(CREF(LPRDOFRMFrame) pFrame)
		: m_pFrame(pFrame)
	{}
	virtual ~RDOFRMItem()
	{}

	CREF(LPRDOFRMFrame) getFrame() const { return m_pFrame; }

private:
	LPRDOFRMFrame m_pFrame;
};

#define DECLARE_RDOFRMIItem \
private:                    \
	PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

#define RDOFRM_ITEM(A) \
PREDECLARE_POINTER(A); \
CLASS(A): INSTANCE_OF(RDOFRMItem)

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMText)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMText)
public:
	typedef rdoAnimation::RDOTextElement::TextAlign Align;

	void setText(Align align, CREF(LPRDOCalc) pValue);
	void setText(Align align, CREF(tstring)   text  );

private:
	RDOFRMText(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);

	Align        m_align;
	LPRDOCalc    m_pValue;
	tstring      m_text;
	rbool        m_isTextString;

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapBase
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMBitmapBase)
{
protected:
	virtual void getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
	{
		list.push_back(m_pictFilename);
		if (!m_maskFilename.empty())
		{
			list.push_back(m_maskFilename);
		}
	}

	tstring m_pictFilename;
	tstring m_maskFilename;

protected:
	RDOFRMBitmapBase(CREF(LPRDOFRMFrame) pFrame, CREF(tstring) pictFilename, CREF(tstring) maskFilename = _T(""))
		: RDOFRMItem    (pFrame      )
		, m_pictFilename(pictFilename)
		, m_maskFilename(maskFilename)
	{}
	virtual ~RDOFRMBitmapBase()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
CLASS(RDOFRMBitmap): INSTANCE_OF(RDOFRMBitmapBase)
{
DECLARE_FACTORY(RDOFRMBitmap)
private:
	RDOFRMBitmap(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmap();

	RDOFRMFrame::LPRDOFRMPosition m_pX;
	RDOFRMFrame::LPRDOFRMPosition m_pY;

	DECLARE_RDOFRMIItem;
};
DECLARE_POINTER(RDOFRMBitmap)

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
CLASS(RDOFRMBitmapStretch):
	    INSTANCE_OF(RDOFRMBitmapBase  )
	AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMBitmapStretch)
private:
	RDOFRMBitmapStretch(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmapStretch();

	DECLARE_RDOFRMIItem;
};
DECLARE_POINTER(RDOFRMBitmapStretch);

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
class RDOFRMRect: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMRect( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
class RDOFRMRectRound: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMRectRound( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
class RDOFRMEllipse: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMEllipse( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
class RDOFRMLine: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	RDOFRMFrame::LPRDOFRMColor color;

protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMLine( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x1, CREF(RDOFRMFrame::LPRDOFRMPosition) _y1, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMColor) _color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
class RDOFRMTriang: public RDOFRMItem, public RDOFRMColoredItem
{
private:
	RDOFRMFrame::LPRDOFRMPosition x1;
	RDOFRMFrame::LPRDOFRMPosition y1;
	RDOFRMFrame::LPRDOFRMPosition x2;
	RDOFRMFrame::LPRDOFRMPosition y2;
	RDOFRMFrame::LPRDOFRMPosition x3;
	RDOFRMFrame::LPRDOFRMPosition y3;

protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMTriang( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMPosition) _x3, CREF(RDOFRMFrame::LPRDOFRMPosition) _y3, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
class RDOFRMActive: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	tstring operName;

protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMActive( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(tstring) _operName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
class RDOFRMSpace: public RDOFRMItem, public RDOFRMBoundingItem
{
protected:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMSpace( CREF(LPRDOFRMFrame) pFrame, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
OBJECT(RDOFRMShow) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMShow)
public:
	typedef std::list<LPRDOFRMItem> ItemList;

	void insertItem(CREF(LPRDOFRMItem) pItem);
	REF(ItemList) getItemList();

	bool isShowIf() const { return pConditionCalc != NULL; }

	bool checkCondition(CREF(LPRDORuntime) pRuntime);
	virtual void getBitmaps(REF(RDOFRMFrame::ImageNameList) list);

private:
	RDOFRMShow(CREF(LPRDOCalc) _pConditionCalc);
	virtual ~RDOFRMShow();

	ItemList  m_itemList;
	LPRDOCalc pConditionCalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOFRAME_H_
