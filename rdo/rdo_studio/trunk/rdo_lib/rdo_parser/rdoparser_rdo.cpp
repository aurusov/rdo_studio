/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_common/rdostream.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_repository/rdorepository.h"
#include "rdo_lib/rdo_runtime/calc/std_fun.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from)
	: RDOParserItem(type, parser_fun, error_fun, lexer_fun, from)
	, m_pLexer(NULL)
{}

RDOParserRDOItem::~RDOParserRDOItem()
{
	if (m_pLexer)
	{
		delete m_pLexer;
		m_pLexer = NULL;
	}
}

void RDOParserRDOItem::parse(PTR(RDOParser) pParser)
{
	ASSERT(pParser);

	rdo::binarystream in_stream;
	switch (m_from)
	{
		case sf_repository:
			kernel->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData(m_type, in_stream));
			break;
		case sf_editor:
			kernel->sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData(m_type, in_stream));
			break;
	}
	if (in_stream.good())
	{
		parse(pParser, in_stream);
	}
}

void RDOParserRDOItem::parse(PTR(RDOParser) pParser, REF(std::istream) in_stream)
{
	ASSERT(pParser);

	if (m_pLexer)
		delete m_pLexer;

	std::ostringstream out_stream;
	m_pLexer = getLexer(pParser, &in_stream, &out_stream);

	if (m_pLexer && m_parser_fun)
		m_parser_fun(m_pLexer);
}

PTR(RDOLexer) RDOParserRDOItem::getLexer(PTR(RDOParser) pParser, PTR(std::istream) in_stream, PTR(std::ostream) out_stream)
{
	ASSERT(pParser);
	return new RDOLexer(pParser, in_stream, out_stream);
}

ruint RDOParserRDOItem::lexer_loc_line()
{
	if (m_pLexer)
	{
		return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
	}
	else
	{
		return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE;
	}
}

ruint RDOParserRDOItem::lexer_loc_pos()
{
	return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS(StreamFrom from)
	: RDOParserRDOItem(rdoModelObjects::RSS, rssparse, rsserror, rsslex, from)
{}

void RDOParserRSS::parse(PTR(RDOParser) pParser)
{
	ASSERT(pParser);
	pParser->setHaveKWResources   (false);
	pParser->setHaveKWResourcesEnd(false);
	RDOParserRDOItem::parse(pParser);
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse(PTR(RDOParser) pParser)
{
	ASSERT(pParser);

	//! В режиме совместимости со старым РДО создаем ресурсы по номерам их типов, а не по номерам самих ресурсов из RSS
#ifdef RDOSIM_COMPATIBLE
	STL_FOR_ALL_CONST(pParser->getRTPResType(), rtp_it)
	{
#endif
		STL_FOR_ALL_CONST(pParser->getRSSResources(), rss_it)
		{
#ifdef RDOSIM_COMPATIBLE
			if ((*rss_it)->getType() == *rtp_it)
			{
#endif
				rdoRuntime::LPRDOCalc calc = (*rss_it)->createCalc();
				pParser->runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSMRPost
// ----------------------------------------------------------------------------
RDOParserSMRPost::RDOParserSMRPost()
	: RDOParserItem(rdoModelObjects::SMR, NULL, NULL, NULL)
{}

void RDOParserSMRPost::parse(PTR(RDOParser) pParser)
{
	ASSERT(pParser);

	//! Планирование событий, описанных в SMR
	STL_FOR_ALL_CONST(pParser->getEvents(), eventIt)
	{
		LPRDOEvent pEvent = *eventIt;
		ASSERT(pEvent);

		rdoRuntime::LPRDOCalc pInitCalc = pEvent->getInitCalc();
		if (pInitCalc)
		{
			pParser->runtime()->addInitCalc(pInitCalc);
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserEVNPost
// ----------------------------------------------------------------------------
void RDOParserEVNPost::parse(PTR(RDOParser) pParser)
{
	ASSERT(pParser);

	//! Позднее связывание для планирования событий
	STL_FOR_ALL_CONST(pParser->getEvents(), eventIt)
	{
		LPRDOEvent pEvent = *eventIt;

		LPRDOPATPattern pPattern = pParser->findPATPattern(pEvent->name());
		if (!pPattern)
		{
			STL_FOR_ALL_CONST(pEvent->getCalcList(), calcIt)
			{
				pParser->error().push_only((*calcIt)->src_info(), rdo::format(_T("Попытка запланировать неизвестное событие: %s"), pEvent->name().c_str()));
			}
			pParser->error().push_done();
		}
		if (pPattern->getType() != RDOPATPattern::PT_Event)
		{
			STL_FOR_ALL_CONST(pEvent->getCalcList(), calcIt)
			{
				pParser->error().push_only((*calcIt)->src_info(), rdo::format(_T("Паттерн %s не является событием: %s"), pEvent->name().c_str()));
			}
			pParser->error().push_done();
		}

		if (pEvent->getRegular())
		{
			LPIBaseOperation pRuntimeEvent = static_cast<PTR(rdoRuntime::RDOPatternEvent)>(pPattern->getPatRuntime())->createActivity(pParser->runtime()->m_pMetaLogic, pParser->runtime(), pEvent->name());
			ASSERT(pRuntimeEvent);
			pEvent->setRuntimeEvent(pRuntimeEvent);

			STL_FOR_ALL(pEvent->getCalcList(), calcIt)
			{
				(*calcIt)->setEvent(pRuntimeEvent);
			}
		}
		else
		{
			NEVER_REACH_HERE; //потому что нерегулярных событий больше нет
		}
	}
}

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

class Test: public rdoRuntime::RDOFunCalc
{
public:
	REF(rdoRuntime::RDOValue) doCalc(PTR(rdoRuntime::RDORuntime) runtime)
	{
		typedef rdoRuntime::std_fun1<double, double>  StdFun_D_D;
		typedef rdoRuntime::RDOFunCalcStd<StdFun_D_D> Function_D_D;

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<Function_D_D>::create(sin);
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

void RDOParserSTDFUN::parse(PTR(RDOParser) pParser)
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
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
#ifndef SPEED_TEST
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sin));
#else
	fun->setFunctionCalc(rdo::Factory<Test>::create());
#endif

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
