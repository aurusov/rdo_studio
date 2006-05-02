#ifndef RDOCOMMON_H
#define RDOCOMMON_H

#pragma warning(disable : 4786)  

#include <string>
#include <vector>

namespace rdo {

std::string format( const char* str, ... );
std::string format( unsigned int resource, ... );
void trim( std::string& str );
void trimLeft( std::string& str );
void trimRight( std::string& str );
std::string extractFilePath( const std::string& fileName );
bool isFileExists( const std::string& fileName );

};

namespace rdoModelObjects {

struct RDOSMRFileInfo {
	std::string model_name;
	std::string resource_file;
	std::string oprIev_file;
	std::string frame_file;
	std::string statistic_file;
	std::string results_file;
	std::string trace_file;
	bool error;

	RDOSMRFileInfo(): model_name( "" ), resource_file( "" ), oprIev_file( "" ), frame_file( "" ), statistic_file( "" ), results_file( "" ), trace_file( "" ), error( false ) {};
};

enum RDOFileType  { PAT = 0, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC };

enum RDOParseType {
	obNONE = 0x0000,
	obPRE  = 0x0001,
	obRTP  = 0x0002,
	obRSS  = 0x0004,
	obFUN  = 0x0008,
	obPAT  = 0x0010,
	obOPR  = 0x0020,
	obDPT  = 0x0040,
	obPMD  = 0x0080,
	obFRM  = 0x0100,
	obSMR  = 0x0200,
	obPOST = 0x0400,
	obALL  = 0x07FF
};

};

namespace rdoModel {

enum RDOExitCode { EC_OK = 0, EC_ParserError, EC_RunTimeError, EC_UserBreak, EC_ModelNotFound, EC_NoMoreEvents };

};

namespace RDOSimulatorNS
{

enum ShowMode
{
	SM_NoShow,
	SM_Monitor,
	SM_Animation
};

struct RDOSyntaxError
{
	std::string message;
	int lineNo;
	rdoModelObjects::RDOFileType file;
	RDOSyntaxError(std::string _message, int _lineNo, rdoModelObjects::RDOFileType _file)
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

	RDOTextElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground, std::string &_text, RDOTextAlign _align);
	std::string strText;
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
	bool hasMask;
	std::string bmp, mask;
	RDOSBmpElement(double _x, double _y, double _w, double _h, std::string &_bmp, std::string &_mask);
	RDOSBmpElement(double _x, double _y, double _w, double _h, std::string &_bmp);
};

struct RDOBitmapElement: public RDOFrameElement
{
	bool hasMask;
	std::string bmp, mask;
	double x, y;
	RDOBitmapElement(double _x, double _y, std::string &_bmp, std::string &_mask);
	RDOBitmapElement(double _x, double _y, std::string &_bmp);
};

struct RDOActiveElement: public RDOBoundedElement, public RDOFrameElement
{
	std::string operName;
	RDOActiveElement(double _x, double _y, double _w, double _h, std::string &_operName);
};

struct RDONullElement: public RDOFrameElement
{
	RDONullElement() { type = null_type; }
};

struct RDOFrame
{
	int r, g, b;	// background
	bool hasBackPicture;
	std::string* picFileName;	// back picture
	int width, height;	// frame size
	std::vector<RDOFrameElement *> elements;
	~RDOFrame();
};

} //namespace RDOSimulatorNS

#endif // RDOCOMMON_H
