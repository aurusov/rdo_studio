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
// ---------- RDOCalcSetLocalVaribleEqualType
// ----------------------------------------------------------------------------
template <EqualType equalType>
class RDOCalcSetLocalVaribleEqualType: public RDOCalc
{
	DECLARE_FACTORY(RDOCalcSetLocalVaribleEqualType)
private:
	RDOCalcSetLocalVaribleEqualType(CREF(tstring) name, LPRDOCalc pCalc = NULL)
		: m_name   (name )
		, m_calc   (pCalc)
	{}

	tstring   m_name;
	LPRDOCalc m_calc;

	DECALRE_ICalc;
};

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_EQUAL>::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, m_calc->calcValue(runtime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_PLUS>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) + m_calc->calcValue(runtime);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_MINUS>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) - m_calc->calcValue(runtime);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_MULTIPLY>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) * m_calc->calcValue(runtime);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_DIVIDE>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) / m_calc->calcValue(runtime);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_INCR>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) + RDOValue(1);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

template <>
inline REF(RDOValue) RDOCalcSetLocalVaribleEqualType<ET_DECR>::doCalc(PTR(RDORuntime) runtime)
{
	rdoRuntime::RDOValue pValue = runtime->getMemoryStack()->getLocalMemory()->getLocalVarible(m_name) - RDOValue(1);
	runtime->getMemoryStack()->getLocalMemory()->setLocalVarible(m_name, pValue);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_LOCVAR_H_
