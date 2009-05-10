#ifndef RDOCOMMON_H
#define RDOCOMMON_H

#pragma warning(disable : 4786)

// ====================================================================== INCLUDES
#include <string>
#include <vector>
#include <sstream>
// ====================================================================== SYNOPSIS
#include "rdotypes.h"
#include "rdomacros.h"
// ===============================================================================

namespace rdo {

template< class T >
T rmin(T v1, T v2)
{
	return v1 <= v2 ? v1 : v2;
}

template< class T >
T rmax(T v1, T v2)
{
	return v1 >= v2 ? v1 : v2;
}

tstring format(CPTR(tchar) str, ...);
tstring format(CPTR(tchar) str, REF(va_list) params);
tstring format(ruint resource, ...);
tstring format(ruint resource, REF(va_list) params);

void toLower  (REF(tstring) str);
void trim     (REF(tstring) str);
void trimLeft (REF(tstring) str);
void trimRight(REF(tstring) str);

tstring extractFilePath(CREF(tstring) fileName);

template< class T >
inline tstring toString(T value)
{
	std::ostringstream str;
	str << value;
	return str.str();
}

template<class T> void deleteAllObjects(REF(T) container)
{
	T::reverse_iterator it = container.rbegin();
	while (it != container.rend())
	{
		delete *it;
		++it;
	}
	container.clear();
}

// ----------------------------------------------------------------------------
// ---------- vector
// ----------------------------------------------------------------------------
template <class T>
class vector: public std::vector<T>
{
public:
	vector()
	{}

	vector(CREF(T) item)
	{
		push_back( item );
	}
	REF(vector) operator() (CREF(T) item)
	{
		push_back(item);
		return *this;
	}
};

} // namespace rdo

namespace rdoModelObjects {

struct RDOSMRFileInfo
{
	tstring  model_name;
	tstring  resource_file;
	tstring  oprIev_file;
	tstring  frame_file;
	tstring  statistic_file;
	tstring  results_file;
	tstring  trace_file;
	rbool    error;

	RDOSMRFileInfo()
		: model_name    (_T(""))
		, resource_file (_T(""))
		, oprIev_file   (_T(""))
		, frame_file    (_T(""))
		, statistic_file(_T(""))
		, results_file  (_T(""))
		, trace_file    (_T(""))
		, error         (false )
	{}
};

enum RDOFileType  { PAT = 0, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC };

enum RDOParseType
{
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

enum RDOExitCode
{
	EC_OK = 0,
	EC_ParserError,
	EC_RunTimeError,
	EC_UserBreak,
	EC_ModelNotFound,
	EC_NoMoreEvents
};

struct RDOSyntaxError
{
	enum ErrorCode
	{
		UNKNOWN = 1,
	};

	ErrorCode                     error_code;
	tstring                       message;
	int                           error_line;
	int                           error_pos;
	rbool                         warning;
	rdoModelObjects::RDOFileType  file;

	RDOSyntaxError(ErrorCode _error_code, CREF(tstring) _message, int _error_line, int _error_pos, rdoModelObjects::RDOFileType _file, rbool _warning = false)
		: error_code(_error_code)
		, message   (_message   )
		, error_line(_error_line)
		, error_pos (_error_pos )
		, file      (_file      )
		, warning   (_warning   )
	{}

	static tstring getMessage(ErrorCode _error_code, REF(va_list) params);
};

struct RDOFrameElement
{
	enum Type
	{
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
	};

	RDOFrameElement(Type type)
		: m_type(type)
	{}
	virtual ~RDOFrameElement()
	{}

	Type getType() { return m_type; }

private:
	Type m_type;
};

struct RDOColor
{
	rbool isTransparent;
	int r, g, b;
};

struct RDOColoredElement
{
public:
	RDOColor background;
	RDOColor foreground;

protected:
	RDOColoredElement( CREF(RDOColor) _background, CREF(RDOColor) _foreground );
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

	RDOTextElement( double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground, CREF(tstring) _text, RDOTextAlign _align );
	tstring strText;
};

struct RDORectElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDORectElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground);
};

struct RDORRectElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDORRectElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground);
};

struct RDOEllipseElement: public RDOColoredElement, public RDOBoundedElement, public RDOFrameElement
{
	RDOEllipseElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground);
};

struct RDOTriangElement: public RDOColoredElement, public RDOFrameElement
{
	double x1, y1, x2, y2, x3, y3;
	RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, CREF(RDOColor) _background, CREF(RDOColor) _foreground);
};

struct RDOLineElement: public RDOBoundedElement, public RDOFrameElement
{
	RDOColor foreground;
	RDOLineElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _foreground);
};

struct RDOSBmpElement: public RDOBoundedElement, public RDOFrameElement
{
	tstring bmp, mask;

	RDOSBmpElement(double _x, double _y, double _w, double _h, CREF(tstring) _bmp, CREF(tstring) _mask = _T(""));

	rbool hasMask() const { return !mask.empty(); }
};

struct RDOBitmapElement: public RDOFrameElement
{
	tstring bmp, mask;
	double  x, y;

	RDOBitmapElement(double _x, double _y, CREF(tstring) _bmp, CREF(tstring) _mask = _T(""));

	rbool hasMask() const { return !mask.empty(); }
};

struct RDOActiveElement: public RDOBoundedElement, public RDOFrameElement
{
	tstring operName;
	RDOActiveElement(double _x, double _y, double _w, double _h, CREF(tstring) _operName);
};

struct RDONullElement: public RDOFrameElement
{
	RDONullElement(): RDOFrameElement(null_type) {}
};

struct RDOFrame
{
	RDOColor  bgColor;           // background
	rbool     hasBackPicture;
	tstring   picFileName;       // back picture
	int       width, height;     // frame size

	typedef std::vector<PTR(RDOFrameElement)> Elements;
	Elements elements;

	~RDOFrame();
};

} // namespace rdoSimulator

#endif // RDOCOMMON_H
