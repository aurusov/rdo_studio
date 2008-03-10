#ifndef RDOPARSER_LEXER_H
#define RDOPARSER_LEXER_H

#include "rdogramma.h"
#include <FlexLexer.h>

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
class RDOParser;

class RDOLexer: public yyFlexLexer
{
private:
	std::istream* yyin;
	std::ostream* yyout;

public:
	RDOLexer( RDOParser* parser, std::istream* _yyin, std::ostream* _yyout ):
		m_parser( parser ),
		yyin( _yyin ),
		yyout( _yyout ),
		m_lpval( NULL ),
		m_lploc( NULL ),
		m_enum_param_cnt( 0 )
	{
	};

	RDOParser*   m_parser;
	int*         m_lpval;
	YYLTYPE*     m_lploc;
	int          m_enum_param_cnt;

protected:
	virtual int LexerInput( char* buf, int max_size );
	virtual void LexerOutput( const char* buf, int size );
	virtual void LexerError( const char* msg );
};

} // namespace rdoParse

#endif // RDOPARSER_LEXER_H
