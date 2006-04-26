#ifndef RDOPARSER_LEXER_H
#define RDOPARSER_LEXER_H

#include <FlexLexer.h>

class rdoFlexLexer: public yyFlexLexer
{
private:
	std::istream* yyin;
	std::ostream* yyout;

public:
	rdoFlexLexer( std::istream* _yyin, std::ostream* _yyout ): yyin( _yyin ), yyout( _yyout ) {};

	int* m_lpval;

protected:
	virtual int LexerInput( char* buf, int max_size );
	virtual void LexerOutput( const char* buf, int size );
	virtual void LexerError( const char* msg );
};

#endif // RDOPARSER_LEXER_H
