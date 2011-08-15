/******************************************************************************//**
 * @copyright (c) RDO-Team, 2008
 * @file      rdo_pattern.cpp
 * @authors   Урусов Андрей, Лущан Дмитрий
 * @date      13.04.2008
 * @brief     Описание базового класса для образцов всех типов активностей и событий
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOPattern
// ********************************************************************************
RDOPattern::RDOPattern(rbool trace)
	: RDORuntimeObject  (     )
	, RDOTraceableObject(trace)
{}

// ********************************************************************************
// ******************** RDOPatternEvent
// ********************************************************************************
RDOPatternEvent::RDOPatternEvent(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternEvent::~RDOPatternEvent()
{}

double RDOPatternEvent::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0) return time_next;
	pRuntime->error(rdo::format("Попытка запланировать событие в прошлом. Выражение времени для $Time имеет отрицательное значение: %f", time_next), m_timeCalc);
	return 0;
}

LPIEvent RDOPatternEvent::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) oprName)
{
	LPIEvent ev = F(RDOEvent)::create(pRuntime, this, traceable(), oprName);
	pRuntime->addRuntimeEvent(parent, ev);
	return ev;
}

// ********************************************************************************
// ******************** RDOPatternRule
// ********************************************************************************
RDOPatternRule::RDOPatternRule(rbool trace)
	: RDOPattern(trace)
{}

RDOPatternRule::~RDOPatternRule()
{}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIRule rule = F(RDORule)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeRule(logic, rule);
	return rule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIRule rule = F(RDORule)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeRule(logic, rule);
	return rule;
}

// ********************************************************************************
// ******************** RDOPatternOperation
// ********************************************************************************
RDOPatternOperation::RDOPatternOperation(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternOperation::~RDOPatternOperation()
{}

double RDOPatternOperation::getNextTimeInterval(CREF(LPRDORuntime) pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0) return time_next;
	pRuntime->error(rdo::format("Попытка запланировать окончание операции в прошлом. Выражение времени для $Time имеет отрицательное значение: %f", time_next), m_timeCalc);
	return 0;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIOperation operation = F(RDOOperation)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeOperation(parent, operation);
	return operation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIOperation operation = F(RDOOperation)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeOperation(parent, operation);
	return operation;
}

// ********************************************************************************
// ******************** RDOPatternKeyboard
// ********************************************************************************
RDOPatternKeyboard::RDOPatternKeyboard(rbool trace)
	: RDOPatternOperation(trace)
{}

RDOPatternKeyboard::~RDOPatternKeyboard()
{}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = F(RDOKeyboard)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = F(RDOKeyboard)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

CLOSE_RDO_RUNTIME_NAMESPACE
