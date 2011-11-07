/*!
  \copyright (c) RDO-Team, 2011
  \file      std_fun.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Стандартные функции языка
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_STD_FUN_H_
#define _LIB_RUNTIME_CALC_STD_FUN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   std_fun1
  \brief   Функция с одним параметром
*/
template <class RT, class P1>
class std_fun1
{
public:
	enum { arity = 1 };

	typedef RT return_type;
	typedef P1 arg1_type;
	typedef RT (*function_type)(P1);
};

/*!
  \class   std_fun2
  \brief   Функция с двумя параметрами
*/
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

#ifdef  OST_LINUX
	template<typename T>
	struct identity { typedef T type; };
#endif

/*!
  \class   RDOFunCalcStd
  \brief   Функции из пространства имен std C++
*/
template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
	typedef typename F::function_type function_type;

	RDOFunCalcStd(CREF(function_type) pFunction);

private:
	function_type m_pFunction;

	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	//template <int paramCount>
	FORCE_INLINE void calc(CREF(LPRDORuntime) pRuntime, ruint paramCount)
	{
		#if paramCount == 1
			m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0));
		#elif paramCount == 2
			m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0), getParam<F::arg2_type>(pRuntime, 1));
		#else
			NEVER_REACH_HERE;
		#endif
	}

#ifdef OST_WINDOWS
	template <class T>
	FORCE_INLINE T getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber);

	template <>
	FORCE_INLINE double getParam<double>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getDouble();
	}

	template <>
	FORCE_INLINE int getParam<int>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getInt();
	}
#endif // OST_WINDOWS

#ifdef OST_LINUX
	template <class T>
	FORCE_INLINE identity<T> getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber);

	template <class T>
	FORCE_INLINE identity<double> getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getDouble();
	}

	template <class T>
	FORCE_INLINE identity<int> getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getInt();
	}
#endif // OST_LINUX
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/std_fun.inl"

#endif // _LIB_RUNTIME_CALC_STD_FUN_H_
