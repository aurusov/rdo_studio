/*
 * copyright: (c) RDO-Team, 2011
 * filename : logic.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/logic.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcAnd::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();

	if (!m_pLeft->calcValue(runtime).getAsBool())
		return m_value_false;

	if (!m_pRight->calcValue(runtime).getAsBool())
		return m_value_false;

	return m_value_true;
}

REF(RDOValue) RDOCalcOr::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();

	if (m_pLeft->calcValue(runtime).getAsBool())
		return m_value_true;

	if (m_pRight->calcValue(runtime).getAsBool())
		return m_value_true;

	return m_value_false;
}

REF(RDOValue) RDOCalcNot::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();
	m_value = !m_pCalc->calcValue(runtime).getAsBool();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
