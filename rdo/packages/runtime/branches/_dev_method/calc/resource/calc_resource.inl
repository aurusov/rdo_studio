/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_relevant.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     RDOCalc для подбора релевантных ресурсов
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
	
// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParam
// --------------------------------------------------------------------------------
template <SetOperationType::Type setOperationType>
inline RDOSetResourceParam<setOperationType>::RDOSetResourceParam(const LPRDOCalc& getResource, const ruint paramID, const LPRDOCalc& pCalc)
	: m_getResource (getResource)
	, m_paramID     (paramID )
	, m_pCalc       (pCalc   )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

template <SetOperationType::Type setOperationType>
inline RDOSetResourceParam<setOperationType>::~RDOSetResourceParam()
{}

template <>
inline RDOValue RDOSetResourceParam<SetOperationType::NOCHANGE>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_getResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
	ASSERT(pResource);
	UNUSED(pRuntime);
	RDOValue value(true);
	return value;
}

template <>
inline RDOValue RDOSetResourceParam<SetOperationType::SET>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_getResource->calcValue(pRuntime).getPointerByInterface<IResourceType>();
	ASSERT(pResource);
	RDOValue value = m_pCalc->calcValue(pRuntime);
	pResource->setParam(m_paramID, value);
	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE