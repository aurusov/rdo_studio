// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateLocalVariable
// --------------------------------------------------------------------------------
RDOCalcCreateLocalVariable::RDOCalcCreateLocalVariable(const std::string& name, const LPRDOCalc& pValueCalc)
	: m_name      (name      )
	, m_pValueCalc(pValueCalc)
{
	ASSERT(m_pValueCalc);
}

RDOValue RDOCalcCreateLocalVariable::doCalc(const LPRDORuntime& pRuntime)
{
	RDOValue value = m_pValueCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->create(m_name, value);
	return value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetLocalVariable
// --------------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(const std::string& name)
	: m_name(name)
{}

RDOValue RDOCalcGetLocalVariable::doCalc(const LPRDORuntime& pRuntime)
{
	return pRuntime->getMemoryStack()->get(m_name);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcLocalVariableList
// --------------------------------------------------------------------------------
RDOCalcLocalVariableList::RDOCalcLocalVariableList()
{}

void RDOCalcLocalVariableList::addCalcLocalVariable(const LPRDOCalc& pCalc)
{
	ASSERT(pCalc);
	m_calcLocalVariableList.push_back(pCalc);
}

RDOValue RDOCalcLocalVariableList::doCalc(const LPRDORuntime& pRuntime)
{
	for (const auto& calc: m_calcLocalVariableList)
	{
		calc->calcValue(pRuntime);
	}
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetLocalVariable
// --------------------------------------------------------------------------------
RDOCalcSetLocalVariable::RDOCalcSetLocalVariable(const std::string& name, LPRDOCalc pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

RDOCalcSetLocalVariable::~RDOCalcSetLocalVariable()
{}

RDOValue RDOCalcSetLocalVariable::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE
