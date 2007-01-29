#ifndef RDOPARSER_RDO_H
#define RDOPARSER_RDO_H

#include "rdoparser_base.h"
#include "rdoparser_lexer.h"
#include "rdoparser_object.h"
#include <iostream.h>

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserRDO
// ----------------------------------------------------------------------------
class RDOParserRDO: public RDOParserBase
{
protected:
	RDOLexer*            m_lexer;
	YYLTYPE              m_loc;

public:
	RDOParserRDO( RDOParser* _parser, rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): RDOParserBase( _parser, _type, _parser_fun, _error_fun, _lexer_fun ), m_lexer( NULL ) {};
	virtual ~RDOParserRDO();

	virtual void parse();
	virtual void parse( std::istream& in_stream );
	virtual RDOLexer* getLexer( std::istream& in_stream, std::ostream& out_stream );

	virtual void lexer_setvalue( int value );
	virtual void lexer_loc_init();
	virtual void lexer_loc_action();
	virtual void lexer_loc_set( int error_line, int error_pos );
	virtual void lexer_loc_set( void* data );
	virtual void lexer_loc_set( void* data1, void* data2 );
	virtual void lexer_loc_delta_pos( int value );
	virtual void lexer_loc_backup( void* data = NULL );
	virtual void lexer_loc_restore();
	virtual int  lexer_loc_line();
	virtual int  lexer_loc_pos();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRTP
// ----------------------------------------------------------------------------
class RDOParserRTP: public RDOParserRDO
{
public:
	RDOParserRTP( RDOParser* _parser );

	virtual RDOLexer* getLexer( std::istream& in_stream, std::ostream& out_stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOLexerRTP
// ----------------------------------------------------------------------------
class RDOLexerRTP: public RDOLexer
{
public:
	RDOLexerRTP( RDOParserBase* parser, std::istream* _yyin, std::ostream* _yyout ):
		RDOLexer( parser, _yyin, _yyout ),
		enum_param_cnt( 0 )
	{
	};

	int enum_param_cnt;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDO
{
public:
	RDOParserRSS( RDOParser* _parser );

	virtual void parse( std::istream& in_stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserBase
{
public:
	RDOParserRSSPost( RDOParser* _parser ): RDOParserBase( _parser, rdoModelObjects::RSS, NULL, NULL, NULL ) {};

	virtual void parse();
	virtual void parse( std::istream& in_stream ) { parse(); }
};

// ----------------------------------------------------------------------------
// ---------- RDOParserFUN
// ----------------------------------------------------------------------------
class RDOParserFUN: public RDOParserRDO
{
public:
	RDOParserFUN( RDOParser* _parser );

	virtual RDOLexer* getLexer( std::istream& in_stream, std::ostream& out_stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOLexerFUN
// ----------------------------------------------------------------------------
class RDOLexerFUN: public RDOLexer
{
public:
	RDOLexerFUN( RDOParserBase* parser, std::istream* _yyin, std::ostream* _yyout ):
		RDOLexer( parser, _yyin, _yyout ),
		enum_param_cnt( 0 )
	{
	};

	int enum_param_cnt;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserRDO
{
public:
	RDOParserSTDFUN( RDOParser* _parser ): RDOParserRDO( _parser, rdoModelObjects::FUN, NULL, NULL, NULL ) {};

	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
class RDOSMR1OkException: public RDOException
{
public:
	virtual std::string getType() const { return ""; }
	RDOSMR1OkException( const char* str ): RDOException( str ) {};
};

class RDOParserSMR1: public RDOParserRDO
{
public:
	RDOParserSMR1( RDOParser* _parser );

	virtual void parse( std::istream& in_stream );
};

} // namespace rdoParse

#endif // RDOPARSER_RDO_H
