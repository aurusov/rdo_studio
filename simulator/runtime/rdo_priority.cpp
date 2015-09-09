// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternPrior
// --------------------------------------------------------------------------------
RDOPatternPrior::RDOPatternPrior()
{}

RDOPatternPrior::~RDOPatternPrior()
{}

LPRDOCalc RDOPatternPrior::getPrior()
{
    return m_pPrior;
}

bool RDOPatternPrior::setPrior(const LPRDOCalc& pPrior)
{
    m_pPrior = pPrior;
    return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityCompare
// --------------------------------------------------------------------------------
RDODPTActivityCompare::RDODPTActivityCompare(const LPRDORuntime& pRuntime)
    : m_pRuntime(pRuntime)
{}

bool RDODPTActivityCompare::operator()(const LPIBaseOperation& pOpr1, const LPIBaseOperation& pOpr2)
{
    LPIPriority pPattern1 = pOpr1.object_dynamic_cast<IPriority>();
    LPIPriority pPattern2 = pOpr2.object_dynamic_cast<IPriority>();
    if (pPattern1 && pPattern2)
    {
        LPRDOCalc pPrior1 = pPattern1->getPrior();
        LPRDOCalc pPrior2 = pPattern2->getPrior();
        RDOValue value1 = pPrior1 ? pPrior1->calcValue(m_pRuntime) : RDOValue(0.0);
        RDOValue value2 = pPrior2 ? pPrior2->calcValue(m_pRuntime) : RDOValue(0.0);
        return value1 > value2;
    }
    return false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
