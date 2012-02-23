/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/parser/std_fun.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.04.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <cmath>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/parser/std_fun.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/calc/function/calc_function_internal.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSTDFUN
// --------------------------------------------------------------------------------
double cotan(double value)
{
	return 1.0 / tan(value);
}

int intLocal(int value)
{
	return value;
}

double log2Local(double value)
{
	return log(value) / log(2.0);
}

double logNLocal(double value1, double value2)
{
	return log(value1) / log(value2);
}

template <class T>
T maxLocal(T value1, T value2)
{
	return value1 > value2 ? value1 : value2;
}

template <class T>
T minLocal(T value1, T value2)
{
	return value1 < value2 ? value1 : value2;
}

double modf(double value)
{
	double tmp;
	return ::modf(value, &tmp);
}

int roundLocal(double value)
{
	return (int)floor(value + 0.5);
}

//#define SPEED_TEST

#ifdef SPEED_TEST
#include "simulator/runtime/rdobase.h"

class MaxCalc: public rdoRuntime::RDOFunCalc
{
private:
	REF(rdoRuntime::RDOValue) doCalc(PTR(rdoRuntime::RDORuntime) pRuntime)
	{
		m_value = max(pRuntime->getFuncArgument(0).getDouble(), pRuntime->getFuncArgument(1).getDouble());
		return m_value;
	}
};

class Test: public rdoRuntime::RDOFunCalc
{
public:
	REF(rdoRuntime::RDOValue) doCalc(PTR(rdoRuntime::RDORuntime) runtime)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<MaxCalc>::create();
		ASSERT(pCalc);


		boost::posix_time::ptime tnow = boost::posix_time::microsec_clock::local_time();
		ruint tnow1 = rdoRuntime::RDOSimulatorBase::getMSec(tnow);

		for (ruint i = 0; i < 10000000; i++)
		{
			pCalc->calcValue(runtime);
		}

		tnow = boost::posix_time::microsec_clock::local_time();
		ruint tnow2 = rdoRuntime::RDOSimulatorBase::getMSec(tnow);
		ruint delta = tnow2 - tnow1;
		YYLTYPE pos = {1, 1, 1, 1, 1, 1};
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(pos), rdo::format(_T("delay %d"), delta));

		return m_value;
	}
};
#endif

void RDOParserSTDFUN::generate(CREF(tstring) name, CREF(rdoRuntime::LPRDOFunCalc) pCalc, CREF(LPRDOParam) pReturnType, CREF(ParamList) paramList) const
{
	tstring nameLower = name;
	rdo::toLower(nameLower);
	generateReal(name,      pCalc, pReturnType, paramList);
	generateReal(nameLower, pCalc, pReturnType, paramList);
}

void RDOParserSTDFUN::generateReal(CREF(tstring) name, CREF(rdoRuntime::LPRDOFunCalc) pCalc, CREF(LPRDOParam) pReturnType, CREF(ParamList) paramList) const
{
	ASSERT(pCalc);

	LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(name, pReturnType);
	ASSERT(pFunction);

	ruint paramIndex = 1;
	STL_FOR_ALL_CONST(paramList, it)
	{
		LPRDOParam pParam = rdo::Factory<RDOParam>::create(rdo::format(_T("p%d"), paramIndex), *it);
		ASSERT(pParam);
		pFunction->add(pParam);
		paramIndex++;
	}

	pFunction->setFunctionCalc(pCalc);
	pFunction->end();
}

void RDOParserSTDFUN::parse(CREF(LPRDOParser) pParser)
{
	UNUSED(pParser);

	typedef rdoRuntime::std_fun1<double, double>         StdFun_D_D;
	typedef rdoRuntime::std_fun2<double, double, double> StdFun_D_DD;
	typedef rdoRuntime::std_fun2<double, double, int>    StdFun_D_DI;
	typedef rdoRuntime::std_fun1<int,    int>            StdFun_I_I;
	typedef rdoRuntime::std_fun2<int,    int, int>       StdFun_I_II;
	typedef rdoRuntime::std_fun1<int,    double>         StdFun_I_D;

	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_D>  Function_D_D;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_DD> Function_D_DD;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_DI> Function_D_DI;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_I>  Function_I_I;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_II> Function_I_II;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_D>  Function_I_D;

	LPTypeInfo  intType     = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(),  RDOParserSrcInfo());
	LPTypeInfo  realType    = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	LPRDOParam  pIntReturn  = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), intType );
	LPRDOParam  pRealReturn = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), realType);
	
	generate(_T("Abs"),      rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(fabs),          pRealReturn, ParamList(realType));
	generate(_T("ArcCos"),   rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(acos),          pRealReturn, ParamList(realType));
	generate(_T("ArcSin"),   rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(asin),          pRealReturn, ParamList(realType));
	generate(_T("ArcTan"),   rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(atan),          pRealReturn, ParamList(realType));
	generate(_T("Cos"),      rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(cos),           pRealReturn, ParamList(realType));
	generate(_T("Cotan"),    rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(cotan),         pRealReturn, ParamList(realType));
	generate(_T("Exp"),      rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(exp),           pRealReturn, ParamList(realType));
	generate(_T("Floor"),    rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(floor),         pIntReturn,  ParamList(realType));
	generate(_T("Frac"),     rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(modf),          pRealReturn, ParamList(realType));
	generate(_T("IAbs"),     rdo::Factory<Function_I_I> ::create<Function_I_I ::function_type>(static_cast<Function_I_I::function_type>(abs)),
	                                                                             pIntReturn,  ParamList(intType));
	generate(_T("IMax"),     rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(maxLocal<int>), pIntReturn,  ParamList(intType)(intType));
	generate(_T("IMin"),     rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(minLocal<int>), pIntReturn,  ParamList(intType)(intType));
	generate(_T("Int"),      rdo::Factory<Function_I_I> ::create<Function_I_I ::function_type>(intLocal),      pIntReturn,  ParamList(realType));
	generate(_T("IntPower"), rdo::Factory<Function_D_DI>::create<Function_D_DI::function_type>(static_cast<Function_D_DI::function_type>(std::pow)),
	                                                                             pRealReturn, ParamList(realType)(intType));
	generate(_T("Ln"),       rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(log),           pRealReturn, ParamList(realType));
	generate(_T("Log10"),    rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(log10),         pRealReturn, ParamList(realType));
	generate(_T("Log2"),     rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(log2Local),     pRealReturn, ParamList(realType));
	generate(_T("LogN"),     rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(logNLocal),     pRealReturn, ParamList(realType)(realType));
	generate(_T("Max"),      rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(maxLocal<double>),
	                                                                             pRealReturn, ParamList(realType)(realType));
	generate(_T("Min"),      rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(minLocal<double>),
	                                                                             pRealReturn, ParamList(realType)(realType));
	generate(_T("Power"),    rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(static_cast<Function_D_DD::function_type>(pow)),
	                                                                             pRealReturn, ParamList(realType)(realType));
	generate(_T("Round"),    rdo::Factory<Function_I_D> ::create<Function_I_D ::function_type>(roundLocal),    pIntReturn,  ParamList(realType));
	generate(_T("Sin"),      rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(sin),           pRealReturn, ParamList(realType));
	generate(_T("Sqrt"),     rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(sqrt),          pRealReturn, ParamList(realType));
	generate(_T("Tan"),      rdo::Factory<Function_D_D> ::create<Function_D_D ::function_type>(tan),           pRealReturn, ParamList(realType));

#ifndef SPEED_TEST
#else
	fun->setFunctionCalc(rdo::Factory<Test>::create());
#endif
}

CLOSE_RDO_PARSER_NAMESPACE
