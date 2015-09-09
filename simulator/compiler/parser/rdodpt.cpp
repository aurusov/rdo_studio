// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/mbuilder/rdo_resources.h"

#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_keyboard.h"
#include "simulator/runtime/rdo_logic_dptsome.h"
#include "simulator/runtime/rdo_logic_dptprior.h"
#include "simulator/runtime/rdodptrtime.h"
#include "simulator/runtime/rdo_dptsearch_activity.h"
#include "simulator/runtime/calc/calc_pattern.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int dptlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void dpterror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
    : RDOParserSrcInfo(src_info)
    , m_currParam     (0       )
{
    RDOParser::s_parser()->checkActivityName(src_info);
    m_pPattern = RDOParser::s_parser()->findPATPattern(pattern_src_info.src_text());
    if (!m_pPattern)
    {
        RDOParser::s_parser()->error().error(pattern_src_info, rdo::format("Не найден образец: %s", pattern_src_info.src_text().c_str()));
    }
    RDOParser::s_parser()->contextStack()->push(this);
    RDOParser::s_parser()->insertDPTActivity(this);
}

RDODPTActivity::~RDODPTActivity()
{}

Context::LPFindResult RDODPTActivity::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    return m_pPattern->onFindContext(method, params, srcInfo);
}

void RDODPTActivity::addParam(const LPRDOValue& pParam)
{
    ASSERT(pParam);

    if (m_pPattern->m_paramList.size() <= m_currParam)
    {
        if (pParam->src_pos().m_first_line == src_pos().m_first_line)
        {
            RDOParser::s_parser()->error().push_only(pParam->src_pos(), rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", m_pPattern->name().c_str(), name().c_str()));
            RDOParser::s_parser()->error().push_only(m_pPattern->src_info(), "См. образец");
            RDOParser::s_parser()->error().push_done();
        }
        else
        {
            RDOParser::s_parser()->error().error(pParam->src_pos(), "Имя активности должно заканчиваться двоеточием");
        }
    }
    rdo::runtime::RDOValue val;
    LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
    if (pParam->value().getAsString() == "*")
    {
        if (!pPatternParam->getDefault()->defined())
        {
            RDOParser::s_parser()->error().push_only(pParam->src_pos(), rdo::format("Нет значения по умолчанию для параметра '%s'", pPatternParam->src_text().c_str()));
            RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format("См. параметр '%s', тип '%s'", pPatternParam->src_text().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
            RDOParser::s_parser()->error().push_done();
        }
        val = pPatternParam->getDefault()->value();
    }
    else
    {
        val = pPatternParam->getTypeInfo()->value_cast(pParam)->value();
    }

    rdo::runtime::LPRDOCalc pParamCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(val);
    pParamCalc->setSrcInfo(RDOParserSrcInfo(pParam->getPosAsYY(), rdo::format("Параметр образца %s.%s = %s", m_pPattern->name().c_str(), pPatternParam->name().c_str(), pParam->value().getAsString().c_str())));
    m_pActivity->addParamCalc(pParamCalc);
    m_currParam++;
}

void RDODPTActivity::endParam(const YYLTYPE& param_pos)
{
    if (m_pPattern->m_paramList.size() > m_currParam)
    {
        LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
        RDOParser::s_parser()->error().push_only(param_pos, rdo::format("Указаны не все параметра образца '%s':", m_pPattern->src_text().c_str()));
        for (std::size_t i = m_currParam; i < m_pPattern->m_paramList.size(); i++)
        {
            pPatternParam = m_pPattern->m_paramList.at(i);
            RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", pPatternParam->name().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
        }
        RDOParser::s_parser()->error().push_done();
    }
    if (m_pPattern->getType() == RDOPATPattern::Type::KEYBOARD)
    {
        LPIKeyboard pKeyboard = m_pActivity.object_dynamic_cast<IKeyboard>();
        ASSERT(pKeyboard);
        if (!pKeyboard->hasHotKey())
        {
            RDOParser::s_parser()->error().push_only(param_pos, "Для активности должна быть указана клавиша");
            RDOParser::s_parser()->error().push_only(m_pPattern->src_info(), "См. образец");
            RDOParser::s_parser()->error().push_done();
        }
    }
    RDOParser::s_parser()->contextStack()->pop<RDODPTActivity>();
}

bool RDODPTActivity::setPrior(LPRDOFUNArithm& pPrior)
{
    LPIPriority pPriorActivity = m_pActivity.object_dynamic_cast<IPriority>();
    if (pPriorActivity)
    {
        return pPriorActivity->setPrior(pPrior->createCalc());
    }
    return false;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
    : RDODPTActivity(src_info, pattern_src_info)
{
    switch (pattern()->getType())
    {
    case RDOPATPattern::Type::RULE:
        m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternRule>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name()).object_dynamic_cast<IActivity>();
        break;

    case RDOPATPattern::Type::OPERATION:
        m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternOperation>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name()).object_dynamic_cast<IActivity>();
        break;

    case RDOPATPattern::Type::KEYBOARD:
        m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternKeyboard>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name()).object_dynamic_cast<IActivity>();
        break;

    default:
        RDOParser::s_parser()->error().push_only(this->src_info(), "Неизвестный тип образца");
        RDOParser::s_parser()->error().push_only(pattern()->src_info(), "См. образец");
        RDOParser::s_parser()->error().push_done();
    }
}

RDODPTActivityHotKey::~RDODPTActivityHotKey()
{}

void RDODPTActivityHotKey::addHotKey(const std::string& hotKey, const YYLTYPE& hotkey_pos)
{
    if (pattern()->getType() != RDOPATPattern::Type::KEYBOARD)
    {
        RDOParser::s_parser()->error().push_only(hotkey_pos, "Горячие клавиши используются только в клавиатурных операциях");
        RDOParser::s_parser()->error().push_only(pattern()->src_info(), "См. образец");
        RDOParser::s_parser()->error().push_done();
    }
    LPIKeyboard pKeyboard = m_pActivity.object_dynamic_cast<IKeyboard>();
    ASSERT(pKeyboard);
    switch (pKeyboard->addHotKey(RDOParser::s_parser()->runtime(), hotKey))
    {
    case rdo::runtime::RDOKeyboard::AddHotKeyResult::OK:
        break;

    case rdo::runtime::RDOKeyboard::AddHotKeyResult::ALREADY:
        RDOParser::s_parser()->error().error(hotkey_pos, rdo::format("Для активности '%s' клавиша уже назначена", src_text().c_str()));
        break;

    case rdo::runtime::RDOKeyboard::AddHotKeyResult::NOTFOUND:
        RDOParser::s_parser()->error().error(hotkey_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()));
        break;

    case rdo::runtime::RDOKeyboard::AddHotKeyResult::DONT:
        RDOParser::s_parser()->error().push_only(src_info(), rdo::format("Операция '%s' не является клавиатурной", src_text().c_str()));
        RDOParser::s_parser()->error().push_only(pattern()->src_info(), "См. образец");
        RDOParser::s_parser()->error().push_done();
        break;

    default:
        RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка: RDOOPROperation::addHotKey");
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
    : RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

RDODPTSomeActivity::~RDODPTSomeActivity()
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
    : RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

RDODPTPriorActivity::~RDODPTPriorActivity()
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
RDODPTSome::RDODPTSome(const RDOParserSrcInfo& src_info, LPILogic pParent)
    : RDOLogic<rdo::runtime::RDODPTSome, RDODPTSomeActivity>(src_info)
{
    RDOParser::s_parser()->checkDPTName(this->src_info());
    m_pRuntimeLogic = rdo::Factory<rdo::runtime::RDODPTSome>::create(RDOParser::s_parser()->runtime(), pParent.object_dynamic_cast<IBaseOperationContainer>()).object_dynamic_cast<ILogic>();
    ASSERT(m_pRuntimeLogic);
    m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
    RDOParser::s_parser()->insertDPTSome(this);
    RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTSome::~RDODPTSome()
{}

Context::LPFindResult RDODPTSome::onFindContext(const std::string& /*method*/, const Context::Params& /*params*/, const RDOParserSrcInfo& /*srcInfo*/) const
{
    // Добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
    return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(const RDOParserSrcInfo& src_info, LPILogic pParent)
    : RDOLogic<rdo::runtime::RDODPTPrior, RDODPTPriorActivity>(src_info)
{
    RDOParser::s_parser()->checkDPTName(this->src_info());
    m_pRuntimeLogic = rdo::Factory<rdo::runtime::RDODPTPrior>::create(RDOParser::s_parser()->runtime(), pParent.object_dynamic_cast<IBaseOperationContainer>());
    ASSERT(m_pRuntimeLogic);
    m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
    RDOParser::s_parser()->insertDPTPrior(this);
    RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTPrior::~RDODPTPrior()
{}

Context::LPFindResult RDODPTPrior::onFindContext(const std::string& /*method*/, const Context::Params& /*params*/, const RDOParserSrcInfo& /*srcInfo*/) const
{
    // Добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
    return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
    : RDODPTActivity(src_info, pattern_src_info   )
    , m_value       (IDPTSearchActivity::CostTime::BEFORE)
{
    if (pattern()->getType() != RDOPATPattern::Type::RULE)
    {
        RDOParser::s_parser()->error().push_only(this->src_info(), "Только продукционные правила могут быть использованы в точке принятия решений типа search");
        RDOParser::s_parser()->error().push_only(pattern()->src_info(), "См. образец");
        RDOParser::s_parser()->error().push_done();
    }
    for (RDOPATPattern::RelResList::const_iterator it = pattern()->rel_res_begin(); it != pattern()->rel_res_end(); ++it)
    {
        if (((*it)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::CREATE) || ((*it)->m_statusBegin == rdo::runtime::RDOResource::ConvertStatus::ERASE))
        {
            RDOParser::s_parser()->error().push_only(this->src_info(), rdo::format("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search", src_text().c_str()));
            RDOParser::s_parser()->error().push_only(pattern()->src_info(), "См. образец");
            RDOParser::s_parser()->error().push_only((*it)->src_info(), "См. релевантный ресурс");
            RDOParser::s_parser()->error().push_done();
        }
    }
    m_pActivity = rdo::Factory<rdo::runtime::RDORule>::create(RDOParser::s_parser()->runtime(), pattern()->getPatRuntime<rdo::runtime::RDOPatternRule>(), true, name());
    ASSERT(m_pActivity);
}

RDODPTSearchActivity::~RDODPTSearchActivity()
{}

void RDODPTSearchActivity::setValue(IDPTSearchActivity::CostTime value, const LPRDOFUNArithm& pRuleCost)
{
    m_value     = value;
    m_pRuleCost = pRuleCost;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(const RDOParserSrcInfo& src_info, rdo::runtime::RDODPTSearchTrace::TraceFlag trace, LPILogic pParent)
    : RDOLogic<rdo::runtime::RDODPTSearchRuntime, RDODPTSearchActivity>(src_info)
    , m_pParent(pParent)
    , m_closed (false  )
    , m_trace  (trace  )
{
    RDOParser::s_parser()->checkDPTName   (this->src_info());
    RDOParser::s_parser()->insertDPTSearch(this);
    RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTSearch::~RDODPTSearch()
{}

Context::LPFindResult RDODPTSearch::onFindContext(const std::string& /*method*/, const Context::Params& /*params*/, const RDOParserSrcInfo& /*srcInfo*/) const
{
    // Добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
    return rdo::Factory<FindResult>::create();
}

void RDODPTSearch::end()
{
    rdo::runtime::LPRDOCalc pCalcCondition = m_pConditon     ? m_pConditon->getCalc()     : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();
    rdo::runtime::LPRDOCalc pCalcTerminate = m_pTermConditon ? m_pTermConditon->getCalc() : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();

    m_pRuntimeLogic = rdo::Factory<rdo::runtime::RDODPTSearchRuntime>::create(RDOParser::s_parser()->runtime(),
        m_pParent.object_dynamic_cast<IBaseOperationContainer>(),
        pCalcCondition,
        pCalcTerminate,
        m_pEvalBy->createCalc(),
        m_compTops,
        m_trace);
    ASSERT(m_pRuntimeLogic);
    m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());

    int size = getActivities().size();
    for (int i = 0; i < size; i++)
    {
        LPRDODPTSearchActivity pSearchActivity = getActivities().at(i);
        ASSERT(pSearchActivity);
        LPIDPTSearchActivity pActivity = rdo::Factory<rdo::runtime::RDODPTSearchActivity>::create(
            pSearchActivity->activity().object_dynamic_cast<IRule>(),
            pSearchActivity->getValue(),
            pSearchActivity->getRuleCost()->createCalc()
        );
        ASSERT(pActivity);
        LPIDPTSearchLogic pSearchLogic = m_pRuntimeLogic.object_dynamic_cast<IDPTSearchLogic>();
        ASSERT(pSearchLogic);
        pSearchLogic->addActivity(pActivity);
    }
    m_closed = true;

    // TODO проверить, можно ли избавиться от повторения этой строки. Уже есть в RDOLogicBase::end()
    RDOParser::s_parser()->contextStack()->pop<RDODPTSearch>();
}

CLOSE_RDO_PARSER_NAMESPACE
