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


namespace rdoParse 
{

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

RDOFRMText::RDOFRMText(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color)
	: RDOFRMBoundingItem(x, y, width, height), RDOFRMColoredItem(bgColor, color)
{}

void RDOFRMText::setText(int _align, RDOFUNArithm *_value)
{
	align = _align;
	value = _value?_value->createCalc():NULL;
	type = _value->type;
	enu = _value->enu;

	isTextString = false;
}

void RDOFRMText::setText(int _align, string *_txt)
{
	align = _align;
	txt = _txt;
	isTextString = true;
}

RDOFRMBitmap::RDOFRMBitmap(RDOFUNArithm *_x, RDOFUNArithm *_y, string *_picFileName, string *_mask)
	: picFileName(_picFileName), mask(_mask)
{
	x = _x?_x->createCalc():NULL;
	y = _y?_y->createCalc():NULL;
}

RDOFRMS_bmp::RDOFRMS_bmp(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, string *_picFileName, string *_mask)
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

RDOFRMActive::RDOFRMActive(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, string *_operName)
	: RDOFRMBoundingItem(x, y, width, height), operName(_operName)
{}

RDOFRMShow::RDOFRMShow(RDOFUNLogic* logic)
{
	conditionCalc = logic?logic->calc:NULL;
}

RDOFRMFrame::RDOFRMFrame(string *_name, RDOFUNLogic *logic)
	: name(_name)
{
	conditionCalc = logic?logic->calc:NULL;
	currParser->runTime->addRuntimeFrame(this);
}

void RDOFRMFrame::setBackground(int _r, int _g, int _b)
{
	r = _r; g = _g; b = _b;
}

void RDOFRMFrame::setBackPicture(string* _picFileName)
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

bool RDOFRMFrame::checkCondition(RDORuntime *sim)
{
	if(!conditionCalc)
		return true;

	return conditionCalc->calcValue(sim) != 0;
}

bool RDOFRMShow::checkCondition(RDORuntime *sim)
{
	if(!conditionCalc)
		return true;

	return conditionCalc->calcValue(sim) != 0;
}

RDOSimulatorNS::RDOFrame* RDOFRMFrame::createFrame(RDORuntime *sim)
{
	RDOSimulatorNS::RDOFrame *frame = new RDOSimulatorNS::RDOFrame();

	frame->r = r;
	frame->g = g;
	frame->b = b;
	frame->hasBackPicture = hasBackPicture;
	frame->picFileName = picFileName;
	frame->width = width;
	frame->height = height;


	int size = shows.size();
	for(int i = 0; i < size; i++)
	{
		RDOFRMShow *currShow = shows.at(i);
		if(!currShow->checkCondition(sim))
			continue;

		int size = currShow->items.size();
		for(int j = 0; j < size; j++)
		{
			RDOFRMItem *currItem = currShow->items.at(j);
			frame->elements.push_back(currItem->createElement(sim));
		}
	}


	return frame;
}

RDOSimulatorNS::RDOFrameElement *RDOFRMItem::createElement(RDORuntime *sim)
{
	return new RDOSimulatorNS::RDONullElement();
}

RDOSimulatorNS::RDOColor RDOFRMColoredItem::getBg()
{
	RDOSimulatorNS::RDOColor bg;
	bg.isTransparent = bgColor.isTransparent;
	bg.r = bgColor.red;
	bg.g = bgColor.green;
	bg.b = bgColor.blue;
	return bg;
}

RDOSimulatorNS::RDOColor RDOFRMColoredItem::getFg()
{
	RDOSimulatorNS::RDOColor fg;
	fg.isTransparent = color.isTransparent;
	fg.r = color.red;
	fg.g = color.green;
	fg.b = color.blue;
	return fg;
}

RDOSimulatorNS::RDOFrameElement* RDOFRMText::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor bg = getBg();
	RDOSimulatorNS::RDOColor fg = getFg();

	string t;
	if(isTextString)
		t = *txt;
	else
	{
		double val = value->calcValue(sim);

		if(type == 2)	// enumeration
			t = *enu->enumVals.at(val);
		else
			t = toString(val);
	}


	return new RDOSimulatorNS::RDOTextElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		bg, fg,
		t, (align == 1) ? RDOSimulatorNS::RDOTextElement::left : ((align == 3) ? RDOSimulatorNS::RDOTextElement::right : RDOSimulatorNS::RDOTextElement::center)
		);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMRect::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor bg = getBg();
	RDOSimulatorNS::RDOColor fg = getFg();
	return new RDOSimulatorNS::RDORectElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		bg, fg);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMR_rect::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor bg = getBg();
	RDOSimulatorNS::RDOColor fg = getFg();
	return new RDOSimulatorNS::RDORRectElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		bg, fg);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMEllipse::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor bg = getBg();
	RDOSimulatorNS::RDOColor fg = getFg();
	return new RDOSimulatorNS::RDOEllipseElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		bg, fg);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMTriang::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor bg = getBg();
	RDOSimulatorNS::RDOColor fg = getFg();
	return new RDOSimulatorNS::RDOTriangElement(
		x1->calcValue(sim), 
		y1->calcValue(sim),
		x2->calcValue(sim), 
		y2->calcValue(sim),
		x3->calcValue(sim), 
		y3->calcValue(sim),
		bg, fg);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMLine::createElement(RDORuntime *sim)
{
	RDOSimulatorNS::RDOColor fg;
	fg.isTransparent = color.isTransparent;
	fg.r = color.red;
	fg.g = color.green;
	fg.b = color.blue;

	return new RDOSimulatorNS::RDOLineElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		fg);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMS_bmp::createElement(RDORuntime *sim)
{
	if(mask != NULL)
		return new RDOSimulatorNS::RDOSBmpElement(
			x->calcValue(sim), 
			y->calcValue(sim),
			width->calcValue(sim),
			height->calcValue(sim), 
			*picFileName, *mask);
	else
		return new RDOSimulatorNS::RDOSBmpElement(
			x->calcValue(sim), 
			y->calcValue(sim),
			width->calcValue(sim),
			height->calcValue(sim), 
			*picFileName);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMBitmap::createElement(RDORuntime *sim)
{
	if(mask != NULL)
		return new RDOSimulatorNS::RDOBitmapElement(
			x->calcValue(sim), 
			y->calcValue(sim),
			*picFileName, *mask);
	else
		return new RDOSimulatorNS::RDOBitmapElement(
			x->calcValue(sim), 
			y->calcValue(sim),
			*picFileName);
}

RDOSimulatorNS::RDOFrameElement* RDOFRMActive::createElement(RDORuntime *sim)
{
	return new RDOSimulatorNS::RDOActiveElement(
		x->calcValue(sim), 
		y->calcValue(sim),
		width->calcValue(sim),
		height->calcValue(sim), 
		*operName);
}

RDOFRMShow::~RDOFRMShow()
{
	for(vector<RDOFRMItem *>::iterator it = items.begin(); it != items.end(); it++)
		delete (*it);

	items.clear();
}

RDOFRMFrame::~RDOFRMFrame()
{
	for(vector<RDOFRMShow *>::iterator it = shows.begin(); it != shows.end(); it++)
		delete (*it);

	shows.clear();
}

void RDOFRMFrame::getAllBitmaps(vector<const string *> &vect)
{
	if(hasBackPicture)
		vect.push_back(picFileName);

	for(vector<RDOFRMShow *>::iterator it = shows.begin(); it != shows.end(); it++)
		(*it)->getAllBitmaps(vect);
}

void RDOFRMShow::getAllBitmaps(vector<const string *> &vect)
{
	for(vector<RDOFRMItem *>::iterator it = items.begin(); it != items.end(); it++)
		(*it)->getAllBitmaps(vect);
}

void RDOFRMBitmap::getAllBitmaps(vector<const string *> &vect)
{
	vect.push_back(picFileName);
	if(mask)
		vect.push_back(mask);
}

void RDOFRMS_bmp::getAllBitmaps(vector<const string *> &vect)
{
	vect.push_back(picFileName);
	if(mask)
		vect.push_back(mask);
}

}		// namespace rdoParse 
