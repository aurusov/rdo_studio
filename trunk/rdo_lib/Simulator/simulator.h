#ifndef RDO_SIMULATOR_INTERFACE__
#define RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>

using namespace std;

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

enum RdoShowMode
{
	RDO_SM_NoShow,
	RDO_SM_Monitor,
	RDO_SM_Animation
};

struct RDOConfig
{
//////// Interactive /////////////////
	RdoShowMode showAnimation;
	vector<string> allFrameNames;
	int currFrameToShow;
	vector<int> keysPressed;
	bool mouseClicked;
	vector<string> activeAreasMouseClicked;
	
//////// Frame /////////////////////
	RDOFrame *frame;

//////// Timing ///////////////////
	double currTime;			// model time
	double newTime;			// model time
	double showRate;
	double realTimeDelay;	// msec
};

typedef void (*TracerCallBack) (string *newString, void *param);
typedef void (*FrameCallBack) (RDOConfig *config, void *param);

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

class RdoModel
{
	rdoRuntime::RDORuntime *runtime;
	ostringstream consol;
	RDOTrace *tracer;
	rdoRuntime::RDOResult *resulter;
	rdoParse::RDOParser *parser;

public:
	bool parseModel(string smrFileName);
	string getConsole();
	void executeModel(TracerCallBack tracerCallBack, FrameCallBack frameCallBack, void *param);
	~RdoModel();
};

#endif //RDO_SIMULATOR_INTERFACE__
