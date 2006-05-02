#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_lexer.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOFlexLexer
// ----------------------------------------------------------------------------
#ifdef YY_INTERACTIVE
int RDOFlexLexer::LexerInput( char* buf, int /* max_size */ )
#else
int RDOFlexLexer::LexerInput( char* buf, int max_size )
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

void RDOFlexLexer::LexerOutput( const char* buf, int size )
{
	(void) yyout->write( buf, size );
}

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

void RDOFlexLexer::LexerError( const char msg[] )
{
	std::cerr << msg << '\n';
	exit( YY_EXIT_FAILURE );
}

extern "C"
{
	int yywrap(void) 
	{ 
		return 1; 
	}
}

} // namespace rdoParse
