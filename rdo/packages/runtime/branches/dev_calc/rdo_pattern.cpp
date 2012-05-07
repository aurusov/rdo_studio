/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_pattern.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      13.04.2008
  \brief     Описание базового класса для образцов всех типов активностей и событий
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_event.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_keyboard.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPattern
// --------------------------------------------------------------------------------
RDOPattern::RDOPattern(rbool trace)
	: RDORuntimeObject  (     )
	, RDOTraceableObject(trace)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternEvent::~RDOPatternEvent()
{}

double RDOPatternEvent::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	m_timeCalc->calcValue(pRuntime);
	double time_next = pRuntime->stack().pop().getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}
	pRuntime->error().push(rdo::format(_T("Попытка запланировать событие в прошлом. Выражение времени для $Time имеет отрицательное значение: %f"), time_next), m_timeCalc->srcInfo());
	return 0;
}

LPIEvent RDOPatternEvent::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(tstring) oprName)
{
	LPIEvent pEvent = RF(RDOEvent)::create(pRuntime, this, traceable(), oprName);
	ASSERT(pEvent);
	pRuntime->addRuntimeEvent(pLogic, pEvent);
	return pEvent;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(rbool trace)
	: RDOPattern(trace)
{}

RDOPatternRule::~RDOPatternRule()
{}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIRule pRule = RF(RDORule)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pRule);
	pRuntime->addRuntimeRule(pLogic, pRule);
	return pRule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIRule pRule = RF(RDORule)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pRule);
	pRuntime->addRuntimeRule(pLogic, pRule);
	return pRule;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternOperation::~RDOPatternOperation()
{}

double RDOPatternOperation::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	m_timeCalc->calcValue(pRuntime);
	double time_next = pRuntime->stack().pop().getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}
	pRuntime->error().push(rdo::format(_T("Попытка запланировать окончание операции в прошлом. Выражение времени для $Time имеет отрицательное значение: %f"), time_next), m_timeCalc->srcInfo());
	return 0;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIOperation pOperation = RF(RDOOperation)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pOperation);
	pRuntime->addRuntimeOperation(pLogic, pOperation);
	return pOperation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIOperation pOperation = RF(RDOOperation)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pOperation);
	pRuntime->addRuntimeOperation(pLogic, pOperation);
	return pOperation;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(rbool trace)
	: RDOPatternOperation(trace)
{}

RDOPatternKeyboard::~RDOPatternKeyboard()
{}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIKeyboard pKeyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard);
	return pKeyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer pLogic, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIKeyboard pKeyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard);
	return pKeyboard;
}

CLOSE_RDO_RUNTIME_NAMESPACE
