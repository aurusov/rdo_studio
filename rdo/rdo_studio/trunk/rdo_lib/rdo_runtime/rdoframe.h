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

		int getX(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_x;                     break;
				case RDOFRMPosition::gabarit: res += frame->last_x + frame->last_width; break;
				case RDOFRMPosition::mult   : res *= frame->last_x;                     break;
				case RDOFRMPosition::rulet  : res += frame->getRuletX( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
		int getY(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_y;                      break;
				case RDOFRMPosition::gabarit: res += frame->last_y + frame->last_height; break;
				case RDOFRMPosition::mult   : res *= frame->last_y;                      break;
				case RDOFRMPosition::rulet  : res += frame->getRuletY( pRuntime, rulet_id );  break;
			}
			return res.getInt();
		}
		int getWidth(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_width; break;
				case RDOFRMPosition::mult   : res *= frame->last_width; break;
				case RDOFRMPosition::rulet  : res += frame->getRuletX( pRuntime, rulet_id ); break;
			}
			return res.getInt();
		}
		int getHeight(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
			RDOValue res = pCalc->calcValue( pRuntime );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_height; break;
				case RDOFRMPosition::mult   : res *= frame->last_height; break;
				case RDOFRMPosition::rulet  : res += frame->getRuletY( pRuntime, rulet_id ); break;
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

		rdoAnimation::RDOColor getColor(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) const;

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
	void setBackPicture( const std::string& _picFileName );
	void setBackPicture( int _width, int _height );
	void startShow(CREF(LPRDOCalc) pCalc = NULL);
	LPRDOFRMShow getLastShow() const { return !shows.empty() ? shows.back() : NULL; }
	void addItem(CREF(LPRDOFRMItem) pItem);
	void addRulet( RDOFRMRulet* rulet );
	bool checkCondition(CREF(LPRDORuntime) pRuntime);
	rdoAnimation::RDOFrame* createFrame(CREF(LPRDORuntime) pRuntime) {
		rdoAnimation::RDOFrame* frame = new rdoAnimation::RDOFrame();
		return prepareFrame( frame, pRuntime );
	}
	rdoAnimation::RDOFrame* prepareFrame( rdoAnimation::RDOFrame* frame, CREF(LPRDORuntime) pRuntime );
	const std::string& name() const { return src_text(); }
	void getBitmaps( std::list< std::string >& list ) const;

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
public:
	struct RDOFRMBoundingData {
		RDOFRMFrame::LPRDOFRMPosition x;
		RDOFRMFrame::LPRDOFRMPosition y;
		RDOFRMFrame::LPRDOFRMPosition width;
		RDOFRMFrame::LPRDOFRMPosition height;
	};

private:
	RDOFRMBoundingData data;

protected:
	RDOFRMBoundingItem( CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height )
	{
		data.x = _x;
		data.y = _y;
		data.width = _width;
		data.height = _height;
	}
	RDOFRMBoundingItem( const RDOFRMBoundingData* _data )
	{
		data.x      = _data->x;
		data.y      = _data->y;
		data.width  = _data->width;
		data.height = _data->height;
	}
	int getX(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.x->getX( pRuntime, frame );
	}
	int getY(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.y->getY( pRuntime, frame );
	}
	int getWidth(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.width->getWidth( pRuntime, frame );
	}
	int getHeight(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.height->getHeight( pRuntime, frame );
	}
	int getWidthAsX(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.width->getX( pRuntime, frame );
	}
	int getHeightAsY(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) {
		return data.height->getY( pRuntime, frame );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMColoredItem - цветной объект
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMColoredItem
{
protected:
	RDOFRMFrame::LPRDOFRMColor bgColor;
	RDOFRMFrame::LPRDOFRMColor fgColor;

	RDOFRMColoredItem(CREF(RDOFRMFrame::LPRDOFRMColor) _bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) _fgColor ):
		bgColor( _bgColor ),
		fgColor( _fgColor )
	{
	}
	virtual ~RDOFRMColoredItem() {}

public:
	CREF(RDOFRMFrame::LPRDOFRMColor) getBgColor() const { return bgColor; }
	CREF(RDOFRMFrame::LPRDOFRMColor) getFgColor() const { return fgColor; }
	rdoAnimation::RDOColor getBg(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) { return bgColor->getColor( pRuntime, frame ); }
	rdoAnimation::RDOColor getFg(CREF(LPRDORuntime) pRuntime, PTR(RDOFRMFrame) frame) { return fgColor->getColor( pRuntime, frame ); }
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMItem - базовый для всех элементов
// ----------------------------------------------------------------------------
OBJECT(RDOFRMItem) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMItem)
public:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime) = 0;
	virtual void getBitmaps( std::list< std::string >& list )
	{}

protected:
	RDOFRMItem( RDOFRMFrame* _parent ):
		frame( _parent )
	{
	}
	virtual ~RDOFRMItem()
	{
	}
	RDOFRMFrame* getFrame() const { return frame; }

private:
	RDOFRMFrame* frame;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
class RDOFRMText: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
private:
	rdoAnimation::RDOTextElement::TextAlign align;
	LPRDOCalc            pValue;
	std::string          txt;
	bool                 isTextString;

protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMText( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
	void setText( rdoAnimation::RDOTextElement::TextAlign _align, CREF(LPRDOCalc) _pValue );
	void setText( rdoAnimation::RDOTextElement::TextAlign _align, const std::string& _txt );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapBase
// ----------------------------------------------------------------------------
class RDOFRMBitmapBase: public RDOFRMItem
{
protected:
	std::string pict_filename;
	std::string mask_filename;

	virtual void getBitmaps( std::list< std::string >& list ) {\
		list.push_back( pict_filename );
		if ( !mask_filename.empty() ) {
			list.push_back( mask_filename );
		}
	}

public:
	RDOFRMBitmapBase( RDOFRMFrame* _parent, const std::string& _pict_filename, const std::string& _mask_filename = "" ):
		RDOFRMItem( _parent ),
		pict_filename( _pict_filename ),
		mask_filename( _mask_filename )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
class RDOFRMBitmap: public RDOFRMBitmapBase
{
private:
	RDOFRMFrame::LPRDOFRMPosition x;
	RDOFRMFrame::LPRDOFRMPosition y;

protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMBitmap( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, const std::string& _pict_filename, const std::string& _mask_filename = "" );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
class RDOFRMBitmapStretch: public RDOFRMBitmapBase, public RDOFRMBoundingItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMBitmapStretch( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, const std::string& _pict_filename, const std::string& _mask_filename = "" );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
class RDOFRMRect: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMRect( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
class RDOFRMRectRound: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMRectRound( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
class RDOFRMEllipse: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMEllipse( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
class RDOFRMLine: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	RDOFRMFrame::LPRDOFRMColor color;

protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMLine( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x1, CREF(RDOFRMFrame::LPRDOFRMPosition) _y1, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMColor) _color );
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
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMTriang( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _x2, CREF(RDOFRMFrame::LPRDOFRMPosition) _y2, CREF(RDOFRMFrame::LPRDOFRMPosition) _x3, CREF(RDOFRMFrame::LPRDOFRMPosition) _y3, CREF(RDOFRMFrame::LPRDOFRMColor) bgColor, CREF(RDOFRMFrame::LPRDOFRMColor) fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
class RDOFRMActive: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	std::string operName;

protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMActive( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height, const std::string& _operName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
class RDOFRMSpace: public RDOFRMItem, public RDOFRMBoundingItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement(CREF(LPRDORuntime) pRuntime);

public:
	RDOFRMSpace( RDOFRMFrame* _parent, CREF(RDOFRMFrame::LPRDOFRMPosition) _x, CREF(RDOFRMFrame::LPRDOFRMPosition) _y, CREF(RDOFRMFrame::LPRDOFRMPosition) _width, CREF(RDOFRMFrame::LPRDOFRMPosition) _height );
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
	virtual void getBitmaps(REF(std::list<tstring>) list);

private:
	RDOFRMShow(CREF(LPRDOCalc) _pConditionCalc);
	virtual ~RDOFRMShow();

	ItemList  m_itemList;
	LPRDOCalc pConditionCalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOFRAME_H_
