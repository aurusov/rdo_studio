#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSTDFUN
// --------------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
    RDOParserSTDFUN()
        : RDOParserItem(rdo::model::FUN, NULL, NULL)
    {}

    virtual void parse(const LPRDOParser& pParser);

    typedef rdo::vector<LPTypeInfo> ParamList;
    void generate(const std::string& name, const rdo::runtime::LPRDOFunCalc& pCalc, const LPRDOParam& pReturnType, const ParamList& paramList) const;
    void generateReal(const std::string& name, const rdo::runtime::LPRDOFunCalc& pCalc, const LPRDOParam& pReturnType, const ParamList& paramList) const;
};

CLOSE_RDO_PARSER_NAMESPACE
