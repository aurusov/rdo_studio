#ifndef LEXRDO_H
#define LEXRDO_H

#include <ctype.h>
#include "utils/rdotypes.h"
#include "thirdparty/sci/include/ILexer.h"
#include "thirdparty/sci/lexlib/LexerModule.h"

#define SCN_RDO_CLICK         20001

static inline bool isRDOLexerOperator( char ch )
{
	if ( ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
	     ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ',' ||
	     ch == '<' || ch == '>' || ch == '=' || ch == '.' || ch == '!' ||
	     ch == '{' || ch == '}' || ch == ';') return true;
	return false;
}

static inline bool isRDOLexerSuchAsIdentifierStyle( char ch )
{
	if ( ch == '%' ) return true;
	return false;
}

static inline bool isRDOLexerIdentifier( char ch )
{
	if ( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
	     (ch >= 'à' && ch <= 'ÿ') || (ch >= 'À' && ch <= 'ß') || ch == '¨' || ch == '¸' ||
	     ch == '_' || ch == '$' || isRDOLexerSuchAsIdentifierStyle(ch) || isdigit((rbyte)ch) ) return true;
	return false;
}

extern LexerModule lexerRDOSyntax;

#endif // LEXRDO_H
