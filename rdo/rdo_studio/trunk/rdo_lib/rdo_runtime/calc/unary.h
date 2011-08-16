/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      unary.h
 * @authors   Барс Александр, Урусов Андрей
 * @date      13.03.2011
 * @brief     Унарные операторы
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_UNARY_H_
#define _LIB_RUNTIME_CALC_UNARY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalcUnaryBase
 * @brief   Базовый класс для унарного оператора
 *********************************************************************************/
class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);

protected:
	RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;
};

/******************************************************************************//**
 * @class   RDOCalcUnary
 * @tparam  ret_type  Возвращаемое значение
 * @tparam  pOperator Оператор
 * @tparam  CalcType  Тип бинарного оператора
 * @brief   Базовый унарный оператор
 *********************************************************************************/
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
class RDOCalcUnary: public RDOCalcUnaryBase
{
friend class rdo::Factory<RDOCalcUnary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)() const;

	static RDOSrcInfo     getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);
	static value_operator getOperation    ();

protected:
	RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation);

private:
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);
};

/******************************************************************************//**
 * @typedef RDOCalcUMinus
 * @brief   Унарный оператор плюс
 *********************************************************************************/
typedef RDOCalcUnary<RDOValue, (&RDOValue::operator-), OperatorType::OT_ARITHM> RDOCalcUMinus;

/******************************************************************************//**
 * @typedef RDOCalcDoubleToInt
 * @brief   Оператор праобразования вещественного числа в целое
 *********************************************************************************/
typedef RDOCalcUnary<rsint,    (&RDOValue::getInt   ), OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

/******************************************************************************//**
 * @class   RDOCalcDoubleToIntByResult
 * @brief   Преобразование вещественного в целое как результат
 *********************************************************************************/
CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalc)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound();

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper);

	rbool     m_round;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcCheckDiap
 * @brief   Проверка диапазона
 *********************************************************************************/
CALC_SUB(RDOCalcCheckDiap, RDOCalc)
{
DECLARE_FACTORY(RDOCalcCheckDiap)
private:
	RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper);

	RDOValue  m_min_value;
	RDOValue  m_max_value;
	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcInt
 * @brief   Приведение к целому
 *********************************************************************************/
CALC_SUB(RDOCalcInt, RDOCalc)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/unary.inl"

#endif // _LIB_RUNTIME_CALC_UNARY_H_
