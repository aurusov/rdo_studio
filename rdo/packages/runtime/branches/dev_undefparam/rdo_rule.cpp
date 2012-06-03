/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_rule.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Продукционные правила
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORule
// --------------------------------------------------------------------------------
RDORule::RDORule(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternRule) pPattern, rbool trace, CREF(tstring) name)
	: RDOActivityPattern<RDOPatternRule>(pPattern, trace, name)
	, RDOPatternPrior                   (                     )
	, m_pRuntime                        (pRuntime             )
{
	init();
}

RDORule::RDORule(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternRule) pPattern, rbool trace, CREF(LPRDOCalc) pCondition, CREF(tstring) name)
	: RDOActivityPattern<RDOPatternRule>(pPattern, trace, name)
	, RDOPatternPrior                   (                     )
	, m_pRuntime                        (pRuntime             )
	, m_pAdditionalCondition            (pCondition           )
{
	init();
}

RDORule::~RDORule()
{}

void RDORule::init()
{
	setTraceID(m_pRuntime->getFreeActivityId());
	m_traceOFF = false;
}

void RDORule::onBeforeChoiceFrom(CREF(LPRDORuntime) pRuntime)
{
	setPatternParameters(pRuntime);
}

rbool RDORule::choiceFrom(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	if (m_pAdditionalCondition && !m_pAdditionalCondition->calcValue(pRuntime).getAsBool())
	{
		return false;
	}
	return m_pPattern->choiceFrom(pRuntime);
}

void RDORule::onBeforeRule(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDORule::convertRule(CREF(LPRDORuntime) pRuntime)
{ 
	pRuntime->setCurrentActivity(this);
	m_pPattern->convertRule(pRuntime);
}

void RDORule::onAfterRule(CREF(LPRDORuntime) pRuntime, rbool inSearch)
{
	updateConvertStatus(pRuntime, m_pPattern->m_convertorStatus);
	if (!inSearch)
	{
		trace();
	}
	m_pPattern->convertErase(pRuntime);
	updateRelRes(pRuntime);
}

void RDORule::trace()
{
	if (!m_traceOFF)
	{
		m_pRuntime->getTracer()->writeRule(this, m_pRuntime);
	}
}

rbool RDORule::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	onBeforeChoiceFrom(pRuntime);
	pRuntime->inc_cnt_choice_from();
	rbool result = choiceFrom(pRuntime);
	if (result)
	{
		m_traceOFF = true;
		LPRDORuntime pClone = pRuntime->clone();
		ASSERT(pClone);
		if (onDoOperation(pClone) != IBaseOperation::BOR_done)
		{
			/// @todo Реакция на плохой onDoOperation - это вообще-то спортный вопрос
			return false;
		}
		if (pClone->equal(pRuntime))
		{
			result = false;
		}
		m_traceOFF = false;
	}
	return result;
}

IBaseOperation::BOResult RDORule::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	onBeforeRule(pRuntime);
	convertRule (pRuntime);
	onAfterRule (pRuntime, false);
	return IBaseOperation::BOR_done;
}

void                     RDORule::onStart     (CREF(LPRDORuntime) pRuntime)                   { UNUSED(pRuntime); }
void                     RDORule::onStop      (CREF(LPRDORuntime) pRuntime)                   { UNUSED(pRuntime); }
void                     RDORule::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam) { UNUSED(pRuntime); UNUSED(pParam); }
IBaseOperation::BOResult RDORule::onContinue  (CREF(LPRDORuntime) pRuntime)                   { UNUSED(pRuntime); return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE
