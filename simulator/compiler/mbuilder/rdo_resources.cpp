// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/rdo_resources.h"

#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/type/range.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResType
// --------------------------------------------------------------------------------
// ---- Инициализация типа ресурса по существующему в памяти
// ---- Собирает все параметры существующего в памяти типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(const parser::LPRDORTPResType& rtp)
    : parser::RDOParserSrcInfo(rtp->src_info())
    , m_name (rtp->name())
    , m_exist(true)
    , m_kind (rtp->isPermanent() ? Kind::PERMANENT : Kind::TEMPORARY)
    , m_id   (rtp->getNumber())
{
    for (const auto& parser_param: rtp->getParams())
    {
        Param param(parser_param);
        param.m_id = m_params.size();
        m_params.append(param);
    }
}

RDOResType::Param::Param(const parser::LPRDORTPParam& param)
    : parser::RDOParserSrcInfo(param->src_info())
    , m_name    (param->name())
    , m_exist   (true)
    , m_pType   (param->getTypeInfo())
    , m_pDefault(param->getDefault())
    , m_id      (-1)
{
    switch (typeID())
    {
        case rdo::runtime::RDOType::Type::INT:
        {
            parser::LPRDOTypeIntRange pRange = param->getTypeInfo()->itype().object_dynamic_cast<parser::RDOTypeIntRange>();
            if (pRange)
            {
                m_pMin = pRange->range()->getMin();
                m_pMax = pRange->range()->getMax();
            }
            break;
        }
        case rdo::runtime::RDOType::Type::REAL:
        {
            parser::LPRDOTypeRealRange pRange = param->getTypeInfo()->itype().object_dynamic_cast<parser::RDOTypeRealRange>();
            if (pRange)
            {
                m_pMin = pRange->range()->getMin();
                m_pMax = pRange->range()->getMax();
            }
            break;
        }
        default:
            break;
    }
}

// --------------------------------------------------------------------------------
// ---- Инициализация *нового* типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(const std::string& name, Kind kind)
    : m_name (name)
    , m_exist(false)
    , m_kind (kind)
    , m_id   (std::size_t(~0))
{}

bool RDOResType::ParamList::append(Param& param)
{
    if (std::find_if(begin(), end(), parser::compareNameRef<Param>(param.name())) != end())
    {
        return false;
    }

    m_list.push_back(param);
    return true;
}

RDOResType::Param::Param(const std::string& name, const parser::LPTypeInfo& pType, const parser::LPRDOValue& pDefault)
    : m_name    (name)
    , m_exist   (true)
    , m_pType   (pType)
    , m_pDefault(pDefault)
    , m_id      (-1)
{}

RDOResType::Param::Param(const std::string& name, const rdo::intrusive_ptr<parser::RDOType__INT>& pType, const parser::LPRDOValue& pDefault)
    : m_name    (name)
    , m_exist   (true)
    , m_pDefault(pDefault)
    , m_id      (-1)
{
    initType(pType);
}

RDOResType::Param::Param(const std::string& name, const rdo::intrusive_ptr<parser::RDOType__REAL>& pType, const parser::LPRDOValue& pDefault)
    : m_name    (name)
    , m_exist   (true)
    , m_pDefault(pDefault)
    , m_id      (-1)
{
    initType(pType);
}

RDOResType::Param::Param(const std::string& name, const rdo::runtime::RDOEnumType::Enums& enums, const parser::LPRDOValue& pDefault)
    : m_name    (name)
    , m_exist   (true)
    , m_pDefault(pDefault)
    , m_id      (-1)
{
    initType(enums);
}

void RDOResType::Param::setRange(const parser::LPRDOValue& pMin, const parser::LPRDOValue& pMax)
{
    ASSERT(pMin);
    ASSERT(pMax);

    m_pMin = pMin;
    m_pMax = pMax;
}

void RDOResType::Param::setDefault(const parser::LPRDOValue& pDefault)
{
    ASSERT(pDefault);

    m_pDefault = pDefault;
}

bool RDOResType::Param::operator== (const Param& /*param*/) const
{
    NEVER_REACH_HERE;
    return true;
}

std::string RDOResType::Param::typeStr() const
{
    return m_pType->itype().object_dynamic_cast<rdo::compiler::parser::IType>()->name();
}

// --------------------------------------------------------------------------------
// -------------------- RDOResTypeList
// --------------------------------------------------------------------------------
// ---- Собирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResTypeList::RDOResTypeList(const parser::LPRDOParser& pParser)
    : RDOList<RDOResType>(pParser)
{
    for (const auto& parser_rtp: m_pParser->getRTPResTypes())
    {
        const RDOResType rtp(parser_rtp);
        m_list.push_back(rtp);
    }
}

// --------------------------------------------------------------------------------
// ---- Добавление *нового* типа ресурса
// --------------------------------------------------------------------------------
parser::LPRDORTPResType RDOResTypeList::appendBefore(RDOResType& rtp)
{
    if (std::find_if(begin(), end(), parser::compareNameRef<RDOResType>(rtp.name())) != end())
    {
        return parser::LPRDORTPResType(NULL);
    }

    parser::LPRDORTPResType pResourceType = rdo::Factory<parser::RDORTPResType>::create(m_pParser, parser::RDOParserSrcInfo(rtp.name()), rtp.isPermanent());
    ASSERT(pResourceType);
    return pResourceType;
}

bool RDOResTypeList::appendAfter(RDOResType& rtp, const parser::LPRDORTPResType& pResourceType)
{
    ASSERT(pResourceType);

    for (const auto& param: rtp.m_params)
    {
        parser::LPTypeInfo pParamType;
        parser::LPRDOValue pDefault = param.hasDefault() ? param.getDefault() : rdo::Factory<parser::RDOValue>::create();
        ASSERT(pDefault);
        switch (param.typeID())
        {
            case rdo::runtime::RDOType::Type::INT:
            {
                if (param.hasRange())
                {
                    parser::LPRDOTypeRangeRange pRange    = rdo::Factory<parser::RDOTypeRangeRange>::create(param.getMin(), param.getMax(), parser::RDOParserSrcInfo());
                    parser::LPRDOTypeIntRange   pIntRange = rdo::Factory<parser::RDOTypeIntRange>::create(pRange);
                    pParamType = rdo::Factory<parser::TypeInfo>::create(pIntRange, parser::RDOParserSrcInfo());
                }
                else
                {
                    pParamType = param.type();
                }
                break;
            }
            case rdo::runtime::RDOType::Type::REAL:
            {
                if (param.hasRange())
                {
                    parser::LPRDOTypeRangeRange pRange     = rdo::Factory<parser::RDOTypeRangeRange>::create(param.getMin(), param.getMax(), parser::RDOParserSrcInfo());
                    parser::LPRDOTypeRealRange  pRealRange = rdo::Factory<parser::RDOTypeRealRange>::create(pRange);
                    pParamType = rdo::Factory<parser::TypeInfo>::create(pRealRange, parser::RDOParserSrcInfo());
                }
                else
                {
                    pParamType = param.type();
                }
                break;
            }
            case rdo::runtime::RDOType::Type::ENUM:
            {
                pParamType = param.type();
                break;
            }
            default:
            {
                m_pParser->removeRTPResType(pResourceType);
                return false;
            }
        }
        parser::LPRDORTPParam pParam = rdo::Factory<parser::RDORTPParam>::create(pParamType, pDefault, parser::RDOParserSrcInfo(param.name()));
        ASSERT(pParam);
        pResourceType->addParam(pParam);
    }
    rtp.m_exist = true;
    rtp.m_id    = pResourceType->getNumber();
    m_list.push_back(rtp);
    return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
// ---- Инициализация ресурса по существующему в памяти
// ---- Собирает все параметры существующего в памяти ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(const parser::LPRDORSSResource& rss)
    : parser::RDOParserSrcInfo(rss->src_info())
    , m_name (rss->name())
    , m_exist(true)
    , m_rtp  (rss->getType())
    , m_id   (rss->getID())
{
    if (m_rtp.m_params.size() == rss->params().size())
    {
        std::size_t index = 0;
        for (const auto& param: m_rtp.m_params)
        {
            m_params[param.name()] = rss->params()[index].param();
            index++;
        }
    }
}

RDOResource::Params::const_iterator RDOResource::operator[](const std::string& param) const
{
    return m_params.find(param);
}

RDOResource::Params::mapped_type& RDOResource::operator[](const std::string& param)
{
    RDOResource::Params::iterator param_it = m_params.find(param);
    if (param_it != m_params.end())
    {
        return param_it->second;
    }
    else
    {
        static Params::mapped_type tmpValue;
        return tmpValue;
    }
}

parser::LPRDORSSResource RDOResource::getParserResource(const parser::LPRDOParser& pParser) const
{
    if (!exist())
    {
        return parser::LPRDORSSResource(NULL);
    }

    return pParser->findRSSResource(name());
}

bool RDOResource::fillParserResourceParams(parser::LPRDORSSResource& pToParserRSS) const
{
    for (const auto& param: getType().m_params)
    {
        RDOResource::Params::const_iterator value_it = operator[](param.name());
        if (value_it == end())
            return false;

        runtime::RDOValue value = value_it->second->calc().object_dynamic_cast<runtime::RDOCalcConst>()->getValue();
        parser::LPRDOValue pValue = Factory<parser::RDOValue>::create(value, pToParserRSS->src_info(), value_it->second->typeInfo());
        ASSERT(pValue);
        pValue = param.type()->value_cast(pValue);
        // TODO а почему тут toParserRSS->src_info(), а не value_it->src_info() ?
        pValue->setSrcInfo(pToParserRSS->src_info());
        pToParserRSS->addParam(pValue);
    }
    return true;
}

// --------------------------------------------------------------------------------
// ---- Инициализация *нового* ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(const RDOResType& rtp, const std::string& name)
    : m_name (name)
    , m_exist(false)
    , m_rtp  (rtp)
    , m_id   (parser::RDORSSResource::UNDEFINED_ID)
{
    for (const auto& param: m_rtp.m_params)
    {
        parser::LPRDOValue pValue = rdo::Factory<parser::RDOValue>::create(param.type(), param.src_info());
        ASSERT(pValue);
        if (param.hasDefault())
        {
            pValue = param.getDefault();
        }
        else if (param.hasRange())
        {
            pValue = param.getMin();
        }
        parser::LPExpression pValueExpression = rdo::Factory<parser::Expression>::create(pValue);
        m_params[param.name()] = pValueExpression;
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOResourceList
// --------------------------------------------------------------------------------
// ---- Собирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResourceList::RDOResourceList(const parser::LPRDOParser& pParser)
    : RDOList<RDOResource>(pParser)
{
    for (const auto& parser_rss: m_pParser->getRSSResources())
    {
        const RDOResource rss(parser_rss);
        m_list.push_back(rss);
    }
}

CLOSE_COMPILER_MBUILDER_NAMESPACE
