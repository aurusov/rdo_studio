// stdafx.cpp : source file that includes just the standard includes
//	chat.pch will be the pre-compiled header
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
			size = _vsnprintf( s.begin(), s.size(), static_cast<LPCTSTR>(str), paramList );
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

struct uppercase {
private:
	const std::locale& loc;
public:
	uppercase( const std::locale& loc ): loc( loc ) {}
	char operator() ( char c ) { return std::toupper( c, loc ); }
};

void toupper( std::string& str, int pos, int count )
{
	if ( count == std::string::npos ) {
		count = str.length();
	}
	if ( pos + count <= str.length() ) {
		std::transform( str.begin() + pos, str.begin() + count, str.begin() + pos, uppercase( std::locale() ) );
	}
}

struct lowercase {
private:
	const std::locale& loc;
public:
	lowercase( const std::locale& loc ): loc( loc ) {}
	char operator() ( char c ) { return std::tolower( c, loc ); }
};

void tolower( std::string& str, int pos, int count )
{
	if ( count == std::string::npos ) {
		count = str.length();
	}
	if ( pos + count <= str.length() ) {
		std::transform( str.begin() + pos, str.begin() + count, str.begin() + pos, lowercase( std::locale() ) );
	}
}

void appendMenu( CMenu* from, const int from_index, CMenu* to )
{
	CString s;
	from->GetMenuString( from_index, s, MF_BYPOSITION );

	CMenu* m_from = from->GetSubMenu( from_index );
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {

		CMenu* m_to = new CMenu();
		m_to->CreateMenu();
		to->AppendMenu( MF_STRING | MF_POPUP, (int)m_to->m_hMenu, s );

		for ( int i = 0; i < item_count; i++ ) {
			appendMenu( m_from, i, m_to );
		}

	} else {
		UINT itemID = from->GetMenuItemID( from_index );
		if ( itemID ) {
			to->AppendMenu( MF_STRING, itemID, s );
		} else {
			to->AppendMenu( MF_SEPARATOR );
		}
	}
}

void eraseMenu( CMenu* from, const int from_index )
{
	CMenu* m_from;
	if ( from_index != -1 )	{
		m_from = from->GetSubMenu( from_index );
	} else {
		m_from = from;
	}
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {
		for ( int i = item_count-1; i >= 0; i-- ) {
			eraseMenu( m_from, i );
		}
		if ( from_index != -1 ) delete m_from;
	}

	if ( from_index != -1 ) from->DeleteMenu( from_index, MF_BYPOSITION );
}
