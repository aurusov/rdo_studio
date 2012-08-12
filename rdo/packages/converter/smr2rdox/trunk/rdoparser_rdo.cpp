/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_rdo.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_rdo.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdosmr.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdosmr.h"
#include "converter/smr2rdox/rdopat.h"
#include "simulator/runtime/rdo_pattern.h"
#include "utils/rdostream.h"
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/calc/function/calc_function_internal.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdo::converter::smr2rdox::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun)
	: RDOParserItem(type, parser_fun, error_fun, lexer_fun)
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

void RDOParserRDOItem::parse(PTR(Converter) pParser, REF(std::istream) streamIn)
{
	ASSERT(pParser);

	if (!streamIn.good())
		return;

	if (m_pLexer)
		delete m_pLexer;

	std::ostringstream streamOut;
	m_pLexer = getLexer(pParser, &streamIn, &streamOut);

	if (m_pLexer && m_parser_fun)
		m_parser_fun(m_pLexer);
}

PTR(RDOLexer) RDOParserRDOItem::getLexer(PTR(Converter) pParser, PTR(std::istream) streamIn, PTR(std::ostream) streamOut)
{
	ASSERT(pParser);
	return new RDOLexer(pParser, streamIn, streamOut);
}

ruint RDOParserRDOItem::lexer_loc_line()
{
	if (m_pLexer)
	{
		return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
	}
	else
	{
		return ruint(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
	}
}

ruint RDOParserRDOItem::lexer_loc_pos()
{
	return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSS
// --------------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS()
	: RDOParserRDOItem(rdo::converter::smr2rdox::RSS_IN, cnv_rssparse, cnv_rsserror, cnv_rsslex)
{}

void RDOParserRSS::parse(PTR(Converter) pParser, REF(std::istream) streamIn)
{
	ASSERT(pParser);
	pParser->setHaveKWResources   (false);
	pParser->setHaveKWResourcesEnd(false);
	RDOParserRDOItem::parse(pParser, streamIn);
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSSPost
// --------------------------------------------------------------------------------
RDOParserRSSPost::RDOParserRSSPost()
	: RDOParserItem(rdo::converter::smr2rdox::RSS_IN, NULL, NULL, NULL)
{
	m_needStream = false;
}

void RDOParserRSSPost::parse(PTR(Converter) pParser)
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
				rdo::runtime::LPRDOCalc calc = (*rss_it)->createCalc();
				pParser->runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserSTDFUN
// --------------------------------------------------------------------------------
RDOParserSTDFUN::RDOParserSTDFUN()
	: RDOParserItem(rdo::converter::smr2rdox::FUN_IN, NULL, NULL, NULL)
{
	m_needStream = false;
}

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

void RDOParserSTDFUN::parse(PTR(Converter) pParser)
{
	UNUSED(pParser);

	typedef rdo::runtime::std_fun1<double, double>         StdFun_D_D;
	typedef rdo::runtime::std_fun2<double, double, double> StdFun_D_DD;
	typedef rdo::runtime::std_fun2<double, double, int>    StdFun_D_DI;
	typedef rdo::runtime::std_fun1<int,    int>            StdFun_I_I;
	typedef rdo::runtime::std_fun2<int,    int, int>       StdFun_I_II;
	typedef rdo::runtime::std_fun1<int,    double>         StdFun_I_D;

	typedef rdo::runtime::RDOFunCalcStd<StdFun_D_D>  Function_D_D;
	typedef rdo::runtime::RDOFunCalcStd<StdFun_D_DD> Function_D_DD;
	typedef rdo::runtime::RDOFunCalcStd<StdFun_D_DI> Function_D_DI;
	typedef rdo::runtime::RDOFunCalcStd<StdFun_I_I>  Function_I_I;
	typedef rdo::runtime::RDOFunCalcStd<StdFun_I_II> Function_I_II;
	typedef rdo::runtime::RDOFunCalcStd<StdFun_I_D>  Function_I_D;

	LPRDOTypeParam intType     = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(),  RDOParserSrcInfo());
	LPRDOTypeParam realType    = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	LPRDOParam     pIntReturn  = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), intType );
	LPRDOParam     pRealReturn = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), realType);

	LPRDOFUNFunction fun   = rdo::Factory<RDOFUNFunction>::create(_T("Abs"), pRealReturn);
	LPRDOParam       param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcCos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcSin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcTan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D ::function_type>(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IAbs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IntPower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create<Function_D_DI::function_type>(static_cast<Function_D_DI::function_type>(std::pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("LogN"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(tan));

	// -----------------------------------------
	// И для маленьких букв
	// -----------------------------------------
	fun   = rdo::Factory<RDOFUNFunction>::create(_T("abs"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arccos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arcsin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arctan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("iabs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("intpower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create<Function_D_DI::function_type>(static_cast<Function_D_DI::function_type>(std::pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("logn"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create<Function_I_D::function_type>(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(tan));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
