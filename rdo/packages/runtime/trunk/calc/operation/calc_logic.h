/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_logic.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Логические операторы
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_
#define _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Оператор логическое И
class RDOCalcAnd: public RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator&&>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator&&> parent_type;

	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! Оператор логическое ИЛИ
class RDOCalcOr: public RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator||>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator||> parent_type;

	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! Оператор отрицания
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcUnary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator!)> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcUnary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator! > RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif

//! Оператор равенства
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator==)> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator== > RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif

//! Оператор неравенства
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator!=)> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator!= > RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif

//! Оператор меньше
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator<)> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator< > RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif

//! Оператор больше
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator>)> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator> > RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif

//! Оператор меньше или равно
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator<=)> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator<= > RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif

//! Оператор больше или равно
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator>=)> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator>= > RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_logic.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_
