#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOGetResourceByRelevantResourceID);
class RDOGetResourceByRelevantResourceID: public RDOCalc
{
DECLARE_FACTORY(RDOGetResourceByRelevantResourceID)
private:
    RDOGetResourceByRelevantResourceID(std::size_t relevantResourceID);

    std::size_t m_relevantResourceID;

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOEraseResRelCalc);
class RDOEraseResRelCalc: public RDOCalc
{
DECLARE_FACTORY(RDOEraseResRelCalc)
public:
    const std::string& getName() const;

private:
    RDOEraseResRelCalc(std::size_t relResID, const std::string& relResName);

    std::size_t m_relResID;
    std::string m_relResName;

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetGroupFunctionResource);
class RDOCalcGetGroupFunctionResource: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetGroupFunctionResource)
private:
    RDOCalcGetGroupFunctionResource();

    DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
