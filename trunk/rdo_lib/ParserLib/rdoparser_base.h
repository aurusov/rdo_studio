#ifndef RDOPARSER_BASE_H
#define RDOPARSER_BASE_H

typedef int  (*t_bison_parse_fun)();
typedef void (*t_bison_error_fun)( char* );
typedef int  (*t_flex_lexer_fun)( int* );

class rdoFlexLexer;

class rdoParserBase
{
public:
	rdoFlexLexer* m_lexer;

	t_bison_parse_fun parser_fun;
	t_bison_error_fun error_fun;
	t_flex_lexer_fun  lexer_fun;

	rdoParserBase(): m_lexer(NULL), parser_fun(NULL), error_fun(NULL), lexer_fun(NULL) {}
	rdoParserBase( const rdoParserBase& parser ): m_lexer(parser.m_lexer), parser_fun(parser.parser_fun), error_fun(parser.error_fun), lexer_fun(parser.lexer_fun) {}
	rdoParserBase( t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, std::istream* lexer_arg_yyin, std::ostream* lexer_arg_yyout );
	~rdoParserBase();
};

#endif // RDOPARSER_BASE_H
