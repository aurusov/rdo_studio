/*
 * copyright: (c) RDO-Team, 2011
 * filename : std_fun.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_STD_FUN_H_
#define _RDOCALC_STD_FUN_H_

// ====================================================================== INCLUDES
#include <boost/function.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------

template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
	RDOFunCalcStd(CREF(F) function)
		: m_function(function)
	{}

private:
	F m_function;

	REF(RDOValue) doCalc(PTR(RDORuntime) pRuntime)
	{
		calc<F::arity>(pRuntime);
		return m_value;
	}

	template <int paramCount>
	inline void calc(PTR(RDORuntime) pRuntime);

	template <>
	inline void calc<1>(PTR(RDORuntime) pRuntime)
	{
		m_value = m_function(getParam<F::arg1_type>(pRuntime, 0));
	}

	template <>
	inline void calc<2>(PTR(RDORuntime) pRuntime)
	{
		m_value = m_function(getParam<F::arg1_type>(pRuntime, 0), getParam<F::arg2_type>(pRuntime, 1));
	}

	template <class T>
	inline T getParam(PTR(RDORuntime) pRuntime, ruint paramNumber);

	template <>
	inline double getParam<double>(PTR(RDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getDouble();
	}

	template <>
	inline int getParam<int>(PTR(RDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getInt();
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_STD_FUN_H_
