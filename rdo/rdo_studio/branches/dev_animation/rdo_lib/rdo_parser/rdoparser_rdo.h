/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_RDO_H_
#define _RDOPARSER_RDO_H_

// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_base.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdoparser_object.h"
#include "rdo_lib/rdo_runtime/rdoruntime_object.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
class RDOParserRDOItem: public RDOParserItem
{
public:
	RDOParserRDOItem(PTR(RDOParser) _parser, rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository);
	virtual ~RDOParserRDOItem();

	virtual void          parse   ();
	virtual void          parse   (REF(std::istream) in_stream);
	virtual PTR(RDOLexer) getLexer(PTR(std::istream) in_stream, PTR(std::ostream) out_stream);

	virtual ruint  lexer_loc_line();
	virtual ruint  lexer_loc_pos ();

protected:
	PTR(RDOLexer)  m_lexer;
	YYLTYPE        m_loc;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
public:
	RDOParserRSS(PTR(RDOParser) parser, StreamFrom from = sf_repository);
	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
public:
	RDOParserRSSPost(PTR(RDOParser) parser)
		: RDOParserItem(parser, rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
public:
	RDOParserSTDFUN(PTR(RDOParser) parser)
		: RDOParserItem(parser, rdoModelObjects::FUN, NULL, NULL, NULL)
	{}
	virtual void parse();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_RDO_H_
