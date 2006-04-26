#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_base.h"
#include "rdoparser_lexer.h"
#include "rdoparselex.h"

int* rdoParse::rtplval = NULL;

rdoParserBase::rdoParserBase( t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, std::istream* lexer_arg_yyin, std::ostream* lexer_arg_yyout ):
	m_lexer( NULL ),
	parser_fun( _parser_fun ),
	error_fun( _error_fun ),
	lexer_fun( _lexer_fun )
{
	m_lexer = new rdoFlexLexer( lexer_arg_yyin, lexer_arg_yyout );
}

rdoParserBase::~rdoParserBase()
{
	if ( m_lexer ) {
		delete m_lexer;
		m_lexer = NULL;
	}
}
