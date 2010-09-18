/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopatpreparse.inl
 * author   : Ћущан ƒмитрий
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
inline RDOEvent::RDOEvent(CREF(tstring) name, bool pRegular)
	: m_name(name),
	m_regullar(pRegular)
{}

inline RDOEvent::~RDOEvent()
{}

inline CREF(tstring) RDOEvent::name() const
{
	return m_name;
}

inline void RDOEvent::attachCalc(CREF(rdoRuntime::LPRDOCalcEventStop) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
}

inline void RDOEvent::attachCalc(CREF(rdoRuntime::LPRDOCalcEventPlan) pCalc)
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

inline bool RDOEvent::getRegular() const
{
	return m_regullar;
}

inline REF(RDOEvent::CalcList) RDOEvent::getCalcList()
{
	return m_calcList;
}

CLOSE_RDO_PARSER_NAMESPACE
