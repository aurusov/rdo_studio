// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/result/result.h"
#include "converter/smr2rdox/rdopmd.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_pmdlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void cnv_pmderror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResult::RDOPMDResult(const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
{
    LPRDOPMDResult pResult = Converter::s_converter()->findPMDResult(src_text());
    if (pResult)
    {
        Converter::s_converter()->error().push_only(this->src_info(), rdo::format("Показатель '%s' уже существует", src_text().c_str()));
        Converter::s_converter()->error().push_only(pResult->src_info(), "См. первое определение");
        Converter::s_converter()->error().push_done();
    }
}

RDOPMDResult::~RDOPMDResult()
{}

void RDOPMDResult::endOfCreation(const LPIResult& pResult)
{
    m_pResult = pResult;
    LPITrace trace = m_pResult.object_dynamic_cast<ITrace>();
    if (trace)
    {
        trace->setTraceID(Converter::s_converter()->getPMD_id());
    }
    Converter::s_converter()->insertPMDResult(this);
    // TODO перенести в конструктор rdo::runtime::RDOPMDResult
    Converter::s_converter()->runtime()->addRuntimeResult(m_pResult);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_src_info, const RDOParserSrcInfo& par_src_info)
    : RDOPMDResult(src_info)
{
    LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(res_src_info.src_text());
    if (!pResource)
    {
        Converter::s_converter()->error().error(res_src_info, rdo::format("Ресурс '%s' не найден", res_src_info.src_text().c_str()));
    }
/*
    if (!pResource->getType()->isPermanent())
    {
        Converter::s_converter()->error().push_only(res_src_info, "Наблюдать (watch_par) можно только за параметром постоянного ресурса");
        Converter::s_converter()->error().push_only(pResource->getType()->src_info(), "См. тип ресурса");
        Converter::s_converter()->error().push_done();
    }
*/
    LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_src_info.src_text());
    if (!pParam)
    {
        Converter::s_converter()->error().push_only(par_src_info, rdo::format("Параметр '%s' не найден", par_src_info.src_text().c_str()));
        Converter::s_converter()->error().push_only(pResource->src_info(), "См. ресурс");
        Converter::s_converter()->error().push_only(pResource->getType()->src_info(), "См. тип ресурса");
        Converter::s_converter()->error().push_done();
    }
    rdo::runtime::RDOType::Type typeID = pParam->getType()->type()->typeID();
    if (typeID != rdo::runtime::RDOType::Type::INT && typeID != rdo::runtime::RDOType::Type::REAL)
    {
        Converter::s_converter()->error().push_only(par_src_info, "Наблюдать можно только за параметром целого или вещественного типа");
        Converter::s_converter()->error().push_only(pParam->getType()->src_info(), "См. тип параметра");
        Converter::s_converter()->error().push_done();
    }

    rdo::runtime::LPRDOCalcConst pResID = rdo::Factory<rdo::runtime::RDOCalcConst>::create(rdo::runtime::RDOValue(pResource->getID()));
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchPar>::create(Converter::s_converter()->runtime(), src_text(), trace, res_src_info.src_text(), par_src_info.src_text(), pResID, pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(const RDOParserSrcInfo& src_info, bool trace, LPRDOFUNLogic pLogic)
    : RDOPMDResult(src_info)
{
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchState>::create(Converter::s_converter()->runtime(), src_text(), trace, pLogic->getCalc()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDResult(src_info)
{
    LPRDORTPResType pResType = Converter::s_converter()->findRTPResType(res_type_src_info.src_text());
    if (!pResType)
    {
        Converter::s_converter()->error().error(res_type_src_info, rdo::format("Тип ресурса '%s' не найден", res_type_src_info.src_text().c_str()));
    }
    if (!pResType->isTemporary())
    {
        Converter::s_converter()->error().push_only(res_type_src_info, "Показатель собирает информацию по временным ресурсам (temporary)");
        Converter::s_converter()->error().push_only(pResType->src_info(), "См. тип ресурса");
        Converter::s_converter()->error().push_done();
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDWatchTemp(src_info, res_type_src_info)
{
    LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::Type::UNKNOW, RDOParserSrcInfo(res_type_src_info.src_text()));
    ASSERT(pGroupLogic);
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchQuant>::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(LPRDOFUNLogic& pLogic)
{
    LPIResultWatchQuant pQuant = m_pResult.object_dynamic_cast<IResultWatchQuant>();
    ASSERT(pQuant);
    pQuant->setLogicCalc(pLogic->getCalc());
    Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
    LPIResultWatchQuant pQuant = m_pResult.object_dynamic_cast<IResultWatchQuant>();
    ASSERT(pQuant);
    pQuant->setLogicCalc(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
    Converter::s_converter()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(const RDOParserSrcInfo& src_info, bool trace, const RDOParserSrcInfo& res_type_src_info)
    : RDOPMDWatchTemp(src_info, res_type_src_info)
{
    LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::Type::UNKNOW, RDOParserSrcInfo(res_type_src_info.src_text()));
    ASSERT(pGroupLogic);
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDWatchValue>::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(LPRDOFUNLogic& pLogic, LPRDOFUNArithm& pArithm)
{
    LPIResultWatchValue pWatch = m_pResult.object_dynamic_cast<IResultWatchValue>();
    ASSERT(pWatch);
    pWatch->setLogicCalc (pLogic->getCalc()    );
    pWatch->setArithmCalc(pArithm->createCalc());
    Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(LPRDOFUNArithm& pArithm)
{
    LPIResultWatchValue pWatch = m_pResult.object_dynamic_cast<IResultWatchValue>();
    ASSERT(pWatch);
    pWatch->setLogicCalc (rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
    pWatch->setArithmCalc(pArithm->createCalc());
    Converter::s_converter()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(const RDOParserSrcInfo& src_info, LPRDOFUNArithm pArithm)
    : RDOPMDResult(src_info)
{
    endOfCreation(rdo::Factory<rdo::runtime::RDOPMDGetValue>::create(Converter::s_converter()->runtime(), src_text(), pArithm->createCalc()));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
