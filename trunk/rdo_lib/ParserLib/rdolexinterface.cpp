#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FlexLexer.h"
#include "rdolexinterface.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

rdoFlexLexer *currLexer;

rdoLexer::rdoLexer(std::istream* arg_yyin, std::ostream* arg_yyout)
{
	currLexer = new rdoFlexLexer(arg_yyin, arg_yyout);
}

rdoLexer::~rdoLexer()
{
	delete currLexer;
}

int rdoLineno() 
{ 
	return currLexer->lineno(); 
}

const char *rdoYYText() 
{ 
	return currLexer->YYText(); 
}

int rdoYylex() 
{ 
	return currLexer->yylex(); 
}

}

extern "C"
{
	int yywrap(void) 
	{ 
		return 1; 
	}
}
