// stdafx.cpp : source file that includes just the standard includes
//	rdo_studio.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>

std::string format( const char* str, ... )
{
	std::vector< char > s;
	s.resize( 256 );
	va_list paramList;
	int size = -1;
	while ( size == -1 ) {
		va_start( paramList, str );
		size = _vsnprintf( s.begin(), s.size(), str, paramList );
		va_end( paramList );
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return std::string( s.begin(), s.end() );
}

std::string format( UINT resource, ... )
{
	CString str;
	if ( str.LoadString( resource ) ) {
		std::vector< char > s;
		s.resize( 256 );
		va_list paramList;
		int size = -1;
		while ( size == -1 ) {
			va_start( paramList, resource );
			size = _vsnprintf( s.begin(), s.size(), (LPCTSTR)str, paramList );
			va_end( paramList );
			if ( size == -1 ) {
				s.resize( s.size() + 256 );
			}
		}
		s.resize( size );
		return std::string( s.begin(), s.end() );
	}
	return "";
}

void trim( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( 0, str.find_first_not_of( szDelims ) );
	str.erase( str.find_last_not_of( szDelims ) + 1, std::string::npos );
}

void trimLeft( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( 0, str.find_first_not_of( szDelims ) );
}

void trimRight( std::string& str )
{
	static char szDelims[] = " \t\n\r";
	str.erase( str.find_last_not_of( szDelims ) + 1, std::string::npos );
}
