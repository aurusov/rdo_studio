/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_pattern-inl.h
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
inline void RDOPattern::addPreSelectRelRes(const LPRDOCalc& pCalc)
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

inline void RDOPattern::preSelectRelRes(const LPRDORuntime& pRuntime)
{
	runCalcs(m_preSelectRelRes, pRuntime);
}

inline void RDOPattern::runCalcs(CalcList& calcList, const LPRDORuntime& pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	for (const auto& calc: calcList)
		calc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->pop();
}

inline bool RDOPattern::runCalcsBool(CalcList& calcList, const LPRDORuntime& pRuntime)
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
inline void RDOPatternEvent::addConvertorCalc(const LPRDOCalc& pCalc)
{
	m_convertor.push_back(pCalc);
}

inline void RDOPatternEvent::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

inline void RDOPatternEvent::addEraseCalc(const LPRDOCalc& pCalc)
{
	m_erase.push_back(pCalc);
}

inline void RDOPatternEvent::convertEvent(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	runCalcs(m_convertor, pRuntime);
}

inline void RDOPatternEvent::convertErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
inline void RDOPatternRule::addChoiceFromCalc(const LPRDOCalc& pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

inline void RDOPatternRule::addConvertorCalc(const LPRDOCalc& pCalc)
{
	m_convertor.push_back(pCalc);
}

inline void RDOPatternRule::addConvertorStatus(RDOResource::ConvertStatus status)
{
	m_convertorStatus.push_back(status);
}

inline void RDOPatternRule::addEraseCalc(const LPRDOCalc& pCalc)
{
	m_erase.push_back(pCalc);
}

inline bool RDOPatternRule::choiceFrom(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

inline void RDOPatternRule::convertRule(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertor, pRuntime);
}

inline void RDOPatternRule::convertErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_erase, pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
inline void RDOPatternOperation::addChoiceFromCalc(const LPRDOCalc& pCalc)
{
	m_choiceFrom.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorBeginCalc(const LPRDOCalc& pCalc)
{
	m_convertorBegin.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorBeginStatus(RDOResource::ConvertStatus status)
{
	m_convertorBeginStatus.push_back(status);
}

inline void RDOPatternOperation::addEraseBeginCalc(const LPRDOCalc& pCalc)
{
	m_eraseBegin.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorEndCalc(const LPRDOCalc& pCalc)
{
	m_convertorEnd.push_back(pCalc);
}

inline void RDOPatternOperation::addConvertorEndStatus(RDOResource::ConvertStatus status)
{
	m_convertorEndStatus.push_back(status);
}

inline void RDOPatternOperation::addEraseEndCalc(const LPRDOCalc& pCalc)
{
	m_eraseEnd.push_back(pCalc);
}

inline void RDOPatternOperation::setTime(const LPRDOCalc& pCalc)
{
	m_timeCalc = pCalc;
}

inline bool RDOPatternOperation::choiceFrom(const LPRDORuntime& pRuntime)
{
	preSelectRelRes(pRuntime);
	return runCalcsBool(m_choiceFrom, pRuntime);
}

inline void RDOPatternOperation::convertBegin(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertorBegin, pRuntime);
}

inline void RDOPatternOperation::convertBeginErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_eraseBegin, pRuntime);
}

inline void RDOPatternOperation::convertEnd(const LPRDORuntime& pRuntime)
{
	runCalcs(m_convertorEnd, pRuntime);
}

inline void RDOPatternOperation::convertEndErase(const LPRDORuntime& pRuntime)
{
	runCalcs(m_eraseEnd, pRuntime);
}

CLOSE_RDO_RUNTIME_NAMESPACE
