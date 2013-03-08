/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор GENERATE
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/process/generate.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
void RDOPROCGenerate::onStart(CREF(LPRDORuntime) pRuntime)
{
	calcNextTimeInterval(pRuntime);
}

rbool RDOPROCGenerate::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	if (m_maxCreateTransactCount && m_createdTransactCount >= m_maxCreateTransactCount.get())
	{
		return false;
	}

	return pRuntime->getCurrentTime() >= timeNext ? true : false;
}

IBaseOperation::BOResult RDOPROCGenerate::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	++m_createdTransactCount;

	if (m_pStatistics)
		m_pStatistics->setTransCount(m_createdTransactCount);

	LPRDOPROCTransact pTransact = m_pCreateAndGoOnTransactCalc->calcValue(pRuntime).getPointerSafety<RDOResourceTypeTransact>();
	ASSERT(pTransact);

	pTransact->setBlock(this);
	pTransact->next();

	PTR(RDOTrace) tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << pTransact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}

	calcNextTimeInterval(pRuntime);
	return IBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->addTimePoint( timeNext = m_pTimeCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime(), m_process, this );
}

void RDOPROCGenerate::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCGenerate::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCGenerate::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCGenerate::setStatistics(CREF(LPIInternalStatistics) pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
