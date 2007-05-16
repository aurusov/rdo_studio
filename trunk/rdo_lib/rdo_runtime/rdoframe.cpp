#include "pch.h"
#include "rdoframe.h"

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
rdoSimulator::RDOColor RDOFRMColor::getColor() const
{
	rdoSimulator::RDOColor color;
	color.isTransparent = isTransparent;
	color.r             = red;
	color.g             = green;
	color.b             = blue;
	return color;
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
RDOFRMText::RDOFRMText( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, color ),
	value( NULL ),
	txt( "" ),
	isTextString( true ),
	enu( NULL )
{
	color_reparent( this );
}

void RDOFRMText::setText( int _align, RDOCalc* _value, RDORTPEnum* _enu )
{
	align = _align;
	value = _value;
	enu   = _enu;

	isTextString = false;
}

void RDOFRMText::setText( int _align, std::string* _txt )
{
	align = _align;
	txt   = *_txt;
	isTextString = true;
}

rdoSimulator::RDOFrameElement* RDOFRMText::createElement( RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();

	std::string t;
	if ( isTextString ) {
		t = txt;
	} else {
		RDOValue val = value->calcValueBase( sim );
		if ( enu ) {
//qq			t = *enu->enumVals.at( val );
			t = "//qq";
		} else {
			t = toString( val );
		}
	}

	sim->memory_insert( sizeof(rdoSimulator::RDOTextElement) );
	return new rdoSimulator::RDOTextElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		bg, fg,
		t, (align == 1) ? rdoSimulator::RDOTextElement::left : ((align == 3) ? rdoSimulator::RDOTextElement::right : rdoSimulator::RDOTextElement::center)
		);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
RDOFRMBitmap::RDOFRMBitmap( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, std::string* _picFileName, std::string* _mask ):
	RDOFRMItem( _parent ),
	x( _x ),
	y( _y ),
	picFileName( *_picFileName ),
	mask( "" )
{
	if ( _mask ) mask = *_mask;
}

rdoSimulator::RDOFrameElement* RDOFRMBitmap::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOBitmapElement) );
	if ( !mask.empty() ) {
		return new rdoSimulator::RDOBitmapElement(
			x->calcValueBase( sim ), 
			y->calcValueBase( sim ),
			picFileName, mask);
	} else {
		return new rdoSimulator::RDOBitmapElement(
			x->calcValueBase( sim ), 
			y->calcValueBase( sim ),
			picFileName);
	}
}

void RDOFRMBitmap::getBitmaps( std::list< std::string >& list )
{
	list.push_back( picFileName );
	if ( !mask.empty() ) {
		list.push_back( mask );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMS_bmp
// ----------------------------------------------------------------------------
RDOFRMS_bmp::RDOFRMS_bmp( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, std::string* _picFileName, std::string* _mask ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	picFileName( *_picFileName ),
	mask( "" )
{
	if ( _mask ) mask = *_mask;
}

rdoSimulator::RDOFrameElement* RDOFRMS_bmp::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOSBmpElement) );
	if ( !mask.empty() ) {
		return new rdoSimulator::RDOSBmpElement(
			x->calcValueBase( sim ), 
			y->calcValueBase( sim ),
			width->calcValueBase( sim ),
			height->calcValueBase( sim ), 
			picFileName, mask);
	} else {
		return new rdoSimulator::RDOSBmpElement(
			x->calcValueBase( sim ), 
			y->calcValueBase( sim ),
			width->calcValueBase( sim ),
			height->calcValueBase( sim ), 
			picFileName);
	}
}

void RDOFRMS_bmp::getBitmaps( std::list< std::string >& list )
{
	list.push_back( picFileName );
	if ( !mask.empty() ) {
		list.push_back( mask );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
RDOFRMRect::RDOFRMRect( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, color )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMRect::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDORectElement) );
	return new rdoSimulator::RDORectElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		getBg(), getFg());
}
	  
// ----------------------------------------------------------------------------
// ---------- RDOFRMR_rect
// ----------------------------------------------------------------------------
RDOFRMR_rect::RDOFRMR_rect( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, color )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMR_rect::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDORRectElement) );
	return new rdoSimulator::RDORRectElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		getBg(), getFg());
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
RDOFRMLine::RDOFRMLine( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* _color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	color( _color )
{
	color->reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMLine::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOLineElement) );
	return new rdoSimulator::RDOLineElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		color->getColor());
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
RDOFRMEllipse::RDOFRMEllipse( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	RDOFRMColoredItem( bgColor, color )
{
	color_reparent( this );
}

rdoSimulator::RDOFrameElement* RDOFRMEllipse::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOEllipseElement) );
	return new rdoSimulator::RDOEllipseElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		getBg(), getFg());
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
RDOFRMTriang::RDOFRMTriang( RDORuntimeParent* _parent, RDOCalc* _x1, RDOCalc* _y1, RDOCalc* _x2, RDOCalc* _y2, RDOCalc* _x3, RDOCalc* _y3, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMItem( _parent ),
	RDOFRMColoredItem( bgColor, color ),
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
	return new rdoSimulator::RDOTriangElement(
		x1->calcValueBase( sim ), 
		y1->calcValueBase( sim ),
		x2->calcValueBase( sim ), 
		y2->calcValueBase( sim ),
		x3->calcValueBase( sim ), 
		y3->calcValueBase( sim ),
		getBg(), getFg());
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
RDOFRMActive::RDOFRMActive( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, std::string* _operName ):
	RDOFRMItem( _parent ),
	RDOFRMBoundingItem( _x, _y, _width, _height ),
	operName( *_operName )
{
}

rdoSimulator::RDOFrameElement* RDOFRMActive::createElement( RDORuntime* sim )
{
	sim->memory_insert( sizeof(rdoSimulator::RDOActiveElement) );
	return new rdoSimulator::RDOActiveElement(
		x->calcValueBase( sim ), 
		y->calcValueBase( sim ),
		width->calcValueBase( sim ),
		height->calcValueBase( sim ), 
		operName);
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
RDOFRMShow::RDOFRMShow( RDORuntimeParent* _parent, RDOCalc* _conditionCalc ):
	RDORuntimeParent( _parent ),
	conditionCalc( _conditionCalc )
{
}

RDOFRMShow::~RDOFRMShow()
{
}

void RDOFRMShow::getBitmaps( std::list< std::string >& list )
{
	std::list< RDORuntimeObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		static_cast<RDOFRMItem*>(*it)->getBitmaps( list );
		it++;
	}
}

bool RDOFRMShow::checkCondition( RDORuntime* sim )
{
	if ( !conditionCalc ) return true;
	return conditionCalc->calcValueBase( sim ) != 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDORuntime* _runtime, std::string* _name, RDOCalc* _conditionCalc ):
	RDORuntimeParent( _runtime ),
	name( *_name ),
	conditionCalc( _conditionCalc )
{
	getRuntime()->addRuntimeFrame( this );
}

RDOFRMFrame::~RDOFRMFrame()
{
}

void RDOFRMFrame::setBackground( int _r, int _g, int _b )
{
	r = _r;
	g = _g;
	b = _b;
}

void RDOFRMFrame::setBackPicture( std::string* _picFileName )
{
	hasBackPicture = true;
	picFileName = *_picFileName;
}

void RDOFRMFrame::setBackPicture(int _width, int _height)
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
	item->reparent( shows.back() );
}

void RDOFRMFrame::end()
{
}

bool RDOFRMFrame::checkCondition( RDORuntime* sim )
{
	if ( !conditionCalc ) return true;
	return conditionCalc->calcValueBase( sim ) != 0;
}

rdoSimulator::RDOFrame* RDOFRMFrame::prepareFrame( rdoSimulator::RDOFrame* frame, RDORuntime* sim )
{
	frame->r = r;
	frame->g = g;
	frame->b = b;
	frame->hasBackPicture = hasBackPicture;
	frame->picFileName    = picFileName;
	frame->width  = width;
	frame->height = height;

	if ( checkCondition( sim ) ) {
		std::list< RDOFRMShow* >::iterator it_show = shows.begin();
		while ( it_show != shows.end() ) {
			if ( (*it_show)->checkCondition(sim) ) {
				std::list< RDORuntimeObject* >::iterator it_obj = (*it_show)->objects.begin();
				while ( it_obj != (*it_show)->objects.end() ) {
					frame->elements.push_back( static_cast<RDOFRMItem*>(*it_obj)->createElement(sim) );
					it_obj++;
				}
			}
			it_show++;
		}
	}

	return frame;
}

void RDOFRMFrame::getBitmaps( std::list< std::string >& list )
{
	if ( hasBackPicture ) {
		list.push_back( picFileName );
	}
	std::list< RDOFRMShow* >::iterator it = shows.begin();
	while ( it != shows.end() ) {
		(*it)->getBitmaps( list );
		it++;
	}
}

} // namespace rdoRuntime
