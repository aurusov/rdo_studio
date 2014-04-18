/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_operation.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Операции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOOperation
// --------------------------------------------------------------------------------
RDOOperation::RDOOperation(const LPRDORuntime& pRuntime, const LPRDOPatternOperation& pPattern, bool trace, const std::string& name)
	: RDOActivityPattern<RDOPatternOperation>(pPattern, trace, name)
	, RDOPatternPrior                        (                     )
{
	setTrace(trace);
	m_haveAdditionalCondition = false;
	setTraceID(pRuntime->getFreeActivityId());
}

RDOOperation::RDOOperation(const LPRDORuntime& pRuntime, const LPRDOPatternOperation& pPattern, bool trace, const LPRDOCalc& pCondition, const std::string& name)
	: RDOActivityPattern<RDOPatternOperation>(pPattern, trace, name)
	, RDOPatternPrior                        (                     )
	, m_pAdditionalCondition                 (pCondition           )
{
	setTrace(trace);
	m_haveAdditionalCondition = true;
	setTraceID(pRuntime->getFreeActivityId());
}

RDOOperation::RDOOperation(const LPRDORuntime& pRuntime, const RDOOperation& originForClone)
	: RDOActivityPattern<RDOPatternOperation>(originForClone.m_pPattern, originForClone.traceable(), originForClone.m_oprName)
	, m_pAdditionalCondition                 (NULL)
{
	setTrace(originForClone.traceable());
	m_haveAdditionalCondition = false;
	setTraceID(pRuntime->getFreeActivityId());

	m_paramsCalcs.insert(m_paramsCalcs.begin(), originForClone.m_paramsCalcs.begin(), originForClone.m_paramsCalcs.end());
	m_relResID   .insert(m_relResID   .begin(), originForClone.m_relResID   .begin(), originForClone.m_relResID   .end());
	setTraceID(originForClone.getTraceID());
	m_operId = pRuntime->getFreeOperationId();
}

RDOOperation::~RDOOperation()
{}

bool RDOOperation::onCheckCondition(const LPRDORuntime& pRuntime)
{
	// Если операция может начаться, то создать её клон и поместить его в список
	onBeforeChoiceFrom(pRuntime);
	pRuntime->inc_cnt_choice_from();
	return choiceFrom(pRuntime);
}

IBaseOperation::BOResult RDOOperation::onDoOperation(const LPRDORuntime& pRuntime)
{
	LPIOperation newOper = RF(RDOOperation)::create(pRuntime, *this);
	newOper->onBeforeOperationBegin(pRuntime);
	newOper->convertBegin(pRuntime);

	std::vector<RDOValue> params;
	params.reserve(m_paramsCalcs.size());
	BOOST_FOREACH(const LPRDOCalc& param, m_paramsCalcs)
	{
		params.push_back(param->calcValue(pRuntime));
	}

	LPIEvent event(newOper);
	pRuntime->addTimePoint(
		newOper->getNextTimeInterval(pRuntime) + pRuntime->getCurrentTime(),
		event,
		boost::bind(&IEvent::onMakePlaned, event.get(), pRuntime, params)
	);
	newOper->onAfterOperationBegin(pRuntime);
	return IBaseOperation::BOR_planned_and_run;
}

void RDOOperation::onMakePlaned(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params)
{
	// Выполняем событие конца операции-клона
	pRuntime->inc_cnt_events();
	onBeforeOperationEnd(pRuntime, params);
	convertEnd(pRuntime);
	onAfterOperationEnd(pRuntime);
}

bool RDOOperation::choiceFrom(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);
	if (m_haveAdditionalCondition)
	{
		if (!m_pAdditionalCondition->calcValue(pRuntime).getAsBool())
		{
			return false;
		}
	}
	return m_pPattern->choiceFrom(pRuntime); 
}

void RDOOperation::convertBegin(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);
	m_pPattern->convertBegin(pRuntime);
}

void RDOOperation::convertEnd(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);
	m_pPattern->convertEnd(pRuntime);
}

void RDOOperation::onBeforeChoiceFrom(const LPRDORuntime& pRuntime)
{
	setPatternParameters(pRuntime, m_paramsCalcs);
}

void RDOOperation::onBeforeOperationEnd(const LPRDORuntime& pRuntime, const std::vector<rdo::runtime::RDOValue>& params)
{
	setPatternParameters(pRuntime, params);
}

void RDOOperation::onAfterOperationBegin(const LPRDORuntime& pRuntime)
{
	updateConvertStatus(pRuntime, m_pPattern->m_convertorBeginStatus);
	pRuntime->getTracer()->writeAfterOperationBegin(this, pRuntime);
	m_pPattern->convertBeginErase(pRuntime);
	updateRelRes(pRuntime);
	incrementRelevantResourceReference(pRuntime);
}

void RDOOperation::onAfterOperationEnd(const LPRDORuntime& pRuntime)
{
	decrementRelevantResourceReference(pRuntime);
	updateConvertStatus(pRuntime, m_pPattern->m_convertorEndStatus);
	pRuntime->getTracer()->writeAfterOperationEnd(this, pRuntime); 
	pRuntime->freeOperationId(m_operId);
	m_pPattern->convertEndErase(pRuntime);
	updateRelRes(pRuntime);
}

double RDOOperation::getNextTimeInterval(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);
	return m_pPattern->getNextTimeInterval(pRuntime);
}

std::string RDOOperation::traceOperId() const
{
	return rdo::toString(m_operId);
}

namespace
{
bool mustBeExist(RDOResource::ConvertStatus status_begin, RDOResource::ConvertStatus status_end)
{
	switch (status_begin)
	{
	case RDOResource::CS_Keep:
		switch (status_end)
		{
		case RDOResource::CS_Keep    : return true;
		case RDOResource::CS_Create  : NEVER_REACH_HERE;
		case RDOResource::CS_Erase   : return true;
		case RDOResource::CS_NonExist: NEVER_REACH_HERE;
		case RDOResource::CS_NoChange: return true;
		default: NEVER_REACH_HERE;
		}
	case RDOResource::CS_Create:
		switch (status_end)
		{
		case RDOResource::CS_Keep    : return true;
		case RDOResource::CS_Create  : NEVER_REACH_HERE;
		case RDOResource::CS_Erase   : return true;
		case RDOResource::CS_NonExist: NEVER_REACH_HERE;
		case RDOResource::CS_NoChange: return true;
		default: NEVER_REACH_HERE;
		}
	case RDOResource::CS_Erase:
		switch (status_end)
		{
		case RDOResource::CS_Keep    : NEVER_REACH_HERE;
		case RDOResource::CS_Create  : NEVER_REACH_HERE;
		case RDOResource::CS_Erase   : NEVER_REACH_HERE;
		case RDOResource::CS_NonExist: return false;
		case RDOResource::CS_NoChange: NEVER_REACH_HERE;
		default: NEVER_REACH_HERE;
		}
	case RDOResource::CS_NonExist:
		switch (status_end)
		{
		case RDOResource::CS_Keep    : NEVER_REACH_HERE;
		case RDOResource::CS_Create  : return false;
		case RDOResource::CS_Erase   : NEVER_REACH_HERE;
		case RDOResource::CS_NonExist: NEVER_REACH_HERE;
		case RDOResource::CS_NoChange: NEVER_REACH_HERE;
		default: NEVER_REACH_HERE;
		}
	case RDOResource::CS_NoChange:
		switch (status_end)
		{
		case RDOResource::CS_Keep    : return true;
		case RDOResource::CS_Create  : NEVER_REACH_HERE;
		case RDOResource::CS_Erase   : return true;
		case RDOResource::CS_NonExist: NEVER_REACH_HERE;
		case RDOResource::CS_NoChange: return true;
		default: NEVER_REACH_HERE;
		}
	default: NEVER_REACH_HERE;
	}
	NEVER_REACH_HERE;
	return true;
}
}

void RDOOperation::incrementRelevantResourceReference(const LPRDORuntime& pRuntime)
{
	ASSERT(m_pPattern->m_convertorBeginStatus.size() == m_pPattern->m_convertorEndStatus.size());

	for (size_t index = 0; index < m_pPattern->m_convertorBeginStatus.size(); ++index)
	{
		if (mustBeExist(m_pPattern->m_convertorBeginStatus[index], m_pPattern->m_convertorEndStatus[index]))
		{
			LPRDOResource resource = pRuntime->getResourceByID(m_relResID.at(index));
			resource->incRef();
		}
	}
}

void RDOOperation::decrementRelevantResourceReference(const LPRDORuntime& pRuntime)
{
	ASSERT(m_pPattern->m_convertorBeginStatus.size() == m_pPattern->m_convertorEndStatus.size());

	for (size_t index = 0; index < m_pPattern->m_convertorBeginStatus.size(); ++index)
	{
		if (mustBeExist(m_pPattern->m_convertorBeginStatus[index], m_pPattern->m_convertorEndStatus[index]))
		{
			LPRDOResource resource = pRuntime->getResourceByID(m_relResID.at(index));
			resource->decRef();
		}
	}
}

void                     RDOOperation::onBeforeOperationBegin(const LPRDORuntime& /*pRuntime*/) {}
void                     RDOOperation::onStart               (const LPRDORuntime& /*pRuntime*/) {}
void                     RDOOperation::onStop                (const LPRDORuntime& /*pRuntime*/) {}
IBaseOperation::BOResult RDOOperation::onContinue            (const LPRDORuntime& /*pRuntime*/) { return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE
