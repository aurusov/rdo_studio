#include "unicode_log.h"

#include <qtextcodec.h>

// -------------------------------------------------------
// -------- WGUnicodeLog
// -------------------------------------------------------
WGUnicodeLog::WGUnicodeLog(): QStringList()
{
}

WGUnicodeLog::~WGUnicodeLog()
{
	unicode_clear();
}

unsigned int WGUnicodeLog::unicode_count() const
{
	return size();
}

QString& WGUnicodeLog::unicode_at( unsigned int index )
{
	return *at( index );
}

QString& WGUnicodeLog::unicode_last()
{
	return back();
}

QString& WGUnicodeLog::native_at( unsigned int index )
{
	return *native.at( index );
}

QString& WGUnicodeLog::native_last()
{
	return native.back();
}

void WGUnicodeLog::unicode_append( QString& str )
{
	push_back( QTextCodec::codecForLocale()->toUnicode( str ) );
	native.push_back( str );
}

void WGUnicodeLog::unicode_append( const char* str )
{
	QString s = str;
	unicode_append( s );
}

void WGUnicodeLog::unicode_remove_last()
{
	if ( !unicode_count() ) return;
	erase( at(size()-1) );
	native.erase( native.at(native.size()-1) );
}

void WGUnicodeLog::unicode_clear()
{
	clear();
	native.clear();
}

