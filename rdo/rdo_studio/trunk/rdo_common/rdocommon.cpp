#pragma warning(disable : 4786)  

// ====================================================================== INCLUDES
#include <stdio.h>
#include <stdarg.h>
#include <locale>
#include <algorithm>
#include <windows.h>
#include <io.h>
// ====================================================================== SYNOPSIS
#include "rdocommon.h"
// ===============================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static tchar THIS_FILE[] = __FILE__;
#endif

#include "../rdo_studio/resource.h"

namespace rdo {

tstring format(CPTR(tchar) str, ...)
{
	va_list params;
	va_start( params, str );
	tstring res = format( str, params );
	va_end( params );
	return res;
}

tstring format( CPTR(tchar) str, REF(va_list) params )
{
	std::vector< tchar > s;
	s.resize( 256 );
	int size = -1;
	while ( size == -1 ) {
#pragma warning(disable: 4996)
		size = _vsnprintf( &s[0], s.size(), str, params );
#pragma warning(default: 4996)
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return tstring( s.begin(), s.end() );
}

tstring format(ruint resource, ...)
{
	va_list params;
	va_start( params, resource );
	tstring res = format( resource, params );
	va_end( params );
	return res;
}

tstring format(ruint resource, REF(va_list) params)
{
	tchar buffer[1024];
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule)
	{
		if (LoadString(hModule, resource, buffer, sizeof(buffer)/sizeof(tchar)))
			return format(buffer, params);
	}
	return _T("");
}

struct _toLower {
	_toLower( std::locale loc ): m_loc(loc) {};
	tchar operator()(tchar c) {
		return std::tolower( c, m_loc );
	}
	std::locale m_loc;
};

void toLower( REF(tstring) str )
{
	_toLower tr1( std::locale(_T("rus")) );
	std::transform( str.begin(), str.end(), str.begin(), tr1 );
	_toLower tr2( std::locale(_T("C")) );
}

void trim( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( 0, str.find_first_not_of( szDelims ) );
	str.erase( str.find_last_not_of( szDelims ) + 1, tstring::npos );
}

void trimLeft( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( 0, str.find_first_not_of( szDelims ) );
}

void trimRight( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( str.find_last_not_of( szDelims ) + 1, tstring::npos );
}

tstring extractFilePath( CREF(tstring) fileName )
{
	tstring s;
	tstring::size_type pos = fileName.find_last_of( _T('\\') );
	if ( pos == tstring::npos ) {
		pos = fileName.find_last_of( _T('/') );
	}
	if ( pos == tstring::npos ) {
		return "";
	}
	if ( pos != tstring::npos && pos < fileName.length() - 1 ) {
		s.assign( &fileName[0], pos + 1 );
		static tchar szDelims[] = _T(" \t\n\r");
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, tstring::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( _T('\\') );
	if ( pos == tstring::npos ) {
		pos = s.find_last_of( _T('/') );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += _T("/");
	}
	return s;
}

} // namespace rdo

namespace rdoSimulator
{

tstring RDOSyntaxError::getMessage(ErrorCode _error_code, REF(va_list) params)
{
	int res_id = -1;
	switch (_error_code)
	{
		case rdoSimulator::RDOSyntaxError::UNKNOWN: res_id = IDS_ER_UNKNOWN; break;
	}
	return rdo::format(res_id, params);
}

RDOColoredElement::RDOColoredElement(CREF(RDOColor) _background, CREF(RDOColor) _foreground)
	: background(_background)
	, foreground(_foreground)
{}

RDOTextElement::RDOTextElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground, CREF(tstring) _text, RDOTextAlign _align)
	: RDOFrameElement  (text_type               )
	, RDOBoundedElement(_x, _y, _w, _h          )
	, RDOColoredElement(_background, _foreground)
	, strText          (_text                   )
	, align            (_align                  )
{}

RDOBoundedElement::RDOBoundedElement(double _x, double _y, double _w, double _h)
	: x(_x)
	, y(_y)
	, w(_w)
	, h(_h)
{}

RDORectElement::RDORectElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground)
	: RDOFrameElement  (rect_type               )
	, RDOBoundedElement(_x, _y, _w, _h          )
	, RDOColoredElement(_background, _foreground)
{}

RDOLineElement::RDOLineElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _foreground)
	: RDOFrameElement  (line_type     )
	, RDOBoundedElement(_x, _y, _w, _h)
	, foreground       (_foreground   )
{}

RDOSBmpElement::RDOSBmpElement(double _x, double _y, double _w, double _h, CREF(tstring) _bmp, CREF(tstring) _mask)
	: RDOFrameElement  (s_bmp_type    )
	, RDOBoundedElement(_x, _y, _w, _h)
	, bmp              (_bmp          )
	, mask             (_mask         )
{}

RDOBitmapElement::RDOBitmapElement(double _x, double _y, CREF(tstring) _bmp, CREF(tstring) _mask)
	: RDOFrameElement(bitmap_type)
	, x              (_x         )
	, y              (_y         )
	, bmp            (_bmp       )
	, mask           (_mask      )
{}

RDOActiveElement::RDOActiveElement(double _x, double _y, double _w, double _h, CREF(tstring) _operName)
	: RDOFrameElement  (active_type   )
	, RDOBoundedElement(_x, _y, _w, _h)
	, operName         (_operName     )
{}

RDORRectElement::RDORRectElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground)
	: RDOFrameElement  (r_rect_type             )
	, RDOBoundedElement(_x, _y, _w, _h          )
	, RDOColoredElement(_background, _foreground)
{}

RDOEllipseElement::RDOEllipseElement(double _x, double _y, double _w, double _h, CREF(RDOColor) _background, CREF(RDOColor) _foreground)
	: RDOFrameElement  (ellipse_type            )
	, RDOBoundedElement(_x, _y, _w, _h          )
	, RDOColoredElement(_background, _foreground)
{}

RDOTriangElement::RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, CREF(RDOColor) _background, CREF(RDOColor) _foreground)
	: RDOFrameElement  (triang_type             )
	, x1               (_x1                     )
	, y1               (_y1                     )
	, x2               (_x2                     )
	, y2               (_y2                     )
	, x3               (_x3                     )
	, y3               (_y3                     )
	, RDOColoredElement(_background, _foreground)
{}

RDOFrame::~RDOFrame()
{
	for(Elements::iterator it = elements.begin(); it != elements.end(); it++)
		delete (*it);

	elements.clear();
}

} // namespace rdoSimulator
