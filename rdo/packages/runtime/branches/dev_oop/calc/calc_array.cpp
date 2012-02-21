/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_array.cpp
  \author    Чирков Михаил
  \date      02.05.2011
  \brief     Агрегатный тип данных - массив
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcArraySize
// --------------------------------------------------------------------------------
RDOCalcArraySize::RDOCalcArraySize(CREF(LPRDOCalc) pCalc)
	: m_pCalc(pCalc)
{}

REF(RDOValue) RDOCalcArraySize::doCalc(CREF(LPRDORuntime) pRuntime)
{
	REF(RDOValue) value = m_pCalc->calcValue(pRuntime);
	LPRDOArrayType pArrayType = value.type().object_dynamic_cast<RDOArrayType>();
	ASSERT(pArrayType);
	CREF(LPRDOArrayValue) pArrayValue = value.getPointer<RDOArrayValue>();
	ASSERT(pArrayValue);
	m_value = RDOValue(pArrayValue->size());
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcArrayItem
// --------------------------------------------------------------------------------
RDOCalcArrayItem::RDOCalcArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd)
	: m_pArray   (pArray   )
	, m_pArrayInd(pArrayInd)
{
	setSrcInfo(m_pArrayInd->srcInfo());
}

REF(RDOValue) RDOCalcArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	REF(RDOValue) value = m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	m_value = pArrayValue->getItem(m_pArrayInd->calcValue(pRuntime), srcInfo(), pRuntime);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetArrayItem
// --------------------------------------------------------------------------------
RDOCalcSetArrayItem::RDOCalcSetArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd, CREF(LPRDOCalc) pSetItem)
	: m_pArray   (pArray   )
	, m_pArrayInd(pArrayInd)
	, m_pSetItem (pSetItem )
{
	setSrcInfo(m_pArrayInd->srcInfo());
}

REF(RDOValue) RDOCalcSetArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pArray->calcValue(pRuntime);
	
	CREF(LPRDOArrayValue) pArrayValue = m_value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);
	pArrayValue->setItem(m_pArrayInd->calcValue(pRuntime), m_pSetItem->calcValue(pRuntime), srcInfo(), pRuntime);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
