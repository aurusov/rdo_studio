/*
 * copyright: (c) RDO-Team, 2011
 * filename : unary.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/unary.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcUMinus::doCalc(PTR(RDORuntime) runtime)
{
	m_value = -m_pOperation->calcValue(runtime);
	return m_value;
}

REF(RDOValue) RDOCalcDoubleToInt::doCalc(PTR(RDORuntime) runtime)
{
	m_value = m_pOperation->calcValue( runtime ).getInt();
	return m_value;
}

REF(RDOValue) RDOCalcDoubleToIntByResult::doCalc(PTR(RDORuntime) runtime)
{
	m_value = m_round ? RDOValue(m_pOperation->calcValue(runtime).getInt()) : m_pOperation->calcValue(runtime);
	return m_value;
}

REF(RDOValue) RDOCalcCheckDiap::doCalc(PTR(RDORuntime) runtime)
{
	m_value = m_pOperation->calcValue(runtime);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	return m_value; 
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcInt::doCalc(PTR(RDORuntime) runtime)
{
	RDOValue res = m_pOperation->calcValue(runtime);
	m_value = res > 0 ? RDOValue((int)(res.getDouble() + 0.5)) : RDOValue((int)(res.getDouble() - 0.5));
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
