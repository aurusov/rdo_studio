/*
 * copyright: (c) RDO-Team, 2011
 * filename : std_fun.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 02.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/parser/std_fun.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_parser/param.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/calc/std_fun.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
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
#include "rdo_lib/rdo_runtime/rdobase.h"

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

		SYSTEMTIME tnow;
		::GetSystemTime(&tnow);
		ruint tnow1 = rdoRuntime::RDOSimulatorBase::getMSec(tnow);

		for (ruint i = 0; i < 10000000; i++)
		{
			pCalc->calcValue(runtime);
		}

		::GetSystemTime(&tnow);
		ruint tnow2 = rdoRuntime::RDOSimulatorBase::getMSec(tnow);
		ruint delta = tnow2 - tnow1;
		YYLTYPE pos = {1, 1, 1, 1, 1, 1};
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(pos), rdo::format(_T("delay %d"), delta));

		return m_value;
	}
};
#endif

void RDOParserSTDFUN::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

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

	LPRDOTypeParam intType     = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(),  RDOParserSrcInfo());
	LPRDOTypeParam realType    = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	LPRDOParam     pIntReturn  = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), intType );
	LPRDOParam     pRealReturn = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), realType);

	LPRDOFUNFunction fun   = rdo::Factory<RDOFUNFunction>::create(_T("Abs"), pRealReturn);
	LPRDOParam       param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcCos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcSin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcTan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IAbs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IntPower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create(static_cast<Function_D_DI::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("LogN"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
#ifndef SPEED_TEST
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(static_cast<Function_D_DD::function_type>(pow)));
#else
	fun->setFunctionCalc(rdo::Factory<Test>::create());
#endif

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(tan));

	// -----------------------------------------
	// И для маленьких букв
	// -----------------------------------------
	fun   = rdo::Factory<RDOFUNFunction>::create(_T("abs"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arccos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arcsin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arctan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("iabs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("intpower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create(static_cast<Function_D_DI::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("logn"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(tan));
}

CLOSE_RDO_PARSER_NAMESPACE
