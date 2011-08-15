/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      binary.h
 * @authors   Барс Александр, Урусов Андрей
 * @date      13.03.2011
 * @brief     Бинарные операторы
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_BINARY_H_
#define _LIB_RUNTIME_CALC_BINARY_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/operation_type.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalcBinaryBase
 * @brief   unknown
 *********************************************************************************/
class RDOCalcBinaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(LPRDOCalc) pFirst, CREF(LPRDOCalc) pSecond);

protected:
	RDOCalcBinaryBase(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;
};

/******************************************************************************//**
 * @class   RDOCalcBinary
 * @tparam  unknown
 * @brief   unknown
 *********************************************************************************/
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalcBinaryBase
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)(CREF(RDOValue) rdovalue) const;

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

	static RDOSrcInfo     getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);
	static value_operator getOperation    ();

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

private:
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/binary.inl"

#endif // _LIB_RUNTIME_CALC_BINARY_H_
