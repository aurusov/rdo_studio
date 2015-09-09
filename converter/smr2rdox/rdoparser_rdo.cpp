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
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/calc/function/calc_function_internal.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdo::converter::smr2rdox::FileTypeIn type, t_bison_parse_fun parser_fun, t_flex_lexer_fun lexer_fun)
    : RDOParserItem(type, parser_fun, lexer_fun)
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

void RDOParserRDOItem::parse(Converter* pParser, std::istream& streamIn)
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

RDOLexer* RDOParserRDOItem::getLexer(Converter* pParser, std::istream* streamIn, std::ostream* streamOut)
{
    ASSERT(pParser);
    return new RDOLexer(pParser, streamIn, streamOut);
}

std::size_t RDOParserRDOItem::lexer_loc_line()
{
    if (m_pLexer)
    {
        return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
    }
    else
    {
        return std::size_t(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
    }
}

std::size_t RDOParserRDOItem::lexer_loc_pos()
{
    return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSS
// --------------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS()
    : RDOParserRDOItem(rdo::converter::smr2rdox::FileTypeIn::RSS, cnv_rssparse, cnv_rsslex)
{}

void RDOParserRSS::parse(Converter* pParser, std::istream& streamIn)
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
    : RDOParserItem(rdo::converter::smr2rdox::FileTypeIn::RSS, NULL, NULL)
{
    m_needStream = false;
}

void RDOParserRSSPost::parse(Converter* pParser)
{
    ASSERT(pParser);

    // В режиме совместимости со старым РДО создаем ресурсы по номерам их типов, а не по номерам самих ресурсов из RSS
#ifdef RDOSIM_COMPATIBLE
    for (const auto& rtp: pParser->getRTPResTypes())
    {
#endif
        for (const auto& rss: pParser->getRSSResources())
        {
#ifdef RDOSIM_COMPATIBLE
            if (rss->getType() == rtp)
            {
#endif
                rdo::runtime::LPRDOCalc calc = rss->createCalc();
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
    : RDOParserItem(rdo::converter::smr2rdox::FileTypeIn::FUN, NULL, NULL)
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

void RDOParserSTDFUN::parse(Converter* /*pParser*/)
{
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

    LPRDOTypeParam intType     = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__INT>::create(),  RDOParserSrcInfo());
    LPRDOTypeParam realType    = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__REAL>::create(), RDOParserSrcInfo());
    LPRDOParam     pIntReturn  = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), intType );
    LPRDOParam     pRealReturn = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), realType);

    LPRDOFUNFunction fun   = rdo::Factory<RDOFUNFunction>::create("Abs", pRealReturn);
    LPRDOParam       param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(fabs));

    fun   = rdo::Factory<RDOFUNFunction>::create("ArcCos", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(acos));

    fun   = rdo::Factory<RDOFUNFunction>::create("ArcSin", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(asin));

    fun   = rdo::Factory<RDOFUNFunction>::create("ArcTan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(atan));

    fun   = rdo::Factory<RDOFUNFunction>::create("Cos", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cos));

    fun   = rdo::Factory<RDOFUNFunction>::create("Cotan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D ::function_type>(cotan));

    fun   = rdo::Factory<RDOFUNFunction>::create("Exp", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(exp));

    fun   = rdo::Factory<RDOFUNFunction>::create("Floor", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(floor));

    fun   = rdo::Factory<RDOFUNFunction>::create("Frac", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(modf));

    fun   = rdo::Factory<RDOFUNFunction>::create("IAbs", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(static_cast<Function_I_I::function_type>(abs)));

    fun   = rdo::Factory<RDOFUNFunction>::create("IMax", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(maxLocal<int>));

    fun   = rdo::Factory<RDOFUNFunction>::create("IMin", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(minLocal<int>));

    fun   = rdo::Factory<RDOFUNFunction>::create("Int", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(intLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("IntPower", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create<Function_D_DI::function_type>(static_cast<Function_D_DI::function_type>(std::pow)));

    fun   = rdo::Factory<RDOFUNFunction>::create("Ln", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log));

    fun   = rdo::Factory<RDOFUNFunction>::create("Log10", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log10));

    fun   = rdo::Factory<RDOFUNFunction>::create("Log2", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log2Local));

    fun   = rdo::Factory<RDOFUNFunction>::create("LogN", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(logNLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("Max", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(maxLocal<double>));

    fun   = rdo::Factory<RDOFUNFunction>::create("Min", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(minLocal<double>));

    fun   = rdo::Factory<RDOFUNFunction>::create("Power", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(static_cast<Function_D_DD::function_type>(pow)));

    fun   = rdo::Factory<RDOFUNFunction>::create("Round", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("Sin", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sin));

    fun   = rdo::Factory<RDOFUNFunction>::create("Sqrt", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sqrt));

    fun   = rdo::Factory<RDOFUNFunction>::create("Tan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(tan));

    // -----------------------------------------
    // И для маленьких букв
    // -----------------------------------------
    fun   = rdo::Factory<RDOFUNFunction>::create("abs", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(fabs));

    fun   = rdo::Factory<RDOFUNFunction>::create("arccos", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(acos));

    fun   = rdo::Factory<RDOFUNFunction>::create("arcsin", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(asin));

    fun   = rdo::Factory<RDOFUNFunction>::create("arctan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(atan));

    fun   = rdo::Factory<RDOFUNFunction>::create("cos", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cos));

    fun   = rdo::Factory<RDOFUNFunction>::create("cotan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(cotan));

    fun   = rdo::Factory<RDOFUNFunction>::create("exp", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(exp));

    fun   = rdo::Factory<RDOFUNFunction>::create("floor", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(floor));

    fun   = rdo::Factory<RDOFUNFunction>::create("frac", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(modf));

    fun   = rdo::Factory<RDOFUNFunction>::create("iabs", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(static_cast<Function_I_I::function_type>(abs)));

    fun   = rdo::Factory<RDOFUNFunction>::create("imax", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(maxLocal<int>));

    fun   = rdo::Factory<RDOFUNFunction>::create("imin", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", intType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_II>::create<Function_I_II::function_type>(minLocal<int>));

    fun   = rdo::Factory<RDOFUNFunction>::create("int", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_I>::create<Function_I_I::function_type>(intLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("intpower", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", intType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create<Function_D_DI::function_type>(static_cast<Function_D_DI::function_type>(std::pow)));

    fun   = rdo::Factory<RDOFUNFunction>::create("ln", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log));

    fun   = rdo::Factory<RDOFUNFunction>::create("log10", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log10));

    fun   = rdo::Factory<RDOFUNFunction>::create("log2", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(log2Local));

    fun   = rdo::Factory<RDOFUNFunction>::create("logn", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(logNLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("max", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(maxLocal<double>));

    fun   = rdo::Factory<RDOFUNFunction>::create("min", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(minLocal<double>));

    fun   = rdo::Factory<RDOFUNFunction>::create("power", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    param = rdo::Factory<RDOParam>::create("p2", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create<Function_D_DD::function_type>(static_cast<Function_D_DD::function_type>(pow)));

    fun   = rdo::Factory<RDOFUNFunction>::create("round", pIntReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_I_D>::create<Function_I_D::function_type>(roundLocal));

    fun   = rdo::Factory<RDOFUNFunction>::create("sin", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sin));

    fun   = rdo::Factory<RDOFUNFunction>::create("sqrt", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(sqrt));

    fun   = rdo::Factory<RDOFUNFunction>::create("tan", pRealReturn);
    param = rdo::Factory<RDOParam>::create("p1", realType);
    fun->add(param);
    fun->setFunctionCalc(rdo::Factory<Function_D_D>::create<Function_D_D::function_type>(tan));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
