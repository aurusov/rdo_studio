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
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl()
{
//создать от параметра-ресурса транзакт и проинициализировать его первым блоком процесса
}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) pRuntime)
{
	LPIBaseOperation pLogic = *(++(pRuntime->m_pMetaLogic->begin()));
	if (pLogic)
	{
		LPIPROCProcess pProcess = pLogic.query_cast<IPROCProcess>();
		if (pProcess)
		{
			LPIBaseOperationContainer pBlockContainer = pProcess.query_cast<IBaseOperationContainer>();
			ASSERT(pBlockContainer);
			if (!pBlockContainer->empty())
			{
				LPIBaseOperation pBlock = *pBlockContainer->begin();
				ASSERT(pBlock);
				PTR(RDOPROCTransact) pTransact = new RDOPROCTransact(pRuntime, pBlock);
				ASSERT(pTransact);

				pTransact->setBlock(pBlock);
				// Записываем в конец списка этого блока перемещаемый транзакт
				pBlock.query_cast<IPROCBlock>()->transactGoIn(pTransact);
			}
		}
	}

//	LPIPROCBlock   block     = dynamic_cast<LPIPROCBlock>(process-begin());
//	RDOLogicSimple process = std::find(metaLogic->begin(), metaLogic->end(), 'Process');
//	process->begin();//первый блок
//в простейшей модели (без БЗ), только с одним процессом должен сработать халявный вызов:
//	new RDOPROCTransact( pRuntime, block );
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
