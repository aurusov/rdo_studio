#ifndef LEXRDO_H
#define LEXRDO_H

#include <ctype.h>
#include "utils/rdotypes.h"
#include "thirdparty/scintilla/include/ILexer.h"
#include "thirdparty/scintilla/lexlib/LexerModule.h"

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
	     (ch >= 'а' && ch <= 'я') || (ch >= 'А' && ch <= 'Я') || ch == 'Ё' || ch == 'ё' ||
	     ch == '_' || ch == '$' || isRDOLexerSuchAsIdentifierStyle(ch) || isdigit((rbyte)ch) ) return true;
	return false;
}

extern LexerModule lexerRDOSyntax;

#endif // LEXRDO_H
