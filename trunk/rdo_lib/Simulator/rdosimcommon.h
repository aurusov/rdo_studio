#ifndef RDO_FRAME_INTERFACE__
#define RDO_FRAME_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <vector>

using namespace std;

namespace RDOSimulatorNS
{

enum ShowMode
{
	SM_NoShow,
	SM_Monitor,
	SM_Animation
};

enum FileToParse
{
	RTP_FILE,
	RSS_FILE,
	FUN_FILE,
	PAT_FILE,
	OPR_FILE,
	DPT_FILE,
	PMD_FILE,
	FRM_FILE,
	SMR1_FILE,
	SMR2_FILE
};

struct RDOSyntaxError
{
	string message;
	int lineNo;
	FileToParse file;
	RDOSyntaxError(string _message, int _lineNo, FileToParse _file)
		: message(_message), lineNo(_lineNo), file(_file)	{}
};

struct RDOFrameElement
{
	enum Type{
		null_type,
		text_type,
		bitmap_type,
		rect_type,
		line_type,
		ellipse_type,
		r_rect_type,
		triang_type,
		s_bmp_type,
		active_type
	} type;

	Type GetType() {return type;}
	virtual ~RDOFrameElement() {}
};

struct RDOColor
{
	bool isTransparent;
	int r, g, b;
};

struct RDOColoredElement
{
	RDOColor background;
	RDOColor foreground;
protected:
	RDOColoredElement(RDOColor &_background, RDOColor &_foreground);
};

struct RDOBoundedElement
{
	double x, y, w, h;
protected:
	RDOBoundedElement(double _x, double _y, double _w, double _h);
};

struct RDOTextElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	enum RDOTextAlign {
		left,
		right,
		center
	} align;

	RDOTextElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground, string &_text, RDOTextAlign _align);
	string strText;
};

struct RDORectElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDORectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground);
};

struct RDORRectElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDORRectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground);
};

struct RDOEllipseElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDOEllipseElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground);
};

struct RDOTriangElement: public RDOColoredElement, public RDOFrameElement
{
	double x1, y1, x2, y2, x3, y3;
	RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, RDOColor &_background, RDOColor &_foreground);
};

struct RDOLineElement: public RDOBoundedElement, public RDOFrameElement
{
	RDOColor foreground;
	RDOLineElement(double _x, double _y, double _w, double _h, RDOColor &_foreground);
};

struct RDOSBmpElement: public RDOBoundedElement, public RDOFrameElement
{
	string bmp, map;
	RDOSBmpElement(double _x, double _y, double _w, double _h, string &_bmp, string &_map);
};

struct RDOBitmapElement: public RDOFrameElement
{
	string bmp, map;
	double x, y;
	RDOBitmapElement(double _x, double _y, string &_bmp, string &_map);
};

struct RDOActiveElement: public RDOBoundedElement, public RDOFrameElement
{
	string operName;
	RDOActiveElement(double _x, double _y, double _w, double _h, string &_operName);
};

struct RDONullElement: public RDOFrameElement
{
	RDONullElement() { type = null_type; }
};

struct RDOFrame
{
	int r, g, b;	// background
	bool hasBackPicture;
	string* picFileName;	// back picture
	int width, height;	// frame size
	vector<RDOFrameElement *> elements;
	~RDOFrame();
};

} //namespace RDOSimulatorNS

#endif //RDO_FRAME_INTERFACE__
