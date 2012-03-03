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
		yyFlexLexer( _yyin, _yyout ),
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

	void loc_init()
	{
		if ( m_lploc ) {
			m_lploc->first_line   = 0;
			m_lploc->first_column = 0;
			m_lploc->last_line    = 0;
			m_lploc->last_column  = 0;
		}
	}
	void loc_action()
	{
		if ( m_lploc ) {
			m_lploc->first_line   = m_lploc->last_line;
			m_lploc->first_column = m_lploc->last_column;
			for ( int i = 0; i < YYLeng(); i++ ) {
				if ( YYText()[i] == '\n' ) {
					m_lploc->last_line++;
					m_lploc->last_column = 0;
				} else if ( YYText()[i] == '\r' ) {
					m_lploc->last_column = 0;
				} else {
					m_lploc->last_column++;
				}
			}
		}
	}
	void loc_delta_pos( int value )
	{
		if ( m_lploc ) {
			m_lploc->first_column += value;
			m_lploc->last_column  += value;
		}
	}
	void setvalue( int value )
	{
		*m_lpval = value;
	}

protected:
	virtual int LexerInput( char* buf, int max_size );
	virtual void LexerOutput( const char* buf, int size );
	virtual void LexerError( const char* msg );
};

} // namespace rdoParse

#endif // RDOPARSER_LEXER_H
