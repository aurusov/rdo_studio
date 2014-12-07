// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORule
// --------------------------------------------------------------------------------
RDORule::RDORule(const LPRDORuntime& pRuntime, const LPRDOPatternRule& pPattern, bool trace, const std::string& name)
	: RDOActivityPattern<RDOPatternRule>(pPattern, trace, name)
	, RDOPatternPrior                   (                     )
	, m_pRuntime                        (pRuntime             )
{
	init();
}

RDORule::RDORule(const LPRDORuntime& pRuntime, const LPRDOPatternRule& pPattern, bool trace, const LPRDOCalc& pCondition, const std::string& name)
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

void RDORule::onBeforeChoiceFrom(const LPRDORuntime& pRuntime)
{
	setPatternParameters(pRuntime, m_paramsCalcs);
}

bool RDORule::choiceFrom(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);
	if (m_pAdditionalCondition && !m_pAdditionalCondition->calcValue(pRuntime).getAsBool())
	{
		return false;
	}
	return m_pPattern->choiceFrom(pRuntime);
}

void RDORule::onBeforeRule(const LPRDORuntime& /*pRuntime*/)
{}

void RDORule::convertRule(const LPRDORuntime& pRuntime)
{ 
	pRuntime->setCurrentActivity(this);
	m_pPattern->convertRule(pRuntime);
}

void RDORule::onAfterRule(const LPRDORuntime& pRuntime, bool inSearch)
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

bool RDORule::onCheckCondition(const LPRDORuntime& pRuntime)
{
	onBeforeChoiceFrom(pRuntime);
	pRuntime->inc_cnt_choice_from();
	bool result = choiceFrom(pRuntime);
	if (result)
	{
		m_traceOFF = true;
		LPRDORuntime pClone = pRuntime->clone();
		ASSERT(pClone);
		if (onDoOperation(pClone) != IBaseOperation::BOR_done)
		{
			// TODO Реакция на плохой onDoOperation - это вообще-то спортный вопрос
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

IBaseOperation::BOResult RDORule::onDoOperation(const LPRDORuntime& pRuntime)
{
	onBeforeRule(pRuntime);
	convertRule (pRuntime);
	onAfterRule (pRuntime, false);
	return IBaseOperation::BOR_done;
}

void                     RDORule::onStart   (const LPRDORuntime& /*pRuntime*/) {}
void                     RDORule::onStop    (const LPRDORuntime& /*pRuntime*/) {}
IBaseOperation::BOResult RDORule::onContinue(const LPRDORuntime& /*pRuntime*/) { return IBaseOperation::BOR_cant_run; }

CLOSE_RDO_RUNTIME_NAMESPACE
