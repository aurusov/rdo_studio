// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_funlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void cnv_funerror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
    for (const auto& calc: m_intOrDouble)
    {
        calc->needRound();
    }
}

// --------------------------------------------------------------------------------
// -------------------- Набор макросов для генерации логических и арифметических выражений
// --------------------------------------------------------------------------------
#define CREATE_CALC(CALC, OPR) \
rdo::runtime::LPRDOCalcConst pConstCalc1 = m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>(); \
rdo::runtime::LPRDOCalcConst pConstCalc2 = pSecond->m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>(); \
rdo::runtime::LPRDOCalc pNewCalc; \
if (pConstCalc1 && pConstCalc2) \
{ \
    pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pConstCalc1->getValue() OPR pConstCalc2->getValue()); \
    pNewCalc->setSrcInfo(rdo::runtime::RDOCalc##CALC::getStaticSrcInfo(pConstCalc1, pConstCalc2)); \
} \
else \
{ \
    pNewCalc = rdo::Factory<rdo::runtime::RDOCalc##CALC>::create(m_pCalc, pSecond->m_pCalc); \
}

#define RETURN_LOGIC() \
LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false); \
pLogic->setSrcInfo(pNewCalc->srcInfo()); \
pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pLogic;

#define GENERATE_LOGIC(CALC, OPR) \
CREATE_CALC(CALC, OPR); \
RETURN_LOGIC();

#define CAST_ARITHM_VALUE(OPR, ERROR) \
try \
{ \
    if (beforeCastValue(pSecond) == CastResult::CONTINUE) \
    { \
        value()->value() OPR pSecond->value()->value(); \
    } \
} \
catch (const rdo::runtime::RDOValueException&) \
{ \
    Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(ERROR, type()->name().c_str(), pSecond->type()->name().c_str())); \
}

#define GET_ARITHM_PRE_TYPE() \
LPRDOType pNewType = getPreType(pSecond);

#define GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
CAST_ARITHM_VALUE(OPR, ERROR) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC(CALC, OPR)

#define RETURN_ARITHM() \
LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(rdo::Factory<RDOValue>::create(pNewType, pNewCalc->srcInfo()), pNewCalc); \
pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pArithm;

#define GENERATE_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_LOGIC()

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(const LPRDOFUNArithm& pArithm)
    : RDOParserSrcInfo(pArithm->src_info())
{
    switch (pArithm->typeID())
    {
    case rdo::runtime::RDOType::Type::BOOL: m_pCalc = pArithm->calc(); break;
    default                           : break;
    }
    if (!m_pCalc)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Ожидается логическое выражение, найдено: %s", src_text().c_str()));
    }
}

RDOFUNLogic::RDOFUNLogic(const rdo::runtime::LPRDOCalc& pCalc, bool hideWarning)
    : RDOParserSrcInfo(     )
    , m_pCalc         (pCalc)
{
    if (m_pCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
        srcInfo.setSrcFileType(src_filetype());
        m_pCalc->setSrcInfo(srcInfo);
    }
    if (!hideWarning)
    {
        rdo::runtime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>();
        if (pConstCalc)
        {
            if (pConstCalc->calcValue(Converter::s_converter()->runtime()).getAsBool())
            {
                Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format("Логическое выражение всегда истинно: %s", pConstCalc->srcInfo().src_text().c_str()));
            }
            else
            {
                Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format("Логическое выражение всегда ложно: %s", pConstCalc->srcInfo().src_text().c_str()));
            }
        }
    }
}

RDOFUNLogic::~RDOFUNLogic()
{}

rdo::runtime::LPRDOCalc RDOFUNLogic::getCalc(rdo::runtime::RDOType::Type id)
{
    if (id != rdo::runtime::RDOType::Type::REAL)
    {
        m_intOrDouble.roundCalc();
    }
    return m_pCalc;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (const LPRDOFUNLogic& pSecond)
{
    GENERATE_LOGIC(And, &&);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (const LPRDOFUNLogic& pSecond)
{
    GENERATE_LOGIC(Or, ||);
}

LPRDOFUNLogic RDOFUNLogic::operator_not()
{
    rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcNot>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
    LPRDOFUNLogic         pLogic   = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false);
    pLogic->setSrcInfo(pNewCalc->srcInfo());
    pLogic->m_intOrDouble.insert(m_intOrDouble);
    return pLogic;
}

void RDOFUNLogic::setSrcInfo(const RDOParserSrcInfo& src_info)
{
    RDOParserSrcInfo::setSrcInfo(src_info);
    if (m_pCalc)
    {
        m_pCalc->setSrcInfo(src_info);
    }
}

void RDOFUNLogic::setSrcPos(const RDOSrcInfo::Position& position)
{
    RDOParserSrcInfo::setSrcPos(position);
    if (m_pCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
        srcInfo.setSrcPos(position);
        m_pCalc->setSrcInfo(srcInfo);
    }
}

void RDOFUNLogic::setSrcText(const std::string& value)
{
    RDOParserSrcInfo::setSrcText(value);
    if (m_pCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
        srcInfo.setSrcText(value);
        m_pCalc->setSrcInfo(srcInfo);
    }
}

void RDOFUNLogic::setSrcPos(const YYLTYPE& error_pos)
{
    RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNLogic::setSrcPos(const YYLTYPE& pos_begin, const YYLTYPE& pos_end)
{
    RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(const LPRDOValue& pValue, const rdo::runtime::LPRDOCalc& pCalc)
    : m_pValue(pValue)
    , m_pCalc (pCalc )
{
    setSrcInfo(m_pValue->src_info());
}

RDOFUNArithm::RDOFUNArithm(const LPRDOValue& pValue)
{
    init(pValue);
}

RDOFUNArithm::RDOFUNArithm(const LPRDOValue& pResName, const LPRDOValue& pParName)
{
    init(pResName, pParName);
}

RDOFUNArithm::~RDOFUNArithm()
{}

void RDOFUNArithm::init(const LPRDOValue& pValue)
{
    setSrcInfo(pValue->src_info());

    if (pValue->constant())
    {
        m_pValue = rdo::Factory<RDOValue>::create(pValue);
        m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pValue->value());
        m_pCalc->setSrcInfo(src_info());
        return;
    }

    if (pValue->value().getIdentificator() == "Time_now" || pValue->value().getIdentificator() == "time_now" || pValue->value().getIdentificator() == "Системное_время" || pValue->value().getIdentificator() == "системное_время")
    {
        m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__REAL>::create().object_parent_cast<RDOType>());
        m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetTimeNow>::create();
        m_pCalc->setSrcInfo(src_info());
        return;
    }
    else if (pValue->value().getIdentificator() == "Terminate_counter" || pValue->value().getIdentificator() == "terminate_counter")
    {
        m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__INT>::create().object_parent_cast<RDOType>());
        m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetTermNow>::create();
        m_pCalc->setSrcInfo(src_info());
        return;
    }
    else if (pValue->value().getIdentificator() == "Seconds" || pValue->value().getIdentificator() == "seconds")
    {
        m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__REAL>::create().object_parent_cast<RDOType>());
        m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetSeconds>::create();
        m_pCalc->setSrcInfo(src_info());
        return;
    }

    // Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
    // перечислимых типах, поэтому какой именно из них выбрать - вопрос
    {ErrorBlockMonicker errorBlockMonicker;
        const Converter::PreCastTypeList& typeList = Converter::s_converter()->getPreCastTypeList();
        for (const auto& type: typeList)
        {
            LPRDOValue pTryCastValue = type->value_cast(pValue);
            if (pTryCastValue->defined())
            {
                m_pValue = rdo::Factory<RDOValue>::create(pValue);
                return;
            }
        }
    }

    // Ищем параметр релевантного ресурса
    if (Converter::s_converter()->getFileToParse() == rdo::converter::smr2rdox::FileTypeIn::PAT)
    {
        LPRDOPATPattern pPattern = Converter::s_converter()->getLastPATPattern();
        if (pPattern && pPattern->m_pCurrRelRes)
        {
            LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(pValue->value().getIdentificator());
            if (pParam)
            {
                LPRDOValue pParamName = rdo::Factory<RDOValue>::create(pPattern->m_pCurrRelRes->m_bodySrcInfo);
                init(pParamName, pValue);
                return;
            }
        }
    }

    // Ищем параметры паттерна или функции по имени
    LPRDOParam pFunctionParam;
    switch (Converter::s_converter()->getFileToParse())
    {
    case rdo::converter::smr2rdox::FileTypeIn::PAT: pFunctionParam = Converter::s_converter()->getLastPATPattern ()->findPATPatternParam (pValue->value().getIdentificator()); break;
    case rdo::converter::smr2rdox::FileTypeIn::FUN: pFunctionParam = Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParam(pValue->value().getIdentificator()); break;
    default: break;
    }

    // Ищем константы по имени
    LPRDOFUNConstant pConstant = Converter::s_converter()->findFUNConstant(pValue->value().getIdentificator());

    if (pConstant && pFunctionParam)
        Converter::s_converter()->error().error(src_info(), rdo::format("Имя параметра образца совпадает с именем константы: %s", pValue->value().getIdentificator().c_str()));

    if (pConstant)
    {
        m_pValue = rdo::Factory<RDOValue>::create(pConstant->getType()->type());
        m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetConst>::create(pConstant->getNumber());
        m_pCalc->setSrcInfo(src_info());
        return;
    }

    // Ищем последовательность по имени
    LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(pValue->value().getIdentificator());
    if (pSequence && pFunctionParam)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Имя параметра образца совпадает с именем последовательности: %s", pValue->value().getIdentificator().c_str()));
    }

    if (pSequence)
    {
        LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create();
        ASSERT(pParams);
        LPRDOFUNArithm pArithm = pParams->createSeqCall(pValue->value().getIdentificator());
        ASSERT(pArithm);
        pArithm->setSrcInfo(src_info());
        m_pValue =rdo::Factory<RDOValue>::create(pArithm->value());
        m_pCalc  = pArithm->m_pCalc;
        return;
    }

    if (pFunctionParam)
    {
        // Это параметр
        m_pValue = rdo::Factory<RDOValue>::create(pFunctionParam->getType()->type());
        switch (Converter::s_converter()->getFileToParse())
        {
        case rdo::converter::smr2rdox::FileTypeIn::PAT: m_pCalc = rdo::Factory<rdo::runtime::RDOCalcPatParam> ::create(Converter::s_converter()->getLastPATPattern ()->findPATPatternParamNum (pValue->value().getIdentificator())); break;
        case rdo::converter::smr2rdox::FileTypeIn::FUN: m_pCalc = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParamNum(pValue->value().getIdentificator()), pFunctionParam->src_info()); break;
        default: break;
        }

        if (m_pCalc)
            m_pCalc->setSrcInfo(src_info());

        return;
    }

    Converter::s_converter()->error().error(pValue->src_info(), rdo::format("Неизвестный идентификатор: %s", pValue->value().getIdentificator().c_str()));
}

void RDOFUNArithm::init(const LPRDOValue& pResName, const LPRDOValue& pParName)
{
    setSrcInfo(pResName->src_info(), ".", pParName->src_info());
    LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(pResName->value().getIdentificator());
    if (pResource)
    {
        // Это ресурс с закладки RSS
        const std::size_t parNumb = pResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
        if (parNumb == RDORTPResType::UNDEFINED_PARAM)
        {
            Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pParName->value().getIdentificator().c_str()));
        }
        if (pResource->getType()->isPermanent())
        {
            m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
        }
        else if (pResource->getType()->isTemporary() && Converter::s_converter()->getFileToParse() == rdo::converter::smr2rdox::FileTypeIn::FRM)
        {
            m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
        }
        else
        {
            Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Нельзя использовать временный ресурс: %s", pResName->value().getIdentificator().c_str()));
        }
        m_pCalc->setSrcInfo(src_info());
        m_pValue = rdo::Factory<RDOValue>::create(pResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
        return;
    }
    // Это не ресурс, но возможно, ресурс внутри групповой функции
    else if (!Converter::s_converter()->getFUNGroupStack().empty() && Converter::s_converter()->getFUNGroupStack().back()->getResType()->name() == pResName->value().getIdentificator())
    {
        // Это ресурс внутри групповой функции
        LPRDOFUNGroup pFUNGroup = Converter::s_converter()->getFUNGroupStack().back();
        const std::size_t parNumb = pFUNGroup->getResType()->getRTPParamNumber(pParName->value().getIdentificator());
        if (parNumb == RDORTPResType::UNDEFINED_PARAM)
        {
            Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pParName->value().getIdentificator().c_str()));
        }
        m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
        m_pCalc->setSrcInfo(src_info());
        m_pValue = rdo::Factory<RDOValue>::create(pFUNGroup->getResType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
        return;
    }
    else
    {
        // Возможно, это релевантный ресурс
        switch (Converter::s_converter()->getFileToParse())
        {
        case rdo::converter::smr2rdox::FileTypeIn::PAT:
            if (Converter::s_converter()->getLastPATPattern() && Converter::s_converter()->getLastPATPattern()->findRelevantResource(pResName->value().getIdentificator()))
            {
                // Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
                LPRDOPATPattern       pPattern          = Converter::s_converter()->getLastPATPattern();
                LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
                if (!pPattern->m_pCurrRelRes)
                {
                    // Внутри with_min-common-choice или $Time
                    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST || pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
                    {
                        Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", pRelevantResource->name().c_str()));
                    }
                }
                else
                {
                    // Внутри $Body
                    // Проверяем использование неинициализированного рел.ресурса (pRelevantResource) в Choice from другом рел.ресурсе (pPattern->m_pCurrRelRes)
                    if (pPattern->m_pCurrRelRes->isChoiceFromState())
                    {
                        if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
                        {
                            Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс не определен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", pRelevantResource->name().c_str()));
                        }
                        if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                        {
                            Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", pRelevantResource->name().c_str()));
                        }
                        if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
                        {
                            Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", pRelevantResource->name().c_str()));
                        }
                    }
                    // Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
                    if (pRelevantResource->getType()->isTemporary())
                    {
                        // В конверторе начала
                        if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_BEGIN)
                        {
                            if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE && !pRelevantResource->m_alreadyHaveConverter)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", pRelevantResource->name().c_str()));
                            }
                            if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE && pRelevantResource->m_alreadyHaveConverter)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", pRelevantResource->name().c_str()));
                            }
                            if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", pRelevantResource->name().c_str()));
                            }
                        }
                        // В конверторе конца
                        if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_END)
                        {
                            if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::CREATE && !pRelevantResource->m_alreadyHaveConverter)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", pRelevantResource->name().c_str()));
                            }
                            if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::ERASE && pRelevantResource->m_alreadyHaveConverter)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", pRelevantResource->name().c_str()));
                            }
                            if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                            {
                                Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", pRelevantResource->name().c_str()));
                            }
                        }
                    }
                    // Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
                    LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(pParName->value().getIdentificator());
                    if (pParam && pPattern->m_pCurrRelRes->name() == pResName->value().getIdentificator())
                    {
                        // В конверторе начала
                        if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_BEGIN && pPattern->m_pCurrRelRes->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
                        {
                            if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
                            {
                                if (!pParam->getDefault()->defined())
                                {
                                    Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по умолчанию в типе ресурса", pParName->value().getIdentificator().c_str()));
                                }
                            }
                        }
                        // В конверторе конца
                        if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_END && pPattern->m_pCurrRelRes->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::CREATE)
                        {
                            if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
                            {
                                if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
                                {
                                    if (!pParam->getDefault()->defined())
                                    {
                                        Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по умолчанию в типе ресурса", pParName->value().getIdentificator().c_str()));
                                    }
                                }
                            }
                        }
                    }
                }
                const std::size_t parNumb = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
                if (parNumb == RDORTPResType::UNDEFINED_PARAM)
                {
                    Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pParName->value().getIdentificator().c_str()));
                }
                m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
                m_pCalc->setSrcInfo(src_info());
                m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
                return;
            }
            break;

        case rdo::converter::smr2rdox::FileTypeIn::DPT:
            if (Converter::s_converter()->isCurrentDPTSearch() && Converter::s_converter()->getLastDPTSearch()->getLastActivity())
            {
                LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTSearch()->getLastActivity()->pattern();
                if (pPattern && pPattern->findRelevantResource(pResName->value().getIdentificator())) {
                    // Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
                    LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
                    const std::size_t parNumb = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
                    if (parNumb == RDORTPResType::UNDEFINED_PARAM)
                    {
                        Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pParName->value().getIdentificator().c_str()));
                    }
                    m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
                    m_pCalc->setSrcInfo(src_info());
                    m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
                    return;
                }
            }
            if (Converter::s_converter()->isCurrentDPTPrior() && Converter::s_converter()->getLastDPTPrior()->getLastActivity())
            {
                LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTPrior()->getLastActivity()->pattern();
                if (pPattern && pPattern->findRelevantResource(pResName->value().getIdentificator())) {
                    // Это ресурс, который используется в выражении приоритета активности DPTPrior
                    LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
                    const std::size_t parNumb = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
                    if (parNumb == RDORTPResType::UNDEFINED_PARAM)
                    {
                        Converter::s_converter()->error().error(pParName->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pParName->value().getIdentificator().c_str()));
                    }
                    m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
                    m_pCalc->setSrcInfo(src_info());
                    m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
                    return;
                }
            }
            break;

        default:
            break;
        }
    }
    Converter::s_converter()->error().error(pResName->src_info(), rdo::format("Неизвестный ресурс: %s", pResName->value().getIdentificator().c_str()));
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue(LPRDOFUNArithm pSecond)
{
    if (typeID() == rdo::runtime::RDOType::Type::ENUM && pSecond->typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        pSecond->m_pValue = rdo::Factory<RDOValue>::create(
            type()->value_cast(
                pSecond->value(),
                pSecond->src_info(),
                src_info()
            )->value(),
            enumType(),
            pSecond->m_pValue->src_info()
        );
        pSecond->m_pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pSecond->m_pValue->value());
        pSecond->m_pCalc->setSrcInfo(pSecond->src_info());
        return CastResult::DONE;
    }
    else if (typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR && pSecond->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        m_pValue = rdo::Factory<RDOValue>::create(
            pSecond->type()->value_cast(
                value(),
                src_info(),
                pSecond->src_info()
            )->value(),
            pSecond->enumType(),
            m_pValue->src_info()
        );
        m_pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pValue->value());
        m_pCalc->setSrcInfo(src_info());
        return CastResult::DONE;
    }
    return CastResult::CONTINUE;
}

LPRDOType RDOFUNArithm::getPreType(const LPRDOFUNArithm& pSecond)
{
    if (typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Внутренняя ошибка парсера: неопределенное значение арифметического выражения: %s", src_text().c_str()));
    }
    if (pSecond->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        Converter::s_converter()->error().error(pSecond->src_info(), rdo::format("Внутренняя ошибка парсера: неопределенное значение арифметического выражения: %s", pSecond->src_text().c_str()));
    }
    if (typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Неизвестный идентификатор: %s", value()->value().getIdentificator().c_str()));
    }
    if (pSecond->typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        Converter::s_converter()->error().error(pSecond->src_info(), rdo::format("Неизвестный идентификатор: %s", pSecond->value()->value().getIdentificator().c_str()));
    }

    // TODO смущают два одинаковых src_info(), проверить и доказать правильность
    return type()->type_cast(pSecond->type(), pSecond->src_info(), src_info(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (const LPRDOFUNArithm& pSecond)
{
    GENERATE_ARITHM(Plus, +, "Ну не могу я сложить %s и %s");
}

LPRDOFUNArithm RDOFUNArithm::operator- (const LPRDOFUNArithm& pSecond)
{
    GENERATE_ARITHM(Minus, -, "Нельзя из %s вычесть %s");
}

LPRDOFUNArithm RDOFUNArithm::operator* (const LPRDOFUNArithm& pSecond)
{
    GENERATE_ARITHM(Mult, *, "Нельзя %s умножить на %s");
}

LPRDOFUNArithm RDOFUNArithm::operator/ (const LPRDOFUNArithm& pSecond)
{
    GENERATE_ARITHM_CALC(Div, /, "Нельзя %s разделить на %s");
    // TODO перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
    // Ответ: с умножением надо делать тоже самое, только непонятно как
    if (pNewType->type()->typeID() == rdo::runtime::RDOType::Type::INT)
    {
        rdo::runtime::LPRDOCalc pNewCalcDiv = pNewCalc;
        pNewCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
        pNewCalc->setSrcInfo(pNewCalcDiv->srcInfo());
    }
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(pNewType, pNewCalc->srcInfo()),
        pNewCalc
    );
    if (pNewType->type()->typeID() == rdo::runtime::RDOType::Type::INT)
    {
        rdo::runtime::LPRDOCalcDoubleToIntByResult pResult = pNewCalc.object_static_cast<rdo::runtime::RDOCalcDoubleToIntByResult>();
        pArithm->m_intOrDouble.push_back(pResult);
    }
    pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
    return pArithm;
}

LPRDOFUNLogic RDOFUNArithm::operator< (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsLess, <, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator> (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsGreater, >, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator<= (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsLEQ, <=, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator>= (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsGEQ, >, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator== (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsEqual, ==, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator!= (const LPRDOFUNArithm& pSecond)
{
    GENERATE_LOGIC_FROM_ARITHM(IsNotEqual, !=, "Нельзя сравнивать %s и %s");
}

void RDOFUNArithm::checkParamType(const LPRDOTypeParam& pType)
{
    pType->type()->type_cast(type(), src_info(), pType->src_info(), src_info());
    rdo::runtime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
    if (pConstCalc)
    {
        rdo::runtime::RDOValue value = pConstCalc->getValue();
        pType->value_cast(rdo::Factory<RDOValue>::create(value, type(), src_info()));
    }
}

rdo::runtime::LPRDOCalc RDOFUNArithm::createCalc(const LPRDOTypeParam& pForType)
{
    if (typeID() != rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        if (!pForType)
        {
            return m_pCalc;
        }
        if (pForType->type()->typeID() != rdo::runtime::RDOType::Type::INT)
        {
            if (pForType->type()->typeID() == rdo::runtime::RDOType::Type::ENUM)
            {
                m_intOrDouble.roundCalc();
            }
            return m_pCalc;
        }
        else
        {
            m_intOrDouble.roundCalc();
            rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
            pNewCalc->setSrcInfo(src_info());
            return pNewCalc;
        }
    }
    else if (typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR && !pForType)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Неизвестный идентификатор: %s", value()->value().getAsString().c_str()));
    }

    if (!pForType)
    {
        Converter::s_converter()->error().error(src_info(), "Неизвестный тип параметра");
    }

    rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pForType->value_cast(rdo::Factory<RDOValue>::create(src_info()))->value());
    pNewCalc->setSrcInfo(src_info());
    return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(const RDOParserSrcInfo& src_info)
{
    RDOParserSrcInfo::setSrcInfo(src_info);
    if (m_pCalc)
    {
        m_pCalc->setSrcInfo(src_info);
    }
}

void RDOFUNArithm::setSrcPos(const RDOSrcInfo::Position& position)
{
    RDOParserSrcInfo::setSrcPos(position);
    if (m_pCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
        srcInfo.setSrcPos(position);
        m_pCalc->setSrcInfo(srcInfo);
    }
}

void RDOFUNArithm::setSrcText(const std::string& value)
{
    RDOParserSrcInfo::setSrcText(value);
    if (m_pCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
        srcInfo.setSrcText(src_info().src_text());
        m_pCalc->setSrcInfo(srcInfo);
    }
}

void RDOFUNArithm::setSrcInfo(const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end)
{
    RDOParserSrcInfo::setSrcInfo(begin, delim, end);
}

void RDOFUNArithm::setSrcPos(const YYLTYPE& error_pos)
{
    RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNArithm::setSrcPos(const YYLTYPE& pos_begin, const YYLTYPE& pos_end)
{
    RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(const RDOParserSrcInfo& src_info, const LPRDOTypeParam& pType, const LPRDOValue& pDefault)
    : RDOParam(src_info, pType, pDefault)
{
    if (!getDefault() || !getDefault()->defined())
    {
        ASSERT(pType);
        Converter::s_converter()->error().error(pType->src_info(), "Константа должна иметь значение");
    }

    m_number = Converter::s_converter()->getFUNCONST_id();
    Converter::s_converter()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// --------------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams()
    : RDOParserSrcInfo()
{}

RDOFUNParams::~RDOFUNParams()
{}

rdo::runtime::LPRDOCalc RDOFUNParams::getCalc(std::size_t paramID, const LPRDOTypeParam& pType)
{
    ASSERT(paramID < m_paramList.size());
    rdo::runtime::LPRDOCalc pCalc = m_paramList[paramID]->createCalc(pType);
    ASSERT(pCalc);
    return pCalc;
}

void RDOFUNParams::addParameter(const LPRDOFUNArithm& pParam)
{
    ASSERT(pParam);
    m_paramList.push_back(pParam);
}

LPRDOFUNArithm RDOFUNParams::createCall(const std::string& funName)
{
    LPRDOFUNFunction pFunction = Converter::s_converter()->findFUNFunction(funName);
    if (!pFunction)
    {
        return createSeqCall(funName);
    }

    const std::size_t nParams = pFunction->getParams().size();
    if (nParams != m_paramList.size())
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Неверное количество параметров функции: %s", funName.c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pFunction->getFunctionCalc());
    pFunction->insertPostLinked(pFuncCall);
    pFuncCall->setSrcInfo(src_info());
    for (std::size_t i = 0; i < nParams; i++)
    {
        LPRDOTypeParam pFuncParam = pFunction->getParams()[i]->getType();
        LPRDOFUNArithm pArithm = m_paramList[i];
        ASSERT(pArithm);
        pArithm->checkParamType(pFuncParam);
        pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->type()));
    }

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(pFunction->getReturn()->getType()->type(), src_pos()),
        pFuncCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(src_info());

    return pArithm;
}

LPRDOFUNArithm RDOFUNParams::createSeqCall(const std::string& seqName)
{
    LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(seqName);
    if (!pSequence)
    {
        Converter::s_converter()->error().error(m_funseqName, rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()));
    }
    LPRDOFUNParams pThis(this);
    ASSERT(pThis);
    LPRDOFUNArithm pArithm = pSequence->createCallCalc(pThis, src_info());
    ASSERT(pArithm);
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequence
// --------------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOParserSrcInfo(pHeader->src_info())
    , m_pHeader       (pHeader            )
    , m_seed          (seed               )
{
    Converter::s_converter()->insertFUNSequence(this);
}

RDOFUNSequence::~RDOFUNSequence()
{}

void RDOFUNSequence::initResult()
{
    initCalcSrcInfo();
    m_pNextCalc->m_res_real = true;
    switch (m_pHeader->getType()->type()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT:
        {
            LPRDOTypeIntRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
            if (pRange)
            {
                m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
            }
            break;
        }
        case rdo::runtime::RDOType::Type::REAL:
        {
            m_pNextCalc->m_res_real = true;
            LPRDOTypeRealRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
            if (pRange)
            {
                m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
            }
            break;
        }
        default:
            break;
    }
}

void RDOFUNSequence::initCalcSrcInfo()
{
    if (m_pInitCalc)
    {
        rdo::runtime::RDOSrcInfo srcInfo(m_pHeader->src_info());
        srcInfo.setSrcText("Установка базы последовательности " + m_pHeader->src_info().src_text());
        m_pInitCalc->setSrcInfo(srcInfo);
    }
    m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceUniform
// --------------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        Converter::s_converter()->error().error(src_info(), "Последовательность типа uniform не может возвращять значения перечислимого типа");
    }
}

void RDOFUNSequenceUniform::createCalcs()
{
    rdo::runtime::RandGeneratorUniform* pGenerator = new rdo::runtime::RandGeneratorUniform();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextUniform>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const
{
    if (pParamList->getParamList().size() != 2)
    {
        Converter::s_converter()->error().error(seq_src_info, rdo::format("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__REAL>::create(), RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
    rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

    pFuctionCall->addParameter(pArg1);
    pFuctionCall->addParameter(pArg2);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
        pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(seq_src_info);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        Converter::s_converter()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getType()->type()->typeID() != rdo::runtime::RDOType::Type::INT && m_pHeader->getType()->type()->typeID() != rdo::runtime::RDOType::Type::REAL)
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()));
    }
}

void RDOFUNSequenceExponential::createCalcs()
{
    rdo::runtime::RandGeneratorExponential* pGenerator = new rdo::runtime::RandGeneratorExponential();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextExponential>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const
{
    if (pParamList->getParamList().size() != 1)
    {
        Converter::s_converter()->error().error(seq_src_info, rdo::format("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__REAL>::create(), RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

    pFuctionCall->addParameter(pArg1);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
        pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(seq_src_info);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        Converter::s_converter()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        Converter::s_converter()->error().error(src_info(), "Последовательность типа normal не может возвращять значения перечислимого типа");
    }
}

void RDOFUNSequenceNormal::createCalcs()
{
    rdo::runtime::RandGeneratorNormal* pGenerator = new rdo::runtime::RandGeneratorNormal();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextNormal>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seq_src_info) const
{
    if (pParamList->getParamList().size() != 2)
    {
        Converter::s_converter()->error().error(seq_src_info, rdo::format("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__REAL>::create(), RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
    rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

    pFuctionCall->addParameter(pArg1);
    pFuctionCall->addParameter(pArg2);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
        pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(seq_src_info);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        Converter::s_converter()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- Гистограмма
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHist
// --------------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(const LPRDOFUNSequenceByHistHeader& pHeader)
    : RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& src_info) const
{
    if (pParamList->getParamList().size() != 0)
    {
        Converter::s_converter()->error().error(src_info, rdo::format("Гистограмма '%s' должна вызываться без параметров", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
        pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(src_info);
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(const LPRDOFUNSequenceByHistHeader& pHeader, const LPRDOValue& pFrom, const LPRDOValue& pTo, const LPRDOValue& pFreq)
    : RDOFUNSequenceByHist(pHeader)
{
    addReal(pFrom, pTo, pFreq);
}

void RDOFUNSequenceByHistReal::addReal(const LPRDOValue& pFrom, const LPRDOValue& pTo, const LPRDOValue& pFreq)
{
    ASSERT(pFrom);
    ASSERT(pTo  );
    ASSERT(pFreq);

    if (!m_to.empty() && pFrom->value() != m_to.back())
    {
        Converter::s_converter()->error().error(pFrom->src_info(), "Начало интервала должно совпадать с окончанием предыдущего");
    }
    if (pFrom->value() > pTo->value())
    {
        Converter::s_converter()->error().error(pTo->src_info(), "Начало интервала должно быть меньше его конца");
    }
    if (pFreq->value() == 0)
    {
        Converter::s_converter()->error().warning(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    else if (pFreq->value() < 0)
    {
        Converter::s_converter()->error().error(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    m_pHeader->getType()->value_cast(pFrom);
    m_pHeader->getType()->value_cast(pTo  );
    m_from.push_back(pFrom->value());
    m_to  .push_back(pTo  ->value());
    m_freq.push_back(pFreq->value());
}

void RDOFUNSequenceByHistReal::createCalcs()
{
    rdo::runtime::RandGeneratorByHistReal* pGenerator = new rdo::runtime::RandGeneratorByHistReal();
    int size = m_from.size();
    for (int i = 0; i < size; i++)
    {
        pGenerator->addValues(m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble());
    }

    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
    initResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistEnum
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(const LPRDOFUNSequenceByHistHeader& pHeader, const LPRDOValue& pValue, const LPRDOValue& pFreq)
    : RDOFUNSequenceByHist(pHeader)
{
    addEnum(pValue, pFreq);
}

void RDOFUNSequenceByHistEnum::addEnum(const LPRDOValue& pValue, const LPRDOValue& pFreq)
{
    ASSERT(pValue);
    ASSERT(pFreq );

    if (pFreq->value() == 0)
    {
        Converter::s_converter()->error().warning(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    else if (pFreq->value() < 0)
    {
        Converter::s_converter()->error().error(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    m_pHeader->getType()->value_cast(pValue);
    rdo::runtime::RDOValue enum_id = m_pHeader->getType()->value_cast(pValue)->value();
    if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
    {
        Converter::s_converter()->error().error(pValue->src_info(), rdo::format("Перечислимое значение определено дважды: %s", pValue->value().getIdentificator().c_str()));
    }
    m_values.push_back(enum_id);
    m_freq  .push_back(pFreq->value());
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
    rdo::runtime::RandGeneratorByHistEnum* pGenerator = new rdo::runtime::RandGeneratorByHistEnum();
    const std::size_t size = m_values.size();
    for (std::size_t i = 0; i < size; i++)
    {
        pGenerator->addValues(m_values[i], m_freq[i].getDouble());
    }
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
    initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& src_info) const
{
    if (pParamList->getParamList().size() != 0)
    {
        Converter::s_converter()->error().error(src_info, rdo::format("Перечисление '%s' должно вызываться без параметров", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
        pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
    );
    ASSERT(pArithm);
    pArithm->setSrcInfo(src_info);
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
    rdo::runtime::RandGeneratorEnumerative* pGenerator = new rdo::runtime::RandGeneratorEnumerative();
    for (std::size_t i = 0; i < m_valueList.size(); i++)
    {
        pGenerator->addValue(m_valueList[i]->value());
    }
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
    initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdo::runtime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(const LPRDOTypeParam& pRetType, const rdo::runtime::LPRDOCalcFuncParam& pFuncParam, const RDOParserSrcInfo& src_pos) const
{
    rdo::runtime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
    return rdo::Factory<rdo::runtime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(const RDOParserSrcInfo& src_info)
    : RDOFUNFunctionListElement(src_info)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const
{
    rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
        pRetType->value_cast(
            rdo::Factory<RDOValue>::create(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))
        )->value()
    );
    ASSERT(pCalcConst);
    pCalcConst->setSrcInfo(src_pos);
    return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementReal
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementReal::RDOFUNFunctionListElementReal(const YYLTYPE& position, double value)
    : RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format("%f", value)))
    , m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const
{
    rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
        pRetType->value_cast(rdo::Factory<RDOValue>::create(m_value, rdo::Factory<RDOType__REAL>::create(), src_pos))->value()
    );
    pCalcConst->setSrcInfo(src_pos);
    return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementInt
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementInt::RDOFUNFunctionListElementInt(const YYLTYPE& position, int value)
    : RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format("%d", value)))
    , m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(const LPRDOTypeParam& pRetType, const RDOParserSrcInfo& src_pos) const
{
    rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
        pRetType->value_cast(rdo::Factory<RDOValue>::create(m_value, rdo::Factory<RDOType__INT>::create().object_parent_cast<RDOType>(), src_pos))->value()
    );
    pCalcConst->setSrcInfo(src_pos);
    return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(const YYLTYPE& position)
    : RDOFUNFunctionListElement(RDOParserSrcInfo(position, "="))
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(const LPRDOTypeParam& /*pRetType*/, const RDOParserSrcInfo& src_pos) const
{
    Converter::s_converter()->error().error(src_pos, "Внутренняя ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc");
    NEVER_REACH_HERE;
    return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNCalculateIf
// --------------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf(const LPRDOFUNLogic& pCondition, const LPRDOFUNArithm& pAction)
    : m_pCondition(pCondition)
    , m_pAction   (pAction   )
{
    m_pAction->checkParamType(Converter::s_converter()->getLastFUNFunction()->getReturn()->getType());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(const RDOParserSrcInfo& src_info, const LPRDOParam& pReturn)
    : RDOParserSrcInfo(src_info)
    , m_pReturn       (pReturn )
{
    Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::RDOFUNFunction(const std::string& name, const LPRDOParam& pReturn)
    : RDOParserSrcInfo(name   )
    , m_pReturn       (pReturn)
{
    Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(const rdo::runtime::LPRDOFunCalc& pCalc)
{
    m_pFunctionCalc = pCalc;
    if (m_pFunctionCalc->srcInfo().src_empty())
    {
        m_pFunctionCalc->setSrcInfo(src_info());
    }
    for (const auto& calc: m_postLinkedList)
    {
        calc->setFunctionCalc(getFunctionCalc());
    }
}

LPRDOParam RDOFUNFunction::findFUNFunctionParam(const std::string& paramName) const
{
    ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
    return it != m_paramList.end() ? *it : LPRDOParam(NULL);
}

int RDOFUNFunction::findFUNFunctionParamNum(const std::string& paramName) const
{
    ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
    return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

void RDOFUNFunction::add(const LPRDOParam& pParam)
{
    LPRDOParam pParamPrev = findFUNFunctionParam(pParam->name());
    if (pParamPrev)
    {
        Converter::s_converter()->error().push_only(pParam->src_info(), rdo::format("Параметр уже существует: %s", pParam->name().c_str()));
        Converter::s_converter()->error().push_only(pParamPrev->src_info(), "См. первое определение");
        Converter::s_converter()->error().push_done();
    }
    m_paramList.push_back(pParam);
}

void RDOFUNFunction::add(const LPRDOFUNFunctionListElement& pParam)
{
    m_elementList.push_back(pParam);
}

void RDOFUNFunction::add(const LPRDOFUNCalculateIf& pCalculateIf)
{
    m_calculateIfList.push_back(pCalculateIf);
}

void RDOFUNFunction::createListCalc()
{
    if (!m_pReturn->getDefault()->defined())
    {
        Converter::s_converter()->error().error(m_pReturn->src_info(), rdo::format("Функция '%s' должна иметь значение по умолчанию", name().c_str()));
    }
    if (m_paramList.empty())
    {
        Converter::s_converter()->error().error(src_info(), rdo::format("Функция '%s' должна иметь параметры, т.к. её тип list", name().c_str()));
    }
    if (m_elementList.empty())
    {
        Converter::s_converter()->error().warning(src_info(), rdo::format("Функция '%s' не содержит списка", name().c_str()));
    }

    rdo::runtime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getDefault()->value());
    ASSERT(pCalcDefault);
    pCalcDefault->setSrcInfo(m_pReturn->getDefault()->src_info());
    rdo::runtime::LPRDOFunListCalc pFunListCalc = rdo::Factory<rdo::runtime::RDOFunListCalc>::create(pCalcDefault);
    ASSERT(pFunListCalc);
    pFunListCalc->setSrcInfo(src_info());
    ElementList::const_iterator elem_it = m_elementList.begin();
    while (elem_it != m_elementList.end())
    {
        rdo::runtime::LPRDOCalc pCaseCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(true);
        ASSERT(pCaseCalc);
        pCaseCalc->setSrcInfo((*elem_it)->src_info());
        int currParamNumber = 0;
        ParamList::const_iterator param_it = m_paramList.begin();
        while (param_it != m_paramList.end())
        {
            LPRDOParam pParam = *param_it;
            if (elem_it == m_elementList.end())
            {
                --elem_it;
                Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format("Ожидается значение для параметра '%s'", pParam->name().c_str()));
            }
            LPRDOFUNFunctionListElement pListElement = *elem_it;
            ASSERT(pListElement);
            if (pListElement->isEquivalence())
            {
                if ((param_it + 1) == m_paramList.end())
                {
                    Converter::s_converter()->error().error(pListElement->src_info(), rdo::format("Перед знаком равенства ожидается значение для параметра: %s",pParam->name().c_str()));
                }
                else
                {
                    std::string str = (*param_it)->src_text();
                    ++param_it;
                    while (param_it != m_paramList.end())
                    {
                        str += ", ";
                        str += (*param_it)->src_text();
                        ++param_it;
                    }
                    Converter::s_converter()->error().error(pListElement->src_info(), rdo::format("Перед знаком равенства ожидаются значения для параметров: %s", str.c_str()));
                }
            }
            rdo::runtime::LPRDOCalcFuncParam pFuncParam   = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
            ASSERT(pFuncParam);
            rdo::runtime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getType(), pFuncParam, pListElement->src_info());
            ASSERT(pCompareCalc);
            rdo::runtime::LPRDOCalc          pAndCalc     = rdo::Factory<rdo::runtime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
            ASSERT(pAndCalc);
            pCaseCalc = pAndCalc;
            ++elem_it;
            ++param_it;
            ++currParamNumber;
        }
        if (elem_it == m_elementList.end())
        {
            --elem_it;
            --param_it;
            Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format("После значения для параметра '%s' ожидается знак равенства", (*param_it)->name().c_str()));
        }
        if (!(*elem_it)->isEquivalence())
        {
            Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format("Слишком много значений, вместо '%s' ожидается знак равенства", (*elem_it)->src_text().c_str()));
        }
        ++elem_it;
        if (elem_it == m_elementList.end())
        {
            --elem_it;
            Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format("После знака равенства ожидается значение функции '%s'", name().c_str()));
        }
        rdo::runtime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getType(), (*elem_it)->src_info());
        pFunListCalc->addCase(pCaseCalc, pResultCalc);
        ++elem_it;
    }
    setFunctionCalc(pFunListCalc);
}

void RDOFUNFunction::createTableCalc(const YYLTYPE& elements_pos)
{
    ElementList::const_iterator it = m_elementList.begin();
    while (it != m_elementList.end())
    {
        if ((*it)->isEquivalence())
        {
            Converter::s_converter()->error().error((*it)->src_info(), "Символ '=' недопустим в табличной функции");
        }
        ++it;
    }
    const std::size_t param_cnt = m_paramList.size();
    std::size_t range = 1;
    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(0);
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText("0");
    pCalc->setSrcInfo(srcInfo);
    for (std::size_t currParam = 0; currParam < param_cnt; currParam++)
    {
        LPRDOParam pFunctionParam = m_paramList.at(currParam);
        ASSERT(pFunctionParam);
        rdo::runtime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
        ASSERT(pFuncParam);
        rdo::runtime::LPRDOCalc pValue2 = pFuncParam;
        if (pFunctionParam->getType()->type()->typeID() != rdo::runtime::RDOType::Type::ENUM)
        {
            rdo::runtime::LPRDOCalcConst pCalcConst1 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
            pCalcConst1->setSrcInfo(pFunctionParam->src_info());
            pValue2 = rdo::Factory<rdo::runtime::RDOCalcMinus>::create(pValue2, pCalcConst1);
        }
        rdo::runtime::LPRDOCalcConst pCalcConst2 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(range);
        ASSERT(pCalcConst2);
        pCalcConst2->setSrcInfo(pFunctionParam->src_info());
        rdo::runtime::LPRDOCalcMult pCalcMult = rdo::Factory<rdo::runtime::RDOCalcMultEnumSafe>::create(pCalcConst2, pValue2);
        rdo::runtime::LPRDOCalcPlus pCalcAdd  = rdo::Factory<rdo::runtime::RDOCalcPlusEnumSafe>::create(pCalcMult, pCalc);

        switch (pFunctionParam->getType()->type()->typeID())
        {
        case rdo::runtime::RDOType::Type::INT:
            if (dynamic_cast<RDOTypeIntRange*>(pFunctionParam->getType()->type().get()))
            {
                LPRDOTypeIntRange pRange = pFunctionParam->getType()->type().object_static_cast<RDOTypeIntRange>();
                if (pRange->range()->getMin()->value().getInt() != 1)
                {
                    Converter::s_converter()->error().error(pRange->range()->src_info(), rdo::format("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]", pRange->range()->getMin()->value().getInt(), pRange->range()->getMax()->value().getInt()));
                }
                range *= pRange->range()->getMax()->value().getInt() - pRange->range()->getMin()->value().getInt() + 1;
            }
            else
            {
                Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), "Для параметра табличной функции должен быть задан допустимый диапазон");
            }
            break;
        case rdo::runtime::RDOType::Type::ENUM:
            range *= pFunctionParam->getType()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
            break;
        default:
            Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), "Параметр табличной функции может быть целого или перечислимого типа");
        }
        pCalc = pCalcAdd;
    }

    if (m_elementList.size() != range)
    {
        Converter::s_converter()->error().error(elements_pos, rdo::format("Неверное количество элементов табличной функции '%s': %d, требуется %d", name().c_str(), m_elementList.size(), range));
    }

    rdo::runtime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdo::runtime::RDOFuncTableCalc>::create(pCalc);
    ASSERT(pFuncTableCalc);
    pFuncTableCalc->setSrcInfo(src_info());
    for (std::size_t currElem = 0; currElem < range; currElem++)
    {
        LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
        ASSERT(pListElement);
        rdo::runtime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getType(), pListElement->src_info());
        ASSERT(pResultCalc);
        pFuncTableCalc->addResultCalc(pResultCalc);
    }
    setFunctionCalc(pFuncTableCalc);
}

void RDOFUNFunction::createAlgorithmicCalc(const RDOParserSrcInfo& /* body_src_info */)
{
    rdo::runtime::LPRDOFunAlgorithmicCalc pFunAlgorithmicCalc = rdo::Factory<rdo::runtime::RDOFunAlgorithmicCalc>::create();
    ASSERT(pFunAlgorithmicCalc);

    pFunAlgorithmicCalc->setSrcInfo(src_info());
    bool defaultFlag = false;
    bool trueConst = false;
    rdo::runtime::LPRDOCalcConst pCondition;
    int size = m_calculateIfList.size();
    int cnt  = 0;
    for (int i = 0; i < size; i++)
    {
        rdo::runtime::LPRDOCalc pLogicCalc = m_calculateIfList[i]->getCondition()->getCalc(m_pReturn->getType()->type()->typeID());
        ASSERT(pLogicCalc);
        rdo::runtime::LPRDOCalcConst pConditionLast = pLogicCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>();
        if (trueConst)
        {
            Converter::s_converter()->error().warning(m_calculateIfList[i]->getCondition()->src_info(), rdo::format("Условие не используется: %s", m_calculateIfList[i]->getCondition()->src_text().c_str()));
            Converter::s_converter()->error().warning(pCondition->srcInfo(), rdo::format("Последнее рабочее условие функции: %s", pCondition->srcInfo().src_text().c_str()));
        }
        else if (!pConditionLast || pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
        {
            // Игнорируем чистые false-условия предыдущей проверкой
            pFunAlgorithmicCalc->addCalcIf(pLogicCalc, m_calculateIfList[i]->getAction()->createCalc(m_pReturn->getType()));
            cnt++;
        }
        if (!defaultFlag && pConditionLast && pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
        {
            trueConst   = true;
            defaultFlag = true;
            pCondition  = pConditionLast;
        }
    }
    if (!cnt)
    {
        Converter::s_converter()->error().warning(src_info(), rdo::format("Отсутствует тело функции '%s'", name().c_str()));
    }
    if (!trueConst)
    {
        if (m_pReturn->getDefault()->defined())
        {
            rdo::runtime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
            rdo::runtime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getType()->value_cast(m_pReturn->getDefault())->value());
            ASSERT(pCalcCondition);
            ASSERT(pCalcAction   );
            pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
            pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
            pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
            defaultFlag = true;
        }
    }
    if (!defaultFlag)
    {
        // Присвоить автоматическое значение по умолчанию, если оно не задано в явном виде
        rdo::runtime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
        rdo::runtime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getType()->type()->get_default()->value());
        ASSERT(pCalcCondition);
        ASSERT(pCalcAction   );
        pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
        pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
        pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
        Converter::s_converter()->error().warning(src_info(), rdo::format("Для функции '%s' неопределено значение по умолчанию", name().c_str()));
    }
    setFunctionCalc(pFunAlgorithmicCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup(const RDOParserSrcInfo& res_info)
    : RDOParserSrcInfo(res_info)
{
    init(res_info);
}

RDOFUNGroup::~RDOFUNGroup()
{}

void RDOFUNGroup::init(const RDOParserSrcInfo& res_info)
{
    m_pResType = Converter::s_converter()->findRTPResType(res_info.src_text());
    if (!m_pResType)
    {
        Converter::s_converter()->error().error(res_info, rdo::format("Неизвестный тип ресурса: %s", res_info.src_text().c_str()));
    }
    Converter::s_converter()->insertFUNGroup(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(Type type, const RDOParserSrcInfo& res_info)
    : RDOFUNGroup(res_info)
    , m_funType(type)
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(LPRDOFUNLogic& pCondition)
{
    rdo::runtime::LPRDOFunCalcGroup calc;
    switch (m_funType)
    {
    case Type::EXIST    : setSrcText("Exist("     + getResType()->name() + ": " + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::NOTEXIST : setSrcText("NotExist("  + getResType()->name() + ": " + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::FORALL   : setSrcText("ForAll("    + getResType()->name() + ": " + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::NOTFORALL: setSrcText("NotForAll(" + getResType()->name() + ": " + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
    default: Converter::s_converter()->error().error(src_info(), "Внутренняя ошибка: несуществующий тип функции");
    }
    Converter::s_converter()->getFUNGroupStack().pop_back();
    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
    pLogic->setSrcInfo(src_info());
    return pLogic;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSelect
// --------------------------------------------------------------------------------
RDOFUNSelect::RDOFUNSelect(const RDOParserSrcInfo& res_info)
    : RDOFUNGroup(RDOParserSrcInfo(res_info.src_text()))
{}

// Сам Select как выборка по типу и условию
void RDOFUNSelect::initSelect(LPRDOFUNLogic pCondition)
{
    // @warning rdo::runtime::LPRDOFunCalcSelect в рантайме меняет сигнатуру
    // Для конвертора знать про рантайм не обязательно, поэтому не создаём объект m_pCalcSelect
    // Конвертор проверялся на пятнашках, где вместо Exist использовался
    // $Condition Select(Фишка: Фишка.Номер <> Фишка.Местоположение).Exist(1 == 2)
    // Метод createFunSelectGroup отработал с нулевым m_pCalcSelect

    //m_pCalcSelect = rdo::Factory<rdo::runtime::RDOFunCalcSelect>::create(getResType()->getNumber(), pCondition->getCalc());
    //m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

// Групповая функция над выборкой Select'а
LPRDOFUNLogic RDOFUNSelect::createFunSelectGroup(RDOFUNGroupLogic::Type funType, LPRDOFUNLogic& pCondition)
{
    ASSERT(pCondition);
    rdo::runtime::LPRDOFunCalcSelectBase calc;
    switch (funType)
    {
    case RDOFUNGroupLogic::Type::EXIST    : setSrcText(src_text() + ".Exist("     + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
    case RDOFUNGroupLogic::Type::NOTEXIST : setSrcText(src_text() + ".NotExist("  + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
    case RDOFUNGroupLogic::Type::FORALL   : setSrcText(src_text() + ".ForAll("    + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
    case RDOFUNGroupLogic::Type::NOTFORALL: setSrcText(src_text() + ".NotForAll(" + pCondition->src_text() + ")"); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
    default: Converter::s_converter()->error().error(pCondition->src_info(), "Внутренняя ошибка: неизвестный метод для списка ресурсов");
    }
    Converter::s_converter()->getFUNGroupStack().pop_back();
    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
    pLogic->setSrcInfo(src_info());
    return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(const RDOParserSrcInfo& empty_info)
{
    setSrcText(src_text() + "." + empty_info.src_text());
    Converter::s_converter()->getFUNGroupStack().pop_back();
    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdo::runtime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect), false);
    pLogic->setSrcInfo(empty_info);
    return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(const RDOParserSrcInfo& size_info)
{
    setSrcText(src_text() + "." + size_info.src_text());
    Converter::s_converter()->getFUNGroupStack().pop_back();
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
        rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__INT>::create().object_parent_cast<RDOType>(), size_info),
        rdo::Factory<rdo::runtime::RDOFunCalcSelectSize>::create(m_pCalcSelect).object_parent_cast<rdo::runtime::RDOCalc>()
    );
    pArithm->setSrcInfo(size_info);
    return pArithm;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
