/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_ie.h
  \author    
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/runtime/rdo_ie.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

double RDOPatternIrregEvent::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}
	pRuntime->error().push(rdo::format(_T("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f"), time_next), m_timeCalc->srcInfo());
	return 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOIrregEvent
// --------------------------------------------------------------------------------
RDOIrregEvent::RDOIrregEvent(PTR(RDOPatternIrregEvent) pPattern, rbool trace, CREF(tstring) name)
	: RDOActivityPattern<RDOPatternIrregEvent>(pPattern, trace, name)
{
	setTrace  (trace    );
	setTraceID(ruint(~0));
}

void RDOIrregEvent::onStart(CREF(LPRDORuntime) pRuntime)
{
	onBeforeIrregularEvent(pRuntime);
	pRuntime->addTimePoint(getNextTimeInterval(pRuntime) + pRuntime->getCurrentTime(), this);
}

void RDOIrregEvent::onStop(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->removeTimePoint(this);
}

rbool RDOIrregEvent::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return false;
}

IBaseOperation::BOResult RDOIrregEvent::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOIrregEvent::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pParam);

	pRuntime->inc_cnt_events();
	onBeforeIrregularEvent(pRuntime);
	convertEvent(pRuntime);
	pRuntime->addTimePoint(getNextTimeInterval(pRuntime) + pRuntime->getCurrentTime(), this);
	onAfterIrregularEvent(pRuntime);
}

void RDOIrregEvent::convertEvent(CREF(LPRDORuntime) pRuntime) 
{ 
	UNUSED(pRuntime);
	NEVER_REACH_HERE;
}

void RDOIrregEvent::onBeforeIrregularEvent(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	NEVER_REACH_HERE;
}

void RDOIrregEvent::onAfterIrregularEvent(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	NEVER_REACH_HERE;
}

double RDOIrregEvent::getNextTimeInterval(CREF(LPRDORuntime) pRuntime) 
{ 
	pRuntime->setCurrentActivity(this);
	return m_pPattern->getNextTimeInterval(pRuntime); 
}

IBaseOperation::BOResult RDOIrregEvent::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
