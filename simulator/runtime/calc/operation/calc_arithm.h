#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

typedef RDOCalcBinary<BinaryOperatorNonConstP1<RDOValue&, &RDOValue::operator= >, OperatorType::Type::ARITHM> RDOCalcSet;  DECLARE_POINTER(RDOCalcSet);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator+ >, OperatorType::Type::ARITHM> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator- >, OperatorType::Type::ARITHM> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator* >, OperatorType::Type::ARITHM> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

class RDOCalcDiv: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::Type::ARITHM>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
    typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::Type::ARITHM> parent_type;

    RDOCalcDiv(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    DECLARE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

PREDECLARE_POINTER(RDOCalcPlusEnumSafe);
class RDOCalcPlusEnumSafe: public RDOCalcPlus
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
    RDOCalcPlusEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcMultEnumSafe);
class RDOCalcMultEnumSafe: public RDOCalcMult
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
    RDOCalcMultEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
