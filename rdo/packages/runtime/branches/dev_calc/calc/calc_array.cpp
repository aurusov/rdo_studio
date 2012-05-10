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

RDOValue RDOCalcArraySize::doCalc(CREF(LPRDORuntime) pRuntime)
{
	CREF(LPRDOArrayValue) pArrayValue = m_pCalc->calcValue(pRuntime).getPointerSafety<RDOArrayType>();
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
	ASSERT(m_pArray   );
	ASSERT(m_pArrayInd);

	setSrcInfo(m_pArrayInd->srcInfo());
}

RDOValue RDOCalcArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	CREF(LPRDOArrayValue) pArrayValue = m_pArray->calcValue(pRuntime).getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	m_value = pArrayValue->getItem(m_pArrayInd->calcValue(pRuntime));
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
	ASSERT(m_pArray   );
	ASSERT(m_pArrayInd);
	ASSERT(m_pSetItem );

	setSrcInfo(m_pArrayInd->srcInfo());
}

RDOValue RDOCalcSetArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = m_value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);
	pArrayValue->setItem(m_pArrayInd->calcValue(pRuntime), m_pSetItem->calcValue(pRuntime));

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
