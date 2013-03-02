/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocommon.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.06.2009
  \brief     Общесистемные функции и типы
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <locale>
#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>

#ifdef COMPILER_VISUAL_STUDIO
#	include <windows.h>
#	include <io.h>
#else
#	include <stdarg.h>
#	include <wchar.h>
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning(disable : 4786)
#endif

OPEN_RDO_NAMESPACE

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
#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning(disable: 4996)
#ifdef UNICODE
		size = _vsnwprintf( &s[0], s.size(), str, params );
#else
		size = _vsnprintf( &s[0], s.size(), str, params );
#endif
#	pragma warning(default: 4996)
#endif  // COMPILER_VISUAL_STUDIO

#ifdef COMPILER_GCC
#ifdef UNICODE
		size = vswprintf( &s[0], s.size(), str, params );
#else
		size = vsnprintf( &s[0], s.size(), str, params );
#endif
#endif // COMPILER_GCC
		if ( size == -1 )
		{
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return tstring( s.begin(), s.end() );
}

CLOSE_RDO_NAMESPACE
