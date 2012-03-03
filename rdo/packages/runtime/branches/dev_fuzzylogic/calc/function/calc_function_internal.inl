/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_internal.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     Стандартные функции языка
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcStd
// --------------------------------------------------------------------------------
template <class F>
inline RDOFunCalcStd<F>::RDOFunCalcStd(function_type pFunction)
	: m_pFunction(pFunction)
{}

template <class F>
inline REF(RDOValue) RDOFunCalcStd<F>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	calc(pRuntime, typename F::param_count());
	return m_value;
}

template <class F>
FORCE_INLINE void RDOFunCalcStd<F>::calc(CREF(LPRDORuntime) pRuntime, std_fun_one_param)
{
	m_value = m_pFunction(
		GetParam<typename F::arg1_type>::getParam(pRuntime, 0)
	);
}

template <class F>
FORCE_INLINE void RDOFunCalcStd<F>::calc(CREF(LPRDORuntime) pRuntime, std_fun_two_param)
{
	m_value = m_pFunction(
		GetParam<typename F::arg1_type>::getParam(pRuntime, 0),
		GetParam<typename F::arg2_type>::getParam(pRuntime, 1)
	);
}

FORCE_INLINE double GetParam<double>::getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getDouble();
}

FORCE_INLINE int GetParam<int>::getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
{
	return pRuntime->getFuncArgument(paramNumber).getInt();
}

CLOSE_RDO_RUNTIME_NAMESPACE
