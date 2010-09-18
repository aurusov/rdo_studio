/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_CONVERTOR_RDO_H_
#define _CONVERTOR_RDOCONVERTER_CONVERTOR_RDO_H_

// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser_base.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
class Converter;

class RDOParserRDOItem: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRDOItem);
public:
	virtual void  parse         (PTR(Converter) pParser);
	virtual ruint lexer_loc_line();
	virtual ruint lexer_loc_pos ();

protected:
	RDOParserRDOItem(rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository);
	virtual ~RDOParserRDOItem();

	PTR(RDOLexer)  m_pLexer;
	YYLTYPE        m_loc;

private:
	void parse(PTR(Converter) pParser, REF(std::istream) in_stream);
	PTR(RDOLexer) getLexer(PTR(Converter) pParser, PTR(std::istream) in_stream, PTR(std::ostream) out_stream);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserRSS);
private:
	RDOParserRSS(StreamFrom from = sf_repository);
	virtual void parse(PTR(Converter) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRSSPost);
private:
	RDOParserRSSPost()
		: RDOParserItem(rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(Converter) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserEVNPost
// ----------------------------------------------------------------------------
class RDOParserEVNPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserEVNPost);
private:
	RDOParserEVNPost()
		: RDOParserItem(rdoModelObjects::PAT, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(Converter) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
	RDOParserSTDFUN()
		: RDOParserItem(rdoModelObjects::FUN, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(Converter) pParser);
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_CONVERTOR_RDO_H_
