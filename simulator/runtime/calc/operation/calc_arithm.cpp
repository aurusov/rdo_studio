// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/operation/operation_type.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDiv
// --------------------------------------------------------------------------------
RDOCalcDiv::RDOCalcDiv(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: parent_type(pLeft, pRight)
{}

RDOValue RDOCalcDiv::doCalc(const LPRDORuntime& pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	RDOValue right = m_pRight->calcValue(pRuntime);
	if (right == 0)
	{
		pRuntime->error().push("Деление на ноль", srcInfo());
	}
	return m_pLeft->calcValue(pRuntime) / right;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
RDOCalcPlusEnumSafe::RDOCalcPlusEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: RDOCalcPlus(pLeft, pRight)
{}

RDOValue RDOCalcPlusEnumSafe::doCalc(const LPRDORuntime& pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	return m_pLeft->calcValue(pRuntime).getEnumAsInt() + m_pRight->calcValue(pRuntime).getEnumAsInt();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
RDOCalcMultEnumSafe::RDOCalcMultEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: RDOCalcMult(pLeft, pRight)
{}

RDOValue RDOCalcMultEnumSafe::doCalc(const LPRDORuntime& pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	return m_pLeft->calcValue(pRuntime).getEnumAsInt() * m_pRight->calcValue(pRuntime).getEnumAsInt();
}

CLOSE_RDO_RUNTIME_NAMESPACE
