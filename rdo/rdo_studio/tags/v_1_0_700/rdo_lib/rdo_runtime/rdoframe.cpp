#include "pch.h"
#include "rdoframe.h"
#include "rdo_runtime.h"

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

rdoAnimation::RDOColor RDOFRMFrame::RDOFRMColor::getColor(PTR(RDORuntime) sim, PTR(RDOFRMFrame) frame) const
{
	switch (color_type)
	{
		case color_none        : return rdoAnimation::RDOColor(50, 200, 50);
		case color_rgb         : return rdoAnimation::RDOColor(red_calc->calcValue(sim).getInt(), green_calc->calcValue(sim).getInt(), blue_calc->calcValue(sim).getInt());
		case color_transparent : return rdoAnimation::RDOColor();
		case color_last_bg     : return frame->color_last_bg;
		case color_last_fg     : return frame->color_last_fg;
		case color_last_bg_text: return frame->color_last_bg_text;
		case color_last_fg_text: return frame->color_last_fg_text;
		default                : NEVER_REACH_HERE;
	}
	return rdoAnimation::RDOColor();
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDORuntime* _runtime, const RDOSrcInfo& _src_info, RDOCalc* _conditionCalc ):
	RDORuntimeParent( _runtime ),
	RDOSrcInfo( _src_info ),
	conditionCalc( _conditionCalc ),
	background_color( NULL ),
	picFileName(_T("")),
	width( 800 ),
	height( 600 ),
	last_x( 0 ),
	last_y( 0 ),
	last_width( 0 ),
	last_height( 0 )
{
	_runtime->addRuntimeFrame( this );
	color_last_bg      = rdoAnimation::RDOColor(50, 200, 50);
	color_last_fg      = color_last_bg;
	color_last_bg_text = color_last_bg;
	color_last_fg_text = color_last_bg;
}

RDOFRMFrame::~RDOFRMFrame()
{
	std::vector< RDOFRMRulet* >::iterator it = rulets.begin();
	while ( it != rulets.end() ) {
		delete *it;
		it++;
	}
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

void RDOFRMFrame::setBackgroundColor( RDOFRMColor* _background_color )
{
	background_color = _background_color;
}

void RDOFRMFrame::setBackPicture( const std::string& _picFileName )
{
	picFileName = _picFileName;
}

void RDOFRMFrame::setBackPicture( int _width, int _height )
{
	picFileName = _T("");
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

rdoAnimation::RDOFrame* RDOFRMFrame::prepareFrame( rdoAnimation::RDOFrame* frame, RDORuntime* sim )
{
	if (background_color)
	{
		if (background_color->getColorType() == RDOFRMColor::color_rgb)
		{
			rdoAnimation::RDOColor bg_color = background_color->getColor(sim, this);
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

	if ( checkCondition( sim ) ) {
		std::list< RDOFRMShow* >::iterator it_show = shows.begin();
		while ( it_show != shows.end() ) {
			if ( (*it_show)->checkCondition(sim) ) {
				std::vector< RDORuntimeObject* >::iterator it_obj = (*it_show)->m_objects.begin();
				while ( it_obj != (*it_show)->m_objects.end() ) {
					rdoAnimation::FrameItem* element = static_cast<RDOFRMItem*>(*it_obj)->createElement(sim);
					if ( element ) {
						frame->m_elements.push_back( element );
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
	if (!picFileName.empty())
		list.push_back(picFileName);

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
	align(rdoAnimation::RDOTextElement::TETA_LEFT),
	value( NULL ),
	txt( "" ),
	isTextString( true )
{
	color_reparent( this );
}

void RDOFRMText::setText( rdoAnimation::RDOTextElement::TextAlign _align, RDOCalc* _value )
{
	align = _align;
	value = _value;

	isTextString = false;
}

void RDOFRMText::setText( rdoAnimation::RDOTextElement::TextAlign _align, const std::string& _txt )
{
	align = _align;
	txt   = _txt;
	isTextString = true;
}

rdoAnimation::FrameItem* RDOFRMText::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOTextElement) );

	rdoAnimation::RDOColor bg = getBg( sim, getFrame() );
	rdoAnimation::RDOColor fg = getFg( sim, getFrame() );
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

	return new rdoAnimation::RDOTextElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		rdoAnimation::RDOColoredElement(bg, fg),
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

rdoAnimation::FrameItem* RDOFRMBitmap::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOBmpElement) );

	int _x = x->getX( sim, getFrame() );
	int _y = y->getY( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, 0, 0 );

	return new rdoAnimation::RDOBmpElement(
		rdoAnimation::RDOPoint(_x, _y),
		pict_filename, mask_filename
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
RDOFRMBitmapStretch::RDOFRMBitmapStretch( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _pict_filename, const std::string& _mask_filename ):
	RDOFRMBitmapBase( _parent, _pict_filename, _mask_filename ),
	RDOFRMBoundingItem( _x, _y, _width, _height )
{
}

rdoAnimation::FrameItem* RDOFRMBitmapStretch::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOSBmpElement) );

	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoAnimation::RDOSBmpElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		pict_filename, mask_filename
	);
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

rdoAnimation::FrameItem* RDOFRMRect::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDORectElement) );

	rdoAnimation::RDOColor bg = getBg( sim, getFrame() );
	rdoAnimation::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoAnimation::RDORectElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
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

rdoAnimation::FrameItem* RDOFRMRectRound::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDORRectElement) );

	rdoAnimation::RDOColor bg = getBg( sim, getFrame() );
	rdoAnimation::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoAnimation::RDORRectElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
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

rdoAnimation::FrameItem* RDOFRMEllipse::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOEllipseElement) );

	rdoAnimation::RDOColor bg = getBg( sim, getFrame() );
	rdoAnimation::RDOColor fg = getFg( sim, getFrame() );
	getFrame()->setColorLastBG( getBgColor()->getColorType(), bg );
	getFrame()->setColorLastFG( getFgColor()->getColorType(), fg );
	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoAnimation::RDOEllipseElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		rdoAnimation::RDOColoredElement(bg, fg)
	);
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

rdoAnimation::FrameItem* RDOFRMLine::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOLineElement) );

	rdoAnimation::RDOColor fg = color->getColor( sim, getFrame() );
	getFrame()->setColorLastFG( color->getColorType(), fg );
	int _x1 = getX( sim, getFrame() );
	int _y1 = getY( sim, getFrame() );
	int _x2 = getWidthAsX( sim, getFrame() );
	int _y2 = getHeightAsY( sim, getFrame() );
	getFrame()->setLastXYWH( _x1, _y1, _x2 - _x1, _y2 - _y1 );

	return new rdoAnimation::RDOLineElement(
		rdoAnimation::RDOPoint(_x1, _y1),
		rdoAnimation::RDOPoint(_x2, _y2),
		fg
	);
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

rdoAnimation::FrameItem* RDOFRMTriang::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOTriangElement) );

	rdoAnimation::RDOColor bg = getBg( sim, getFrame() );
	rdoAnimation::RDOColor fg = getFg( sim, getFrame() );
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
RDOFRMActive::RDOFRMActive( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height, const std::string& _operName ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	operName( _operName )
{
}

rdoAnimation::FrameItem* RDOFRMActive::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoAnimation::RDOActiveElement) );

	int _x      = getX( sim, getFrame() );
	int _y      = getY( sim, getFrame() );
	int _width  = getWidth( sim, getFrame() );
	int _height = getHeight( sim, getFrame() );
	getFrame()->setLastXYWH( _x, _y, _width, _height );

	return new rdoAnimation::RDOActiveElement(
		rdoAnimation::RDOBoundedElement(rdoAnimation::RDOPoint(_x, _y), rdoAnimation::RDOSize(_width, _height)),
		operName
	);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
RDOFRMSpace::RDOFRMSpace( RDOFRMFrame* _parent, RDOFRMFrame::RDOFRMPosition* _x, RDOFRMFrame::RDOFRMPosition* _y, RDOFRMFrame::RDOFRMPosition* _width, RDOFRMFrame::RDOFRMPosition* _height ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height )
{
}

rdoAnimation::FrameItem* RDOFRMSpace::createElement( RDORuntime* sim )
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
