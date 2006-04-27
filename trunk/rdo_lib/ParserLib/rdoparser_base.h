#ifndef RDOPARSER_BASE_H
#define RDOPARSER_BASE_H

#include <rdocommon.h>

typedef int  (*t_bison_parse_fun)( void* lexer );
typedef void (*t_bison_error_fun)( char* mes );
typedef int  (*t_flex_lexer_fun)( int* lpval, void* lexer );

class rdoFlexLexer;

class rdoParserBase
{
public:
	rdoModelObjects::RDOFileType type;
	rdoFlexLexer* m_lexer;

	t_bison_parse_fun parser_fun;
	t_bison_error_fun error_fun;
	t_flex_lexer_fun  lexer_fun;

	rdoParserBase(): type(rdoModelObjects::PAT), m_lexer(NULL), parser_fun(NULL), error_fun(NULL), lexer_fun(NULL) {};
	rdoParserBase( rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): type( _type ), m_lexer( NULL ), parser_fun( _parser_fun ), error_fun( _error_fun ), lexer_fun( _lexer_fun ) {};
	~rdoParserBase();

	void parse();
};

#endif // RDOPARSER_BASE_H
