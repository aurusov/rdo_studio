// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/operation_name.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcUnaryBase
// --------------------------------------------------------------------------------
template <class T>
LPRDOCalc RDOCalcUnaryBase::generateCalc(const RDOSrcInfo::Position& position, const LPRDOCalc& pUnaryCalc)
{
    ASSERT(pUnaryCalc);

    LPRDOCalcConst pConstCalc = pUnaryCalc.object_dynamic_cast<RDOCalcConst>();
    LPRDOCalc pCalc;
    if (pConstCalc)
    {
        typename T::value_operator pOperation = T::getOperation();
        pCalc = rdo::Factory<RDOCalcConst>::create((pConstCalc->calcValue(NULL).*pOperation)());
        pCalc->setSrcInfo(T::getStaticSrcInfo(position, pConstCalc));
    }
    else
    {
        pCalc = rdo::Factory<T>::create(position, pUnaryCalc);
    }
    ASSERT(pCalc);
    return pCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcUnary
// --------------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOCalcUnary<ret_type, pOperator, CalcType>::RDOCalcUnary(const RDOSrcInfo::Position& position, const LPRDOCalc& pOperation)
    : RDOCalcUnaryBase(pOperation)
{
    setSrcInfo(getStaticSrcInfo(position, m_pOperation));
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOSrcInfo RDOCalcUnary<ret_type, pOperator, CalcType>::getStaticSrcInfo(const RDOSrcInfo::Position& position, const LPRDOCalc& pUnaryCalc)
{
    RDOSrcInfo srcInfo(pUnaryCalc->srcInfo());
    srcInfo.setSrcPos (position);
    srcInfo.setSrcText(rdo::format("%s(%s)", OperatorName<ret_type (RDOValue::*)() const>::name(pOperator).c_str(), pUnaryCalc->srcInfo().src_text().c_str()));
    return srcInfo;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline typename RDOCalcUnary<ret_type, pOperator, CalcType>::value_operator RDOCalcUnary<ret_type, pOperator, CalcType>::getOperation()
{
    return pOperator;
}

template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
inline RDOValue RDOCalcUnary<ret_type, pOperator, CalcType>::doCalc(const LPRDORuntime& pRuntime)
{
    ++OperatorType::getCalcCounter<CalcType>();
    return (m_pOperation->calcValue(pRuntime).*pOperator)();
}

CLOSE_RDO_RUNTIME_NAMESPACE
