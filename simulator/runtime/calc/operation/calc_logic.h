#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalcAnd: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator&& >, OperatorType::Type::LOGIC>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
    typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator&& >, OperatorType::Type::LOGIC> parent_type;

    RDOCalcAnd(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    RDOValue m_value_true;
    RDOValue m_value_false;

    DECLARE_ICalc;
};

class RDOCalcOr: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator|| >, OperatorType::Type::LOGIC>
{
DECLARE_FACTORY(RDOCalcOr)
private:
    typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator|| >, OperatorType::Type::LOGIC> parent_type;

    RDOCalcOr(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    RDOValue m_value_true;
    RDOValue m_value_false;

    DECLARE_ICalc;
};

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcUnary<bool, (&RDOValue::operator!), OperatorType::Type::LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcUnary<bool,  &RDOValue::operator!,  OperatorType::Type::LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator==) >, OperatorType::Type::LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator== >,  OperatorType::Type::LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator!=) >, OperatorType::Type::LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator!= >,  OperatorType::Type::LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator<) >, OperatorType::Type::LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator< >,  OperatorType::Type::LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator>) >, OperatorType::Type::LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator> >,  OperatorType::Type::LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator<=) >, OperatorType::Type::LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator<= >,  OperatorType::Type::LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif

#ifdef COMPILER_VISUAL_STUDIO
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool, (&RDOValue::operator>=) >, OperatorType::Type::LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif
#ifdef COMPILER_GCC
    typedef RDOCalcBinary<BinaryOperatorConstP1<bool,  &RDOValue::operator>= >,  OperatorType::Type::LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif

CLOSE_RDO_RUNTIME_NAMESPACE
