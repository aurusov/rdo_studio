#include "..\\memcheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

#include "rdoframe.h"

RDOFrame::~RDOFrame()
{
	for(vector<RDOFrameElement *>::iterator it = elements.begin(); it != elements.end(); it++)
		delete (*it);

	elements.clear();
}

RDOTextElement::RDOTextElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground, string &_text, RDOTextAlign _align)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground), strText(_text), align(_align)
{ 
	type = text_type; 
}

RDOColoredElement::RDOColoredElement(RDOColor &_background, RDOColor &_foreground)
	: background(_background), foreground(_foreground)
{}

RDOBoundedElement::RDOBoundedElement(double _x, double _y, double _w, double _h)
	: x(_x), y(_y), w(_w), h(_h)
{}

RDORectElement::RDORectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = rect_type; 
}

RDOLineElement::RDOLineElement(double _x, double _y, double _w, double _h, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), foreground(_foreground)
{ 
	type = line_type; 
}

RDOSBmpElement::RDOSBmpElement(double _x, double _y, double _w, double _h, string &_bmp, string &_map)
	: RDOBoundedElement(_x, _y, _w, _h), bmp(_bmp), map(_map)
{ 
	type = s_bmp_type; 
}

RDOBitmapElement::RDOBitmapElement(double _x, double _y, string &_bmp, string &_map)
	: x(_x), y(_y), bmp(_bmp), map(_map)
{
	type = bitmap_type; 
}

RDOActiveElement::RDOActiveElement(double _x, double _y, double _w, double _h, string &_operName)
	: RDOBoundedElement(_x, _y, _w, _h), operName(_operName)
{ 
	type = active_type; 
}

RDORRectElement::RDORRectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = r_rect_type; 
}

RDOEllipseElement::RDOEllipseElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = ellipse_type; 
}

RDOTriangElement::RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, RDOColor &_background, RDOColor &_foreground)
	: x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3), RDOColoredElement(_background, _foreground)
{ 
	type = triang_type; 
}
