// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int patlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void paterror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

int evnlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void evnerror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

int evn_preparse_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void evn_preparse_error(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPATPattern
// --------------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern(const RDOParserSrcInfo& name_src_info)
    : RDOParserSrcInfo    (name_src_info)
    , m_useCommonChoice   (false        )
    , m_currentRelResIndex(0            )
{
    LPRDOPATPattern pPatternExist = RDOParser::s_parser()->findPATPattern(src_text());
    if (pPatternExist)
    {
        parser::g_error().push_only(src_info(), rdo::format("Паттерн '%s' уже существует", name().c_str()));
        parser::g_error().push_only(pPatternExist->src_info(), "См. первое определение");
        parser::g_error().push_done();
    }
    RDOParser::s_parser()->insertPATPattern(this);

    m_pContextMemory = rdo::Factory<ContextMemory>::create();
    ASSERT(m_pContextMemory);
}

RDOPATPattern::~RDOPATPattern()
{}

void RDOPATPattern::pushContext()
{
    RDOParser::s_parser()->contextStack()->push(this);
    RDOParser::s_parser()->contextStack()->push(m_pContextMemory);
    ContextMemory::push();
}

void RDOPATPattern::popContext()
{
    ContextMemory::pop();
    RDOParser::s_parser()->contextStack()->pop<ContextMemory>();
    RDOParser::s_parser()->contextStack()->pop<RDOPATPattern>();
}

std::string RDOPATPattern::typeToString(Type type)
{
    switch (type)
    {
    case Type::EVENT    : return "событие";
    case Type::RULE     : return "продукционное правило";
    case Type::OPERATION: return "операция";
    case Type::KEYBOARD : return "клавиатурная операция";
    default: return "неизвестный";
    }
}

LPRDORelevantResource RDOPATPattern::findRelRes(const std::string& identifier, const RDOParserSrcInfo& srcInfo) const
{
    // Релевантные ресурсы
    LPRDORelevantResource pRelevantResource = findRelevantResource(identifier);
    if (pRelevantResource)
    {
        if (!m_pCurrRelRes)
        {
            // Внутри with_min-common-choice или $Time
            if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST || pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
            {
                RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", pRelevantResource->name().c_str()));
            }
        }
        else
        {
            // Внутри $Body
            // Проверяем использование неинициализированного рел.ресурса (pRelevantResource) в Choice from другом рел.ресурсе (m_pCurrRelRes)
            if (m_pCurrRelRes->isChoiceFromState())
            {
                if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
                {
                    RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс не определен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", pRelevantResource->name().c_str()));
                }
                if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                {
                    RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", pRelevantResource->name().c_str()));
                }
                if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
                {
                    RDOParser::s_parser()->error().error(srcInfo, rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", pRelevantResource->name().c_str()));
                }
            }
            // Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
            if (pRelevantResource->getType()->isTemporary())
            {
                // В конверторе начала
                if (m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_BEGIN)
                {
                    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE && !pRelevantResource->m_alreadyHaveConverter)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", pRelevantResource->name().c_str()));
                    }
                    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE && pRelevantResource->m_alreadyHaveConverter)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", pRelevantResource->name().c_str()));
                    }
                    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", pRelevantResource->name().c_str()));
                    }
                }
                // В конверторе конца
                if (m_pCurrRelRes->m_currentState == RDORelevantResource::State::CONVERT_END)
                {
                    if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::CREATE && !pRelevantResource->m_alreadyHaveConverter)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", pRelevantResource->name().c_str()));
                    }
                    if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::ERASE && pRelevantResource->m_alreadyHaveConverter)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", pRelevantResource->name().c_str()));
                    }
                    if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
                    {
                        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", pRelevantResource->name().c_str()));
                    }
                }
            }
        }
    }
    return pRelevantResource;
}

namespace
{

LPExpression contextParameters(const LPRDOParam& param, std::size_t paramID, const RDOParserSrcInfo& srcInfo)
{
    return rdo::Factory<Expression>::create(
        param->getTypeInfo(),
        rdo::Factory<rdo::runtime::RDOCalcPatParam>::create(paramID),
        srcInfo
    );
}

void pushRelevantResourceContext(const LPRDORelevantResource& pRelevantResource)
{
    RDOParser::s_parser()->contextStack()->push(pRelevantResource);
}

void popRelevantResourceContext()
{
    if (RDOParser::s_parser()->contextStack()->top().object_dynamic_cast<RDORelevantResource>())
    {
        RDOParser::s_parser()->contextStack()->pop<RDORelevantResource>();
    }
}

}

Context::LPFindResult RDOPATPattern::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    Context::LPFindResult result = m_pContextMemory->onFindContext(method, params, srcInfo);
    if (result->getCreateExpression())
    {
        return result;
    }

    if (method == Context::METHOD_GET || method == Context::METHOD_OPERATOR_DOT)
    {
        const std::string identifier = params.identifier();

        LPRDORelevantResource pRelevantResource = findRelRes(identifier, srcInfo);
        if (pRelevantResource)
        {
            if (method == Context::METHOD_OPERATOR_DOT)
            {
                return rdo::Factory<FindResult>::create(SwitchContext(pRelevantResource, params));
            }

            if (method == Context::METHOD_GET)
            {
                return pRelevantResource->find(Context::METHOD_GET, params, srcInfo);
            }
        }
        LPRDOParam pParam = findPATPatternParam(identifier);
        if (pParam)
        {
            if (method == Context::METHOD_GET)
            {
                return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextParameters, pParam, findPATPatternParamNum(identifier), srcInfo)));
            }
            if (method == Context::METHOD_OPERATOR_DOT)
            {
                LPRDOPATPattern pThis(const_cast<RDOPATPattern*>(this));
                return rdo::Factory<FindResult>::create(SwitchContext(pThis, params));
            }
        }
    }

    return rdo::Factory<FindResult>::create();
}

std::string RDOPATPattern::StatusToStr(rdo::runtime::RDOResource::ConvertStatus value)
{
    switch (value)
    {
    case rdo::runtime::RDOResource::ConvertStatus::KEEP    : return "Keep";
    case rdo::runtime::RDOResource::ConvertStatus::CREATE  : return "Create";
    case rdo::runtime::RDOResource::ConvertStatus::ERASE   : return "Erase";
    case rdo::runtime::RDOResource::ConvertStatus::NONEXIST: return "NonExist";
    case rdo::runtime::RDOResource::ConvertStatus::NOCHANGE: return "NoChange";
    default                                    : NEVER_REACH_HERE;
    }
    return std::string();
}

rdo::runtime::RDOResource::ConvertStatus RDOPATPattern::StrToStatus(const std::string& value, const YYLTYPE& convertor_pos)
{
    if (value == "Keep" || value == "keep")
    {
        return rdo::runtime::RDOResource::ConvertStatus::KEEP;
    }
    else if (value == "Create" || value == "create")
    {
        return rdo::runtime::RDOResource::ConvertStatus::CREATE;
    }
    else if (value == "Erase" || value == "erase")
    {
        return rdo::runtime::RDOResource::ConvertStatus::ERASE;
    }
    else if (value == "NonExist" || value == "nonexist")
    {
        return rdo::runtime::RDOResource::ConvertStatus::NONEXIST;
    }
    else if (value == "NoChange" || value == "nochange")
    {
        return rdo::runtime::RDOResource::ConvertStatus::NOCHANGE;
    }
    parser::g_error().error(convertor_pos, rdo::format("Неверный статус конвертора: %s", value.c_str()));
    return rdo::runtime::RDOResource::ConvertStatus::KEEP;
}

void RDOPATPattern::beforeRelRensert(const RDOParserSrcInfo& rel_info)
{
    if (findRelevantResource(rel_info.src_text()))
    {
        parser::g_error().error(rel_info, rdo::format("Релевантный ресурс '%s' уже определен", rel_info.src_text().c_str()));
    }
}

void RDOPATPattern::rel_res_insert(const LPRDORelevantResource& pRelevantResource)
{
    ASSERT(pRelevantResource);

    switch (getType())
    {
    case Type::EVENT    : getPatRuntime<rdo::runtime::RDOPatternEvent    >()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
    case Type::RULE     : getPatRuntime<rdo::runtime::RDOPatternRule     >()->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
    case Type::OPERATION: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
    case Type::KEYBOARD : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
    default: parser::g_error().error(src_info(), "Неизвестный тип образца");
    }
    m_relResList.push_back(pRelevantResource);
}

void RDOPATPattern::addRelResConvert(bool trace, const LPExpression& pStatementList, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos, rdo::runtime::RDOResource::ConvertStatus status)
{
    ASSERT(pStatementList);
    ASSERT(m_pCurrRelRes );

    if (status == rdo::runtime::RDOResource::ConvertStatus::NOCHANGE || status == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
    {
        parser::g_error().error(convertor_pos, getErrorMessage_NotNeedConvertor(m_pCurrRelRes->name(), status));
    }

    if (status == rdo::runtime::RDOResource::ConvertStatus::CREATE)
    {
        addParamSetCalc(createRelRes(trace));
    }
    else
    {
        if (trace)
        {
            parser::g_error().error(trace_pos, "Признак трассировки в данном месте возможен только для создаваемого ресурса");
        }
    }
    rdo::runtime::LPRDOCalcBaseStatementList pBaseStatementList = pStatementList->calc().object_dynamic_cast<rdo::runtime::RDOCalcBaseStatementList>();
    ASSERT(pBaseStatementList);

    rdo::runtime::RDOCalc::RDOCalcList pCalcList = pBaseStatementList->statementList();
    ASSERT(!pCalcList.empty());

    for (const auto& calc: pCalcList)
    {
        ASSERT(calc);
        rdo::runtime::LPRDOCalcStatementList pStatementList = calc.object_dynamic_cast<rdo::runtime::RDOCalcStatementList>();
        if (pStatementList)
        {
            if (pStatementList->statementList().empty() && status == rdo::runtime::RDOResource::ConvertStatus::KEEP)
            {
                parser::g_error().warning(convertor_pos, getWarningMessage_EmptyConvertor(m_pCurrRelRes->name(), status));
            }
        }
    }

    addParamSetCalc(pStatementList->calc());
}

void RDOPATPattern::addParamSetCalc(const rdo::runtime::LPRDOCalc& pCalc)
{
    switch (getType())
    {
    case Type::EVENT    : getPatRuntime<rdo::runtime::RDOPatternEvent>()->addConvertorCalc(pCalc); break;
    case Type::RULE     : getPatRuntime<rdo::runtime::RDOPatternRule >()->addConvertorCalc(pCalc); break;
    case Type::OPERATION: NEVER_REACH_HERE;
    case Type::KEYBOARD : NEVER_REACH_HERE;
    default: parser::g_error().error(src_info(), "Неизвестный тип образца");
    }
}

std::vector<runtime::LPRDOCalc> RDOPATPattern::createParamsCalcs(const std::vector<LPRDOFUNArithm>& params) const
{
    std::vector<runtime::LPRDOCalc> result;
    result.reserve(m_paramList.size());

    std::size_t currParam = 0;
    for (const LPRDOFUNArithm& pParam: params)
    {
        ASSERT(pParam);
        if (currParam < m_paramList.size())
        {
            rdo::runtime::LPRDOCalc pParamValueCalc;
            LPRDOParam pPatternParam = m_paramList[currParam];
            ASSERT(pPatternParam);
            if (pParam->typeInfo()->src_info().src_text() == "*")
            {
                if (!pPatternParam->getDefault()->defined())
                {
                    RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format("Нет значения по умолчанию для параметра '%s'", pPatternParam->src_text().c_str()));
                    RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format("См. параметр '%s', тип '%s'", pPatternParam->src_text().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
                    RDOParser::s_parser()->error().push_done();
                }
                rdo::runtime::RDOValue value = pPatternParam->getDefault()->value();
                ASSERT(value);
                pParamValueCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(value);
            }
            else
            {
                LPTypeInfo pTypeInfo = pPatternParam->getTypeInfo();
                ASSERT(pTypeInfo);
                pParamValueCalc = pParam->createCalc(pTypeInfo);
            }
            ASSERT(pParamValueCalc);
            result.push_back(pParamValueCalc);
            ++currParam;
        }
        else
        {
            RDOParser::s_parser()->error().error(pParam->src_info(), rdo::format("Слишком много параметров для события '%s'", name().c_str()));
        }
    }

    return result;
}

std::string RDOPATPattern::getPatternId() const
{
    return m_pPatRuntime->traceId();
}

void RDOPATPattern::writeModelStructure(std::ostream& stream) const
{
    stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << m_relResList.size();
    for (const auto& resource: m_relResList)
        stream << " " << resource->getType()->getNumber();

    stream << std::endl;
}

LPRDOParam RDOPATPattern::findPATPatternParam(const std::string& paramName) const
{
    ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
    return it != m_paramList.end() ? (*it) : LPRDOParam(NULL);
}

LPRDORelevantResource RDOPATPattern::findRelevantResource(const std::string& resName) const
{
    RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
    return it != m_relResList.end() ? (*it) : LPRDORelevantResource(NULL);
}

int RDOPATPattern::findPATPatternParamNum(const std::string& paramName) const
{
    ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
    return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

std::size_t RDOPATPattern::findRelevantResourceNum(const std::string& resName) const
{
    RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
    return it != m_relResList.end() ? it - m_relResList.begin() : std::size_t(~0);
}

void RDOPATPattern::add(const LPRDOParam& pParam)
{
    ASSERT(pParam);

    LPRDOParam pParamExist = findPATPatternParam(pParam->name());
    if (pParamExist)
    {
        parser::g_error().push_only(pParam->src_info(), rdo::format("Параметр '%s' уже определен", pParam->src_text().c_str()));
        parser::g_error().push_only(pParamExist->src_info(), "См. первое определение");
        parser::g_error().push_done();
    }
    m_paramList.push_back(pParam);
}

void RDOPATPattern::setCommonChoiceFirst()
{
    m_useCommonChoice = true;
    m_pCommonChoice   = NULL;
//    parser::g_error().error("Вызывать нельзя, т.к. в сообщениях об ошибках используется m_pCommonChoice");
}

void RDOPATPattern::setCommonChoiceWithMin(const LPRDOFUNArithm& arithm)
{
    m_useCommonChoice  = true;
    m_useCommonWithMax = false;
    m_pCommonChoice    = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax(const LPRDOFUNArithm& arithm)
{
    m_useCommonChoice  = true;
    m_useCommonWithMax = true;
    m_pCommonChoice    = arithm;
}

void RDOPATPattern::setTime(LPRDOFUNArithm& arithm)
{
    switch (getType())
    {
    case Type::OPERATION: getPatRuntime<rdo::runtime::RDOPatternOperation>()->setTime(arithm->createCalc(NULL)); break;
    case Type::KEYBOARD : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->setTime(arithm->createCalc(NULL)); break;
    default: parser::g_error().error(src_info(), rdo::format("Для образца типа %s недопустимо использование выражения времени", typeToString(getType()).c_str()));
    }
}

void RDOPATPattern::addChoiceFromCalc(const rdo::runtime::LPRDOCalc& pCalc)
{
    switch (getType())
    {
    case Type::EVENT    : getPatRuntime<rdo::runtime::RDOPatternEvent    >()->addPreSelectRelRes(pCalc); break;
    case Type::RULE     : getPatRuntime<rdo::runtime::RDOPatternRule     >()->addChoiceFromCalc (pCalc); break;
    case Type::OPERATION: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addChoiceFromCalc (pCalc); break;
    case Type::KEYBOARD : getPatRuntime<rdo::runtime::RDOPatternKeyboard >()->addChoiceFromCalc (pCalc); break;
    default: parser::g_error().error(src_info(), rdo::format("Для образца типа %s недопустимо использование условния выбора", typeToString(getType()).c_str()));
    }
}

void RDOPATPattern::addRelResBody(const RDOParserSrcInfo& body_name)
{
    RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(body_name.src_text()));
    if (it == m_relResList.end())
    {
        parser::g_error().error(body_name.src_info(), rdo::format("Неизвестный релевантный ресурс: %s", body_name.src_text().c_str()));
    }
    if (findRelevantResourceNum(body_name.src_text()) != m_currentRelResIndex)
    {
        const std::string rel_res_waiting = m_currentRelResIndex < m_relResList.size() ? m_relResList[m_currentRelResIndex]->name().c_str() : "";
        parser::g_error().error(body_name.src_info(), rdo::format("Ожидается описание релевантного ресурса '%s', вместо него найдено: %s", rel_res_waiting.c_str(), body_name.src_text().c_str()));
    }
    if ((*it)->m_alreadyHaveConverter)
    {
        parser::g_error().error(body_name.src_info(), rdo::format("Релевантный ресурс уже используется: %s", body_name.src_text().c_str()));
    }
    m_pCurrRelRes = (*it);
    m_pCurrRelRes->m_bodySrcInfo = body_name;
    m_pCurrRelRes->m_alreadyHaveConverter = true;
    m_currentRelResIndex++;

    popRelevantResourceContext();
    pushRelevantResourceContext(m_pCurrRelRes);
}

void RDOPATPattern::addRelResUsage(const LPRDOPATChoiceFrom& pChoiceFrom, const LPRDOPATChoiceOrder& pChoiceOrder)
{
    if (!m_useCommonChoice)
    {
        if (pChoiceOrder->m_type == rdo::runtime::RDOSelectResourceCalc::Type::EMPTY)
        {
            if ((m_pCurrRelRes->m_statusBegin != rdo::runtime::RDOResource::ConvertStatus::CREATE) && (m_pCurrRelRes->m_statusEnd != rdo::runtime::RDOResource::ConvertStatus::CREATE))
            {
//                pChoiceOrder->m_type = rdo::runtime::RDOSelectResourceCalc::Type::FIRST;
            }
        }
        else if (m_pCurrRelRes->isDirect())
        {
            parser::g_error().warning(pChoiceOrder->src_info(), rdo::format("Правило выбора '%s' релевантного ресурса '%s' не имеет смысла, т.к. релевантный ресурс определен через имя, а не тип, и не может быть связан с каким-либо другим ресурсом", pChoiceOrder->src_text().c_str(), m_pCurrRelRes->name().c_str()));
        }
    }
    else
    {
        if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::Type::EMPTY)
        {
            parser::g_error().push_only(pChoiceOrder->src_info(), "Нельзя указать способ выбора релевантного ресурса, т.к. используется единый для всех релевантных ресурсов способ, указанный до ключевого слова $Body");
            if (m_pCommonChoice)
            {
                parser::g_error().push_only(m_pCommonChoice->src_info(), rdo::format("См. '%s'", m_pCommonChoice->src_text().c_str()));
            }
            parser::g_error().push_done();
        }
    }

    if ((m_pCurrRelRes->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE) || (m_pCurrRelRes->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::CREATE))
    {
        if (pChoiceFrom->m_type != RDOPATChoiceFrom::Type::EMPTY)
        {
            parser::g_error().error(pChoiceFrom->src_info(), "Релевантный ресурс создается, для него нельзя использовать Choice from или Choice NoCheck");
        }
        if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::Type::EMPTY)
        {
            parser::g_error().error(pChoiceOrder->src_info(), rdo::format("Релевантный ресурс создается, для него нельзя использовать правило выбора '%s'", pChoiceOrder->asString().c_str()));
        }
    }

    m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
    m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

void RDOPATPattern::end()
{
    popRelevantResourceContext();

    int size = m_relResList.size();
    for (int i = 0; i < size; i++)
    {
        LPRDORelevantResource pCurrRelRes = m_relResList.at(i);
        if (!pCurrRelRes->m_alreadyHaveConverter)
        {
            // TODO А почему нельзя сделать warning ? Возможно, есть жесткое требование недопустить пустого рел. ресурса.
            parser::g_error().error(pCurrRelRes->src_info(), rdo::format("Релевантный ресурс '%s' не используется в образце '%s'", pCurrRelRes->name().c_str(), name().c_str()));
        }
        m_pPatRuntime->addPreSelectRelRes(pCurrRelRes->createPreSelectRelResCalc());
    }

    if (m_useCommonChoice)
    {
        // first
        // Работает неправильно, а как обыкновенный first
        if (!m_pCommonChoice)
        {
            // first
            std::vector<rdo::runtime::LPIRDOSelectResourceCommon> resSelectors;
            for (int i = 0; i < size; i++)
            {
                if (m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::KEEP || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
                {
                    resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
                }
            }
            rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, rdo::runtime::LPRDOCalc(NULL));
            ASSERT(pCalc);
            rdo::runtime::RDOSrcInfo srcInfo(src_info());
            srcInfo.setSrcText("first");
            pCalc->setSrcInfo(srcInfo);
            addChoiceFromCalc(pCalc);
        }
        else
        {
            // with_min/with_max
            std::vector<rdo::runtime::LPIRDOSelectResourceCommon> resSelectors;
            for (int i = 0; i < size; i++)
            {
                if (m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::KEEP || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE || m_relResList.at(i)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
                {
                    resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
                }
            }
            rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, m_pCommonChoice->createCalc());
            pCalc->setSrcInfo(m_pCommonChoice->src_info());
            addChoiceFromCalc(pCalc);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            rdo::runtime::LPRDOCalc pCalc = m_relResList.at(i)->createSelectResourceChoiceCalc();
            addChoiceFromCalc(pCalc);
        }
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternEvent
// --------------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(const RDOParserSrcInfo& name_src_info, bool trace)
    : RDOPATPattern(name_src_info)
{
    m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternEvent>::create(trace);
    ASSERT(m_pPatRuntime);
    m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());

    m_pRuntimeEvent = getPatRuntime<rdo::runtime::RDOPatternEvent>()->createActivity(
        RDOParser::s_parser()->runtime()->m_pMetaLogic,
        RDOParser::s_parser()->runtime(),
        name()
    ).object_dynamic_cast<IBaseOperation>();
    ASSERT(m_pRuntimeEvent);
}

void RDOPatternEvent::addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos)
{
    beforeRelRensert(rel_info);
    if (beg == rdo::runtime::RDOResource::ConvertStatus::NONEXIST || beg == rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
    {
        parser::g_error().error(convertor_pos, "Статусы конверторов NonExist и NoChange не могут быть использованы в событии");
    }

    LPRDORelevantResource pRelevantResource;
    LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
    if (res)
    {
        switch (beg)
        {
        case rdo::runtime::RDOResource::ConvertStatus::CREATE: parser::g_error().error(type_info, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
        case rdo::runtime::RDOResource::ConvertStatus::ERASE : parser::g_error().error(convertor_pos, "Удалять ресурсы в событии нельзя"); break;
        default: break;
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    else
    {
        LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
        if (!pResType)
        {
            parser::g_error().error(type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()));
        }
        switch (beg)
        {
        case rdo::runtime::RDOResource::ConvertStatus::CREATE: if (!pResType->isTemporary()) parser::g_error().error(type_info, rdo::format("Тип ресурса '%s' постоянный. Динамически создавать от него ресурсы нельзя", type_info.src_text().c_str())); break;
        case rdo::runtime::RDOResource::ConvertStatus::KEEP  :
        case rdo::runtime::RDOResource::ConvertStatus::ERASE : parser::g_error().error(convertor_pos, "Статусы конверторов Keep и Erase могут быть использованы в событии с описателем в виде ресурса, но не типа ресурса"); break;
        default: break;
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE)
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
        ASSERT(pCalc);
        rdo::runtime::RDOSrcInfo srcInfo(rel_info);
        srcInfo.setSrcText(rdo::format("Удаление временного ресурса %s", rel_info.src_text().c_str()));
        pCalc->setSrcInfo(srcInfo);
        getPatRuntime<rdo::runtime::RDOPatternEvent>()->addEraseCalc(pCalc);
    }
}

void RDOPatternEvent::addRelResUsage(const LPRDOPATChoiceFrom& pChoiceFrom, const LPRDOPATChoiceOrder& pChoiceOrder)
{
    if (pChoiceFrom->m_type != RDOPATChoiceFrom::Type::EMPTY)
    {
        parser::g_error().error(pChoiceFrom->src_info(), "Релевантные ресурсы события нельзя выбирать с помощью Choice from или Choice NoCheck");
    }
    if (pChoiceOrder->m_type != rdo::runtime::RDOSelectResourceCalc::Type::EMPTY)
    {
        parser::g_error().error(pChoiceFrom->src_info(), rdo::format("Для релевантных ресурсов события нельзя использовать правило выбора '%s'", pChoiceOrder->asString().c_str()));
    }

    m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
    m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

namespace
{

std::vector<rdo::runtime::LPRDOCalc> fillDefaultParams(LPRDORTPResType pType, RDOParserSrcInfo srcInfo)
{
    std::vector<rdo::runtime::LPRDOCalc> params_default;
    for (const auto& param: pType->getParams())
    {
        LPRDORTPResType pResType = param->getTypeInfo()->itype().object_dynamic_cast<RDORTPResType>();
        if (pResType)
        {
            std::vector<rdo::runtime::LPRDOCalc> nested_default_params = fillDefaultParams(pResType, srcInfo);
            rdo::runtime::LPRDOCalc pResCalc = rdo::Factory<rdo::runtime::RDOCalcCreateResource>::create(
                pResType->getNumber(),
                nested_default_params,
                false, // TODO задавать такую же трассировку, как у родительского ресурса
                false, // TODO проверить, что ресурс временный
                true   // ресурс является вложенным
            );
            ASSERT(pResCalc);
            params_default.push_back(pResCalc);
        }
        else
        {
            if (!param->getDefault()->defined())
            {
                params_default.push_back(rdo::Factory<rdo::runtime::RDOCalcConst>::create(rdo::runtime::RDOValue(0)));
                if (!param->getDefined())
                {
                    parser::g_error().error(srcInfo, rdo::format("При создании ресурса необходимо определить все его параметры. Не найдено определение параметра: %s", param->name().c_str()));
                }
            }
            else
            {
                params_default.push_back(rdo::Factory<rdo::runtime::RDOCalcConst>::create((param->getDefault()->value())));
            }
        }
    }
    return params_default;
}

}

rdo::runtime::LPRDOCalc RDOPATPattern::createRelRes(bool trace) const
{
    std::vector<rdo::runtime::LPRDOCalc> params_default = fillDefaultParams(m_pCurrRelRes->getType(), m_pCurrRelRes->src_info());

    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcCreateResource>::create(
        m_pCurrRelRes->getType()->getNumber(),
        params_default,
        trace,
        false, // TODO проверить, что ресурс временный
        false, // релеватный ресурс не может быть вложенным
        m_pCurrRelRes->m_relResID
    );
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(m_pCurrRelRes->src_info());
    srcInfo.setSrcText(rdo::format("Создание временного ресурса %s", m_pCurrRelRes->name().c_str()));
    pCalc->setSrcInfo(srcInfo);
    return pCalc;
}

std::string RDOPatternEvent::getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    return rdo::format("Для релевантного ресурса '%s' не требуется конвертор (Convert_event), т.к. его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

std::string RDOPatternEvent::getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_event), хотя его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

rdo::runtime::LPRDOCalc RDOPatternEvent::getBeforeStartModelPlaning() const
{
    return m_beforeStartModelPlaning;
}

void RDOPatternEvent::setBeforeStartModelPlaning(const rdo::runtime::LPRDOCalc& beforeStartModelPlaning)
{
    ASSERT(beforeStartModelPlaning);
    m_beforeStartModelPlaning = beforeStartModelPlaning;
}

runtime::LPRDOCalcEventPlan RDOPatternEvent::planning(const runtime::LPRDOCalc& time, const LPArithmContainer& params) const
{
    return rdo::Factory<runtime::RDOCalcEventPlan>::create(
            m_pRuntimeEvent.object_dynamic_cast<IEvent>(),
            time,
            createParamsCalcs(params->getContainer())
    );
}

runtime::LPRDOCalcEventStop RDOPatternEvent::stoping() const
{
    return rdo::Factory<runtime::RDOCalcEventStop>::create(m_pRuntimeEvent.object_dynamic_cast<IEvent>());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternRule
// --------------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(const RDOParserSrcInfo& name_src_info, bool trace)
    : RDOPATPattern(name_src_info)
{
//    RDOParser::s_parser()->runtime()->addRuntimeRule((RDOPatternRule *)(m_pPatRuntime = new RDOPatternRule(RDOParser::s_parser()->runtime(), _trace)));
    m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternRule>::create(trace);
    ASSERT(m_pPatRuntime);
    m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

void RDOPatternRule::addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos)
{
    beforeRelRensert(rel_info);
    if (beg == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
    {
        parser::g_error().error(convertor_pos, rdo::format("Нельзя использовать статус конвертора '%s' в продукционном правиле", RDOPATPattern::StatusToStr(beg).c_str()));
    }

    LPRDORelevantResource pRelevantResource;
    LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
    if (res)
    {
        if (beg == rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            parser::g_error().error(type_info, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()));
        }
        if (beg == rdo::runtime::RDOResource::ConvertStatus::ERASE)
        {
            parser::g_error().error(convertor_pos, rdo::format("Недопустимый статус конвертора для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()));
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    else
    {
        LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
        if (!pResType)
        {
            parser::g_error().error(type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()));
        }
        if (!pResType->isTemporary() && (beg == rdo::runtime::RDOResource::ConvertStatus::CREATE || beg == rdo::runtime::RDOResource::ConvertStatus::ERASE))
        {
            parser::g_error().error(type_info, rdo::format("Для создании или удалении ресурса его тип должен быть временным (temporary), а не постоянным (permanent), как у '%s'", type_info.src_text().c_str()));
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE)
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
        ASSERT(pCalc);
        rdo::runtime::RDOSrcInfo srcInfo(rel_info);
        srcInfo.setSrcText(rdo::format("Удаление временного ресурса %s", rel_info.src_text().c_str()));
        pCalc->setSrcInfo(srcInfo);
        getPatRuntime<rdo::runtime::RDOPatternRule>()->addEraseCalc(pCalc);
    }
}

std::string RDOPatternRule::getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    return rdo::format("Для релевантного ресурса '%s' не требуется конвертор (Convert_rule), т.к. его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

std::string RDOPatternRule::getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор (Convert_rule), хотя его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternOperation
// --------------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(const RDOParserSrcInfo& name_src_info, bool trace)
    : RDOPATPattern  (name_src_info)
    , m_convertorType(ConvertorType::UNKNOW)
{
    m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternOperation>::create(trace);
    ASSERT(m_pPatRuntime);
    m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

RDOPatternOperation::RDOPatternOperation(bool /*trace*/, const RDOParserSrcInfo& name_src_info)
    : RDOPATPattern  (name_src_info)
    , m_convertorType(ConvertorType::UNKNOW)
{}

void RDOPatternOperation::rel_res_insert(const LPRDORelevantResource& pRelevantResource)
{
    ASSERT(pRelevantResource);
    RDOPATPattern::rel_res_insert(pRelevantResource);
    getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorEndStatus(pRelevantResource->m_statusEnd);
}

void RDOPatternOperation::addRelRes(const RDOParserSrcInfo& /*rel_info*/, const RDOParserSrcInfo& /*type_info*/, rdo::runtime::RDOResource::ConvertStatus /*beg*/, const YYLTYPE& convertor_pos)
{
    parser::g_error().error(convertor_pos, "Внутренняя ошибка парсера");
}

void RDOPatternOperation::addRelRes(const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdo::runtime::RDOResource::ConvertStatus beg, rdo::runtime::RDOResource::ConvertStatus end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos)
{
    beforeRelRensert(rel_info);
    switch (beg)
    {
    case rdo::runtime::RDOResource::ConvertStatus::KEEP:
        if (end != rdo::runtime::RDOResource::ConvertStatus::KEEP && end != rdo::runtime::RDOResource::ConvertStatus::ERASE && end != rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
        }
        break;
    case rdo::runtime::RDOResource::ConvertStatus::CREATE:
        if (end != rdo::runtime::RDOResource::ConvertStatus::KEEP && end != rdo::runtime::RDOResource::ConvertStatus::ERASE && end != rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
        }
        break;
    case rdo::runtime::RDOResource::ConvertStatus::ERASE:
        if (end != rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
        }
        break;
    case rdo::runtime::RDOResource::ConvertStatus::NONEXIST:
        if (end != rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
        }
        break;
    case rdo::runtime::RDOResource::ConvertStatus::NOCHANGE:
        if (end != rdo::runtime::RDOResource::ConvertStatus::KEEP && end != rdo::runtime::RDOResource::ConvertStatus::ERASE && end != rdo::runtime::RDOResource::ConvertStatus::NOCHANGE)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Статус конвертора конца несовместим со статусом конвертора начала: %s и %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
        }
        break;
    default:
        break;
    };

    LPRDORelevantResource pRelevantResource;
    LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
    if (res)
    {
        if (beg == rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            parser::g_error().error(convertor_begin_pos, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()));
        }
        if (end == rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("При создания ресурса '%s' требуется указать его тип, но указан просто ресурс (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()));
        }
        if (beg == rdo::runtime::RDOResource::ConvertStatus::ERASE || beg == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            parser::g_error().error(convertor_begin_pos, rdo::format("Недопустимый статус конвертора начала для ресурса: %s", RDOPATPattern::StatusToStr(beg).c_str()));
        }
        if (end == rdo::runtime::RDOResource::ConvertStatus::ERASE || end == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            parser::g_error().error(convertor_end_pos, rdo::format("Недопустимый статус конвертора конца для ресурса: %s", RDOPATPattern::StatusToStr(end).c_str()));
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg, end);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    else
    {
        LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
        if (!pResType)
        {
            parser::g_error().error(type_info, rdo::format("Неизвестный тип ресурса: %s", type_info.src_text().c_str()));
        }
        if (pResType->isPermanent())
        {
            if (beg == rdo::runtime::RDOResource::ConvertStatus::CREATE || beg == rdo::runtime::RDOResource::ConvertStatus::ERASE || beg == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
            {
                parser::g_error().error(convertor_begin_pos, rdo::format("Недопустимый статус конвертора начала для постоянного типа: %s", RDOPATPattern::StatusToStr(beg).c_str()));
            }
            if (end == rdo::runtime::RDOResource::ConvertStatus::CREATE || end == rdo::runtime::RDOResource::ConvertStatus::ERASE || end == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
            {
                parser::g_error().error(convertor_end_pos, rdo::format("Недопустимый статус конвертора конца для постоянного типа: %s", RDOPATPattern::StatusToStr(end).c_str()));
            }
        }
        pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg, end);
        ASSERT(pRelevantResource);
        rel_res_insert(pRelevantResource);
    }
    if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE)
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
        ASSERT(pCalc);
        rdo::runtime::RDOSrcInfo srcInfo(rel_info);
        srcInfo.setSrcText(rdo::format("Удаление временного ресурса %s", rel_info.src_text().c_str()));
        pCalc->setSrcInfo(srcInfo);
        getPatRuntime<rdo::runtime::RDOPatternOperation>()->addEraseBeginCalc(pCalc);
    }
    if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::ERASE)
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOEraseResRelCalc>::create(pRelevantResource->m_relResID, pRelevantResource->name());
        ASSERT(pCalc);
        rdo::runtime::RDOSrcInfo srcInfo(rel_info);
        srcInfo.setSrcText(rdo::format("Удаление временного ресурса %s", rel_info.src_text().c_str()));
        pCalc->setSrcInfo(srcInfo);
        getPatRuntime<rdo::runtime::RDOPatternOperation>()->addEraseEndCalc(pCalc);
    }
}

void RDOPatternOperation::addRelResConvertBeginEnd(bool trace_begin, const LPExpression& pBeginStatementList, bool trace_end, const LPExpression& pEndStatementList, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos)
{
    if (pBeginStatementList)
    {
        m_convertorType = ConvertorType::CONVERT_BEGIN;
        ASSERT(m_pCurrRelRes);
        addRelResConvert(trace_begin, pBeginStatementList, convertor_begin_pos, trace_begin_pos, m_pCurrRelRes->m_statusBegin);
        m_convertorType = ConvertorType::UNKNOW;
    }
    if (pEndStatementList)
    {
        m_convertorType = ConvertorType::CONVERT_END;
        ASSERT(m_pCurrRelRes);
        addRelResConvert(trace_end, pEndStatementList, convertor_end_pos, trace_end_pos, m_pCurrRelRes->m_statusEnd);
        m_convertorType = ConvertorType::UNKNOW;
    }
}

void RDOPatternOperation::addParamSetCalc(const rdo::runtime::LPRDOCalc& pCalc)
{
    switch (m_convertorType)
    {
    case ConvertorType::CONVERT_BEGIN: getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorBeginCalc(pCalc); break;
    case ConvertorType::CONVERT_END  : getPatRuntime<rdo::runtime::RDOPatternOperation>()->addConvertorEndCalc  (pCalc); break;
    default: NEVER_REACH_HERE;
    }
}

std::string RDOPatternOperation::getErrorMessage_NotNeedConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    switch (m_convertorType)
    {
    case ConvertorType::CONVERT_BEGIN: return rdo::format("Для релевантного ресурса '%s' не требуется конвертор начала (Convert_begin), т.к. его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
    case ConvertorType::CONVERT_END  : return rdo::format("Для релевантного ресурса '%s' не требуется конвертор конца (Convert_end), т.к. его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
    default: NEVER_REACH_HERE;
    }
    return std::string();
}

std::string RDOPatternOperation::getWarningMessage_EmptyConvertor(const std::string& name, rdo::runtime::RDOResource::ConvertStatus status)
{
    switch (m_convertorType)
    {
    case ConvertorType::CONVERT_BEGIN: return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор начала (Convert_begin), хотя его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
    case ConvertorType::CONVERT_END  : return rdo::format("Для релевантного ресурса '%s' указан пустой конвертор конца (Convert_end), хотя его статус: %s", name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
    default: NEVER_REACH_HERE;
    }
    return std::string();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPatternKeyboard
// --------------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(const RDOParserSrcInfo& name_src_info, bool trace)
    : RDOPatternOperation(trace, name_src_info)
{
    m_pPatRuntime = rdo::Factory<rdo::runtime::RDOPatternKeyboard>::create(trace);
    ASSERT(m_pPatRuntime);
    m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResource
// --------------------------------------------------------------------------------
RDORelevantResource::RDORelevantResource(const RDOParserSrcInfo& src_info, const int relResID, const rdo::runtime::RDOResource::ConvertStatus statusBegin, const rdo::runtime::RDOResource::ConvertStatus statusEnd)
    : RDOParserSrcInfo      (src_info)
    , m_relResID            (relResID)
    , m_alreadyHaveConverter(false)
    , m_statusBegin         (statusBegin)
    , m_statusEnd           (statusEnd)
    , m_currentState        (State::NONE)
{}

RDORelevantResource::~RDORelevantResource()
{}

namespace
{

LPExpression contextGetRelevantResource(const LPRDORelevantResource& relevantResource, const RDOParserSrcInfo& srcInfo)
{
    return rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::create(relevantResource->getType(), srcInfo),
        rdo::Factory<rdo::runtime::RDOGetResourceByRelevantResourceID>::create(relevantResource->m_relResID),
        srcInfo
    );
}

}

Context::LPFindResult RDORelevantResource::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_GET)
    {
        const std::string paramName = params.identifier();

        if (paramName == name())
        {
            LPRDORelevantResource pThis(const_cast<RDORelevantResource*>(this));
            return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextGetRelevantResource, pThis, srcInfo)));
        }

        const std::size_t parNumb = getType()->getRTPParamNumber(paramName);
        if (parNumb == RDORTPResType::UNDEFINED_PARAM)
            return rdo::Factory<FindResult>::create();
        // Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
        LPRDORTPParam pParam = getType()->findRTPParam(paramName);
        ASSERT(pParam);
        // В конверторе начала
        if (m_currentState == RDORelevantResource::State::CONVERT_BEGIN && m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            if (!pParam->getDefined())
            {
                if (!pParam->getDefault()->defined())
                {
                    RDOParser::s_parser()->error().error(srcInfo, rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по умолчанию в типе ресурса", paramName.c_str()));
                }
            }
        }
        // В конверторе конца
        if (m_currentState == RDORelevantResource::State::CONVERT_END && m_statusEnd == rdo::runtime::RDOResource::ConvertStatus::CREATE)
        {
            if (!pParam->getDefined())
            {
                if (!pParam->getDefault()->defined())
                {
                    RDOParser::s_parser()->error().error(srcInfo, rdo::format("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по умолчанию в типе ресурса", paramName.c_str()));
                }
            }
        }

        Context::Params params_;
        params_[RDORSSResource::GET_RESOURCE] = params.get<LPExpression>(RDORSSResource::GET_RESOURCE);
        params_[RDOParam::CONTEXT_PARAM_PARAM_ID] = parNumb;

        return pParam->find(Context::METHOD_GET, params_, srcInfo);
    }

    if (method == Context::METHOD_OPERATOR_DOT)
    {
        const std::string paramName = params.identifier();

        const std::size_t parNumb = getType()->getRTPParamNumber(paramName);
        if (parNumb == RDORTPResType::UNDEFINED_PARAM)
            return rdo::Factory<FindResult>::create();

        RDOParserSrcInfo srcInfo_(srcInfo);
        srcInfo_.setSrcText(rdo::format("%s.%s", src_text().c_str(), params.identifier().c_str()));

        const rdo::runtime::LPRDOCalc resourceCalc = rdo::Factory<rdo::runtime::RDOGetResourceByRelevantResourceID>::create(m_relResID);
        const LPExpression resourceExpression = rdo::Factory<Expression>::create(
            rdo::Factory<TypeInfo>::create(getType(), srcInfo),
            resourceCalc,
            srcInfo
        );

        Context::Params params_;
        params_[Context::Params::IDENTIFIER] = paramName;
        params_[RDORSSResource::GET_RESOURCE] = resourceExpression;

        return getType()->find(Context::METHOD_OPERATOR_DOT, params_, srcInfo_);
    }

    if (method == Context::METHOD_SET)
    {
        LPRDORTPParam pParam = getType()->findRTPParam(params.identifier());
        if (pParam)
        {
            RDOParserSrcInfo srcInfo_(srcInfo);
            srcInfo_.setSrcText(rdo::format("%s.%s", src_text().c_str(), pParam->name().c_str()));

            const rdo::runtime::LPRDOCalc resourceCalc = rdo::Factory<rdo::runtime::RDOGetResourceByRelevantResourceID>::create(m_relResID);
            const LPExpression resourceExpression = rdo::Factory<Expression>::create(
                rdo::Factory<TypeInfo>::create(getType(), srcInfo_),
                resourceCalc,
                srcInfo_
            );

            Context::Params params_;
            params_[RDOParam::CONTEXT_PARAM_PARAM_ID] = getType()->getRTPParamNumber(params.identifier());
            params_[RDORSSResource::GET_RESOURCE] = resourceExpression;
            params_[Expression::CONTEXT_PARAM_SET_OPERATION_TYPE] = params.get<rdo::runtime::SetOperation::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE);

            if (params.exists(Expression::CONTEXT_PARAM_SET_EXPRESSION))
                params_[Expression::CONTEXT_PARAM_SET_EXPRESSION] = params.get<LPExpression>(Expression::CONTEXT_PARAM_SET_EXPRESSION);
            if (params.exists(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM))
                params_[RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM] = params.get<LPRDOFUNArithm>(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM);

            return pParam->find(Context::METHOD_SET, params_, srcInfo_);
        }
    }

    return rdo::Factory<FindResult>::create();
}

rdo::runtime::LPRDOCalc RDORelevantResource::getChoiceCalc()
{
    if (m_pChoiceFrom && m_pChoiceFrom->m_type == RDOPATChoiceFrom::Type::FROM)
    {
        return m_pChoiceFrom->m_pLogic->getCalc(rdo::runtime::RDOType::Type::INT);
    }
    return NULL;
}

rdo::runtime::LPRDOCalc RDORelevantResource::getSelectCalc()
{
    if (m_pChoiceOrder && m_pChoiceOrder->m_pArithm)
    {
        return m_pChoiceOrder->m_pArithm->createCalc(NULL);
    }
    return NULL;
}

rdo::runtime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
    return m_pChoiceOrder ? m_pChoiceOrder->m_type : rdo::runtime::RDOSelectResourceCalc::Type::EMPTY;
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceDirect - по имени ресурса
// --------------------------------------------------------------------------------
rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createPreSelectRelResCalc()
{
    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID());
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText(rdo::format("Предварительный выбор рел. ресурса %s", src_text().c_str()));
    pCalc->setSrcInfo(srcInfo);
    return pCalc;
}

rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc(), getSelectCalc(), getSelectType());
    pCalc->setSrcInfo(m_pChoiceFrom->src_info());
    return pCalc;
}

rdo::runtime::LPRDOCalc RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
    return rdo::Factory<rdo::runtime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
}

rdo::runtime::LPIRDOSelectResourceCommon RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
    rdo::runtime::LPRDOSelectResourceDirectCommonCalc pDirectCommonCalc = rdo::Factory<rdo::runtime::RDOSelectResourceDirectCommonCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
    rdo::runtime::LPIRDOSelectResourceCommon pSelectResourceCommon = pDirectCommonCalc.interface_cast<rdo::runtime::IRDOSelectResourceCommon>();
    ASSERT(pSelectResourceCommon);
    return pSelectResourceCommon;
}

LPRDORTPResType RDORelevantResourceDirect::getType() const
{
    return m_pResource->getType();
}

// --------------------------------------------------------------------------------
// -------------------- RDORelevantResourceByType - по имени типа
// --------------------------------------------------------------------------------
rdo::runtime::LPRDOCalc RDORelevantResourceByType::createPreSelectRelResCalc()
{
    if ((m_statusBegin != rdo::runtime::RDOResource::ConvertStatus::CREATE) && (m_statusEnd != rdo::runtime::RDOResource::ConvertStatus::CREATE))
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber());
        ASSERT(pCalc);
        rdo::runtime::RDOSrcInfo srcInfo(src_info());
        srcInfo.setSrcText(rdo::format("Предварительный выбор рел. ресурса %s", src_text().c_str()));
        pCalc->setSrcInfo(srcInfo);
        return pCalc;
    }
    else
    {
        if (m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
        }
        else
        {
            rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
            ASSERT(pCalc);
            rdo::runtime::RDOSrcInfo srcInfo(src_info());
            srcInfo.setSrcText(rdo::format("Предварительный выбор рел. ресурса перед созданием %s", src_text().c_str()));
            pCalc->setSrcInfo(srcInfo);
            return pCalc;
        }
    }
}

rdo::runtime::LPRDOCalc RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
    if ((m_statusBegin != rdo::runtime::RDOResource::ConvertStatus::CREATE) && (m_statusEnd != rdo::runtime::RDOResource::ConvertStatus::CREATE))
    {
        rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType());
        pCalc->setSrcInfo(m_pChoiceFrom->src_info());
        return pCalc;
    }
    else
    {
        if (m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
        }
        else
        {
            rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
            ASSERT(pCalc);
            rdo::runtime::RDOSrcInfo srcInfo(src_info());
            srcInfo.setSrcText(rdo::format("Перед созданием рел. ресурса %s", src_text().c_str()));
            pCalc->setSrcInfo(srcInfo);
            return pCalc;
        }
    }
}

rdo::runtime::LPRDOCalc RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
    if ((m_statusBegin != rdo::runtime::RDOResource::ConvertStatus::CREATE) && (m_statusEnd != rdo::runtime::RDOResource::ConvertStatus::CREATE))
    {
        return rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
    }
    else
    {
        if (m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::NONEXIST)
        {
            return rdo::Factory<rdo::runtime::RDOSelectResourceNonExistCalc>::create(m_relResID);
        }
        else
        {
            return rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
        }
    }
}

rdo::runtime::LPIRDOSelectResourceCommon RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
    rdo::runtime::LPRDOSelectResourceByTypeCommonCalc pByTypeCommonCalc = rdo::Factory<rdo::runtime::RDOSelectResourceByTypeCommonCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
    rdo::runtime::LPIRDOSelectResourceCommon pSelectResourceCommon = pByTypeCommonCalc.interface_cast<rdo::runtime::IRDOSelectResourceCommon>();
    ASSERT(pSelectResourceCommon);
    return pSelectResourceCommon;
}

CLOSE_RDO_PARSER_NAMESPACE
