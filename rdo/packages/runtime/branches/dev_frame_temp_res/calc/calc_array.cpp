/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_array.cpp
  \author    Чирков Михаил
  \date      02.05.2011
  \brief     Агрегатный тип данных - массив
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_res_type.h"
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
	RDOValue value = m_pCalc->calcValue(pRuntime);
	CREF(LPRDOArrayValue) pArrayValue = value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);
	return pArrayValue->size();
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
	RDOValue value = m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	return pArrayValue->getItem(m_pArrayInd->calcValue(pRuntime));
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcArrayItemParam
// --------------------------------------------------------------------------------
RDOCalcArrayItemParam::RDOCalcArrayItemParam(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd, CREF(LPRDOCalc) pParamInd)
	: m_pArray   (pArray   )
	, m_pArrayInd(pArrayInd)
	, m_pParamInd(pParamInd)
{
	ASSERT(m_pArray   );
	ASSERT(m_pArrayInd);
	ASSERT(m_pParamInd);

	//! @todo Надо назначать на основе обоих индексов
	setSrcInfo(m_pArrayInd->srcInfo());
}

RDOValue RDOCalcArrayItemParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue value = m_pArray->calcValue(pRuntime);
	RDOValue param_ind = m_pParamInd->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);

	RDOValue pArrayItem = pArrayValue->getItem(m_pArrayInd->calcValue(pRuntime));

	LPRDOResource pResource = pArrayItem.getPointerSafety<RDOResourceType>();
	ASSERT(pResource);

	return pResource->getParam(param_ind.getInt());
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
	RDOValue value = m_pArray->calcValue(pRuntime);

	CREF(LPRDOArrayValue) pArrayValue = value.getPointerSafety<RDOArrayType>();
	ASSERT(pArrayValue);
	pArrayValue->setItem(m_pArrayInd->calcValue(pRuntime), m_pSetItem->calcValue(pRuntime));

	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
