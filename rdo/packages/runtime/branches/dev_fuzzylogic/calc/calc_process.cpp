/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_process.cpp
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      10.03.2011
  \brief     RDOCalc для передачи управления в процесс из БЗ и событий
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_activity_i.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcProcessControl
// --------------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum)
	: m_Block    (pBlock   )
	, m_relResNum(relResNum)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(CREF(LPRDORuntime) pRuntime)
{
	//по m_relResNum нужно найти ресурс (m_Transact) и передать его в процесс
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResNum);
	LPRDOResource     pResource = pRuntime->getResourceByID(resID);
	/// @todo проверить, можно ли перенести проверку в парсер, чтобы сделать object_static_cast вместо object_dynamic_cast
	LPRDOPROCTransact pTransact = pResource.object_dynamic_cast<RDOPROCTransact>();
	if (pTransact)
	{
		pTransact->setBlock(m_Block);
		// Записываем в конец списка этого блока перемещаемый транзакт
		m_Block.query_cast<IPROCBlock>()->transactGoIn(pTransact);
	}

	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCreateAndGoInTransact
// --------------------------------------------------------------------------------
RDOCalcCreateAndGoInTransact::RDOCalcCreateAndGoInTransact(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag)
	: m_pResType     (pType        )
	, m_traceFlag    (traceFlag    )
	, m_permanentFlag(permanentFlag)
{
	m_paramsCalcs.insert(m_paramsCalcs.begin(), rParamsCalcs.begin(), rParamsCalcs.end());
}

void RDOCalcCreateAndGoInTransact::setBlock(LPIPROCBlock pBlock)
{
	ASSERT(pBlock);
	m_pBlock = pBlock;
}

REF(RDOValue) RDOCalcCreateAndGoInTransact::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pBlock);

	LPRDOResource pResource = m_pResType->createRes(pRuntime, pRuntime->getResourceId(), m_paramsCalcs, m_traceFlag, m_permanentFlag);
	ASSERT(pResource);

        LPRDOType pType = m_pResType;
        ASSERT(pType);

        m_value = RDOValue(pType, pResource);
        ASSERT(m_value);

        return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTermNow
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTermNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getCurrentTerm();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
