#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

PREDECLARE_POINTER(RDOLogicBase);
class RDOLogicBase
    : public virtual rdo::counter_reference
    , public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOLogicBase);
public:
    const std::string& name () const;
    bool setPrior(LPRDOFUNArithm& pPrior);
    bool getMultithreading() const;
    void setMultithreading(bool multithreading = false);
    void setCondition(const LPRDOFUNLogic& pConditon = NULL);
    LPRDOFUNLogic getConditon() const;
    LPILogic getLogic() const;
    virtual void end();

protected:
    RDOLogicBase(const RDOParserSrcInfo& src_info);
    virtual ~RDOLogicBase();

    LPRDOFUNLogic m_pConditon;
    LPILogic m_pRuntimeLogic;
    bool m_multithreading;
};

CLOSE_RDO_PARSER_NAMESPACE
