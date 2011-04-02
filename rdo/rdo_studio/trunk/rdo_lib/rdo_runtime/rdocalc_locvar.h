/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdocalc_locvar.h
 * author   : Чирков Михаил
 * date     : 03.12.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_LOCVAR_H_
#define _RDOCALC_LOCVAR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcSetLocalVariableEqualType
// ----------------------------------------------------------------------------
template <EqualType equalType>
class RDOCalcSetLocalVariableEqualType: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariableEqualType)
private:
	RDOCalcSetLocalVariableEqualType(CREF(tstring) name, LPRDOCalc pCalc = NULL)
		: m_name (name )
		, m_pCalc(pCalc)
	{}
	virtual ~RDOCalcSetLocalVariableEqualType()
	{}

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_EQUAL>::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_PLUS>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_MINUS>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_MULTIPLY>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) * m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_DIVIDE>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) / m_pCalc->calcValue(pRuntime);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_INCR>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) + RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVariableEqualType<ET_DECR>::doCalc(PTR(RDORuntime) pRuntime)
{
	rdoRuntime::RDOValue pValue = pRuntime->getMemoryStack()->get(m_name) - RDOValue(1);
	pRuntime->getMemoryStack()->set(m_name, pValue);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_LOCVAR_H_
