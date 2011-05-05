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
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock, PTR(RDOPROCTransact) pTransact)
	: m_Block   (pBlock   )
	, m_Transact(pTransact)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) pRuntime)
{
	m_Transact->setBlock(m_Block);
	// Записываем в конец списка этого блока перемещаемый транзакт
	m_Block.query_cast<IPROCBlock>()->transactGoIn(m_Transact);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
