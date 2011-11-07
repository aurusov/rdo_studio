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
inline REF(RDOValue) RDOFunCalcStd<F>::doCalc(CREF(LPRDORuntime) pRuntime)
{
	calc(pRuntime, F::arity);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
