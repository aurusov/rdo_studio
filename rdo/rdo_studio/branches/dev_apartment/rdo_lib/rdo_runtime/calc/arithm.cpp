/*
 * copyright: (c) RDO-Team, 2011
 * filename : arithm.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/arithm.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcDiv::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<calc_type>();
	REF(RDOValue) rVal = m_pRight->calcValue(runtime);
	if (rVal == 0)
	{
		runtime->error(_T("Деление на ноль") , this);
//		runtime->error(_T("Division by zero"), this);
	}
	m_value = m_pLeft->calcValue(runtime) / rVal;
	return m_value;
}

REF(RDOValue) RDOCalcPlusEnumSafe::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<calc_type>();
	m_value = m_pLeft->calcValue(runtime).getEnumAsInt() + m_pRight->calcValue(runtime).getEnumAsInt();
	return m_value;
}

REF(RDOValue) RDOCalcMultEnumSafe::doCalc(PTR(RDORuntime) runtime)
{
	++OperatorType::getCalcCounter<calc_type>();
	m_value = m_pLeft->calcValue(runtime).getEnumAsInt() * m_pRight->calcValue(runtime).getEnumAsInt();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
