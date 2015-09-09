// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBinaryBase
// --------------------------------------------------------------------------------
RDOCalcBinaryBase::RDOCalcBinaryBase(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
    : m_pLeft (pLeft )
    , m_pRight(pRight)
{
    ASSERT(m_pLeft );
    ASSERT(m_pRight);
}

CLOSE_RDO_RUNTIME_NAMESPACE
