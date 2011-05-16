/*
 * copyright: (c) RDO-Team, 2011
 * filename : array.cpp
 * author   : Чирков Михаил
 * date     : 02.05.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/array.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcArraySize
// ----------------------------------------------------------------------------
RDOCalcArraySize::RDOCalcArraySize(CREF(LPRDOCalc) pArray)
	: m_pArray(pArray)
{}

REF(RDOValue) RDOCalcArraySize::doCalc(PTR(RDORuntime) pRuntime)
{
	CREF(RDOArrayValue) arrayValue = m_pArray->calcValue(pRuntime).getArray();
	m_value = RDOValue(arrayValue.arraySize());
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcArrayItem
// ----------------------------------------------------------------------------
RDOCalcArrayItem::RDOCalcArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd)
	: m_pArray   (pArray   )
	, m_pArrayInd(pArrayInd)
{}

REF(RDOValue) RDOCalcArrayItem::doCalc(PTR(RDORuntime) pRuntime)
{
	CREF(RDOArrayValue) arrayValue = m_pArray->calcValue(pRuntime).getArray();
	m_value = arrayValue[m_pArrayInd->calcValue(pRuntime)];
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
