#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcProcessControl);
class RDOCalcProcessControl: public RDOCalc
{
DECLARE_FACTORY(RDOCalcProcessControl)
public:
protected:
    RDOCalcProcessControl(LPIPROCBlock pBlock, int relResNum);
    DECLARE_ICalc;
private:
    LPIPROCBlock m_Block;
    int m_relResNum;
};

PREDECLARE_POINTER(RDOCalcProcAssign);
class RDOCalcProcAssign: public RDOCalc
{
DECLARE_FACTORY(RDOCalcProcAssign)
protected:
    RDOCalcProcAssign(const LPRDOCalc& pCalc, std::size_t res, std::size_t param);
    DECLARE_ICalc;
private:
    LPRDOCalc m_pCalc;
    std::size_t m_res;
    std::size_t m_param;
};

PREDECLARE_POINTER(RDOCalcGetTermNow);
class RDOCalcGetTermNow: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
