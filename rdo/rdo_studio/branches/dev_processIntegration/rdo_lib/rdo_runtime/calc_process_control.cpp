/*
 * copyright: (c) RDO-Team, 2011
 * filename : calc_process_control.cpp
 * author   : Лущан Дмитрий
 * date     : 10.03.2011
 * bref     : RDOCalc для передачи управления в процесс из БЗ и событий
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc_process_control.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcProcessControl
// ----------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum)
	: m_Block    (pBlock   )
	, m_relResNum(relResNum)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) pRuntime)
{
//по m_relResNum нужно найти ресурс (m_Transact) и передать его в процесс
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResNum);
	PTR(RDOResource) pResource = pRuntime->getResourceByID(resID);
	PTR(RDOPROCTransact) pTransact = dynamic_cast<PTR(RDOPROCTransact)>(pResource);
	if (pTransact)
	{
		pTransact->setBlock(m_Block);
		// Записываем в конец списка этого блока перемещаемый транзакт
		m_Block.query_cast<IPROCBlock>()->transactGoIn(pTransact);
	}

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
