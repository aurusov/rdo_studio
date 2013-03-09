/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_pattern.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      04.08.2011
  \brief     Описание базового класса для образцов всех типов активностей и событий
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPattern
// --------------------------------------------------------------------------------
inline void RDOPattern::addPreSelectRelRes(CREF(LPRDOCalc) pCalc)
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

inline RDOPattern::~RDOPattern()
{}

inline void RDOPattern::preSelectRelRes(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_preSelectRelRes, pRuntime);
}

inline void RDOPattern::runCalcs(REF(CalcList) calcList, CREF(LPRDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	STL_FOR_ALL(calcList, calcIt)
		(*calcIt)->calcValue(pRuntime);
	pRuntime->getMemoryStack()->pop();
}

inline rbool RDOPattern::runCalcsBool(REF(CalcList) calcList, CREF(LPRDORuntime) pRuntime)
{
	STL_FOR_ALL(calcList, calcIt)
	{
		if (!(*calcIt)->calcValue(pRuntime).getAsBool())
		{
			return false;
		}
	}
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
inline void RDOPatternEvent::addConvertorCalc(CREF(LPRDOCalc) pCalc)
{
	m_convertor.push_back(pCalc);
}

inline void RDOPatternEvent::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

inline void RDOPatternEvent::addEraseCalc(CREF(LPRDOCalc) pCalc)
{
	m_erase.push_back(pCalc);
}

inline void RDOPatternEvent::convertEvent(CREF(LPRDORuntime) pRuntime)
{
	preSelectRelRes(pRuntime);
	runCalcs(m_convertor, pRuntime);
}

inline void RDOPatternEvent::convertErase(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
inline void RDOPatternRule::addChoiceFromCalc(CREF(LPRDOCalc) pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

inline void RDOPatternRule::addConvertorCalc(CREF(LPRDOCalc) pCalc)
{
	m_convertor.push_back(pCalc);
}

inline void RDOPatternRule::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

inline void RDOPatternRule::addEraseCalc(CREF(LPRDOCalc) pCalc)
{
	m_erase.push_back(pCalc);
}

inline rbool RDOPatternRule::choiceFrom(CREF(LPRDORuntime) pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

inline void RDOPatternRule::convertRule(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_convertor, pRuntime);
}

inline void RDOPatternRule::convertErase(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
inline void RDOPatternOperation::addChoiceFromCalc(CREF(LPRDOCalc) pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorBeginCalc(CREF(LPRDOCalc) pCalc)
{
	m_convertorBegin.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorBeginStatus(RDOResource::ConvertStatus status)
{
	m_convertorBeginStatus.push_back(status);
}

inline void RDOPatternOperation::addEraseBeginCalc(CREF(LPRDOCalc) pCalc)
{
	m_eraseBegin.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorEndCalc(CREF(LPRDOCalc) pCalc)
{
	m_convertorEnd.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorEndStatus(RDOResource::ConvertStatus status)
{
	m_convertorEndStatus.push_back(status);
}

inline void RDOPatternOperation::addEraseEndCalc(CREF(LPRDOCalc) pCalc)
{
	m_eraseEnd.push_back(pCalc);
}

inline void RDOPatternOperation::setTime(CREF(LPRDOCalc) pCalc)
{
	m_timeCalc = pCalc;
}

inline rbool RDOPatternOperation::choiceFrom(CREF(LPRDORuntime) pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

inline void RDOPatternOperation::convertBegin(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_convertorBegin, pRuntime);
}

inline void RDOPatternOperation::convertBeginErase(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_eraseBegin, pRuntime);
}

inline void RDOPatternOperation::convertEnd(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_convertorEnd, pRuntime);
}

inline void RDOPatternOperation::convertEndErase(CREF(LPRDORuntime) pRuntime)
{
	runCalcs(m_eraseEnd, pRuntime);
}

CLOSE_RDO_RUNTIME_NAMESPACE
