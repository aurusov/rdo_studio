/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_operation.cpp
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
RDOOperation::RDOOperation(CREF(LPRDORuntime) pRuntime, RDOPatternOperation* pattern, bool trace, const std::string& name):
	RDOActivityPattern<RDOPatternOperation>(pattern, trace, name),
	RDOPatternPrior(),
	additionalCondition(NULL)
{
	setTrace(trace);
	haveAdditionalCondition = false;
	setTraceID(pRuntime->getFreeActivityId());
}

RDOOperation::RDOOperation( CREF(LPRDORuntime) pRuntime, RDOPatternOperation* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name ):
	RDOActivityPattern<RDOPatternOperation>(pattern, trace, name),
	RDOPatternPrior(),
	additionalCondition(pCondition)
{
	setTrace( trace );
	haveAdditionalCondition = true;
	setTraceID( pRuntime->getFreeActivityId() );
}

RDOOperation::~RDOOperation()
{}

bool RDOOperation::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	// Если операция может начаться, то создать её клон и поместить его в список
	onBeforeChoiceFrom(pRuntime);
	pRuntime->inc_cnt_choice_from();
	return choiceFrom(pRuntime);
}

IBaseOperation::BOResult RDOOperation::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	LPIOperation newOper = F(RDOOperation)::create(pRuntime, *this);
	newOper->onBeforeOperationBegin(pRuntime);
	newOper->convertBegin(pRuntime);
	pRuntime->addTimePoint(newOper->getNextTimeInterval(pRuntime) + pRuntime->getCurrentTime(), newOper);
	newOper->onAfterOperationBegin(pRuntime);
	return IBaseOperation::BOR_planned_and_run;
}

void RDOOperation::onMakePlaned(CREF(LPRDORuntime) pRuntime, void* param)
{
	// Выполняем событие конца операции-клона
	pRuntime->inc_cnt_events();
	onBeforeOperationEnd(pRuntime);
	convertEnd(pRuntime);
	onAfterOperationEnd(pRuntime);
}

bool RDOOperation::choiceFrom(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	if (haveAdditionalCondition)
	{
		if (!additionalCondition->calcValue(pRuntime).getAsBool())
		{
			return false;
		}
	}
	return m_pattern->choiceFrom(pRuntime); 
}

void RDOOperation::convertBegin(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	m_pattern->convertBegin(pRuntime);
}

void RDOOperation::convertEnd(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	m_pattern->convertEnd(pRuntime);
}

void RDOOperation::onBeforeChoiceFrom(CREF(LPRDORuntime) pRuntime)
{
	setPatternParameters(pRuntime);
}

void RDOOperation::onBeforeOperationEnd(CREF(LPRDORuntime) pRuntime)
{
	setPatternParameters(pRuntime);
}

void RDOOperation::onAfterOperationBegin(CREF(LPRDORuntime) pRuntime)
{
	updateConvertStatus(pRuntime, m_pattern->m_convertorBeginStatus);
	pRuntime->getTracer()->writeAfterOperationBegin(this, pRuntime);
	m_pattern->convertBeginErase(pRuntime);
	updateRelRes(pRuntime);
	updateConvertStatus( pRuntime, m_pattern->m_convertorEndStatus);
	incrementRelevantResourceReference(pRuntime);
	updateConvertStatus( pRuntime, m_pattern->m_convertorBeginStatus);
}

void RDOOperation::onAfterOperationEnd(CREF(LPRDORuntime) pRuntime)
{
	updateConvertStatus(pRuntime, m_pattern->m_convertorEndStatus);
	decrementRelevantResourceReference(pRuntime);
	pRuntime->getTracer()->writeAfterOperationEnd(this, pRuntime); 
	pRuntime->freeOperationId(m_operId);
	m_pattern->convertEndErase(pRuntime);
	updateRelRes(pRuntime);
}

double RDOOperation::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	return m_pattern->getNextTimeInterval(pRuntime);
}

RDOOperation::RDOOperation(CREF(LPRDORuntime) pRuntime, CREF(RDOOperation) originForClone)
	: RDOActivityPattern<RDOPatternOperation>(originForClone.m_pattern, originForClone.traceable(), originForClone.m_oprName)
	, additionalCondition(NULL)
{
	setTrace( originForClone.traceable() );
	haveAdditionalCondition = false;
	setTraceID( pRuntime->getFreeActivityId() );

	m_paramsCalcs.insert(m_paramsCalcs.begin(), originForClone.m_paramsCalcs.begin(), originForClone.m_paramsCalcs.end());
	m_relResID   .insert(m_relResID   .begin(), originForClone.m_relResID   .begin(), originForClone.m_relResID   .end());
	setTraceID(originForClone.getTraceID());
	m_operId = pRuntime->getFreeOperationId();
}

tstring RDOOperation::traceOperId() const
{
	return rdo::toString(m_operId);
}

void                     RDOOperation::onBeforeOperationBegin(CREF(LPRDORuntime) pRuntime) {}
void                     RDOOperation::onStart               (CREF(LPRDORuntime) pRuntime) {}
void                     RDOOperation::onStop                (CREF(LPRDORuntime) pRuntime) {}
IBaseOperation::BOResult RDOOperation::onContinue            (CREF(LPRDORuntime) pRuntime) { return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE
