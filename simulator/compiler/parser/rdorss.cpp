// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/resource/calc_create_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
const std::string RDORSSResource::GET_RESOURCE = "resource_expression";

RDORSSResource::RDORSSResource(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id)
    : RDOParserSrcInfo(src_info                                      )
    , m_pResType      (pResType                                      )
    , m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
    , trace           (false                                         )
    , isNested        (false                                         )
{
    ASSERT(m_pResType);
    m_currParam = m_pResType->getParams().begin();
    RDOParser::s_parser()->contextStack()->push(this);
}

RDORSSResource::~RDORSSResource()
{}

void RDORSSResource::end()
{
    RDOParser::s_parser()->contextStack()->pop<RDORSSResource>();
}

namespace
{

LPExpression contextSetTrace(const rdo::runtime::LPRDOCalc& getResource, bool traceValue, const RDOParserSrcInfo& srcInfo)
{
    return rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::delegate<RDOType__EMPTY>(srcInfo),
        rdo::Factory<rdo::runtime::RDOCalcSetResourceTrace>::create(getResource, traceValue),
        srcInfo
    );
}

LPExpression contextGetResource(const LPRDORSSResource& resource, const RDOParserSrcInfo& srcInfo)
{
    return resource->createGetResourceExpression(srcInfo);
}

}

Context::LPFindResult RDORSSResource::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_OPERATOR_DOT)
    {
        const std::string paramName = params.identifier();

        const std::size_t parNumb = getType()->getRTPParamNumber(paramName);
        if (parNumb == RDORTPResType::UNDEFINED_PARAM)
        {
            return rdo::Factory<FindResult>::create();
        }

        Context::Params params_;
        params_[Context::Params::IDENTIFIER] = paramName;
        params_[RDORSSResource::GET_RESOURCE] = createGetResourceExpression(srcInfo);

        return getType()->find(Context::METHOD_OPERATOR_DOT, params_, srcInfo);
    }

    if (method == Context::METHOD_GET)
    {
        const std::string paramName = params.identifier();

        if (paramName == name())
        {
            LPRDORSSResource pThis(const_cast<RDORSSResource*>(this));
            return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextGetResource, pThis, srcInfo)));
        }
    }

    if (method == "trace()")
    {
        rdo::runtime::LPRDOCalc getResource = rdo::Factory<rdo::runtime::RDOCalcGetResourceByID>::create(getID());
        const bool traceValue = params.get<bool>("traceValue");
        return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextSetTrace, getResource, traceValue, srcInfo)));
    }

    return rdo::Factory<FindResult>::create();
}

LPExpression RDORSSResource::createGetResourceExpression(const RDOParserSrcInfo& srcInfo) const
{
    return rdo::Factory<Expression>::create(
        rdo::Factory<TypeInfo>::create(getType(), getType()->src_info()),
        rdo::Factory<rdo::runtime::RDOCalcGetResourceByID>::create(getID()),
        srcInfo
    );
}

void RDORSSResource::writeModelStructure(std::ostream& stream) const
{
    stream << (getID() + 1) << " " << name() << " " << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(const LPRDOValue& pParam)
{
    ASSERT(pParam);

    LPRDOValue pAddParamValue;
    LPExpression pAddParam;

    if (m_currParam == getType()->getParams().end())
    {
        RDOParser::s_parser()->error().push_only(pParam->src_info(), "Слишком много параметров");
        RDOParser::s_parser()->error().push_only(getType()->src_info(), "См. тип ресурса");
        RDOParser::s_parser()->error().push_done();
    }

    try
    {
        if (pParam->value().getAsString() == "*")
        {
            if (!(*m_currParam)->getDefault()->defined())
            {
                RDOParser::s_parser()->error().push_only(pParam->src_info(), "Невозможно использовать '*', к.т. отсутствует значение по умолчанию");
                // TODO src_info() без параметра RDOParserSrcInfo()
                RDOParser::s_parser()->error().push_only((*m_currParam)->getTypeInfo()->src_info(RDOParserSrcInfo()), "См. описание параметра");
                RDOParser::s_parser()->error().push_done();
            }
            pAddParamValue = (*m_currParam)->getDefault();
        }
        else if (pParam->value().getAsString() == "#")
        {
            pAddParamValue = (*m_currParam)->getDefault()->defined()
                ? (*m_currParam)->getDefault()
                : rdo::Factory<rdo::compiler::parser::RDOValue>::create(
                    (*m_currParam)->getTypeInfo()->itype()->get_default(),
                    (*m_currParam)->getTypeInfo()->src_info(RDOParserSrcInfo()),
                    (*m_currParam)->getTypeInfo()
                );
            ASSERT(pAddParamValue);
            pAddParamValue->value().setUndefined(true);
        }
        else
        {
            pAddParamValue = (*m_currParam)->getTypeInfo()->value_cast(pParam);
        }
    }
    catch(const RDOSyntaxException&)
    {
        RDOParser::s_parser()->error().modify(rdo::format("Для параметра '%s': ", (*m_currParam)->name().c_str()));
    }

    ASSERT(pAddParamValue);
    try
    {
        if (pAddParamValue->value().type().object_dynamic_cast<rdo::runtime::RDOResourceTypeList>())
        {
            LPRDORSSResource pResourceValue = pAddParamValue->value().getPointerByType<RDORTPResType>();
            ASSERT(pResourceValue);

            pResourceValue->setSrcInfo(pParam->src_info());
            pResourceValue->setSrcText(std::to_string(getID()) + '_' + (*m_currParam)->name());

            rdo::runtime::LPRDOCalc pCalc = pResourceValue->createCalc();

            pAddParam = rdo::Factory<Expression>::create(
                rdo::Factory<TypeInfo>::create(pAddParamValue->typeInfo()),
                pCalc,
                pAddParamValue->src_info()
            );
        }
        else
            pAddParam = rdo::Factory<Expression>::create(
                rdo::Factory<TypeInfo>::create(pAddParamValue->typeInfo()),
                rdo::Factory<rdo::runtime::RDOCalcConst>::create(pAddParamValue->value().clone()),
                pAddParamValue->src_info()
            );
    }
    catch (const rdo::runtime::RDOValueException& e)
    {
        RDOParser::s_parser()->error().error(pParam->src_info(), rdo::format("Для параметра '%s': %s", (*m_currParam)->name().c_str(), e.message().c_str()));
    }
    m_paramList.push_back(Param(pAddParam));
    m_currParam++;
}

bool RDORSSResource::defined() const
{
    return m_currParam == getType()->getParams().end();
}

rdo::runtime::LPRDOCalc RDORSSResource::createCalc() const
{
    std::vector<rdo::runtime::LPRDOCalc> paramList;
    for (const auto& param: params())
        paramList.push_back(param.param()->calc());

    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcCreateResource>::create(
        getType()->getNumber(),
        paramList,
        getTrace(),
        getType()->isPermanent(),
        isNested,
        ~0,
        getID()
    );
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText("Создание ресурса " + src_text());
    pCalc->setSrcInfo(srcInfo);

    return pCalc;
}

std::vector<rdo::runtime::LPRDOCalc> RDORSSResource::createCalcList() const
{
    std::vector<rdo::runtime::LPRDOCalc> calcList;
    rdo::runtime::LPRDOCalc pCalc = createCalc();
    calcList.push_back(pCalc);
    if (m_traceCalc)
        calcList.push_back(m_traceCalc);

    return calcList;
}

std::string RDORSSResource::asString() const
{
    return name();
}

CLOSE_RDO_PARSER_NAMESPACE
