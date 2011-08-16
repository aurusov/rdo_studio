/*!
  \copyright (c) RDO-Team, 2011
  \file      logic.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Логические операторы
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_LOGIC_H_
#define _LIB_RUNTIME_CALC_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/binary.h"
#include "rdo_lib/rdo_runtime/calc/unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcAnd
  \brief   Оператор логическое И
*/
class RDOCalcAnd: public RDOCalcBinary<RDOValue, (&RDOValue::operator&&), OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

/*!
  \class   RDOCalcOr
  \brief   Оператор логическое ИЛИ
*/
class RDOCalcOr: public RDOCalcBinary<RDOValue, (&RDOValue::operator||), OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

/*!
  \typedef RDOCalcNot
  \brief   Оператор отрицания
*/
typedef RDOCalcUnary <rbool, (&RDOValue::operator! ), OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);

/*!
  \typedef RDOCalcIsEqual
  \brief   Оператор равенства
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator==), OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);

/*!
  \typedef RDOCalcIsNotEqual
  \brief   Оператор неравенства
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator!=), OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);

/*!
  \typedef RDOCalcIsLess
  \brief   Оператор меньше
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator< ), OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);

/*!
  \typedef RDOCalcIsGreater
  \brief   Оператор больше
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator> ), OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);

/*!
  \typedef RDOCalcIsLEQ
  \brief   Оператор меньше или равно
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator<=), OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);

/*!
  \typedef RDOCalcIsGEQ
  \brief   Оператор больше или равно
*/
typedef RDOCalcBinary<rbool, (&RDOValue::operator>=), OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/logic.inl"

#endif // _LIB_RUNTIME_CALC_LOGIC_H_
