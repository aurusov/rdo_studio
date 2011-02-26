/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdo_ie.h
 * author   : 
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/runtime/rdo_ie.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent(PTR(RDORuntime) rTime, rbool trace)
	: RDOPattern(rTime, trace)
	, m_timeCalc(NULL        )
{}

double RDOPatternIrregEvent::getNextTimeInterval(PTR(RDORuntime) pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}
	pRuntime->error(rdo::format(_T("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f"), time_next), m_timeCalc);
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
RDOIrregEvent::RDOIrregEvent(PTR(RDORuntime) pRuntime, PTR(RDOPatternIrregEvent) pPattern, rbool trace, CREF(tstring) name)
	: RDOActivityPattern<RDOPatternIrregEvent>(pRuntime, pPattern, trace, name)
{
	setTrace  (trace);
	setTraceID(~0   );
}

void RDOIrregEvent::onStart(PTR(RDOSimulator) pSimulator)
{
	onBeforeIrregularEvent(pSimulator);
	pSimulator->addTimePoint(getNextTimeInterval(pSimulator) + pSimulator->getCurrentTime(), this);
}

void RDOIrregEvent::onStop(PTR(RDOSimulator) pSimulator)
{
	pSimulator->removeTimePoint(this);
}

rbool RDOIrregEvent::onCheckCondition(PTR(RDOSimulator) pSimulator)
{
	return false;
}

IBaseOperation::BOResult RDOIrregEvent::onDoOperation(PTR(RDOSimulator) pSimulator)
{
	return IBaseOperation::BOR_cant_run;
}

void RDOIrregEvent::onMakePlaned(PTR(RDOSimulator) pSimulator, PTR(void) pParam)
{
	pSimulator->inc_cnt_events();
	onBeforeIrregularEvent(pSimulator);
	convertEvent(pSimulator);
	pSimulator->addTimePoint(getNextTimeInterval(pSimulator) + pSimulator->getCurrentTime(), this);
	onAfterIrregularEvent(pSimulator);
}

void RDOIrregEvent::convertEvent(PTR(RDOSimulator) pSimulator) 
{ 
	NEVER_REACH_HERE;
}

void RDOIrregEvent::onBeforeIrregularEvent(PTR(RDOSimulator) pSimulator)
{
	NEVER_REACH_HERE;
}

void RDOIrregEvent::onAfterIrregularEvent(PTR(RDOSimulator) pSimulator)
{
	NEVER_REACH_HERE;
}

double RDOIrregEvent::getNextTimeInterval(PTR(RDOSimulator) pSimulator) 
{ 
	static_cast<PTR(RDORuntime)>(pSimulator)->setCurrentActivity(this);
	return m_pattern->getNextTimeInterval(static_cast<PTR(RDORuntime)>(pSimulator)); 
}

IBaseOperation::BOResult RDOIrregEvent::onContinue(PTR(rdoRuntime::RDOSimulator) pSimulator)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
