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

void RDOPattern::addPreSelectRelRes(const LPRDOCalc& pCalc)
{
	CalcList::iterator it = m_preSelectRelRes.begin();
	while (it != m_preSelectRelRes.end())
	{
		if ((*it)->compare(pCalc))
		{
			return;
		}
		++it;
	}
	m_preSelectRelRes.push_back(pCalc);
}

RDOPattern::~RDOPattern()
{}

void RDOPattern::preSelectRelRes(const LPRDORuntime& pRuntime)
{
	runCalcs(m_preSelectRelRes, pRuntime);
}

void RDOPattern::runCalcs(CalcList& calcList, const LPRDORuntime& pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	for (const auto& calc: calcList)
		calc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->pop();
}

bool RDOPattern::runCalcsBool(CalcList& calcList, const LPRDORuntime& pRuntime)
{
	for (const auto& calc: calcList)
	{
		if (!calc->calcValue(pRuntime).getAsBool())
		{
			return false;
		}
	}
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(bool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternEvent::~RDOPatternEvent()
{}

void RDOPatternEvent::addConvertorCalc(const LPRDOCalc& pCalc)
{
	m_convertor.push_back(pCalc);
}

void RDOPatternEvent::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

void RDOPatternEvent::addEraseCalc(const LPRDOCalc& pCalc)
{
	m_erase.push_back(pCalc);
}

void RDOPatternEvent::convertEvent(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	runCalcs(m_convertor, pRuntime);
}

void RDOPatternEvent::convertErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

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
	LPIEvent pEvent = rdo::Factory<RDOEvent>::create(pRuntime, this, traceable(), oprName);
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

void RDOPatternRule::addChoiceFromCalc(const LPRDOCalc& pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

void RDOPatternRule::addConvertorCalc(const LPRDOCalc& pCalc)
{
	m_convertor.push_back(pCalc);
}

void RDOPatternRule::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

void RDOPatternRule::addEraseCalc(const LPRDOCalc& pCalc)
{
	m_erase.push_back(pCalc);
}

bool RDOPatternRule::choiceFrom(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

void RDOPatternRule::convertRule(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertor, pRuntime);
}

void RDOPatternRule::convertErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName)
{
	LPIRule pRule = rdo::Factory<rdo::runtime::RDORule>::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pRule);
	pRuntime->addRuntimeRule(pLogic, pRule);
	return pRule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIRule pRule = rdo::Factory<rdo::runtime::RDORule>::create(pRuntime, this, traceable(), pCondition, _oprName);
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

void RDOPatternOperation::addChoiceFromCalc(const LPRDOCalc& pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

void RDOPatternOperation::addConvertorBeginCalc(const LPRDOCalc& pCalc)
{
	m_convertorBegin.push_back(pCalc);
}

void RDOPatternOperation::addConvertorBeginStatus(RDOResource::ConvertStatus status)
{
	m_convertorBeginStatus.push_back(status);
}

void RDOPatternOperation::addEraseBeginCalc(const LPRDOCalc& pCalc)
{
	m_eraseBegin.push_back(pCalc);
}

void RDOPatternOperation::addConvertorEndCalc(const LPRDOCalc& pCalc)
{
	m_convertorEnd.push_back(pCalc);
}

void RDOPatternOperation::addConvertorEndStatus(RDOResource::ConvertStatus status)
{
	m_convertorEndStatus.push_back(status);
}

void RDOPatternOperation::addEraseEndCalc(const LPRDOCalc& pCalc)
{
	m_eraseEnd.push_back(pCalc);
}

void RDOPatternOperation::setTime(const LPRDOCalc& pCalc)
{
	m_timeCalc = pCalc;
}

bool RDOPatternOperation::choiceFrom(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

void RDOPatternOperation::convertBegin(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertorBegin, pRuntime);
}

void RDOPatternOperation::convertBeginErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_eraseBegin, pRuntime);
}

void RDOPatternOperation::convertEnd(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertorEnd, pRuntime);
}

void RDOPatternOperation::convertEndErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_eraseEnd, pRuntime);
}

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
	LPIOperation pOperation = rdo::Factory<RDOOperation>::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pOperation);
	pRuntime->addRuntimeOperation(pLogic, pOperation);
	return pOperation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIOperation pOperation = rdo::Factory<RDOOperation>::create(pRuntime, this, traceable(), pCondition, _oprName);
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
	LPIKeyboard pKeyboard = rdo::Factory<RDOKeyboard>::create(pRuntime, this, traceable(), _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard.object_dynamic_cast<IOperation>());
	return pKeyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& pCondition, const std::string& _oprName)
{
	LPIKeyboard pKeyboard = rdo::Factory<RDOKeyboard>::create(pRuntime, this, traceable(), pCondition, _oprName);
	ASSERT(pKeyboard);
	pRuntime->addRuntimeOperation(pLogic, pKeyboard.object_dynamic_cast<IOperation>());
	return pKeyboard;
}

CLOSE_RDO_RUNTIME_NAMESPACE
