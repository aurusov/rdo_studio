#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class std_fun_one_param {};
class std_fun_two_param {};

template <class RT, class P1>
class std_fun1
{
public:
    typedef  RT                 return_type;
    typedef  P1                 arg1_type;
    typedef  RT                 (*function_type)(P1);
    typedef  std_fun_one_param  param_count;
};

template <class RT, class P1, class P2>
class std_fun2
{
public:
    typedef  RT                 return_type;
    typedef  P1                 arg1_type;
    typedef  P2                 arg2_type;
    typedef  RT                 (*function_type)(P1, P2);
    typedef  std_fun_two_param  param_count;
};

template <class T>
struct GetParam         { static T      getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber); };

template <>
struct GetParam<double> { static double getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber); };

template <>
struct GetParam<int>    { static int    getParam(const LPRDORuntime& pRuntime, std::size_t paramNumber); };

template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
    typedef typename F::function_type function_type;

    RDOFunCalcStd(function_type pFunction);

private:
    function_type m_pFunction;

    RDOValue calc(const LPRDORuntime& pRuntime, std_fun_one_param);
    RDOValue calc(const LPRDORuntime& pRuntime, std_fun_two_param);

    DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/function/calc_function_internal-inl.h"
