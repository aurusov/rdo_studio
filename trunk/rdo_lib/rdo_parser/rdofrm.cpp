#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdofun.h"
#include "rdofrm.h"
#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int frmlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void frmerror( char* mes )
{
	rdoParse::parser->error( mes );
}

RDOFRMColor::RDOFRMColor()
{
	isTransparent = true;
}

RDOFRMColor::RDOFRMColor(int r, int g, int b)
{
	isTransparent = false;
	red = r;
	green = g;
	blue = b;
}

RDOFRMBoundingItem::RDOFRMBoundingItem(RDOFUNArithm *_x, RDOFUNArithm *_y, RDOFUNArithm *_width, RDOFUNArithm *_height)
{
	x = _x?_x->createCalc():NULL;
	y = _y?_y->createCalc():NULL;
	width = _width?_width->createCalc():NULL;
	height = _height?_height->createCalc():NULL;
}

RDOFRMColoredItem::RDOFRMColoredItem(RDOFRMColor *_bgColor, RDOFRMColor *_color)
	: bgColor(*_bgColor), color(*_color)
{
	delete _bgColor;
	delete _color;
}

RDOFRMText::RDOFRMText( RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* bgColor, RDOFRMColor* color ):
	RDOFRMBoundingItem( x, y, width, height ),
	RDOFRMColoredItem( bgColor, color ),
	value( NULL ),
	txt( NULL ),
	isTextString( true ),
	enu( NULL )
{
}

void RDOFRMText::setText( int _align, RDOFUNArithm* _value )
{
	align = _align;
	value = _value ? _value->createCalc() : NULL;
	type  = _value->type;
	enu   = _value->enu;

	isTextString = false;
}

void RDOFRMText::setText( int _align, std::string* _txt )
{
	align = _align;
	txt   = _txt;
	isTextString = true;
}

RDOFRMBitmap::RDOFRMBitmap(RDOFUNArithm *_x, RDOFUNArithm *_y, std::string *_picFileName, std::string *_mask)
	: picFileName(_picFileName), mask(_mask)
{
	x = _x?_x->createCalc():NULL;
	y = _y?_y->createCalc():NULL;
}

RDOFRMS_bmp::RDOFRMS_bmp(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, std::string *_picFileName, std::string *_mask)
	: RDOFRMBoundingItem(x, y, width, height), picFileName(_picFileName), mask(_mask)
{}

RDOFRMRect::RDOFRMRect(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color)
	: RDOFRMBoundingItem(x, y, width, height), RDOFRMColoredItem(bgColor, color)
{}
					  
RDOFRMR_rect::RDOFRMR_rect(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color)
	: RDOFRMBoundingItem(x, y, width, height), RDOFRMColoredItem(bgColor, color)
{}

RDOFRMLine::RDOFRMLine(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *_color)
	: RDOFRMBoundingItem(x, y, width, height), color(*_color)
{
	delete _color;
}

RDOFRMEllipse::RDOFRMEllipse(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color)
	: RDOFRMBoundingItem(x, y, width, height), RDOFRMColoredItem(bgColor, color)
{}

RDOFRMTriang::RDOFRMTriang(RDOFUNArithm *_x1, RDOFUNArithm *_y1, RDOFUNArithm *_x2, RDOFUNArithm *_y2, RDOFUNArithm *_x3, RDOFUNArithm *_y3, RDOFRMColor *bgColor, RDOFRMColor *color)
	: RDOFRMColoredItem(bgColor, color)
{
	x1 = _x1?_x1->createCalc():NULL;
	y1 = _y1?_y1->createCalc():NULL;
	x2 = _x2?_x2->createCalc():NULL;
	y2 = _y2?_y2->createCalc():NULL;
	x3 = _x3?_x3->createCalc():NULL;
	y3 = _y3?_y3->createCalc():NULL;
}

RDOFRMActive::RDOFRMActive(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, std::string *_operName)
	: RDOFRMBoundingItem(x, y, width, height), operName(_operName)
{}

RDOFRMShow::RDOFRMShow(RDOFUNLogic* logic)
{
	conditionCalc = logic?logic->calc:NULL;
}

RDOFRMFrame::RDOFRMFrame(std::string *_name, RDOFUNLogic *logic)
	: name(_name)
{
	conditionCalc = logic?logic->calc:NULL;
	parser->runTime->addRuntimeFrame(this);
}

void RDOFRMFrame::setBackground(int _r, int _g, int _b)
{
	r = _r; g = _g; b = _b;
}

void RDOFRMFrame::setBackPicture(std::string* _picFileName)
{
	hasBackPicture = true;
	picFileName = _picFileName;
}

void RDOFRMFrame::setBackPicture(int _width, int _height)
{
	hasBackPicture = false;
	width = _width;
	height = _height;
}

void RDOFRMFrame::startShow(RDOFUNLogic* logic)
{
	shows.push_back(new RDOFRMShow(logic));
}

void RDOFRMFrame::addItem(RDOFRMItem *item)
{
	shows.back()->items.push_back(item);
}

void RDOFRMFrame::end()
{
}

bool RDOFRMFrame::checkCondition( rdoRuntime::RDORuntime* sim )
{
	if(!conditionCalc)
		return true;

	return conditionCalc->calcValueBase(sim) != 0;
}

bool RDOFRMShow::checkCondition( rdoRuntime::RDORuntime* sim )
{
	if(!conditionCalc)
		return true;

	return conditionCalc->calcValueBase(sim) != 0;
}

rdoSimulator::RDOFrame* RDOFRMFrame::prepareFrame( rdoSimulator::RDOFrame* frame, rdoRuntime::RDORuntime* sim )
{
	frame->r = r;
	frame->g = g;
	frame->b = b;
	frame->hasBackPicture = hasBackPicture;
	frame->picFileName    = picFileName;
	frame->width  = width;
	frame->height = height;

	if ( checkCondition( sim ) ) {
		int size = shows.size();
		for ( int i = 0; i < size; i++ ) {
			RDOFRMShow* currShow = shows.at(i);
			if ( !currShow->checkCondition(sim) ) continue;
			int size = currShow->items.size();
			for ( int j = 0; j < size; j++ ) {
				RDOFRMItem* currItem = currShow->items.at(j);
				frame->elements.push_back( currItem->createElement(sim) );
			}
		}
	}

	return frame;
}

rdoSimulator::RDOFrameElement *RDOFRMItem::createElement( rdoRuntime::RDORuntime* sim )
{
	return new rdoSimulator::RDONullElement();
}

rdoSimulator::RDOColor RDOFRMColoredItem::getBg()
{
	rdoSimulator::RDOColor bg;
	bg.isTransparent = bgColor.isTransparent;
	bg.r = bgColor.red;
	bg.g = bgColor.green;
	bg.b = bgColor.blue;
	return bg;
}

rdoSimulator::RDOColor RDOFRMColoredItem::getFg()
{
	rdoSimulator::RDOColor fg;
	fg.isTransparent = color.isTransparent;
	fg.r = color.red;
	fg.g = color.green;
	fg.b = color.blue;
	return fg;
}

rdoSimulator::RDOFrameElement* RDOFRMText::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();

	std::string t;
	if ( isTextString ) {
		t = *txt;
	} else {
		double val = value->calcValueBase( sim );
		if ( type == RDORTPResParam::pt_enum ) {
			t = *enu->enumVals.at( val );
		} else {
			t = toString(val);
		}
	}

	return new rdoSimulator::RDOTextElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		bg, fg,
		t, (align == 1) ? rdoSimulator::RDOTextElement::left : ((align == 3) ? rdoSimulator::RDOTextElement::right : rdoSimulator::RDOTextElement::center)
		);
}

rdoSimulator::RDOFrameElement* RDOFRMRect::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();
	return new rdoSimulator::RDORectElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		bg, fg);
}

rdoSimulator::RDOFrameElement* RDOFRMR_rect::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();
	return new rdoSimulator::RDORRectElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		bg, fg);
}

rdoSimulator::RDOFrameElement* RDOFRMEllipse::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();
	return new rdoSimulator::RDOEllipseElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		bg, fg);
}

rdoSimulator::RDOFrameElement* RDOFRMTriang::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor bg = getBg();
	rdoSimulator::RDOColor fg = getFg();
	return new rdoSimulator::RDOTriangElement(
		x1->calcValueBase(sim), 
		y1->calcValueBase(sim),
		x2->calcValueBase(sim), 
		y2->calcValueBase(sim),
		x3->calcValueBase(sim), 
		y3->calcValueBase(sim),
		bg, fg);
}

rdoSimulator::RDOFrameElement* RDOFRMLine::createElement( rdoRuntime::RDORuntime* sim )
{
	rdoSimulator::RDOColor fg;
	fg.isTransparent = color.isTransparent;
	fg.r = color.red;
	fg.g = color.green;
	fg.b = color.blue;

	return new rdoSimulator::RDOLineElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		fg);
}

rdoSimulator::RDOFrameElement* RDOFRMS_bmp::createElement( rdoRuntime::RDORuntime* sim )
{
	if(mask != NULL)
		return new rdoSimulator::RDOSBmpElement(
			x->calcValueBase(sim), 
			y->calcValueBase(sim),
			width->calcValueBase(sim),
			height->calcValueBase(sim), 
			*picFileName, *mask);
	else
		return new rdoSimulator::RDOSBmpElement(
			x->calcValueBase(sim), 
			y->calcValueBase(sim),
			width->calcValueBase(sim),
			height->calcValueBase(sim), 
			*picFileName);
}

rdoSimulator::RDOFrameElement* RDOFRMBitmap::createElement( rdoRuntime::RDORuntime* sim )
{
	if(mask != NULL)
		return new rdoSimulator::RDOBitmapElement(
			x->calcValueBase(sim), 
			y->calcValueBase(sim),
			*picFileName, *mask);
	else
		return new rdoSimulator::RDOBitmapElement(
			x->calcValueBase(sim), 
			y->calcValueBase(sim),
			*picFileName);
}

rdoSimulator::RDOFrameElement* RDOFRMActive::createElement( rdoRuntime::RDORuntime* sim )
{
	return new rdoSimulator::RDOActiveElement(
		x->calcValueBase(sim), 
		y->calcValueBase(sim),
		width->calcValueBase(sim),
		height->calcValueBase(sim), 
		*operName);
}

RDOFRMShow::~RDOFRMShow()
{
	for(std::vector<RDOFRMItem *>::iterator it = items.begin(); it != items.end(); it++)
		delete (*it);

	items.clear();
}

RDOFRMFrame::~RDOFRMFrame()
{
	for(std::vector<RDOFRMShow *>::iterator it = shows.begin(); it != shows.end(); it++)
		delete (*it);

	shows.clear();
}

void RDOFRMFrame::getAllBitmaps( std::list< std::string >& list )
{
	if ( hasBackPicture ) {
		list.push_back( *picFileName );
	}
	for ( std::vector< RDOFRMShow* >::iterator it = shows.begin(); it != shows.end(); it++ ) {
		(*it)->getAllBitmaps( list );
	}
}

void RDOFRMShow::getAllBitmaps( std::list< std::string >& list )
{
	for ( std::vector< RDOFRMItem* >::iterator it = items.begin(); it != items.end(); it++ ) {
		(*it)->getAllBitmaps( list );
	}
}

void RDOFRMBitmap::getAllBitmaps( std::list< std::string >& list )
{
	list.push_back( *picFileName );
	if ( mask ) {
		list.push_back( *mask );
	}
}

void RDOFRMS_bmp::getAllBitmaps( std::list< std::string >& list )
{
	list.push_back( *picFileName );
	if ( mask ) {
		list.push_back( *mask );
	}
}

} // namespace rdoParse
