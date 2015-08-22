// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcArraySize
// --------------------------------------------------------------------------------
RDOCalcArraySize::RDOCalcArraySize(const LPRDOCalc& pCalc)
    : m_pCalc(pCalc)
{}

RDOValue RDOCalcArraySize::doCalc(const LPRDORuntime& pRuntime)
{
    RDOValue value = m_pCalc->calcValue(pRuntime);
    const LPRDOArrayValue& pArrayValue = value.getPointerByType<RDOArrayType>();
    ASSERT(pArrayValue);
    return pArrayValue->size();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcArrayItem
// --------------------------------------------------------------------------------
RDOCalcArrayItem::RDOCalcArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd)
    : m_pArray   (pArray   )
    , m_pArrayInd(pArrayInd)
{
    ASSERT(m_pArray   );
    ASSERT(m_pArrayInd);

    setSrcInfo(m_pArrayInd->srcInfo());
}

RDOValue RDOCalcArrayItem::doCalc(const LPRDORuntime& pRuntime)
{
    RDOValue value = m_pArray->calcValue(pRuntime);

    const LPRDOArrayValue& pArrayValue = value.getPointerByType<RDOArrayType>();
    ASSERT(pArrayValue);

    return pArrayValue->getItem(m_pArrayInd->calcValue(pRuntime));
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetArrayItem
// --------------------------------------------------------------------------------
RDOCalcSetArrayItem::RDOCalcSetArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd, const LPRDOCalc& pSetItem)
    : m_pArray   (pArray   )
    , m_pArrayInd(pArrayInd)
    , m_pSetItem (pSetItem )
{
    ASSERT(m_pArray   );
    ASSERT(m_pArrayInd);
    ASSERT(m_pSetItem );

    setSrcInfo(m_pArrayInd->srcInfo());
}

RDOValue RDOCalcSetArrayItem::doCalc(const LPRDORuntime& pRuntime)
{
    RDOValue value = m_pArray->calcValue(pRuntime);

    const LPRDOArrayValue& pArrayValue = value.getPointerByType<RDOArrayType>();
    ASSERT(pArrayValue);
    pArrayValue->setItem(m_pArrayInd->calcValue(pRuntime), m_pSetItem->calcValue(pRuntime));

    return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
