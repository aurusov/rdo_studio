/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      logic.h
 * @authors   Барс Александр, Урусов Андрей, Лущан Дмитрий
 * @date      
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDOCALC_LOGIC_H_
#define _RDOCALC_LOGIC_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/binary.h"
#include "rdo_lib/rdo_runtime/calc/unary.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** Логические функции
// ********************************************************************************
class RDOCalcAnd: public RDOCalcBinary<RDOValue, (&RDOValue::operator&&), OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
	}
	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

class RDOCalcOr: public RDOCalcBinary<RDOValue, (&RDOValue::operator||), OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
	}

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECALRE_ICalc;
};

typedef RDOCalcUnary <rbool, (&RDOValue::operator! ), OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
typedef RDOCalcBinary<rbool, (&RDOValue::operator==), OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
typedef RDOCalcBinary<rbool, (&RDOValue::operator!=), OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
typedef RDOCalcBinary<rbool, (&RDOValue::operator< ), OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
typedef RDOCalcBinary<rbool, (&RDOValue::operator> ), OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
typedef RDOCalcBinary<rbool, (&RDOValue::operator<=), OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
typedef RDOCalcBinary<rbool, (&RDOValue::operator>=), OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDOCALC_LOGIC_H_
