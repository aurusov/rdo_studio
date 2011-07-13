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
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------

template <class RT, class P1>
class std_fun1
{
public:
	enum { arity = 1 };

	typedef RT return_type;
	typedef P1 arg1_type;
	typedef RT (*function_type)(P1);
};

template <class RT, class P1, class P2>
class std_fun2
{
public:
	enum { arity = 2 };

	typedef RT return_type;
	typedef P1 arg1_type;
	typedef P2 arg2_type;
	typedef RT (*function_type)(P1, P2);
};

template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
	typedef typename F::function_type function_type;

	RDOFunCalcStd(CREF(function_type) pFunction)
		: m_pFunction(pFunction)
	{}

private:
	function_type m_pFunction;

	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime)
	{
		calc<F::arity>(pRuntime);
		return m_value;
	}

	template <int paramCount>
	FORCEINLINE void calc(CREF(LPRDORuntime) pRuntime);

	template <>
	FORCEINLINE void calc<1>(CREF(LPRDORuntime) pRuntime)
	{
		m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0));
	}

	template <>
	FORCEINLINE void calc<2>(CREF(LPRDORuntime) pRuntime)
	{
		m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0), getParam<F::arg2_type>(pRuntime, 1));
	}

	template <class T>
	FORCEINLINE T getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber);

	template <>
	FORCEINLINE double getParam<double>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getDouble();
	}

	template <>
	FORCEINLINE int getParam<int>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getInt();
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_STD_FUN_H_
