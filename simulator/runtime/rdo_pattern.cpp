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
#include "simulator/runtime/pch/stdpch.h"
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
RDOPattern::RDOPattern(bool trace)
	: RDORuntimeObject  (     )
	, RDOTraceableObject(trace)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(bool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternEvent::~RDOPatternEvent()
{}

double RDOPatternEvent::getNextTimeInterval(const LPRDORuntime& pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}

	pRuntime->error().push(rdo::format("Попытка запланировать событие в прошлом. Выражение времени для $Time имеет отрицательное значение: %f", time_next), m_timeCalc->srcInfo());
	return 0;
}

LPIEvent RDOPatternEvent::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& oprName)
{
	LPIEvent pEvent = RF(RDOEvent)::create(pRuntime, this, traceable(), oprName);
	ASSERT(pEvent);
	pRuntime->addRuntimeEvent(pLogic, pEvent);
	return pEvent;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(bool trace)
	: RDOPattern(trace)
{}

RDOPatternRule::~RDOPatternRule()
{}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName)
{
	LPIRule pRule = RF(RDORule)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pRule);
	pRuntime->addRuntimeRule(pLogic, pRule);
	return pRule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIRule pRule = RF(RDORule)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pRule);
	pRuntime->addRuntimeRule(pLogic, pRule);
	return pRule;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(bool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternOperation::~RDOPatternOperation()
{}

double RDOPatternOperation::getNextTimeInterval(const LPRDORuntime& pRuntime)
{
	double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
	if (time_next >= 0)
	{
		return time_next;
	}
	pRuntime->error().push(rdo::format("Попытка запланировать окончание операции в прошлом. Выражение времени для $Time имеет отрицательное значение: %f", time_next), m_timeCalc->srcInfo());
	return 0;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName)
{
	LPIOperation pOperation = RF(RDOOperation)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pOperation);
	pRuntime->addRuntimeOperation(pLogic, pOperation);
	return pOperation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIOperation pOperation = RF(RDOOperation)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pOperation);
	pRuntime->addRuntimeOperation(pLogic, pOperation);
	return pOperation;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(bool trace)
	: RDOPatternOperation(trace)
{}

RDOPatternKeyboard::~RDOPatternKeyboard()
{}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName)
{
	LPIKeyboard pKeyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard);
	return pKeyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIKeyboard pKeyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard);
	return pKeyboard;
}

CLOSE_RDO_RUNTIME_NAMESPACE
