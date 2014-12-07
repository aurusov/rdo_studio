#pragma once

#ifdef CORBA_ENABLE

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_rdo.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserCorbaRTP
// --------------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRTP);
private:
    RDOParserCorbaRTP()
        : RDOParserRDOItem(rdo::model::RTP, NULL, NULL, NULL)
    {}
    virtual void parse(const LPRDOParser& pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserCorbaRSS
// --------------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRSS);
private:
    RDOParserCorbaRSS()
        : RDOParserRDOItem(rdo::model::RSS, NULL, NULL, NULL)
    {}
    virtual void parse(const LPRDOParser& pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // CORBA_ENABLE
