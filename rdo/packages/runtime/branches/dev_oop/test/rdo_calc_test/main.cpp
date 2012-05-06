/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.05.2012
  \brief     Тест для калков
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOCalc_Test
#include <boost/test/included/unit_test.hpp>
#include <boost/tuple/tuple.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
#include "simulator/runtime/calc/operation/calc_logic.h"
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDOCalc_Test)

static const double const1 = 10.5;
static const double const2 = 11.4;

typedef  boost::tuple<LPRDORuntime, LPRDOCalc, LPRDOCalc>  CalcPair;

template <class T>
RDOValue calc(CREF(CalcPair) calcPair)
{
	BOOST_CHECK(calcPair.get<0>());
	BOOST_CHECK(calcPair.get<1>());
	BOOST_CHECK(calcPair.get<2>());

	rdo::intrusive_ptr<T> pCalc = rdo::Factory<T>::create(calcPair.get<1>(), calcPair.get<2>());
	ASSERT(pCalc);

	return pCalc->calcValue(calcPair.get<0>());
}

CalcPair prepair()
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOCalc pLeft = rdo::Factory<RDOCalcConst>::create(RDOValue(const1));
	BOOST_CHECK(pLeft);

	LPRDOCalc pRight = rdo::Factory<RDOCalcConst>::create(RDOValue(const2));
	BOOST_CHECK(pRight);

	return CalcPair(pRuntime, pLeft, pRight);
}

BOOST_AUTO_TEST_CASE(RDOCalc_MultDouble)
{
	RDOValue result = calc<RDOCalcMult>(prepair());
	BOOST_CHECK(result.getDouble() == const1 * const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_DivDouble)
{
	RDOValue result = calc<RDOCalcDiv>(prepair());
	BOOST_CHECK(result.getDouble() == const1 / const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_PlusDouble)
{
	RDOValue result = calc<RDOCalcPlus>(prepair());
	BOOST_CHECK(result.getDouble() == const1 + const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_MinusDouble)
{
	RDOValue result = calc<RDOCalcMinus>(prepair());
	BOOST_CHECK(result.getDouble() == const1 - const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_RecursSimulator)
{
	struct RecursCalcSimulator
	{
		struct Param
		{
			Param(int value)
				: m_value(value)
			{}

			//! Важно, чтобы тип был int, а не int&
			//! Иначе результаты обоих функций будут по 1
			//! А сейчас 1 и 120
			int value() 
			{
				return m_value;
			}

			Param& dec()
			{
				--m_value;
				return *this;
			}

		private:
			int m_value;
		};

		int funError(Param& param)
		{
			if (param.value() == 1)
			{
				return 1;
			}
			else
			{
				return funError(param.dec()) * param.value();
			}
		}

		int funOk(Param& param)
		{
			if (param.value() == 1)
			{
				return 1;
			}
			else
			{
				return param.value() * funOk(param.dec());
			}
		}
	};

	RecursCalcSimulator calc1;
	RecursCalcSimulator calc2;
	RecursCalcSimulator::Param param1(5);
	RecursCalcSimulator::Param param2(5);

	int resultError = calc1.funError(param1);
	int resultOk    = calc2.funOk   (param2);
	BOOST_CHECK(resultError == 1);
	BOOST_CHECK(resultOk    == 120);
}

BOOST_AUTO_TEST_CASE(RDOCalc_Recurs)
{
	struct generator
	{
		enum MultOrder
		{
			MO_FUN_PARAM,
			MO_PARAM_FUN
		};

		static LPRDOCalc create(MultOrder order)
		{
			//! ручная набивка тела функции вида
			//! 
			//! int fun(int param)
			//! {
			//!     if (param == 1)
			//!     {
			//!        return 1;
			//!     }
			//!     else
			//!     {
			//!        return fun(param - 1) * param;
			//!     }
			//! }

			LPRDOCalcReturnCatch pReturnCatch = rdo::Factory<RDOCalcReturnCatch>::create();
			BOOST_CHECK(pReturnCatch);

			LPRDOCalc pGetFunParam = rdo::Factory<RDOCalcFuncParam>::create(0, RDOSrcInfo());

			//! if (param == 1)
			LPRDOCalc pIFCondition;
			{
				LPRDOCalc pParamLeft = pGetFunParam;
				BOOST_CHECK(pParamLeft);

				LPRDOCalc pParamRight = rdo::Factory<RDOCalcConst>::create(RDOValue(rsint(1)));
				BOOST_CHECK(pParamRight);

				pIFCondition = rdo::Factory<RDOCalcIsEqual>::create(pParamLeft, pParamRight);
			}
			BOOST_CHECK(pIFCondition);

			LPRDOCalcIf pIf = rdo::Factory<RDOCalcIf>::create(pIFCondition);
			BOOST_CHECK(pIf);

			//! return 1
			LPRDOCalc pThen = rdo::Factory<RDOCalcFunReturn>::create(
				rdo::Factory<RDOCalcConst>::create(RDOValue(rsint(1)))
			);
			BOOST_CHECK(pThen);

			//! return fun(param - 1) * param
			LPRDOCalc pElse;
			{
				//! param - 1
				LPRDOCalc pParamValue;
				{
					LPRDOCalc pParamLeft = pGetFunParam;
					BOOST_CHECK(pParamLeft);

					LPRDOCalc pParamRight = rdo::Factory<RDOCalcConst>::create(RDOValue(rsint(1)));
					BOOST_CHECK(pParamRight);

					pParamValue = rdo::Factory<RDOCalcMinus>::create(pParamLeft, pParamRight);
				}
				BOOST_CHECK(pParamValue);

				//! Вызов fun(param - 1)
				LPRDOCalc pFunctionCaller = caller(pReturnCatch, pParamValue);
				BOOST_CHECK(pFunctionCaller);

				//! fun(param - 1) * param
				//! или 
				//! param * fun(param - 1)
				LPRDOCalc pMult;
				switch (order)
				{
				case MO_FUN_PARAM: pMult = rdo::Factory<RDOCalcMult>::create(pFunctionCaller, pGetFunParam); break;
				case MO_PARAM_FUN: pMult = rdo::Factory<RDOCalcMult>::create(pGetFunParam, pFunctionCaller); break;
				}
				BOOST_CHECK(pMult);

				pElse = rdo::Factory<RDOCalcFunReturn>::create(pMult);
			}
			BOOST_CHECK(pElse);

			pIf->setThenStatement(pThen);
			pIf->setElseStatement(pElse);

			pReturnCatch->setTryCalc(pIf);

			return externalCaller(pReturnCatch);
		}

	private:
		//! ручная набивка вызова функции int fun(5)
		static LPRDOCalc externalCaller(CREF(LPRDOCalc) pBody)
		{
			LPRDOCalc pParam = rdo::Factory<RDOCalcConst>::create(RDOValue(rsint(5)));
			BOOST_CHECK(pParam);

			return caller(pBody, pParam);
		}

		static LPRDOCalc caller(CREF(LPRDOCalc) pBody, CREF(LPRDOCalc) pParam)
		{
			BOOST_CHECK(pBody );
			BOOST_CHECK(pParam);

			LPRDOCalcFunctionCaller pFunctionCaller = rdo::Factory<RDOCalcFunctionCaller>::create(pBody);
			BOOST_CHECK(pFunctionCaller);

			pFunctionCaller->addParameter(pParam);

			return pFunctionCaller;
		}
	};

	RDOValue resultFunParam = generator::create(generator::MO_FUN_PARAM)->calcValue(rdo::Factory<RDORuntime>::create());
	tstring resultFunParamStr = resultFunParam.getAsString();
	BOOST_CHECK(resultFunParam.getInt() == 120);

	RDOValue resultParamFun = generator::create(generator::MO_PARAM_FUN)->calcValue(rdo::Factory<RDORuntime>::create());
	tstring resultParamFunStr = resultParamFun.getAsString();
	BOOST_CHECK(resultParamFun.getInt() == 1);
}

BOOST_AUTO_TEST_SUITE_END() // RDOCalc_Test

CLOSE_RDO_RUNTIME_NAMESPACE
