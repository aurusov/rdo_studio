// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/result/result_group.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int pmdlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void pmderror(YYLTYPE* /*llocp*/, void* /*lexer*/, YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResult::RDOPMDResult(const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
{}

RDOPMDResult::~RDOPMDResult()
{}

void RDOPMDResult::endOfCreation(const LPIResult& pResult)
{
    ASSERT(pResult);

    LPContext pContext = RDOParser::s_parser()->context();
    ASSERT(pContext);

    LPRDOResultGroup pResultGroup = pContext->cast<RDOResultGroup>();
    if (!pResultGroup)
    {
        // Показатель создаётся не в контексте группы
        // Такое может быть из rdoproc_rss.y
        pResultGroup = RDOParser::s_parser()->findResultGroup("");
        if (!pResultGroup)
        {
            // Нет даже группы по умолчанию
            pResultGroup = rdo::Factory<RDOResultGroup>::create();
            ASSERT(pResultGroup);
            pResultGroup->init(RDOParserSrcInfo());
        }
    }

    m_pResult = pResult;
    pResultGroup->append(this);

    LPITrace pTrace = m_pResult.object_dynamic_cast<ITrace>();
    if (pTrace)
        pTrace->setTraceID(RDOParser::s_parser()->getPMD_id());
}

// --------------------------------------------------------------------------------
// -------------------- ResultGroup
// --------------------------------------------------------------------------------
RDOResultGroup::RDOResultGroup()
{}

RDOResultGroup::~RDOResultGroup()
{}

void RDOResultGroup::init(const RDOParserSrcInfo& src_info)
{
    setSrcInfo(src_info);

    LPRDOResultGroup pResultGroupFound = RDOParser::s_parser()->findResultGroup(name());
    if (pResultGroupFound)
    {
        RDOParser::s_parser()->error().push_only(src_info, rdo::format("Группа показателей '%s' уже существует", src_text().c_str()));
        RDOParser::s_parser()->error().push_only(pResultGroupFound->src_info(), "См. первое определение");
        RDOParser::s_parser()->error().push_done();
    }
    RDOParser::s_parser()->insertResultGroup(this);
    m_pResultGroup = rdo::Factory<rdo::runtime::RDOPMDResultGroup>::create(name());
    ASSERT(m_pResultGroup);
    RDOParser::s_parser()->runtime()->addRuntimeResult(m_pResultGroup.object_dynamic_cast<IResult>());
}

const std::string& RDOResultGroup::name() const
{
    return src_text();
}

const LPIResultGroup& RDOResultGroup::getRuntime() const
{
    return m_pResultGroup;
}

void RDOResultGroup::append(const LPRDOPMDResult& pResult)
{
    ASSERT(pResult);
    LPRDOPMDResult pResultFound = find(pResult->name());
    if (pResultFound)
    {
        RDOParser::s_parser()->error().push_only(pResult->src_info(), rdo::format("Показатель '%s' уже существует", pResult->src_text().c_str()));
        RDOParser::s_parser()->error().push_only(pResultFound->src_info(), "См. первое определение");
        RDOParser::s_parser()->error().push_done();
    }
    m_resultList.push_back(pResult);
    m_pResultGroup->onAppend(pResult->getRuntime());
    RDOParser::s_parser()->runtime()->addRuntimeResult(pResult->getRuntime());
}

LPRDOPMDResult RDOResultGroup::find(const std::string& resultName) const
{
    for (const auto& result: m_resultList)
    {
        if (result->name() == resultName)
            return result;
    }
    return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(const RDOParserSrcInfo& src_info)
    : RDOPMDResult(src_info)
{}

RDOPMDWatchPar::~RDOPMDWatchPar()
{}

void RDOPMDWatchPar::init(bool trace, const RDOParserSrcInfo& res_src_info, const RDOParserSrcInfo& par_src_info)
{
    LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(res_src_info.src_text());
    if (!pResource)
    {
        RDOParser::s_parser()->error().error(res_src_info, rdo::format("Ресурс '%s' не найден", res_src_info.src_text().c_str()));
    }
/*
    if (!pResource->getType()->isPermanent())
    {
        RDOParser::s_parser()->error().push_only(res_src_info, "Наблюдать (watch_par) можно только за параметром постоянного ресурса");
        RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), "См. тип ресурса");
        RDOParser::s_parser()->error().push_done();
    }
*/
    LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_src_info.src_text());
    if (!pParam)
    {
        RDOParser::s_parser()->error().push_only(par_src_info, rdo::format("Параметр '%s' не найден", par_src_info.src_text().c_str()));
        RDOParser::s_parser()->error().push_only(pResource->src_info(), "См. ресурс");
        RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), "См. тип ресурса");
        RDOParser::s_parser()->error().push_done();
    }
    checkParam(pParam);

    rdo::runtime::LPRDOCalc pResCalc =
            rdo::Factory<rdo::runtime::RDOCalcGetResourceByID>::create(pResource->getID());
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchPar>::create(
            RDOParser::s_parser()->runtime(),
            src_text(),
            trace,
            res_src_info.src_text(),
            par_src_info.src_text(),
            pResCalc,
            pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

void RDOPMDWatchPar::initFromParam(bool trace, const LPRDORTPParam& pParam, const rdo::runtime::LPRDOCalc& pResCalc, const std::size_t paramNum)
{
    const std::string resName;
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchPar>::create(
            RDOParser::s_parser()->runtime(),
            src_text(),
            trace,
            resName,
            pParam->src_info().src_text(),
            pResCalc,
            paramNum));
}

void RDOPMDWatchPar::checkParam(const LPRDORTPParam& pParam)
{
    const rdo::runtime::RDOType::Type typeID = pParam->getTypeInfo()->typeID();
    if (typeID != rdo::runtime::RDOType::Type::INT && typeID != rdo::runtime::RDOType::Type::REAL)
    {
        RDOParser::s_parser()->error().push_only(
            pParam->src_info(),
            "Наблюдать можно только за параметром целого или вещественного типа"
        );
        RDOParser::s_parser()->error().push_only(pParam->getTypeInfo()->src_info(), "См. тип параметра");
        RDOParser::s_parser()->error().push_done();
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(const RDOParserSrcInfo& src_info)
    : RDOPMDResult(src_info)
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

void RDOPMDWatchState::init(bool trace, LPRDOFUNLogic pLogic)
{
    endOfCreation(
            rdo::Factory<rdo::runtime::RDOPMDWatchState>::create(RDOParser::s_parser()->runtime(),
            src_text(),
            trace,
            pLogic->getCalc()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDResult(src_info)
{
    LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(res_type_src_info.src_text());
    if (!pResType)
    {
        RDOParser::s_parser()->error().error(
                res_type_src_info,
                rdo::format("Тип ресурса '%s' не найден", res_type_src_info.src_text().c_str()));
    }

    if (!pResType->isTemporary())
    {
        RDOParser::s_parser()->error().push_only(res_type_src_info, "Показатель собирает информацию по временным ресурсам (temporary)");
        RDOParser::s_parser()->error().push_only(pResType->src_info(), "См. тип ресурса");
        RDOParser::s_parser()->error().push_done();
    }
}

RDOPMDWatchTemp::~RDOPMDWatchTemp()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDWatchTemp(src_info, res_type_src_info)
{}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

void RDOPMDWatchQuant::init(bool trace, const RDOParserSrcInfo& res_type_src_info)
{
    m_pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(
            RDOFUNGroupLogic::Type::UNKNOW,
            RDOParserSrcInfo(res_type_src_info.src_text()));
    ASSERT(m_pGroupLogic);
    endOfCreation(
            rdo::Factory<rdo::runtime::RDOPMDWatchQuant>::create(RDOParser::s_parser()->runtime(),
            src_text(),
            trace,
            res_type_src_info.src_text(),
            m_pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(LPRDOFUNLogic& pLogic)
{
    LPIResultWatchQuant pQuant = m_pResult.object_dynamic_cast<IResultWatchQuant>();
    ASSERT(pQuant);
    pQuant->setLogicCalc(pLogic->getCalc());
    m_pGroupLogic->end();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
    LPIResultWatchQuant pQuant = m_pResult.object_dynamic_cast<IResultWatchQuant>();
    ASSERT(pQuant);
    pQuant->setLogicCalc(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
    m_pGroupLogic->end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDWatchTemp(src_info, res_type_src_info)
{}

RDOPMDWatchValue::~RDOPMDWatchValue()
{}

void RDOPMDWatchValue::init(bool trace, const RDOParserSrcInfo& res_type_src_info)
{
    m_pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(
            RDOFUNGroupLogic::Type::UNKNOW,
            RDOParserSrcInfo(res_type_src_info.src_text()));
    ASSERT(m_pGroupLogic);

    endOfCreation(
            rdo::Factory<rdo::runtime::RDOPMDWatchValue>::create(RDOParser::s_parser()->runtime(),
            src_text(),
            trace,
            res_type_src_info.src_text(),
            m_pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(LPRDOFUNLogic& pLogic, LPRDOFUNArithm& pArithm)
{
    LPIResultWatchValue pWatch = m_pResult.object_dynamic_cast<IResultWatchValue>();
    ASSERT(pWatch);
    pWatch->setLogicCalc(pLogic->getCalc());
    pWatch->setArithmCalc(pArithm->createCalc());
    m_pGroupLogic->end();
}

void RDOPMDWatchValue::setLogicNoCheck(LPRDOFUNArithm& pArithm)
{
    LPIResultWatchValue pWatch = m_pResult.object_dynamic_cast<IResultWatchValue>();
    ASSERT(pWatch);
    pWatch->setLogicCalc(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
    pWatch->setArithmCalc(pArithm->createCalc());
    m_pGroupLogic->end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(const RDOParserSrcInfo& src_info)
    : RDOPMDResult(src_info)
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

void RDOPMDGetValue::init(LPRDOFUNArithm pArithm)
{
    endOfCreation(
            rdo::Factory<rdo::runtime::RDOPMDGetValue>::create(RDOParser::s_parser()->runtime(),
            src_text(),
            pArithm->createCalc()));
}

CLOSE_RDO_PARSER_NAMESPACE
