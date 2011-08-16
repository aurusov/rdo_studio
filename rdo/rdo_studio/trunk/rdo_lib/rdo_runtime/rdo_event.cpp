/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      rdo_event.cpp
 * @authors   Урусов Андрей, Лущан Дмитрий
 * @date      18.08.2010
 * @brief     События
 * @indent    4T
 *********************************************************************************/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
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
	return false;
}

IBaseOperation::BOResult RDOEvent::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	return IBaseOperation::BOR_cant_run;
}

void RDOEvent::onMakePlaned(CREF(LPRDORuntime) pRuntime, void* param)
{
	pRuntime->inc_cnt_events();
	onBeforeEvent(pRuntime);
	convertEvent (pRuntime);
	onAfterEvent (pRuntime);
}

void RDOEvent::convertEvent(CREF(LPRDORuntime) pRuntime)
{ 
	pRuntime->setCurrentActivity(this);
	m_pPattern->convertEvent(pRuntime);
}

void RDOEvent::onBeforeEvent(CREF(LPRDORuntime) pRuntime)
{
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
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
