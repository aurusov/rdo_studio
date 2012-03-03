#include "pch.h"
#include "rdoframe.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime 
{

// ----------------------------------------------------------------------------
// ---------- RDOFRMColor - объект-цвет
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMColor::RDOFRMColor( RDOFRMFrame* _parent, ColorType _type ):
	RDORuntimeObject( _parent ),
	color_type( _type ),
	red_calc( NULL ),
	green_calc( NULL ),
	blue_calc( NULL )
{
}

RDOFRMFrame::RDOFRMColor::RDOFRMColor( RDOFRMFrame* _parent, int _red, int _green, int _blue ):
	RDORuntimeObject( _parent ),
	color_type( color_rgb )
{
	red_calc   = new RDOCalcConst( _parent, _red );
	green_calc = new RDOCalcConst( _parent, _green );
	blue_calc  = new RDOCalcConst( _parent, _blue );
	red_calc->setSrcText( rdo::format("%d, _red") );
	green_calc->setSrcText( rdo::format("%d, _green") );
	blue_calc->setSrcText( rdo::format("%d, _blue") );
}

RDOFRMFrame::RDOFRMColor::RDOFRMColor( RDOFRMFrame* _parent, RDOCalc* _red_calc, RDOCalc* _green_calc, RDOCalc* _blue_calc ):
	RDORuntimeObject( _parent ),
	color_type( color_rgb ),
	red_calc( _red_calc ),
	green_calc( _green_calc ),
	blue_calc( _blue_calc )
{
}

RDOFRMFrame::RDOFRMColor::~RDOFRMColor()
{
}

rdoSimulator::RDOColor RDOFRMFrame::RDOFRMColor::getColor( RDORuntime* sim, RDOFRMFrame* frame ) const
{
	rdoSimulator::RDOColor color;
	switch ( color_type ) {
		case color_none: {
			color.isTransparent = false;
			color.r = 50;
			color.g = 200;
			color.b = 50;
			break;
		}
		case color_rgb: {
			color.isTransparent = false;
			color.r = red_calc->calcValue( sim ).getInt();
			color.g = green_calc->calcValue( sim ).getInt();
			color.b = blue_calc->calcValue( sim ).getInt();
			break;
		}
		case color_transparent: {
			color.isTransparent = true;
			color.r = 50;
			color.g = 200;
			color.b = 50;
			break;
		}
		case color_last_bg: {
			color.isTransparent = frame->color_last_bg.isTransparent;
			color.r = frame->color_last_bg.r;
			color.g = frame->color_last_bg.g;
			color.b = frame->color_last_bg.b;
			break;
		}
		case color_last_fg: {
			color.isTransparent = frame->color_last_fg.isTransparent;
			color.r = frame->color_last_fg.r;
			color.g = frame->color_last_fg.g;
			color.b = frame->color_last_fg.b;
			break;
		}
		case color_last_bg_text: {
			color.isTransparent = frame->color_last_bg_text.isTransparent;
			color.r = frame->color_last_bg_text.r;
			color.g = frame->color_last_bg_text.g;
			color.b = frame->color_last_bg_text.b;
			break;
		}
		case color_last_fg_text: {
			color.isTransparent = frame->color_last_fg_text.isTransparent;
			color.r = frame->color_last_fg_text.r;
			color.g = frame->color_last_fg_text.g;
			color.b = frame->color_last_fg_text.b;
			break;
		}
	}
	return color;
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDORuntime* _runtime, const RDOSrcInfo& _src_info, RDOCalc* _conditionCalc ):
	RDORuntimeParent( _runtime ),
	RDOSrcInfo( _src_info ),
	conditionCalc( _conditionCalc ),
	background_color( NULL ),
	hasBackPicture( false ),
	picFileName( "" ),
	width( 800 ),
	height( 600 ),
	last_x( 0 ),
	last_y( 0 ),
	last_width( 0 ),
	last_height( 0 )
{
	_runtime->addRuntimeFrame( this );
	color_last_bg.isTransparent = false;
	color_last_bg.r = 50;
	color_last_bg.g = 200;
	color_last_bg.b = 50;
	color_last_fg.isTransparent = false;
	color_last_fg.r = 50;
	color_last_fg.g = 200;
	color_last_fg.b = 50;
	color_last_bg_text.isTransparent = false;
	color_last_bg_text.r = 50;
	color_last_bg_text.g = 200;
	color_last_bg_text.b = 50;
	color_last_fg_text.isTransparent = false;
	color_last_fg_text.r = 50;
	color_last_fg_text.g = 200;
	color_last_fg_text.b = 50;
}

RDOFRMFrame::~RDOFRMFrame()
{
	std::vector< RDOFRMRulet* >::iterator it = rulets.begin();
	while ( it != rulets.end() ) {
		delete *it;
		it++;
	}
}

void RDOFRMFrame::setColorLastBG( RDOFRMColor::ColorType type, const rdoSimulator::RDOColor& _last_bg )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_bg = _last_bg;
	}
}

void RDOFRMFrame::setColorLastFG( RDOFRMColor::ColorType type, const rdoSimulator::RDOColor& _last_fg )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_fg = _last_fg;
	}
}

void RDOFRMFrame::setColorLastBGText( RDOFRMColor::ColorType type, const rdoSimulator::RDOColor& _last_bg_text )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_bg_text = _last_bg_text;
	}
}

void RDOFRMFrame::setColorLastFGText( RDOFRMColor::ColorType type, const rdoSimulator::RDOColor& _last_fg_text )
{
	if ( type == RDOFRMColor::color_rgb ) {
		color_last_fg_text = _last_fg_text;
	}
}

void RDOFRMFrame::setBackgroundColor( RDOFRMColor* _background_color )
{
	background_color = _background_color;
}

void RDOFRMFrame::setBackPicture( const std::string& _picFileName )
{
	hasBackPicture = true;
	picFileName = _picFileName;
}

void RDOFRMFrame::setBackPicture( int _width, int _height )
{
	hasBackPicture = false;
	width = _width;
	height = _height;
}

void RDOFRMFrame::startShow( RDOCalc* calc )
{
	shows.push_back( new RDOFRMShow( this, calc ) );
}

void RDOFRMFrame::addItem( RDOFRMItem* item )
{
	if ( shows.empty() ) startShow();
	item->reparent( shows.back() );
}

void RDOFRMFrame::addRulet( RDOFRMRulet* rulet )
{
	rulets.push_back( rulet );
}

bool RDOFRMFrame::checkCondition( RDORuntime* sim )
{
	if ( !conditionCalc ) return true;
	return conditionCalc->calcValue( sim ).getAsBool();
}

rdoSimulator::RDOFrame* RDOFRMFrame::prepareFrame( rdoSimulator::RDOFrame* frame, RDORuntime* sim )
{
	if ( background_color ) {
		if ( background_color->getColorType() == RDOFRMColor::color_rgb ) {
			rdoSimulator::RDOColor bg_color = background_color->getColor( sim, this );
			frame->bgColor = bg_color;
		} else {
			frame->bgColor.r = 50;
			frame->bgColor.g = 200;
			frame->bgColor.b = 50;
			frame->bgColor.isTransparent = true;
		}
	} else {
		frame->bgColor.r = 50;
		frame->bgColor.g = 200;
		frame->bgColor.b = 50;
		frame->bgColor.isTransparent = true;
	}
	frame->hasBackPicture = hasBackPicture;
	frame->picFileName    = picFileName;
	frame->width  = width;
	frame->height = height;

	last_x      = 0;
	last_y      = 0;
	last_width  = 0;
	last_height = 0;

	if ( checkCondition( sim ) ) {
		std::list< RDOFRMShow* >::iterator it_show = shows.begin();
		while ( it_show != shows.end() ) {
			if ( (*it_show)->checkCondition(sim) ) {
				std::vector< RDORuntimeObject* >::iterator it_obj = (*it_show)->m_objects.begin();
				while ( it_obj != (*it_show)->m_objects.end() ) {
					rdoSimulator::RDOFrameElement* element = static_cast<RDOFRMItem*>(*it_obj)->createElement(sim);
					if ( element ) {
						frame->elements.push_back( element );
					}
					it_obj++;
				}
			}
			it_show++;
		}
	}

	return frame;
}

void RDOFRMFrame::getBitmaps( std::list< std::string >& list ) const
{
	if ( hasBackPicture ) {
		list.push_back( picFileName );
	}
	std::list< RDOFRMShow* >::const_iterator it = shows.begin();
	while ( it != shows.end() ) {
		(*it)->getBitmaps( list );
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
RDOFRMText::RDOFRMText( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, fgColor ),
	align( rdoSimulator::RDOTextElement::left ),
	value( NULL ),
	txt( "" ),
	isTextString( true )
{
	color_reparent( this );
}

void RDOFRMText::setText( rdoSimulator::RDOTextElement::RDOTextAlign _align, RDOCalc* _value )
{
	align = _align;
	value = _value;

	isTextString = false;
}

void RDOFRMText::setText( rdoSimulator::RDOTextElement::RDOTextAlign _align, const std::string& _txt )
{
	align = _align;
	txt   = _txt;
	isTextString = true;
}

rdoSimulator::RDOFrameElement* RDOFRMText::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOTextElement) );

	rdoSimulator::RDOColor bg = getBg( sim, getFrame() );
	rdoSimulator::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBGText( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFGText( getFgColor()->getColorType(), fg );

	std::string t;
	if ( isTextString ) {
		t = txt;
	} else {
		RDOValue val = value->calcValue( sim );
		t = val.getAsString();
	}

	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoSimulator::RDOTextElement(
		_x, _y, _width, _height,
		bg, fg,
		t, align
		);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
RDOFRMBitmap::RDOFRMBitmap( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, const std::string& _pict_filename, const std::string& _mask_filename ):
	RDOFRMBitmapBase( _parent, _pict_filename, _mask_filename ),
	x( _x ),
	y( _y )
{
}

rdoSimulator::RDOFrameElement* RDOFRMBitmap::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOBitmapElement) );

	int _x = x->getX( sim, getFrame() );
	int _y = y->getY( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, 0, 0 );

	if ( mask_filename.empty() ) {
		return new rdoSimulator::RDOBitmapElement(
			_x, _y,
			pict_filename);
	} else {
		return new rdoSimulator::RDOBitmapElement(
			_x, _y,
			pict_filename, mask_filename);
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
RDOFRMBitmapStretch::RDOFRMBitmapStretch( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _pict_filename, const std::string& _mask_filename ):
	RDOFRMBitmapBase( _parent, _pict_filename, _mask_filename ),
	RDOFRMBoundingItem( _x, _y, _width, _height )
{
}

rdoSimulator::RDOFrameElement* RDOFRMBitmapStretch::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOSBmpElement) );

	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	if ( mask_filename.empty() ) {
		return new rdoSimulator::RDOSBmpElement(
			_x, _y, _width, _height,
			pict_filename);
	} else {
		return new rdoSimulator::RDOSBmpElement(
			_x, _y, _width, _height,
			pict_filename, mask_filename);
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
RDOFRMRect::RDOFRMRect( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, fgColor )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMRect::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDORectElement) );

	rdoSimulator::RDOColor bg = getBg( sim, getFrame() );
	rdoSimulator::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoSimulator::RDORectElement(
		_x, _y, _width, _height,
		bg, fg );
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
RDOFRMRectRound::RDOFRMRectRound( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, fgColor )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMRectRound::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDORRectElement) );

	rdoSimulator::RDOColor bg = getBg( sim, getFrame() );
	rdoSimulator::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoSimulator::RDORRectElement(
		_x, _y, _width, _height,
		bg, fg );
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
RDOFRMEllipse::RDOFRMEllipse( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, fgColor )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMEllipse::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOEllipseElement) );

	rdoSimulator::RDOColor bg = getBg( sim, getFrame() );
	rdoSimulator::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoSimulator::RDOEllipseElement(
		_x, _y, _width, _height,
		bg, fg );
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
RDOFRMLine::RDOFRMLine( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x1, RDOFRMFrame::RDOFRMPosition* _y1, RDOFRMFrame::RDOFRMPosition* _x2, RDOFRMFrame::RDOFRMPosition* _y2, RDOFRMFrame::RDOFRMColor* _color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x1, _y1, _x2, _y2 ),
	color( _color )
{
	color->reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMLine::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOLineElement) );

	rdoSimulator::RDOColor fg = color->getColor( sim, getFrame() );
	getFrame()->setColorLastFG( color->getColorType(), fg );
	int _x1 = getX( sim, getFrame() );
	int _y1 = getY( sim, getFrame() );
	int _x2 = getWidthAsX( sim, getFrame() );
	int _y2 = getHeightAsY( sim, getFrame() );
	getFrame()->setLastXYWH( _x1, _y1, _x2 - _x1, _y2 - _y1 );

	return new rdoSimulator::RDOLineElement(
		_x1, _y1, _x2, _y2,
		fg);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
RDOFRMTriang::RDOFRMTriang( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x1, RDOFRMFrame::RDOFRMPosition* _y1, RDOFRMFrame::RDOFRMPosition* _x2, RDOFRMFrame::RDOFRMPosition* _y2, RDOFRMFrame::RDOFRMPosition* _x3, RDOFRMFrame::RDOFRMPosition* _y3, RDOFRMFrame::RDOFRMColor* bgColor, RDOFRMFrame::RDOFRMColor* fgColor ):
	RDOFRMItem( _parent ),
	RDOFRMColoredItem( bgColor, fgColor ),
	x1( _x1 ),
	y1( _y1 ),
	x2( _x2 ),
	y2( _y2 ),
	x3( _x3 ),
	y3( _y3 )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMTriang::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOTriangElement) );

	rdoSimulator::RDOColor bg = getBg( sim, getFrame() );
	rdoSimulator::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x1 = x1->getX( sim, getFrame() );
	int _y1 = y1->getY( sim, getFrame() );
	int min_x = _x1;
	int max_x = _x1;
	int min_y = _y1;
	int max_y = _y1;
	getFrame()->setLastXYWH( _x1, _y1, _x1, _y1 );
	int _x2 = x2->getX( sim, getFrame() );
	int _y2 = y2->getY( sim, getFrame() );
	if ( min_x > _x2 ) min_x = _x2;
	if ( max_x < _x2 ) max_x = _x2;
	if ( min_y > _y2 ) min_y = _y2;
	if ( max_y < _x2 ) max_y = _y2;
	getFrame()->setLastXYWH( _x2, _y2, _x2 - _x1, _y2 - _y1 );
	int _x3 = x3->getX( sim, getFrame() );
	int _y3 = y3->getY( sim, getFrame() );
	if ( min_x > _x3 ) min_x = _x3;
	if ( max_x < _x3 ) max_x = _x3;
	if ( min_y > _y3 ) min_y = _y3;
	if ( max_y < _x3 ) max_y = _y3;
	getFrame()->setLastXYWH( min_x, min_y, max_x - min_x, max_y - min_y );

	return new rdoSimulator::RDOTriangElement(
		_x1, _y1, _x2, _y2, _x3, _y3,
		bg, fg);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
RDOFRMActive::RDOFRMActive( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _operName ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	operName( _operName )
{
}

rdoSimulator::RDOFrameElement* RDOFRMActive::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOActiveElement) );

	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoSimulator::RDOActiveElement(
		_x, _y, _width, _height,
		operName);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
RDOFRMSpace::RDOFRMSpace( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height )
{
}

rdoSimulator::RDOFrameElement* RDOFRMSpace::createElement( RDORuntime* sim )
{
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
RDOFRMShow::RDOFRMShow( RDOFRMFrame* _parent, RDOCalc* _conditionCalc ):
	RDORuntimeParent( _parent ),
	conditionCalc( _conditionCalc )
{
}

RDOFRMShow::~RDOFRMShow()
{
}

void RDOFRMShow::getBitmaps( std::list< std::string >& list )
{
	std::vector< RDORuntimeObject* >::iterator it = m_objects.begin();
	while ( it != m_objects.end() ) {
		static_cast<RDOFRMItem*>(*it)->getBitmaps( list );
		it++;
	}
}

bool RDOFRMShow::checkCondition( RDORuntime* sim )
{
	if ( !conditionCalc ) return true;
	return conditionCalc->calcValue( sim ).getAsBool();
}

} // namespace rdoRuntime
