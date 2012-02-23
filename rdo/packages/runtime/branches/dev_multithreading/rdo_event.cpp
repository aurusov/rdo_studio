/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_event.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     События
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_event.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEvent
// --------------------------------------------------------------------------------
RDOEvent::RDOEvent(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternEvent) pPattern, rbool trace, CREF(tstring) name)
	: RDOActivityPattern<RDOPatternEvent>(pPattern, trace, name)
{
	setTrace  (trace);
	setTraceID(pRuntime->getFreeEventId());
}

void RDOEvent::onStart(CREF(LPRDORuntime) pRuntime)
{
	onBeforeEvent(pRuntime);
}

void RDOEvent::onStop(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->removeTimePoint(this);
}

rbool RDOEvent::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return false;
}

IBaseOperation::BOResult RDOEvent::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOEvent::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pParam);

	pRuntime->inc_cnt_events();
	onBeforeEvent(pRuntime);
	convertEvent (pRuntime);
	onAfterEvent (pRuntime);
}

void RDOEvent::convertEvent(CREF(LPRDORuntime) pRuntime)
{
	m_pPattern->convertEvent(pRuntime);
}

void RDOEvent::onBeforeEvent(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setCurrentActivity(this);
	setPatternParameters(pRuntime);
}

void RDOEvent::onAfterEvent(CREF(LPRDORuntime) pRuntime)
{
	updateConvertStatus(pRuntime, m_pPattern->m_convertorStatus);
	pRuntime->getTracer()->writeEvent(this, pRuntime);
	m_pPattern->convertErase(pRuntime);
	updateRelRes(pRuntime);
}

IBaseOperation::BOResult RDOEvent::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
