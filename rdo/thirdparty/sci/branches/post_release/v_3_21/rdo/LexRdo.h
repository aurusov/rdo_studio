#ifndef LEXRDO_H
#define LEXRDO_H

#include <ctype.h>
#include "utils/rdotypes.h"

#define SCN_RDO_POSCHANGED    20000
#define SCN_RDO_CLICK         20001
#define SCN_RDO_BUFFERKEY     20002

static inline bool isRDOLexerOperator( char ch )
{
	if ( ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
	     ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ',' ||
	     ch == '<' || ch == '>' || ch == '=' || ch == '.' || ch == '!' ||
	     ch == '{' || ch == '}' || ch == ';') return true;
	return false;
}

static inline bool isRDOLetter( char ch )
{
	if ( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
	     (ch >= 'à' && ch <= 'ÿ') || (ch >= 'À' && ch <= 'ß') || ch == '¨' || ch == '¸' ) return true;
	return false;
}

static inline bool isRDOLexerIdentifier( char ch )
{
	if ( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
	     (ch >= 'à' && ch <= 'ÿ') || (ch >= 'À' && ch <= 'ß') || ch == '¨' || ch == '¸' ||
	     ch == '_' || ch == '$' || isdigit((rbyte)ch) ) return true;
	return false;
}

static inline bool isRDOUpper( char ch )
{
	if ( ch >= 'à' && ch <= 'ÿ' ) return false;
	if ( ch >= 'À' && ch <= 'ß' ) return true;
	return isupper( ch ) ? true : false;
}

static inline bool isRDOLower( char ch )
{
	if ( ch >= 'à' && ch <= 'ÿ' ) return true;
	if ( ch >= 'À' && ch <= 'ß' ) return false;
	return islower( ch ) ? true : false;
}

static inline char RDOMakeUpperCase( char ch )
{
	if ( ch >= 'a' && ch <= 'z' ) return static_cast<char>( ch - 'a' + 'A' );
	if ( ch >= 'à' && ch <= 'ÿ' ) return static_cast<char>( ch - 'à' + 'À' );
	return ch;
}

static inline char RDOMakeLowerCase( char ch )
{
	if ( ch >= 'A' && ch <= 'Z' ) return static_cast<char>( ch - 'A' + 'a' );
	if ( ch >= 'À' && ch <= 'ß' ) return static_cast<char>( ch - 'À' + 'à' );
	return ch;
}

#endif // LEXRDO_H
