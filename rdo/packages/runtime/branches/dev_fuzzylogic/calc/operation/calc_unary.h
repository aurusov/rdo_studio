/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_unary.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Унарные операторы
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_UNARY_H_
#define _LIB_RUNTIME_CALC_OPERATION_UNARY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовый класс для унарного оператора
class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);

protected:
	RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;
};

/*!
  \class   RDOCalcUnary
  \tparam  ret_type  Возвращаемое значение
  \tparam  pOperator Оператор
  \tparam  CalcType  Тип бинарного оператора
  \brief   Базовый унарный оператор
*/
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
	DECLARE_ICalc;
};

//! Унарный оператор плюс
typedef RDOCalcUnary<RDOValue, &RDOValue::operator-, OperatorType::OT_ARITHM> RDOCalcUMinus;

//! Оператор праобразования вещественного числа в целое
typedef RDOCalcUnary<rsint,    &RDOValue::getInt   , OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

//! Преобразование вещественного в целое по типу lvalue
CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalc)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound();

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper);

	rbool     m_round;
	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

//! Приведение к целому
CALC_SUB(RDOCalcInt, RDOCalc)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_unary.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_UNARY_H_
