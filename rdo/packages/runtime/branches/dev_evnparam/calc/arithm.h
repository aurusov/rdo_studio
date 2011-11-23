/*!
  \copyright (c) RDO-Team, 2011
  \file      arithm.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Арифметические операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_ARITHM_H_
#define _LIB_RUNTIME_CALC_ARITHM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \typedef RDOCalcPlus
  \brief   Бинарный оператор плюс
*/
typedef RDOCalcBinary<RDOValue, &RDOValue::operator+, OperatorType::OT_ARITHM> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);

/*!
  \typedef RDOCalcMinus
  \brief   Бинарный оператор минус
*/
typedef RDOCalcBinary<RDOValue, &RDOValue::operator-, OperatorType::OT_ARITHM> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);

/*!
  \typedef RDOCalcMult
  \brief   Бинарный оператор умножить
*/
typedef RDOCalcBinary<RDOValue, &RDOValue::operator*, OperatorType::OT_ARITHM> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

/*!
  \class   RDOCalcDiv
  \brief   Бинарный оператор умножить
*/
class RDOCalcDiv: public RDOCalcBinary<RDOValue, &RDOValue::operator/, OperatorType::OT_ARITHM>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
	typedef RDOCalcBinary<RDOValue, &RDOValue::operator/, OperatorType::OT_ARITHM> parent_type;

	RDOCalcDiv(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

/*!
  \class   RDOCalcPlusEnumSafe
  \brief   Бинарный оператор умножить
*/
CALC_SUB(RDOCalcPlusEnumSafe, RDOCalcPlus)
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcMultEnumSafe
  \brief   Бинарный оператор умножить
*/
CALC_SUB(RDOCalcMultEnumSafe, RDOCalcMult)
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/arithm.inl"

#endif // _LIB_RUNTIME_CALC_ARITHM_H_
