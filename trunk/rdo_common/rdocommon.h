#ifndef RDOCOMMON_H
#define RDOCOMMON_H

#pragma warning(disable : 4786)  

#include <string>
#include <vector>

namespace rdo {

std::string format( const char* str, ... );
std::string format( const char* str, va_list& params );
std::string format( unsigned int resource, ... );
std::string format( unsigned int resource, va_list& params );
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

namespace rdosim
{

enum ShowMode
{
	SM_NoShow,
	SM_Monitor,
	SM_Animation
};

struct RDOSyntaxError
{
	enum ErrorCode {
		UNKNOWN                          = 1,
		RTP_SECOND_RES_TYPE              = 1001, // Second appearance of the same resource type: %s-type_name
		RTP_WAITING_FOR_PARAMS_KW        = 1002, // Waiting for $Parameters keyword
		RTP_INVALID_SUCHAS_RES_TYPE      = 1003, // Invalid resource type in such_as: %s-type_name
		RTP_INVALID_SUCHAS_PARAM         = 1004, // Invalid resource parameter in such_as: %s-type_name.%s-param_name
		RTP_SECOND_ENUM_VALUE            = 1005, // Second appearance of the same value name: %s-enum_value
		RTP_WRONG_ENUM_PARAM_VALUE       = 1006, // Wrong enumerate parameter value: %s-param_value
		RTP_SECOND_PARAM_NAME            = 1007, // Second appearance of the same parameter name: %s-param_name
		RTP_INVALID_DEFVAULT_INT         = 1008, // Invalid default value
		RTP_INVALID_DEFVAULT_INT_AS_REAL = 1009, // Invalid default value
		RTP_INVALID_DEFVAULT_REAL        = 1010, // Invalid default value
		RTP_INVALID_DEFVAULT_ENUM        = 1011, // Invalid default value
		RTP_INVALID_DEFVAULT_INT_SUCHAS  = 1012, // Invalid default value: %d-value
		RTP_INVALID_DEFVAULT_REAL_SUCHAS = 1013, // Invalid default value: %f-value
		RTP_INVALID_DEFVAULT_ENUM_SUCHAS = 1014, // Invalid default value: %s-value
		RTP_INVALID_RANGE                = 1015, // Invalid range
		RTP_INVALID_INT_RANGE_REAL       = 1016, // Invalid integer range: using real values
		RTP_WAITING_FOR_INT_PARAM_END    = 1017, // Ending of description of integer parameter is expected
		RTP_WAITING_FOR_REAL_PARAM_END   = 1018, // Ending of description of integer parameter is expected
		RTP_WAITING_FOR_ENUM_PARAM_END   = 1019, // Ending of description of integer parameter is expected
		RTP_INVALID_PARAM_TYPE           = 1020, // Invalid param type definition
		RTP_WAITING_FOR_PARAM_DEFINITION = 1021, // Waiting for param definition
		qq
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

} // namespace rdosim

#endif // RDOCOMMON_H
