// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int funlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void funerror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
    for (const auto& intOrDouble: m_intOrDouble)
    {
        intOrDouble->needRound();
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNBase
// --------------------------------------------------------------------------------
RDOFUNBase::RDOFUNBase(const RDOParserSrcInfo& srcInfo)
    : RDOParserSrcInfo(srcInfo)
{}

RDOFUNBase::RDOFUNBase(const LPExpression& pExpression)
    : m_pExpression(pExpression)
{
    ASSERT(m_pExpression);
}

const LPExpression& RDOFUNBase::expression() const
{
    ASSERT(m_pExpression);
    return m_pExpression;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(const LPRDOFUNArithm& pArithm)
    : RDOFUNBase(pArithm->expression())
{
    if (pArithm->typeID() != rdo::runtime::RDOType::Type::BOOL)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Ожидается логическое выражение, найдено: %s", src_text().c_str()));
    }
}

RDOFUNLogic::RDOFUNLogic(const LPExpression& pExpression, bool hideWarning)
    : RDOFUNBase(pExpression)
{
    rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
    srcInfo.setSrcFileType(src_filetype());
    m_pExpression->setSrcInfo(srcInfo);
    if (!hideWarning)
    {
        rdo::runtime::LPRDOCalcConst pConstCalc = m_pExpression->calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
        if (pConstCalc)
        {
            if (pConstCalc->getValue().getAsBool())
            {
                RDOParser::s_parser()->error().warning(pConstCalc->srcInfo(), rdo::format("Логическое выражение всегда истинно: %s", pConstCalc->srcInfo().src_text().c_str()));
            }
            else
            {
                RDOParser::s_parser()->error().warning(pConstCalc->srcInfo(), rdo::format("Логическое выражение всегда ложно: %s", pConstCalc->srcInfo().src_text().c_str()));
            }
        }
    }
}

RDOFUNLogic::~RDOFUNLogic()
{}

LPRDOFUNLogic RDOFUNLogic::generateTrue(const RDOParserSrcInfo& srcInfo)
{
    LPExpression pExpression = rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(srcInfo),
        rdo::Factory<rdo::runtime::RDOCalcConst>::create(true),
        srcInfo
    );
    ASSERT(pExpression);

    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
    ASSERT(pLogic);

    return pLogic;
}

rdo::runtime::LPRDOCalc RDOFUNLogic::getCalc(rdo::runtime::RDOType::Type id)
{
    if (id != rdo::runtime::RDOType::Type::REAL)
    {
        m_intOrDouble.roundCalc();
    }
    return m_pExpression->calc();
}

LPRDOFUNLogic RDOFUNLogic::createLogic(const rdo::runtime::LPRDOCalc& pCalc)
{
    ASSERT(pCalc);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(pCalc->srcInfo()),
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);

    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
    ASSERT(pLogic);

    pLogic->setSrcInfo(pCalc->srcInfo());

    return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(const LPRDOFUNLogic& pSecond)
{
    ASSERT(pSecond);

    rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
    ASSERT(pCalc);
    LPRDOFUNLogic pLogic = createLogic(pCalc);
    pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
    return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(const RDOSrcInfo::Position& position)
{
    rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
    ASSERT(pCalc);
    LPRDOFUNLogic pLogic = createLogic(pCalc);
    pLogic->m_intOrDouble.insert(m_intOrDouble);
    return pLogic;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (const LPRDOFUNLogic& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcAnd>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (const LPRDOFUNLogic& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcOr>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator_not(const RDOSrcInfo::Position& position)
{
    return generateLogic<rdo::runtime::RDOCalcNot>(position);
}

void RDOFUNLogic::setSrcInfo(const RDOParserSrcInfo& srcInfo)
{
    RDOParserSrcInfo::setSrcInfo(srcInfo);
    m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNLogic::setSrcPos(const RDOSrcInfo::Position& position)
{
    RDOParserSrcInfo::setSrcPos(position);
    rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
    srcInfo.setSrcPos(position);
    m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNLogic::setSrcText(const std::string& value)
{
    RDOParserSrcInfo::setSrcText(value);
    rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
    srcInfo.setSrcText(value);
    m_pExpression->setSrcInfo(srcInfo);
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
const std::string RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM = "set_arithm";

RDOFUNArithm::RDOFUNArithm(const LPExpression& pExpression)
    : RDOFUNBase(pExpression)
{
    ASSERT(m_pExpression);
    setSrcInfo(m_pExpression->src_info());
}

RDOFUNArithm::~RDOFUNArithm()
{}

LPRDOFUNArithm RDOFUNArithm::generateByConst(const LPRDOValue& pValue)
{
    ASSERT(pValue);
    ASSERT(pValue->constant());

    LPExpression pExpression = rdo::Factory<Expression>::create(pValue);
    ASSERT(pExpression);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);

    return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(const LPRDOValue& pValue)
{
    ASSERT(pValue);

    LPContext pContext = RDOParser::s_parser()->context();
    ASSERT(pContext);

    Context::Params params;
    params[Context::Params::IDENTIFIER] = pValue->value().getIdentificator();
    Context::LPFindResult result = pContext->find(Context::METHOD_GET, params, pValue->src_info());
    ASSERT(result->getCreateExpression());

    LPExpression pExpression = result->getCreateExpression()();
    ASSERT(pExpression);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);

    return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(const LPRDOValue& pValue1, const LPRDOValue& pValue2)
{
    ASSERT(pValue1);
    ASSERT(pValue2);

    LPContext pContext = RDOParser::s_parser()->context();
    ASSERT(pContext);

    Context::Params params;
    params[Context::Params::IDENTIFIER] = pValue1->value().getIdentificator();
    Context::LPFindResult result = pContext->find(Context::METHOD_OPERATOR_DOT, params, pValue1->src_info());
    ASSERT(result->getSwitchContext());

    params = result->getSwitchContext().params;
    ASSERT(!params.exists(Context::Params::IDENTIFIER));
    params[Context::Params::IDENTIFIER] = pValue2->value().getIdentificator();
    result = result->getSwitchContext().context->find(Context::METHOD_GET, params, pValue2->src_info());
    ASSERT(result->getCreateExpression());

    LPExpression pExpression = result->getCreateExpression()();
    ASSERT(pExpression);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);

    return pArithm;
}

void RDOFUNArithm::castType(const LPRDOFUNArithm& pSecond, const std::string& error)
{
    try
    {
        expression()->typeInfo()->itype()->type_cast(pSecond->expression()->typeInfo()->itype(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
    }
    catch (const RDOSyntaxException&)
    {
        RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), typeInfo()->itype()->name().c_str(), pSecond->typeInfo()->itype()->name().c_str()));
    }
}

void RDOFUNArithm::castValue(const LPRDOFUNArithm& pSecond, const std::string& error)
{
    try
    {
        LPIType pType = expression()->typeInfo()->itype()->type_cast(pSecond->expression()->typeInfo()->itype(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
        ASSERT(pType);
        LPRDOValue pConstant = pSecond->expression()->constant();
        if (pConstant && pConstant->defined())
        {
            pConstant = expression()->typeInfo()->itype()->value_cast(pConstant, expression()->src_info(), pSecond->expression()->src_info());
            ASSERT(pConstant);
            pSecond->m_pExpression = rdo::Factory<Expression>::create(
                rdo::Factory<TypeInfo>::create(
                    pType,
                    expression()->src_info()
                ),
                rdo::Factory<rdo::runtime::RDOCalcConst>::create(pConstant->value()),
                pConstant->src_info()
            );
            ASSERT(pSecond->m_pExpression);
        }
    }
    catch (const RDOSyntaxException&)
    {
        RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), expression()->src_info().src_text().c_str(), pSecond->expression()->src_info().src_text().c_str()));
    }
}

template <class T>
rdo::runtime::LPRDOCalc RDOFUNArithm::generateCalc(const rdo::runtime::RDOSrcInfo::Position& position, const std::string& /*error*/)
{
    rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
    ASSERT(pCalc);
    return pCalc;
}

template <class T>
rdo::runtime::LPRDOCalc RDOFUNArithm::generateCalc(const LPRDOFUNArithm& pSecond, const std::string& error)
{
    castType(pSecond, error);
    rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
    ASSERT(pCalc);
    return pCalc;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(const rdo::runtime::RDOSrcInfo::Position& position, const std::string& error)
{
    rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(position, error);
    ASSERT(pCalc);

    LPTypeInfo pType = getPreType(this);
    ASSERT(pType);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        pType,
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);

    pArithm->m_intOrDouble.insert(m_intOrDouble);
    return pArithm;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(const LPRDOFUNArithm& pSecond, const std::string& error)
{
    rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
    ASSERT(pCalc);

    LPTypeInfo pType = getPreType(pSecond);
    ASSERT(pType);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        pType,
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);

    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);

    pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
    return pArithm;
}

template <class T>
LPRDOFUNLogic RDOFUNArithm::generateLogic(const LPRDOFUNArithm& pSecond, const std::string& error)
{
    rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
    ASSERT(pCalc);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(pCalc->srcInfo()),
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);

    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
    ASSERT(pLogic);

    pLogic->setSrcInfo(pCalc->srcInfo());
    pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
    return pLogic;
}

rdo::runtime::RDOValue RDOFUNArithm::const_value() const
{
    rdo::runtime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
    ASSERT(pCalc);
    return pCalc->getValue();
}

LPTypeInfo RDOFUNArithm::getPreType(const LPRDOFUNArithm& pSecond)
{
    if (typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Внутренняя ошибка парсера: неопределенное значение арифметического выражения: %s", src_text().c_str()));
    }
    if (pSecond->typeID() == rdo::runtime::RDOType::Type::UNKNOW)
    {
        RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format("Внутренняя ошибка парсера: неопределенное значение арифметического выражения: %s", pSecond->src_text().c_str()));
    }
    if (typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        rdo::runtime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
        ASSERT(pCalc);
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Неизвестный идентификатор: %s", const_value().getIdentificator().c_str()));
    }
    if (pSecond->typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format("Неизвестный идентификатор: %s", pSecond->const_value().getIdentificator().c_str()));
    }

    return typeInfo()->type_cast(pSecond->typeInfo(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (const LPRDOFUNArithm& pSecond)
{
    return generateArithm<rdo::runtime::RDOCalcPlus>(pSecond, "Ну не могу я сложить %s и %s");
}

LPRDOFUNArithm RDOFUNArithm::operator- (const LPRDOFUNArithm& pSecond)
{
    return generateArithm<rdo::runtime::RDOCalcMinus>(pSecond, "Нельзя из %s вычесть %s");
}

LPRDOFUNArithm RDOFUNArithm::operator* (const LPRDOFUNArithm& pSecond)
{
    return generateArithm<rdo::runtime::RDOCalcMult>(pSecond, "Нельзя %s умножить на %s");
}

LPRDOFUNArithm RDOFUNArithm::operator/ (const LPRDOFUNArithm& pSecond)
{
    rdo::runtime::LPRDOCalc pCalc = generateCalc<rdo::runtime::RDOCalcDiv>(pSecond, "Нельзя %s разделить на %s");
    ASSERT(pCalc);
    LPTypeInfo pType = getPreType(pSecond);
    ASSERT(pType);

    // TODO перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
    // Ответ: с умножением надо делать тоже самое, только непонятно как
    if (pType->typeID() == rdo::runtime::RDOType::Type::INT)
    {
        rdo::runtime::LPRDOCalc pNewCalcDiv = pCalc;
        pCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
        pCalc->setSrcInfo(pNewCalcDiv->srcInfo());
    }
    LPExpression pExpression = rdo::Factory<Expression>::create(
        pType,
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    if (pType->typeID() == rdo::runtime::RDOType::Type::INT)
    {
        rdo::runtime::LPRDOCalcDoubleToIntByResult pResult = pCalc.object_static_cast<rdo::runtime::RDOCalcDoubleToIntByResult>();
        pArithm->m_intOrDouble.push_back(pResult);
    }
    pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
    return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::setEqual(const LPRDOFUNArithm& pSecond)
{
    castValue(pSecond, "Ошибка присваивания %s = %s");
    return this;
}

LPRDOFUNArithm RDOFUNArithm::uminus(const rdo::runtime::RDOSrcInfo::Position& position)
{
    return generateArithm<rdo::runtime::RDOCalcUMinus>(position, "Нельзя взять унарный минус от %s");
}

LPRDOFUNLogic RDOFUNArithm::operator< (const LPRDOFUNArithm& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcIsLess>(pSecond, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator> (const LPRDOFUNArithm& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcIsGreater>(pSecond, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator<= (const LPRDOFUNArithm& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcIsLEQ>(pSecond, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator>= (const LPRDOFUNArithm& pSecond)
{
    return generateLogic<rdo::runtime::RDOCalcIsGEQ>(pSecond, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator== (const LPRDOFUNArithm& pSecond)
{
    castValue(pSecond, "Ошибка сравнения %s == %s");
    return generateLogic<rdo::runtime::RDOCalcIsEqual>(pSecond, "Нельзя сравнивать %s и %s");
}

LPRDOFUNLogic RDOFUNArithm::operator!= (const LPRDOFUNArithm& pSecond)
{
    castValue(pSecond, "Ошибка сравнения %s <> %s");
    return generateLogic<rdo::runtime::RDOCalcIsNotEqual>(pSecond, "Нельзя сравнивать %s и %s");
}

void RDOFUNArithm::checkParamType(const LPTypeInfo& pType)
{
    pType->itype()->type_cast(typeInfo()->itype(), src_info(), pType->src_info(), src_info());
    rdo::runtime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
    if (pConstCalc)
    {
        rdo::runtime::RDOValue value = pConstCalc->getValue();
        pType->value_cast(rdo::Factory<RDOValue>::create(value, src_info(), typeInfo()));
    }
}

void RDOFUNArithm::wrongVarInitialization(const LPRDOValue& name)
{
    const std::string identificator = name->value().getIdentificator();
    LPRDOFUNFunction pFunction = RDOParser::s_parser()->getLastFUNFunction();
    ASSERT(pFunction);
    LPRDOParam pParam = pFunction->findParam(identificator);
    if (pParam)
    {
        RDOParser::s_parser()->error().error(name->src_info(), rdo::format("Функции не могут изменить свой параметр: %s", identificator.c_str()));
    }
    else
    {
        RDOParser::s_parser()->error().error(name->src_info(), rdo::format("Неизвестный идентификатор: %s", identificator.c_str()));
    }
}

rdo::runtime::LPRDOCalc RDOFUNArithm::createCalc(const LPTypeInfo& pForType)
{
    if (typeID() != rdo::runtime::RDOType::Type::IDENTIFICATOR)
    {
        if (!pForType)
        {
            return m_pExpression->calc();
        }
        if (pForType->typeID() != rdo::runtime::RDOType::Type::INT)
        {
            if (pForType->typeID() == rdo::runtime::RDOType::Type::ENUM)
            {
                m_intOrDouble.roundCalc();
            }
            return m_pExpression->calc();
        }
        else
        {
            m_intOrDouble.roundCalc();
            rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(m_pExpression->calc()->srcInfo().src_pos(), m_pExpression->calc());
            ASSERT(pCalc);
            pCalc->setSrcInfo(src_info());
            return pCalc;
        }
    }
    else if (typeID() == rdo::runtime::RDOType::Type::IDENTIFICATOR && !pForType)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Неизвестный идентификатор: %s", const_value().getAsString().c_str()));
    }

    if (!pForType)
    {
        RDOParser::s_parser()->error().error(src_info(), "Неизвестный тип параметра");
    }

    rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pForType->value_cast(rdo::Factory<RDOValue>::create(src_info()))->value());
    pNewCalc->setSrcInfo(src_info());
    return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(const RDOParserSrcInfo& srcInfo)
{
    RDOParserSrcInfo::setSrcInfo(srcInfo);
    m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNArithm::setSrcPos(const RDOSrcInfo::Position& position)
{
    RDOParserSrcInfo::setSrcPos(position);
    rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
    srcInfo.setSrcPos(position);
    m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNArithm::setSrcText(const std::string& value)
{
    RDOParserSrcInfo::setSrcText(value);
    rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
    srcInfo.setSrcText(this->src_info().src_text());
    m_pExpression->setSrcInfo(srcInfo);
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
RDOFUNConstant::RDOFUNConstant(const RDOParserSrcInfo& srcInfo, const LPTypeInfo& pType, const LPRDOValue& pDefault)
    : RDOParam(srcInfo, pType, pDefault)
{
    if (!getDefault() || !getDefault()->defined())
    {
        ASSERT(pType);
        RDOParser::s_parser()->error().error(pType->src_info(), "Константа должна иметь значение");
    }

    m_number = RDOParser::s_parser()->getFUNCONST_id();
    RDOParser::s_parser()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

namespace
{

LPExpression contextConstant(const LPTypeInfo pTypeInfo, const int& number, const RDOParserSrcInfo& srcInfo)
{
    return rdo::Factory<Expression>::create(
        pTypeInfo,
        rdo::Factory<rdo::runtime::RDOCalcGetConst>::create(number),
        srcInfo
    );
}

}

Context::LPFindResult RDOFUNConstant::onFindContext(const std::string& method, const Context::Params& /*params*/, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_GET)
    {
        return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextConstant, getTypeInfo(), getNumber(), srcInfo)));
    }

    return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- ArithmContainer
// --------------------------------------------------------------------------------
ArithmContainer::ArithmContainer()
    : RDOParserSrcInfo()
{}

ArithmContainer::~ArithmContainer()
{}

void ArithmContainer::addItem(const LPRDOFUNArithm& pArithm)
{
    ASSERT(pArithm);
    m_arithmList.push_back(pArithm);
}

LPFunctionParamType ArithmContainer::getType() const
{
    FunctionParamType::ParamList paramList;

    for (const LPRDOFUNArithm& pArithm: m_arithmList)
    {
        paramList.push_back(pArithm->typeInfo());
    }

    if (paramList.empty())
    {
        paramList.push_back(
            rdo::Factory<TypeInfo>::delegate<RDOType__EMPTY>(src_info())
        );
    }

    LPFunctionParamType pParamType = rdo::Factory<FunctionParamType>::create(paramList, src_info());
    ASSERT(pParamType);
    return pParamType;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// --------------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams(const LPArithmContainer& pArithmContainer)
    : RDOParserSrcInfo  (                )
    , m_pArithmContainer(pArithmContainer)
{
    ASSERT(m_pArithmContainer);
}

RDOFUNParams::~RDOFUNParams()
{}

rdo::runtime::LPRDOCalc RDOFUNParams::getCalc(std::size_t paramID, const LPTypeInfo& pType)
{
    ASSERT(paramID < m_pArithmContainer->getContainer().size());
    rdo::runtime::LPRDOCalc pCalc = m_pArithmContainer->getContainer()[paramID]->createCalc(pType);
    ASSERT(pCalc);
    return pCalc;
}

LPExpression RDOFUNParams::createCallExpression(const LPExpression& pExpression)
{
    ASSERT(pExpression);

    LPFunctionType pFunctionType = pExpression->typeInfo()->itype().object_dynamic_cast<FunctionType>();
    ASSERT(pFunctionType);

    LPFunctionParamType pFunctionParamType = pFunctionType->paramType();
    ASSERT(pFunctionParamType);

    LPFunctionParamType pCallerParamType = m_pArithmContainer->getType();
    ASSERT(pCallerParamType);

    pFunctionParamType.object_dynamic_cast<IType>()->type_cast(
        pCallerParamType,
        pCallerParamType->src_info(),
        pFunctionParamType->src_info(),
        src_info()
    );

    rdo::runtime::LPRDOCalc pCalc = pExpression->calc();
    ASSERT(pCalc);

    rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pCalc);
    ASSERT(pFuncCall);
    pFuncCall->setSrcInfo(src_info());

    ArithmContainer::Container::const_iterator arithmIt = m_pArithmContainer->getContainer().begin();
    for (const LPTypeInfo& pFuncParam: pFunctionParamType->paramList())
    {
        if (pFuncParam->typeID() != rdo::runtime::RDOType::Type::EMPTY)
        {
            LPRDOFUNArithm pArithm = *arithmIt;
            ASSERT(pArithm);

            pArithm->checkParamType(pFuncParam);
            pFuncCall->addParameter(pFuncParam->itype()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->typeInfo()->itype()));

            ++arithmIt;
        }
    }

    LPExpression pResult = rdo::Factory<Expression>::create(
        pFunctionType->returnType(),
        pFuncCall,
        src_info()
    );
    ASSERT(pResult);

    return pResult;
}

LPRDOFUNArithm RDOFUNParams::createCall(const std::string& funName)
{
    LPRDOFUNFunction pFunction = RDOParser::s_parser()->findFUNFunction(funName);
    if (!pFunction)
    {
        return createSeqCall(funName);
    }

    const std::size_t nParams = pFunction->getParams().size();
    if (nParams != m_pArithmContainer->getContainer().size())
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Неверное количество параметров функции: %s", funName.c_str()));
    }

    rdo::runtime::LPRDOCalc pCalc = pFunction->getFunctionCalc();
    ASSERT(pCalc);
    pCalc = pFunction->getReturn()->getTypeInfo()->itype()->calc_cast(pCalc, pFunction->getReturn()->getTypeInfo()->itype());
    ASSERT(pCalc);
    rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pCalc);
    pFuncCall->setSrcInfo(src_info());
    for (std::size_t i = 0; i < nParams; i++)
    {
        LPTypeInfo pFuncParam = pFunction->getParams()[i]->getTypeInfo();
        LPRDOFUNArithm pArithm = m_pArithmContainer->getContainer()[i];
        ASSERT(pArithm);
        pArithm->checkParamType(pFuncParam);
        pFuncCall->addParameter(pFuncParam->itype()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->typeInfo()->itype()));
    }

    LPExpression pExpression = rdo::Factory<Expression>::create(
        pFunction->getReturn()->getTypeInfo(),
        pFuncCall,
        src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(src_info());
    return pArithm;
}

LPRDOFUNArithm RDOFUNParams::createSeqCall(const std::string& seqName)
{
    LPRDOFUNSequence pSequence = RDOParser::s_parser()->findFUNSequence(seqName);
    if (!pSequence)
    {
        RDOParser::s_parser()->error().error(m_funseqName, rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()));
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
    RDOParser::s_parser()->insertFUNSequence(this);
}

RDOFUNSequence::~RDOFUNSequence()
{}

void RDOFUNSequence::initResult()
{
    initCalcSrcInfo();
    m_pNextCalc->m_res_real = true;
    switch (m_pHeader->getTypeInfo()->typeID())
    {
        case rdo::runtime::RDOType::Type::INT:
        {
            LPRDOTypeIntRange pRange = m_pHeader->getTypeInfo()->itype().object_dynamic_cast<RDOTypeIntRange>();
            if (pRange)
            {
                m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
            }
            break;
        }
        case rdo::runtime::RDOType::Type::REAL:
        {
            m_pNextCalc->m_res_real = true;
            LPRDOTypeRealRange pRange = m_pHeader->getTypeInfo()->itype().object_dynamic_cast<RDOTypeRealRange>();
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
        srcInfo.setSrcText("Установка базы последовательности " + m_pHeader->src_text());
        m_pInitCalc->setSrcInfo(srcInfo);
    }
    m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

namespace
{

LPExpression contextSequence(const std::string& name, const RDOParserSrcInfo& srcInfo)
{
    LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create(
        rdo::Factory<ArithmContainer>::create()
    );
    LPRDOFUNArithm pArithm = pParams->createSeqCall(name);
    pArithm->setSrcInfo(srcInfo);

    return rdo::Factory<Expression>::create(
        pArithm->typeInfo(),
        pArithm->calc(),
        srcInfo
    );
}

}

Context::LPFindResult RDOFUNSequence::onFindContext(const std::string& method, const Context::Params& /*params*/, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_GET)
    {
        return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextSequence, name(), srcInfo)));
    }

    return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceUniform
// --------------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getTypeInfo()->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()));
    }
}

void RDOFUNSequenceUniform::createCalcs()
{
    rdo::runtime::RandGeneratorUniform* pGenerator = new rdo::runtime::RandGeneratorUniform();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextUniform>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 2)
    {
        RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__REAL>(RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
    rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

    pFuctionCall->addParameter(pArg1);
    pFuctionCall->addParameter(pArg2);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(seqSrcInfo);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getTypeInfo()->typeID() != rdo::runtime::RDOType::Type::INT && m_pHeader->getTypeInfo()->typeID() != rdo::runtime::RDOType::Type::REAL)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()));
    }
}

void RDOFUNSequenceExponential::createCalcs()
{
    rdo::runtime::RandGeneratorExponential* pGenerator = new rdo::runtime::RandGeneratorExponential();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextExponential>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 1)
    {
        RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__REAL>(RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

    pFuctionCall->addParameter(pArg1);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(seqSrcInfo);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getTypeInfo()->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()));
    }
}

void RDOFUNSequenceNormal::createCalcs()
{
    rdo::runtime::RandGeneratorNormal* pGenerator = new rdo::runtime::RandGeneratorNormal();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextNormal>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 2)
    {
        RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__REAL>(RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
    rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

    pFuctionCall->addParameter(pArg1);
    pFuctionCall->addParameter(pArg2);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(seqSrcInfo);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceTriangular
// ----------------------------------------------------------------------------
RDOFUNSequenceTriangular::RDOFUNSequenceTriangular(const LPRDOFUNSequenceHeader& pHeader, int seed)
    : RDOFUNSequence(pHeader, seed)
{
    if (m_pHeader->getTypeInfo()->typeID() != rdo::runtime::RDOType::Type::INT && m_pHeader->getTypeInfo()->typeID() != rdo::runtime::RDOType::Type::REAL)
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Последовательность '%s' может возвращять значения только целого или вещественного типа", src_text().c_str()));
    }
}

void RDOFUNSequenceTriangular::createCalcs()
{
    rdo::runtime::RandGeneratorTriangular* pGenerator = new rdo::runtime::RandGeneratorTriangular();
    m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextTriangular>::create(pGenerator);
    initResult();
}

LPRDOFUNArithm RDOFUNSequenceTriangular::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& seqSrcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 3)
    {
        RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format("Для треугольного закона распределения '%s' нужно указать три параметра: левую границу, точку под высотой треугольника, правую границу", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__REAL>(RDOParserSrcInfo());
    rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
    rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);
    rdo::runtime::LPRDOCalc pArg3 = pParamList->getCalc(2, pType);

    pFuctionCall->addParameter(pArg1);
    pFuctionCall->addParameter(pArg2);
    pFuctionCall->addParameter(pArg3);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(seqSrcInfo);
    if (pArithm->typeID() == rdo::runtime::RDOType::Type::ENUM)
    {
        RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка парсера");
    }
    return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(const LPRDOFUNSequenceByHistHeader& pHeader)
    : RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& srcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 0)
    {
        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Гистограмма '%s' должна вызываться без параметров", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(srcInfo);
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
        RDOParser::s_parser()->error().error(pFrom->src_info(), "Начало интервала должно совпадать с окончанием предыдущего");
    }
    if (pFrom->value() > pTo->value())
    {
        RDOParser::s_parser()->error().error(pTo->src_info(), "Начало интервала должно быть меньше его конца");
    }
    if (pFreq->value() == 0)
    {
        RDOParser::s_parser()->error().warning(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    else if (pFreq->value() < 0)
    {
        RDOParser::s_parser()->error().error(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    m_pHeader->getTypeInfo()->value_cast(pFrom);
    m_pHeader->getTypeInfo()->value_cast(pTo  );
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
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
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
        RDOParser::s_parser()->error().warning(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    else if (pFreq->value() < 0)
    {
        RDOParser::s_parser()->error().error(pFreq->src_info(), "Относительная вероятность должна быть больше нуля");
    }
    m_pHeader->getTypeInfo()->value_cast(pValue);
    rdo::runtime::RDOValue enum_id = m_pHeader->getTypeInfo()->value_cast(pValue)->value();
    if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
    {
        RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format("Перечислимое значение определено дважды: %s", pValue->value().getIdentificator().c_str()));
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
    RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
    m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
    initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(LPRDOFUNParams& pParamList, const RDOParserSrcInfo& srcInfo) const
{
    if (pParamList->getParamList()->getContainer().size() != 0)
    {
        RDOParser::s_parser()->error().error(srcInfo, rdo::format("Перечисление '%s' должно вызываться без параметров", name().c_str()));
    }

    rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
    ASSERT(pFuctionCall);

    LPExpression pExpression = rdo::Factory<Expression>::create(
        m_pHeader->getTypeInfo(),
        pFuctionCall,
        pParamList->src_info()
    );
    ASSERT(pExpression);
    LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
    ASSERT(pArithm);
    pArithm->setSrcInfo(srcInfo);
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
RDOFUNFunctionListElement::RDOFUNFunctionListElement(const RDOParserSrcInfo& srcInfo)
    : RDOParserSrcInfo(srcInfo)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdo::runtime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(const LPTypeInfo& pRetType, const rdo::runtime::LPRDOCalcFuncParam& pFuncParam, const RDOParserSrcInfo& src_pos) const
{
    rdo::runtime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
    return rdo::Factory<rdo::runtime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(const RDOParserSrcInfo& srcInfo)
    : RDOFUNFunctionListElement(srcInfo)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const
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

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const
{
    LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__REAL>(src_info());
    ASSERT(pType);
    rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
        pRetType->value_cast(
            rdo::Factory<RDOValue>::create(m_value, src_pos, pType)
        )->value()
    );
    ASSERT(pCalcConst);
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

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(const LPTypeInfo& pRetType, const RDOParserSrcInfo& src_pos) const
{
    LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__INT>(src_info());
    ASSERT(pType);
    rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
        pRetType->value_cast(
            rdo::Factory<RDOValue>::create(m_value, src_pos, pType)
        )->value()
    );
    ASSERT(pCalcConst);
    pCalcConst->setSrcInfo(src_pos);
    return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(const YYLTYPE& position)
    : RDOFUNFunctionListElement(RDOParserSrcInfo(position, "="))
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(const LPTypeInfo& /*pRetType*/, const RDOParserSrcInfo& src_pos) const
{
    RDOParser::s_parser()->error().error(src_pos, "Внутренняя ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc");
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
    m_pAction->checkParamType(RDOParser::s_parser()->getLastFUNFunction()->getReturn()->getTypeInfo());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(const RDOParserSrcInfo& srcInfo, const LPRDOParam& pReturn)
    : Function (pReturn->getTypeInfo(), srcInfo)
    , m_pReturn(pReturn)
{
    if (m_pReturn->getDefault()->defined())
    {
        LPRDOValue pDefault = m_pReturn->getTypeInfo()->value_cast(m_pReturn->getDefault());
        ASSERT(pDefault);

        rdo::runtime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pDefault->value());
        ASSERT(pCalcDefault);
        pCalcDefault->setSrcInfo(m_pReturn->getTypeInfo()->src_info());

        Function::setDefaultCalc(pCalcDefault);
    }

    init();
}

RDOFUNFunction::RDOFUNFunction(const std::string& name, const LPRDOParam& pReturn)
    : Function (pReturn->getTypeInfo(), RDOParserSrcInfo(name))
    , m_pReturn(pReturn)
{
    init();
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::init()
{
    RDOParser::s_parser()->insertFUNFunction(this);
    Function::pushContext();
}

void RDOFUNFunction::end()
{
    Function::popContext();
}

const std::string& RDOFUNFunction::name() const
{
    return src_info().src_text();
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
        RDOParser::s_parser()->error().error(m_pReturn->src_info(), rdo::format("Функция '%s' должна иметь значение по умолчанию", name().c_str()));
    }
    if (getParams().empty())
    {
        RDOParser::s_parser()->error().error(src_info(), rdo::format("Функция '%s' должна иметь параметры, т.к. её тип list", name().c_str()));
    }
    if (m_elementList.empty())
    {
        RDOParser::s_parser()->error().warning(src_info(), rdo::format("Функция '%s' не содержит списка", name().c_str()));
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
        ParamList::const_iterator param_it = getParams().begin();
        while (param_it != getParams().end())
        {
            LPRDOParam pParam = *param_it;
            if (elem_it == m_elementList.end())
            {
                --elem_it;
                RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format("Ожидается значение для параметра '%s'", pParam->name().c_str()));
            }
            LPRDOFUNFunctionListElement pListElement = *elem_it;
            ASSERT(pListElement);
            if (pListElement->isEquivalence())
            {
                if ((param_it + 1) == getParams().end())
                {
                    RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format("Перед знаком равенства ожидается значение для параметра: %s",pParam->name().c_str()));
                }
                else
                {
                    std::string str = (*param_it)->src_text();
                    ++param_it;
                    while (param_it != getParams().end())
                    {
                        str += ", ";
                        str += (*param_it)->src_text();
                        ++param_it;
                    }
                    RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format("Перед знаком равенства ожидаются значения для параметров: %s", str.c_str()));
                }
            }
            rdo::runtime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
            ASSERT(pFuncParam);
            rdo::runtime::LPRDOCalcIsEqual pCompareCalc = pListElement->createIsEqualCalc(pParam->getTypeInfo(), pFuncParam, pListElement->src_info());
            ASSERT(pCompareCalc);
            rdo::runtime::LPRDOCalc pAndCalc = rdo::Factory<rdo::runtime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
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
            RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format("После значения для параметра '%s' ожидается знак равенства", (*param_it)->name().c_str()));
        }
        if (!(*elem_it)->isEquivalence())
        {
            RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format("Слишком много значений, вместо '%s' ожидается знак равенства", (*elem_it)->src_text().c_str()));
        }
        ++elem_it;
        if (elem_it == m_elementList.end())
        {
            --elem_it;
            RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format("После знака равенства ожидается значение функции '%s'", name().c_str()));
        }
        rdo::runtime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getTypeInfo(), (*elem_it)->src_info());
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
            RDOParser::s_parser()->error().error((*it)->src_info(), "Символ '=' недопустим в табличной функции");
        }
        ++it;
    }
    int   param_cnt = getParams().size();
    std::size_t range = 1;
    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(0);
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText("0");
    pCalc->setSrcInfo(srcInfo);
    for (int currParam = 0; currParam < param_cnt; currParam++)
    {
        LPRDOParam pFunctionParam = getParams().at(currParam);
        ASSERT(pFunctionParam);
        rdo::runtime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
        ASSERT(pFuncParam);
        rdo::runtime::LPRDOCalc pValue2 = pFuncParam;
        if (pFunctionParam->getTypeInfo()->typeID() != rdo::runtime::RDOType::Type::ENUM)
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

        switch (pFunctionParam->getTypeInfo()->typeID())
        {
        case rdo::runtime::RDOType::Type::INT:
            if (dynamic_cast<const RDOTypeIntRange*>(pFunctionParam->getTypeInfo()->itype().get()))
            {
                LPRDOTypeIntRange pRange = pFunctionParam->getTypeInfo()->itype().object_dynamic_cast<RDOTypeIntRange>();
                if (pRange->range()->getMin()->value().getInt() != 1)
                {
                    RDOParser::s_parser()->error().error(pRange->range()->src_info(), rdo::format("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]", pRange->range()->getMin()->value().getInt(), pRange->range()->getMax()->value().getInt()));
                }
                range *= pRange->range()->getMax()->value().getInt() - pRange->range()->getMin()->value().getInt() + 1;
            }
            else
            {
                RDOParser::s_parser()->error().error(pFunctionParam->getTypeInfo()->src_info(), "Для параметра табличной функции должен быть задан допустимый диапазон");
            }
            break;
        case rdo::runtime::RDOType::Type::ENUM:
            range *= pFunctionParam->getTypeInfo()->itype().object_dynamic_cast<RDOEnumType>()->getValues().size();
            break;
        default:
            RDOParser::s_parser()->error().error(pFunctionParam->getTypeInfo()->src_info(), "Параметр табличной функции может быть целого или перечислимого типа");
        }
        pCalc = pCalcAdd;
    }

    if (m_elementList.size() != range)
    {
        RDOParser::s_parser()->error().error(elements_pos, rdo::format("Неверное количество элементов табличной функции '%s': %d, требуется %d", name().c_str(), m_elementList.size(), range));
    }

    rdo::runtime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdo::runtime::RDOFuncTableCalc>::create(pCalc);
    ASSERT(pFuncTableCalc);
    pFuncTableCalc->setSrcInfo(src_info());
    for (std::size_t currElem = 0; currElem < range; currElem++)
    {
        LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
        ASSERT(pListElement);
        rdo::runtime::LPRDOCalcConst pResultCalc = pListElement->createResultCalc(m_pReturn->getTypeInfo(), pListElement->src_info());
        ASSERT(pResultCalc);
        pFuncTableCalc->addResultCalc(pResultCalc);
    }
    setFunctionCalc(pFuncTableCalc);
}

const LPRDOParam& RDOFUNFunction::getReturn() const
{
    return m_pReturn;
}

rdo::runtime::LPRDOCalc RDOFUNFunction::getFunctionCalc() const
{
    return m_pFunctionCalc
        ? m_pFunctionCalc
        : expression()->calc();
}

void RDOFUNFunction::setFunctionCalc(const rdo::runtime::LPRDOCalc& pCalc)
{
    ASSERT(pCalc);
    m_pFunctionCalc = pCalc;
    if (m_pFunctionCalc->srcInfo().src_empty())
    {
        m_pFunctionCalc->setSrcInfo(src_info());
    }
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
    m_pResType = RDOParser::s_parser()->findRTPResType(res_info.src_text());
    if (!m_pResType)
    {
        RDOParser::s_parser()->error().error(res_info, rdo::format("Неизвестный тип ресурса: %s", res_info.src_text().c_str()));
    }
    RDOParser::s_parser()->insertFUNGroup(this);
    RDOParser::s_parser()->contextStack()->push(this);
}

void RDOFUNGroup::end()
{
    RDOParser::s_parser()->getFUNGroupStack().pop_back();
    RDOParser::s_parser()->contextStack()->pop<RDOFUNGroup>();
}

Context::LPFindResult RDOFUNGroup::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    // Тип ресурса внутри групповой функции
    if (method == Context::METHOD_OPERATOR_DOT)
    {
        if (getResType()->name() == params.identifier())
        {
            Context::Params params;
            params[RDORSSResource::GET_RESOURCE] = rdo::Factory<Expression>::create(
                rdo::Factory<TypeInfo>::create(getResType(), srcInfo),
                rdo::Factory<rdo::runtime::RDOCalcGetGroupFunctionResource>::create(),
                srcInfo
            );
            return rdo::Factory<FindResult>::create(SwitchContext(getResType(), params));
        }
    }

    return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(Type type, const RDOParserSrcInfo& res_info)
    : RDOFUNGroup(res_info)
    , m_type(type)
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(LPRDOFUNLogic& pCondition)
{
    rdo::runtime::LPRDOFunCalcGroup pCalc;
    switch (m_type)
    {
    case Type::EXIST    : setSrcText("Exist("     + getResType()->name() + ": " + pCondition->src_text() + ")"); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::NOTEXIST : setSrcText("NotExist("  + getResType()->name() + ": " + pCondition->src_text() + ")"); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::FORALL   : setSrcText("ForAll("    + getResType()->name() + ": " + pCondition->src_text() + ")"); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
    case Type::NOTFORALL: setSrcText("NotForAll(" + getResType()->name() + ": " + pCondition->src_text() + ")"); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
    default: RDOParser::s_parser()->error().error(src_info(), "Внутренняя ошибка: несуществующий тип функции");
    }
    end();

    LPExpression pExpression = rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(pCalc->srcInfo()),
        pCalc,
        pCalc->srcInfo()
    );
    ASSERT(pExpression);

    LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
    ASSERT(pLogic);

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
    m_pCalcSelect = rdo::Factory<rdo::runtime::RDOFunCalcSelect>::create(getResType().interface_dynamic_cast<rdo::runtime::IResourceType>(), getResType()->getNumber(), pCondition->getCalc());
    m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

namespace
{
    LPExpression getSelectExpression(
            const LPRDOFUNSelect&          select,
            const rdo::runtime::LPRDOCalc& selectCalc,
            const LPTypeInfo&              selectType,
            const RDOParserSrcInfo&        srcInfo)
    {
        select->end();
        return rdo::Factory<Expression>::create(
            selectType,
            selectCalc,
            srcInfo
        );
    }
}

Context::LPFindResult RDOFUNSelect::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    if (method == "select()")
    {
        rdo::runtime::LPRDOCalc selectCalc;
        LPTypeInfo selectType;

        switch (params.get<RDOFUNGroupLogic::Type>("FunGroupType"))
        {
        case RDOFUNGroupLogic::Type::EXIST:
        {
            LPRDOFUNLogic pCondition = params.get<LPRDOFUNLogic>("GroupLogic");
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".Exist(" + pCondition->src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectExist>::create(m_pCalcSelect, pCondition->getCalc());
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(selectCalc->srcInfo());
            break;
        }
        case RDOFUNGroupLogic::Type::NOTEXIST:
        {
            LPRDOFUNLogic pCondition = params.get<LPRDOFUNLogic>("GroupLogic");
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".NotExist(" + pCondition->src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotExist>::create(m_pCalcSelect, pCondition->getCalc());
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(selectCalc->srcInfo());
            break;
        }
        case RDOFUNGroupLogic::Type::FORALL:
        {
            LPRDOFUNLogic pCondition = params.get<LPRDOFUNLogic>("GroupLogic");
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".ForAll(" + pCondition->src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectForAll>::create(m_pCalcSelect, pCondition->getCalc());
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(selectCalc->srcInfo());
            break;
        }
        case RDOFUNGroupLogic::Type::NOTFORALL:
        {
            LPRDOFUNLogic pCondition = params.get<LPRDOFUNLogic>("GroupLogic");
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".NotForAll(" + pCondition->src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc());
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(selectCalc->srcInfo());
            break;
        }
        case RDOFUNGroupLogic::Type::EMPTY:
        {
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".Empty(" + srcInfo.src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect);
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__BOOL>(selectCalc->srcInfo());
            break;
        }
        case RDOFUNGroupLogic::Type::SIZE:
        {
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".Size(" + srcInfo.src_text() + ")");
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectSize>::create(m_pCalcSelect);
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::delegate<RDOType__INT>(srcInfo);
            break;
        }
        case RDOFUNGroupLogic::Type::ARRAY:
        {
            const_cast<RDOFUNSelect*>(this)->setSrcText(src_text() + ".Array(" + srcInfo.src_text() + ")");
            LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(rdo::Factory<TypeInfo>::create(getResType(), srcInfo), srcInfo);
            selectCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectArray>::create(m_pCalcSelect);
            selectCalc->setSrcInfo(src_info());
            selectType = rdo::Factory<TypeInfo>::create(pArrayType, srcInfo);
            break;
        }
        default:
            RDOParser::s_parser()->error().error(srcInfo, "Внутренняя ошибка: неизвестный метод для списка ресурсов");
            break;
        }
        ASSERT(selectCalc);
        ASSERT(selectType);
        return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&getSelectExpression,
            LPRDOFUNSelect(const_cast<RDOFUNSelect*>(this)), selectCalc,selectType, srcInfo)));
    }

    return RDOFUNGroup::onFindContext(method, params, srcInfo);
}

CLOSE_RDO_PARSER_NAMESPACE
