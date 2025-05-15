#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalcBinaryBase: public RDOCalc
{
public:
    template <class T>
    static LPRDOCalc generateCalc(const LPRDOCalc& pFirst, const LPRDOCalc& pSecond);

protected:
    RDOCalcBinaryBase(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

    LPRDOCalc  m_pLeft;
    LPRDOCalc  m_pRight;
};

template <typename ret_type, ret_type (RDOValue::*pMethod)(const RDOValue&) const>
struct BinaryOperatorConstP1
{
    typedef ret_type (RDOValue::*method_type)(const RDOValue&) const;

    static method_type method();
};

template <typename ret_type, ret_type (RDOValue::*pMethod)(const RDOValue&)>
struct BinaryOperatorNonConstP1
{
    typedef ret_type (RDOValue::*method_type)(const RDOValue&);

    static method_type method();
};

template <class F, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalcBinaryBase
{
friend class rdo::Factory<RDOCalcBinary<F, CalcType>>;
public:
    enum class Type
    {
        calc_type = static_cast<int>(CalcType)
    };
    typedef F caller_type;

    LPRDOCalc      getLeft        () const;
    LPRDOCalcConst getRightAsConst() const;
    void           setRight       (const LPRDOCalc& pRight);

    static RDOSrcInfo getStaticSrcInfo(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

protected:
    RDOCalcBinary(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

private:
    DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_binary-inl.h"
