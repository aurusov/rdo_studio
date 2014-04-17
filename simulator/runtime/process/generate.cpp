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
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/process/generate.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
void RDOPROCGenerate::onStart(const LPRDORuntime& pRuntime)
{
	calcNextTimeInterval(pRuntime);
}

bool RDOPROCGenerate::onCheckCondition(const LPRDORuntime& pRuntime)
{
	if (m_maxCreateTransactCount && m_createdTransactCount >= m_maxCreateTransactCount.get())
	{
		return false;
	}

	return pRuntime->getCurrentTime() >= timeNext ? true : false;
}

IBaseOperation::BOResult RDOPROCGenerate::onDoOperation(const LPRDORuntime& pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval(const LPRDORuntime& pRuntime)
{
	timeNext = m_pTimeCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime();
	pRuntime->addTimePoint(
		timeNext,
		this,
		boost::bind(&RDOPROCGenerate::onMakePlaned, this, pRuntime)
	);
}

void RDOPROCGenerate::onStop(const LPRDORuntime& pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCGenerate::onMakePlaned(const LPRDORuntime& pRuntime)
{
	++m_createdTransactCount;

	if (m_pStatistics)
		m_pStatistics->setTransCount(m_createdTransactCount);

	LPRDOPROCTransact pTransact = m_pCreateAndGoOnTransactCalc->calcValue(pRuntime).
			getPointerByType<RDOPROCTransact, RDOResourceTypeList>();
	ASSERT(pTransact);

	pTransact->setBlock(this);
	pTransact->next();

	RDOTrace* tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << pTransact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}

	calcNextTimeInterval(pRuntime);
}

IBaseOperation::BOResult RDOPROCGenerate::onContinue(const LPRDORuntime& pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCGenerate::setStatistics(const LPIInternalStatistics& pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
