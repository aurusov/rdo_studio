#pragma once

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
    virtual void parse(const LPRDOParser& pParser);
    virtual std::size_t lexer_loc_line();
    virtual std::size_t lexer_loc_pos();

    std::string text() const;

protected:
    RDOParserRDOItem(rdo::model::FileType _type, t_bison_parse_fun _parser_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository);
    virtual ~RDOParserRDOItem();

    RDOLexer* m_pLexer;
    YYLTYPE m_loc;

private:
    void parse(const LPRDOParser& pParser, std::istream& in_stream);
    RDOLexer* getLexer(const LPRDOParser& pParser, std::istream* in_stream, std::ostream* out_stream);
};
DECLARE_POINTER(RDOParserRDOItem);

CLOSE_RDO_PARSER_NAMESPACE
