#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_base.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_object.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
class Converter;

class RDOParserRDOItem: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRDOItem);
public:
	virtual void parse(Converter* pParser, std::istream& streamIn);
	virtual std::size_t lexer_loc_line();
	virtual std::size_t lexer_loc_pos();

protected:
	RDOParserRDOItem(rdo::converter::smr2rdox::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_flex_lexer_fun lexer_fun);
	virtual ~RDOParserRDOItem();

	RDOLexer* m_pLexer;
	YYLTYPE m_loc;

private:
	RDOLexer* getLexer(Converter* pParser, std::istream* streamIn, std::ostream* streamOut);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSS
// --------------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserRSS);
private:
	RDOParserRSS();
	virtual void parse(Converter* pParser, std::istream& streamIn);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSSPost
// --------------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRSSPost);
private:
	RDOParserRSSPost();
	virtual void parse(Converter* pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserSTDFUN
// --------------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
	RDOParserSTDFUN();
	virtual void parse(Converter* pParser);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
