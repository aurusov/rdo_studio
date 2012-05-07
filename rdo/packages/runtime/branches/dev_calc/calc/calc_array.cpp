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

void RDOCalcArraySize::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	CREF(LPRDOArrayValue) pArrayValue = pRuntime->stack().pop().getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);
	pRuntime->stack().push(RDOValue(pArrayValue->size()));
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

void RDOCalcArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = pRuntime->stack().pop().getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	m_pArrayInd->calcValue(pRuntime);
	pRuntime->stack().push(pArrayValue->getItem(pRuntime->stack().pop()));
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

void RDOCalcSetArrayItem::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = pRuntime->stack().pop().getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	m_pArrayInd->calcValue(pRuntime);
	m_pSetItem ->calcValue(pRuntime);

	RDOValue value = pRuntime->stack().pop();
	RDOValue index = pRuntime->stack().pop();
	pArrayValue->setItem(index, value);
}

CLOSE_RDO_RUNTIME_NAMESPACE
