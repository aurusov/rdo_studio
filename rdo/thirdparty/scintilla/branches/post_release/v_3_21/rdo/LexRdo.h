#ifndef LEXRDO_H
#define LEXRDO_H

#include <ctype.h>
#include "utils/rdotypes.h"
#include "utils/rdolocale.h"
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
	std::locale locale = rdo::locale::get().c();
	if (std::isalpha((rbyte)ch, locale) || std::isdigit((rbyte)ch, locale) ||
		ch == '_' || ch == '$' || isRDOLexerSuchAsIdentifierStyle(ch)) return true;
	return false;
}

extern LexerModule lexerRDOSyntax;

#endif // LEXRDO_H
