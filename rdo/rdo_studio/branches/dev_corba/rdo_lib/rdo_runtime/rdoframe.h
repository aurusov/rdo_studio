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
#include "namespace.h"
#include "rdocalc.h"
#include "rdoruntime_object.h"
#include <rdoanimation.h>
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
class RDOFRMShow;
class RDOFRMItem;

class RDOFRMFrame: public RDORuntimeParent, public RDOSrcInfo
{
public:
	// ----------------------------------------------------------------------------
	// ---------- RDOFRMPosition
	// ----------------------------------------------------------------------------
	class RDOFRMPosition: public RDORuntimeObject
	{
	public:
		enum PositionType {
			absolute,
			delta,
			gabarit,
			mult,
			rulet
		};
		RDOCalc*     calc;
		PositionType type;
		int          rulet_id;

		RDOFRMPosition( RDOFRMFrame* _parent ):
			RDORuntimeObject( _parent ),
			calc( NULL ),
			type( absolute ),
			rulet_id( 0 )
		{
		}
		RDOFRMPosition( RDOFRMFrame* _parent, RDOCalc* _calc, PositionType _type = absolute, int _rulet_id = 0 ):
			RDORuntimeObject( _parent ),
			calc( _calc ),
			type( _type ),
			rulet_id( _rulet_id )
		{
		}
		int getX( RDORuntime* sim, RDOFRMFrame* frame ) {
			RDOValue res = calc->calcValue( sim );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_x;                     break;
				case RDOFRMPosition::gabarit: res += frame->last_x + frame->last_width; break;
				case RDOFRMPosition::mult   : res *= frame->last_x;                     break;
				case RDOFRMPosition::rulet  : res += frame->getRuletX( sim, rulet_id ); break;
			}
			return res.getInt();
		}
		int getY( RDORuntime* sim, RDOFRMFrame* frame ) {
			RDOValue res = calc->calcValue( sim );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_y;                      break;
				case RDOFRMPosition::gabarit: res += frame->last_y + frame->last_height; break;
				case RDOFRMPosition::mult   : res *= frame->last_y;                      break;
				case RDOFRMPosition::rulet  : res += frame->getRuletY( sim, rulet_id );  break;
			}
			return res.getInt();
		}
		int getWidth( RDORuntime* sim, RDOFRMFrame* frame ) {
			RDOValue res = calc->calcValue( sim );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_width; break;
				case RDOFRMPosition::mult   : res *= frame->last_width; break;
				case RDOFRMPosition::rulet  : res += frame->getRuletX( sim, rulet_id ); break;
			}
			return res.getInt();
		}
		int getHeight( RDORuntime* sim, RDOFRMFrame* frame ) {
			RDOValue res = calc->calcValue( sim );
			switch ( type ) {
				case RDOFRMPosition::delta  : res += frame->last_height; break;
				case RDOFRMPosition::mult   : res *= frame->last_height; break;
				case RDOFRMPosition::rulet  : res += frame->getRuletY( sim, rulet_id ); break;
			}
			return res.getInt();
		}
	};
	friend class RDOFRMPosition;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMColor - объект-цвет
	// ----------------------------------------------------------------------------
	class RDOFRMColor: public RDORuntimeObject
	{
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

	private:
		ColorType color_type;
		RDOCalc* red_calc;
		RDOCalc* green_calc;
		RDOCalc* blue_calc;

	public:
		RDOFRMColor( RDOFRMFrame* _parent, ColorType _type = color_none );
		RDOFRMColor( RDOFRMFrame* _parent, int _red, int _green, int _blue );
		RDOFRMColor( RDOFRMFrame* _parent, RDOCalc* _red_calc, RDOCalc* _green_calc, RDOCalc* _blue_calc );
		~RDOFRMColor();

		rdoAnimation::RDOColor getColor( RDORuntime* sim, RDOFRMFrame* frame ) const;

		ColorType getColorType() const {
			return color_type;
		};
		void setColorType( ColorType _type ) {
			if ( color_type == color_none ) color_type = _type;
		};
	};
	friend class RDOFRMColor;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMRulet
	// ----------------------------------------------------------------------------
	class RDOFRMRulet: public RDOSrcInfo
	{
	public:
		int index;
		RDOFRMPosition* x;
		RDOFRMPosition* y;
		RDOFRMRulet( const RDOSrcInfo& _src_info, int _index, RDOFRMPosition* _x, RDOFRMPosition* _y ):
			RDOSrcInfo( _src_info ),
			index( _index ),
			x(_x),
			y(_y)
		{
		}
		~RDOFRMRulet() {
			delete x;
			delete y;
		}
	};

public:
	RDOFRMFrame( RDORuntime* _runtime, const RDOSrcInfo& _src_info, RDOCalc* _conditionCalc = NULL );
	virtual ~RDOFRMFrame();
	void setBackgroundColor( RDOFRMColor* _background_color );
	void setBackPicture( const std::string& _picFileName );
	void setBackPicture( int _width, int _height );
	void startShow( RDOCalc* calc = NULL );
	const RDOFRMShow* getLastShow() const { return !shows.empty() ? shows.back() : NULL; }
	void addItem( RDOFRMItem* item );
	void addRulet( RDOFRMRulet* rulet );
	bool checkCondition( RDORuntime* sim );
	rdoAnimation::RDOFrame* createFrame( RDORuntime* sim ) {
		rdoAnimation::RDOFrame* frame = new rdoAnimation::RDOFrame();
		return prepareFrame( frame, sim );
	}
	rdoAnimation::RDOFrame* prepareFrame( rdoAnimation::RDOFrame* frame, RDORuntime* sim );
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

	int getRuletX( RDORuntime* sim, int rulet_id ) {
		std::vector< RDOFRMRulet* >::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it)->x->calc->calcValue( sim ).getInt();
			it++;
		}
		return 0;
	}
	int getRuletY( RDORuntime* sim, int rulet_id ) {
		std::vector< RDOFRMRulet* >::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it)->y->calc->calcValue( sim ).getInt();
			it++;
		}
		return 0;
	}
	const RDOFRMRulet* findRulet( int rulet_id ) {
		std::vector< RDOFRMRulet* >::const_iterator it = rulets.begin();
		while ( it != rulets.end() ) {
			if ( (*it)->index == rulet_id ) return (*it);
			it++;
		}
		return NULL;
	}

private:
	typedef std::list  <PTR(RDOFRMShow) > ShowList;
	typedef std::vector<PTR(RDOFRMRulet)> RuletList;

	RDOCalc*     conditionCalc;
	RDOFRMColor* background_color;
	tstring      picFileName;
	ruint        width;
	ruint        height;
	ShowList     shows;
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
		RDOFRMFrame::RDOFRMPosition* x;
		RDOFRMFrame::RDOFRMPosition* y;
		RDOFRMFrame::RDOFRMPosition* width;
		RDOFRMFrame::RDOFRMPosition* height;
	};

private:
	RDOFRMBoundingData data;

protected:
	RDOFRMBoundingItem( RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height )
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
	int getX( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.x->getX( sim, frame );
	}
	int getY( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.y->getY( sim, frame );
	}
	int getWidth( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.width->getWidth( sim, frame );
	}
	int getHeight( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.height->getHeight( sim, frame );
	}
	int getWidthAsX( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.width->getX( sim, frame );
	}
	int getHeightAsY( RDORuntime* sim, RDOFRMFrame* frame ) {
		return data.height->getY( sim, frame );
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
	RDOFRMFrame::RDOFRMColor* bgColor;
	RDOFRMFrame::RDOFRMColor* fgColor;

	RDOFRMColoredItem( RDOFRMFrame::RDOFRMColor* _bgColor, RDOFRMFrame::RDOFRMColor* _fgColor ):
		bgColor( _bgColor ),
		fgColor( _fgColor )
	{
	}
	virtual ~RDOFRMColoredItem() {}

	void color_reparent( RDORuntimeParent* _parent ) {
		bgColor->reparent( _parent );
		fgColor->reparent( _parent );
	}

public:
	const RDOFRMFrame::RDOFRMColor* getBgColor() const { return bgColor; }
	const RDOFRMFrame::RDOFRMColor* getFgColor() const { return fgColor; }
	rdoAnimation::RDOColor getBg( RDORuntime* sim, RDOFRMFrame* frame ) { return bgColor->getColor( sim, frame ); }
	rdoAnimation::RDOColor getFg( RDORuntime* sim, RDOFRMFrame* frame ) { return fgColor->getColor( sim, frame ); }
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMItem - базовый для всех элементов
// ----------------------------------------------------------------------------
class RDOFRMItem: public RDORuntimeParent
{
friend class RDOFRMShow;
friend class RDOFRMFrame;

private:
	RDOFRMFrame* frame;

protected:
	RDOFRMItem( RDOFRMFrame* _parent ):
		RDORuntimeParent( _parent ),
		frame( _parent )
	{
	}
	virtual ~RDOFRMItem()
	{
	}
	virtual void getBitmaps( std::list< std::string >& list )
	{
	}
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim ) = 0;
	RDOFRMFrame* getFrame() const { return frame; }
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
class RDOFRMText: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
private:
	rdoAnimation::RDOTextElement::TextAlign align;
	RDOCalc* value;
	std::string          txt;
	bool                 isTextString;

protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMText( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor );
	void setText( rdoAnimation::RDOTextElement::TextAlign _align, RDOCalc* _value );
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
	RDOFRMFrame::RDOFRMPosition* x;
	RDOFRMFrame::RDOFRMPosition* y;

protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMBitmap( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, const std::string& _pict_filename, const std::string& _mask_filename = "" );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
class RDOFRMBitmapStretch: public RDOFRMBitmapBase, public RDOFRMBoundingItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMBitmapStretch( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _pict_filename, const std::string& _mask_filename = "" );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
class RDOFRMRect: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMRect( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
class RDOFRMRectRound: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMRectRound( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
class RDOFRMEllipse: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMEllipse( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
class RDOFRMLine: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	RDOFRMFrame::RDOFRMColor* color;

protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMLine( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x1, RDOFRMFrame::RDOFRMPosition* _y1, RDOFRMFrame::RDOFRMPosition* _x2, RDOFRMFrame::RDOFRMPosition* _y2, RDOFRMFrame::RDOFRMColor* _color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
class RDOFRMTriang: public RDOFRMItem, public RDOFRMColoredItem
{
private:
	RDOFRMFrame::RDOFRMPosition* x1;
	RDOFRMFrame::RDOFRMPosition* y1;
	RDOFRMFrame::RDOFRMPosition* x2;
	RDOFRMFrame::RDOFRMPosition* y2;
	RDOFRMFrame::RDOFRMPosition* x3;
	RDOFRMFrame::RDOFRMPosition* y3;

protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMTriang( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _x2, RDOFRMFrame::RDOFRMPosition* _y2, RDOFRMFrame::RDOFRMPosition* _x3, RDOFRMFrame::RDOFRMPosition* _y3, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
class RDOFRMActive: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	std::string operName;

protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMActive( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _operName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
class RDOFRMSpace: public RDOFRMItem, public RDOFRMBoundingItem
{
protected:
	virtual rdoAnimation::FrameItem* createElement( RDORuntime* sim );

public:
	RDOFRMSpace( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
// В списке потомков RDOFRMShow должны находится только RDOFRMItem.
// Такова текущая логика добавления объектов к RDOFRMShow.
// Приведение RDORuntimeObject к RDOFRMItem происходит без проверок.
// ----------------------------------------------------------------------------
class RDOFRMShow: public RDORuntimeParent
{
friend class RDOFRMFrame;

private:
	RDOCalc* conditionCalc;

public:
	RDOFRMShow( RDOFRMFrame* _parent, RDOCalc* _conditionCalc );
	virtual ~RDOFRMShow();

	bool isShowIf() const { return conditionCalc != NULL; }

	bool checkCondition( RDORuntime* sim );
	virtual void getBitmaps( std::list< std::string >& list );
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOFRAME_H_
