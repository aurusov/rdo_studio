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
REF(RDOValue) RDOCalcDiv::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<calc_type>();
	REF(RDOValue) rVal = m_pRight->calcValue(pRuntime);
	if (rVal == 0)
	{
		pRuntime->error(_T("Деление на ноль") , this);
//		pRuntime->error(_T("Division by zero"), this);
	}
	m_value = m_pLeft->calcValue(pRuntime) / rVal;
	return m_value;
}

REF(RDOValue) RDOCalcPlusEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<calc_type>();
	m_value = m_pLeft->calcValue(pRuntime).getEnumAsInt() + m_pRight->calcValue(pRuntime).getEnumAsInt();
	return m_value;
}

REF(RDOValue) RDOCalcMultEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<calc_type>();
	m_value = m_pLeft->calcValue(pRuntime).getEnumAsInt() * m_pRight->calcValue(pRuntime).getEnumAsInt();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
