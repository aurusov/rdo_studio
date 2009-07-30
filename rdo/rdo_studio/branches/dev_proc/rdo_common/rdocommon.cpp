/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocommon.cpp
 * author   : Урусов Андрей
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

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

CLOSE_RDO_NAMESPACE
