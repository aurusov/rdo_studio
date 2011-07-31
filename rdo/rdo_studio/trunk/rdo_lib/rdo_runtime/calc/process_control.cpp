/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      calc_process_control.cpp
 * @authors   Лущан Дмитрий
 * @date      10.03.2011
 * @brief     RDOCalc для передачи управления в процесс из БЗ и событий
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/process_control.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_activity_i.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcProcessControl
// ********************************************************************************
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

CLOSE_RDO_RUNTIME_NAMESPACE
