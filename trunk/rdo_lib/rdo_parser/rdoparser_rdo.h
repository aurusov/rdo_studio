#ifndef RDOPARSER_RDO_H
#define RDOPARSER_RDO_H

#include "rdoparser_base.h"
#include "rdoparser_lexer.h"
#include "rdoparser_object.h"
#include <rdoruntime_object.h>
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

	virtual int  lexer_loc_line();
	virtual int  lexer_loc_pos();
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
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserBase
{
public:
	RDOParserSTDFUN( RDOParser* _parser ): RDOParserBase( _parser, rdoModelObjects::FUN, NULL, NULL, NULL ) {};

	virtual void parse();
	virtual void parse( std::istream& in_stream ) { parse(); }
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
class RDOSMR1OkException: public rdoRuntime::RDOException
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
