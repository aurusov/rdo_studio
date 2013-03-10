/*!
  \copyright (c) RDO-Team, 2012
  \file      terminate.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор TERMINATE
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/process/terminate.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
rbool RDOPROCTerminate::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return !m_transacts.empty() ? true : false;
}

IBaseOperation::BOResult RDOPROCTerminate::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	TRACE1("%7.1f TERMINATE\n", pRuntime->getCurrentTime());
	LPRDOPROCTransact transact = m_transacts.front();
	ASSERT(transact);
	transact->setState(RDOResource::CS_Erase);
	PTR(RDOTrace) tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << transact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}
	pRuntime->onEraseRes(transact->getTraceID(), NULL);
	m_transacts.erase(m_transacts.begin());
	ruint termNow = pRuntime->getCurrentTerm();

	++m_terminatedTransactCount;

	if (m_pStatistics)
		m_pStatistics->setTransCount(m_terminatedTransactCount);

	termNow += m_pTermCalc->calcValue(pRuntime).getInt();
	pRuntime->setCurrentTerm(termNow);
	return IBaseOperation::BOR_done;
}

void RDOPROCTerminate::onStart(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCTerminate::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCTerminate::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCTerminate::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCTerminate::setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
