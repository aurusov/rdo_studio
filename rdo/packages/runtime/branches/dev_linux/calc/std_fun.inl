/*!
  \copyright (c) RDO-Team, 2011
  \file      std_fun.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     Стандартные функции языка
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcStd
// --------------------------------------------------------------------------------
template <class F>
inline RDOFunCalcStd<F>::RDOFunCalcStd(CREF(function_type) pFunction)
	: m_pFunction(pFunction)
{}

template <class F>
REF(RDOValue) RDOFunCalcStd<F>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	//	calc<F::arity>(pRuntime);
	return m_value;
}

template <class F>
template <>
FORCE_INLINE void RDOFunCalcStd<F>::calc<1>(CREF(LPRDORuntime) pRuntime)
{
       m_value = m_pFunction(getParam<typename F::arg1_type>(pRuntime, 0));
}

template <class F>
template <>
FORCE_INLINE void RDOFunCalcStd<F>::calc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pFunction(getParam<typename F::arg1_type>(pRuntime, 0), getParam<F::arg2_type>(pRuntime, 1));
}

CLOSE_RDO_RUNTIME_NAMESPACE
