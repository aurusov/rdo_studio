/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopatpreparse.inl
  \author    Ћущан ƒмитрий (dluschan@rk9.bmstu.ru)
  \date      22.05.10
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/rdo_event.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEvent
// --------------------------------------------------------------------------------
inline RDOEvent::RDOEvent(CREF(tstring) name, rbool regular)
	: m_name    (name   )
	, m_regullar(regular)
{
	m_pParamList = rdo::Factory<ArithmContainer>::create();
}

inline RDOEvent::~RDOEvent()
{}

inline CREF(tstring) RDOEvent::name() const
{
	return m_name;
}

inline void RDOEvent::attachCalc(CREF(rdo::runtime::LPRDOCalcEventStop) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
}

inline void RDOEvent::attachCalc(CREF(rdo::runtime::LPRDOCalcEventPlan) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
}

inline void RDOEvent::setRuntimeEvent(LPIBaseOperation pRuntimeEvent)
{
	ASSERT(pRuntimeEvent);
	m_runtimeEvent = pRuntimeEvent;
}

inline LPIBaseOperation RDOEvent::getRuntimeEvent() const
{
	return m_runtimeEvent;
}

inline rbool RDOEvent::getRegular() const
{
	return m_regullar;
}

inline REF(RDOEvent::CalcList) RDOEvent::getCalcList()
{
	return m_calcList;
}

inline rdo::runtime::LPRDOCalc RDOEvent::getInitCalc() const
{
	return m_pInitCalc;
}

inline void RDOEvent::setInitCalc(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_pInitCalc = pCalc;
}

inline void RDOEvent::setParamList(CREF(LPArithmContainer) pParamList)
{
	ASSERT(pParamList);
	m_pParamList = pParamList;
}

inline CREF(LPArithmContainer) RDOEvent::getParamList() const
{
	return m_pParamList;
}

CLOSE_RDO_PARSER_NAMESPACE
