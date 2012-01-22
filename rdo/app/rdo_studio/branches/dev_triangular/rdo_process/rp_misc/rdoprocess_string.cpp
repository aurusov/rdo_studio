#include "app/rdo_studio_mfc/rdo_process/rp_misc/stdafx.h"
#include <vector>
#include <stdarg.h>
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rp {

int string::tohex() const
{
	rp::string str = toupper();
	int result = 0;
	for ( ruint i = 0; i < str.size(); i++ ) {
		result = result << 4;
		switch ( str[i] ) {
			case '0': break;
			case '1': result++; break;
			case '2': result += 2; break;
			case '3': result += 3; break;
			case '4': result += 4; break;
			case '5': result += 5; break;
			case '6': result += 6; break;
			case '7': result += 7; break;
			case '8': result += 8; break;
			case '9': result += 9; break;
			case 'A': result += 10; break;
			case 'B': result += 11; break;
			case 'C': result += 12; break;
			case 'D': result += 13; break;
			case 'E': result += 14; break;
			case 'F': result += 15; break;
		}
	}
	return result;
}

string format( const char* str, ... )
{
	std::vector< char > s;
	s.resize( 256 );
	va_list paramList;
	int size = -1;
	while ( size == -1 ) {
		va_start( paramList, str );
		size = _vsnprintf_s(&s[0], s.size(), _TRUNCATE, str, paramList);
		va_end( paramList );
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
		if ( s.size() > 10000 ) return "";
	}
	s.resize( size );
	return std::string( s.begin(), s.end() );
}

string format( ruint resource, ... )
{
	CString str;
	if ( str.LoadString( resource ) ) {
		std::vector< char > s;
		s.resize( 256 );
		va_list paramList;
		int size = -1;
		while ( size == -1 ) {
			va_start( paramList, resource );
			size = _vsnprintf_s(&s[0], s.size(), _TRUNCATE, static_cast<LPCTSTR>(str), paramList);
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

rp::string extractFilePath( const rp::string& fileName )
{
	rp::string s;
	std::string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos == std::string::npos ) {
		return "";
	}
	if ( pos != std::string::npos && pos < fileName.length() - 1 ) {
		s.assign( &fileName[0], pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, std::string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

} // namespace rp
