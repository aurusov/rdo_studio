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
#include "simulator/runtime/process/terminate.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(const LPIPROCProcess& pProcess, const LPRDOCalc& pCalc)
	: RDOPROCBlock             (pProcess)
	, m_terminatedTransactCount(0       )
	, m_pTermCalc              (pCalc   )
{}

bool RDOPROCTerminate::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
	return !m_transacts.empty() ? true : false;
}

IBaseOperation::BOResult RDOPROCTerminate::onDoOperation(const LPRDORuntime& pRuntime)
{
	TRACE1("%7.1f TERMINATE\n", pRuntime->getCurrentTime());
	LPRDOPROCTransact transact = m_transacts.front();
	ASSERT(transact);
	transact->setState(RDOResource::CS_Erase);
	RDOTrace* tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << transact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}
	pRuntime->onEraseRes(transact->getTraceID(), NULL);
	m_transacts.erase(m_transacts.begin());
	std::size_t termNow = pRuntime->getCurrentTerm();

	++m_terminatedTransactCount;

	if (m_pStatistics)
		m_pStatistics->setTransCount(m_terminatedTransactCount);

	termNow += m_pTermCalc->calcValue(pRuntime).getInt();
	pRuntime->setCurrentTerm(termNow);
	return IBaseOperation::BOR_done;
}

void RDOPROCTerminate::onStart(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCTerminate::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCTerminate::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCTerminate::setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
