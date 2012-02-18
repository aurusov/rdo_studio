/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_rdo.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_RDO_H_
#define _RDOPARSER_RDO_H_

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_base.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/compiler/parser/rdopat.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

class RDOParserRDOItem: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRDOItem);
public:
	virtual void  parse         (CREF(LPRDOParser) pParser);
	virtual ruint lexer_loc_line();
	virtual ruint lexer_loc_pos ();

protected:
	RDOParserRDOItem(rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository);
	virtual ~RDOParserRDOItem();

	PTR(RDOLexer)  m_pLexer;
	YYLTYPE        m_loc;

private:
	void parse(CREF(LPRDOParser) pParser, REF(std::istream) in_stream);
	PTR(RDOLexer) getLexer(CREF(LPRDOParser) pParser, PTR(std::istream) in_stream, PTR(std::ostream) out_stream);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSS
// --------------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserRSS);
private:
	RDOParserRSS(StreamFrom from = sf_repository);
	virtual void parse(CREF(LPRDOParser) pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSSPost
// --------------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRSSPost);
private:
	RDOParserRSSPost()
		: RDOParserItem(rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserSMRPost
// --------------------------------------------------------------------------------
class RDOParserSMRPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSMRPost);
private:
	RDOParserSMRPost();
	virtual void parse(CREF(LPRDOParser) pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserEVNPost
// --------------------------------------------------------------------------------
class RDOParserEVNPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserEVNPost);
private:
	RDOParserEVNPost()
		: RDOParserItem(rdoModelObjects::PAT, NULL, NULL, NULL)
		, m_currParam  (0)
	{}

	ruint             m_currParam;

	virtual void parse(CREF(LPRDOParser) pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserRTPPost
// --------------------------------------------------------------------------------
class RDOParserRTPPost: public RDOParserItem
{
	DECLARE_FACTORY(RDOParserRTPPost);
private:
	RDOParserRTPPost()
		: RDOParserItem(rdoModelObjects::RTP, NULL, NULL, NULL)
	{}

	//! Cоздание типов ресурсов в runtime
	virtual void parse(CREF(LPRDOParser) pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_RDO_H_
