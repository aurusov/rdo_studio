// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcUnaryBase
// --------------------------------------------------------------------------------
RDOCalcUnaryBase::RDOCalcUnaryBase(const LPRDOCalc& pOperation)
	: m_pOperation(pOperation)
{
	ASSERT(m_pOperation);
}

// --------------------------------------------------------------------------------
// -------------------- Унарные операции
// --------------------------------------------------------------------------------
RDOCalcDoubleToIntByResult::RDOCalcDoubleToIntByResult(const LPRDOCalc& pOper)
	: m_round     (false)
	, m_pOperation(pOper)
{}

RDOValue RDOCalcDoubleToIntByResult::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue value = m_pOperation->calcValue(pRuntime);
	return m_round
		? value.getInt()
		: value;
}

void RDOCalcDoubleToIntByResult::needRound()
{
	m_round = true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt (приведение к целому)
// --------------------------------------------------------------------------------
RDOCalcInt::RDOCalcInt(const LPRDOCalc& pOperation)
	: m_pOperation(pOperation)
{}

RDOValue RDOCalcInt::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue res = m_pOperation->calcValue(pRuntime);
	return res > 0
		? RDOValue((int)(res.getDouble() + 0.5))
		: RDOValue((int)(res.getDouble() - 0.5));
}

CLOSE_RDO_RUNTIME_NAMESPACE
