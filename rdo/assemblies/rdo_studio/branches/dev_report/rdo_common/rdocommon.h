#ifndef RDOCOMMON_H
#define RDOCOMMON_H

#pragma warning(disable : 4786)  

#include <string>
#include <vector>

namespace rdo {

template< class T >
T rmin( T v1, T v2 )
{
	return v1 <= v2 ? v1 : v2;
}

template< class T >
T rmax( T v1, T v2 )
{
	return v1 >= v2 ? v1 : v2;
}

std::string format( const char* str, ... );
std::string format( const char* str, va_list& params );
std::string format( unsigned int resource, ... );
std::string format( unsigned int resource, va_list& params );
void toLower( std::string& str );
void trim( std::string& str );
void trimLeft( std::string& str );
void trimRight( std::string& str );
std::string extractFilePath( const std::string& fileName );
bool isFileExists( const std::string& fileName );

template < class Arg > inline std::string toString( Arg value )
{
   std::ostringstream str;
   str << value;
   return str.str();
}

template < class Stor > void deleteAllObjects( Stor& storage )
{
	Stor::reverse_iterator it = storage.rbegin();
	while ( it != storage.rend() ) {
		delete *it;
		it++;
	}
	storage.clear();
}

// ----------------------------------------------------------------------------
// ---------- vector
// ----------------------------------------------------------------------------
template <class T>
class vector: public std::vector<T>
{
public:
	vector()
	{
	}
	vector( const T& item )
	{
		push_back( item );
	}
	vector& operator() ( const T& item )
	{
		push_back( item );
		return *this;
	}
};

} // namespace rdo

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

} // namespace rdoModelObjects

namespace rdoRuntime
{

enum RunTimeMode
{
	RTM_MaxSpeed,
	RTM_Jump,
	RTM_Sync,
	RTM_Pause,
	RTM_BreakPoint
};

} // namespace rdoRuntime

namespace rdoSimulator
{

enum ShowMode
{
	SM_NoShow,
	SM_Animation,
	SM_Monitor
};

enum RDOExitCode {
	EC_OK = 0,
	EC_ParserError,
	EC_RunTimeError,
	EC_UserBreak,
	EC_ModelNotFound,
	EC_NoMoreEvents
};

struct RDOSyntaxError
{
	enum ErrorCode {
		UNKNOWN = 1,
	};
	ErrorCode   error_code;
	std::string message;
	int         error_line;
	int         error_pos;
	bool        warning;
	rdoModelObjects::RDOFileType file;
	RDOSyntaxError( ErrorCode _error_code, const std::string& _message, int _error_line, int _error_pos, rdoModelObjects::RDOFileType _file, bool _warning = false ):
		error_code( _error_code ),
		message( _message ),
		error_line( _error_line ),
		error_pos( _error_pos ),
		file( _file ),
		warning( _warning )
	{
	}
	static std::string getMessage( ErrorCode _error_code, va_list& params );
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
public:
	RDOColor background;
	RDOColor foreground;

protected:
	RDOColoredElement( const RDOColor& _background, const RDOColor& _foreground );
};

struct RDOBoundedElement
{
public:
	double x, y, w, h;

protected:
	RDOBoundedElement( double _x, double _y, double _w, double _h );
};

struct RDOTextElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
public:
	enum RDOTextAlign {
		left,
		right,
		center
	} align;

	RDOTextElement( double _x, double _y, double _w, double _h, const RDOColor& _background, const RDOColor& _foreground, const std::string& _text, RDOTextAlign _align );
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
	RDOColor bgColor;           // background
	bool hasBackPicture;
	std::string picFileName;    // back picture
	int width, height;          // frame size
	std::vector< RDOFrameElement* > elements;
	~RDOFrame();
};

} // namespace rdoSimulator

#endif // RDOCOMMON_H
