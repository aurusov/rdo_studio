#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FlexLexer.h"
#include "rdolexinterface.h"

class rdoFlexLexer : public yyFlexLexer 
{
	std::istream* yyin;
	std::ostream* yyout;
public:
	// arg_yyin and arg_yyout default to the cin and cout, but we
	// only make that assignment when initializing in yylex().
	rdoFlexLexer(std::istream* _yyin, std::ostream* _yyout):
		yyin(_yyin), yyout(_yyout) {}

protected:
	virtual int LexerInput( char* buf, int max_size );
	virtual void LexerOutput( const char* buf, int size );
	virtual void LexerError( const char* msg );
};

#ifdef YY_INTERACTIVE
int rdoFlexLexer::LexerInput( char* buf, int /* max_size */ )
#else
int rdoFlexLexer::LexerInput( char* buf, int max_size )
#endif
{
	if ( yyin->eof() || yyin->fail() )
		return 0;

#ifdef YY_INTERACTIVE
	yyin->get( buf[0] );

	if ( yyin->eof() )
		return 0;

	if ( yyin->bad() )
		return -1;

	return 1;

#else
	(void) yyin->read( buf, max_size );

	if ( yyin->bad() )
		return -1;
	else
		return yyin->gcount();
#endif
}

void rdoFlexLexer::LexerOutput( const char* buf, int size )
{
	(void) yyout->write( buf, size );
}

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

void rdoFlexLexer::LexerError( const char msg[] )
{
	cerr << msg << '\n';
	exit( YY_EXIT_FAILURE );
}



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