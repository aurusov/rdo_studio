// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_rsslex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
    LEXER->m_lpval = lpval;
    LEXER->m_lploc = llocp;
    return LEXER->yylex();
}

void cnv_rsserror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
RDORSSResource::RDORSSResource(Converter* pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id)
    : RDOParserSrcInfo(src_info                                      )
    , m_pResType      (pResType                                      )
    , m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
    , trace           (false                                         )
{
    ASSERT(m_pResType);
    pParser->insertRSSResource(LPRDORSSResource(this));
    m_currParam = m_pResType->getParams().begin();
}

void RDORSSResource::writeModelStructure(std::ostream& stream) const
{
    stream << (getID() + 1) << " " << name() << " " << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(const LPRDOValue& pParam)
{
    ASSERT(pParam);

    if (m_currParam == getType()->getParams().end())
    {
        Converter::s_converter()->error().push_only(pParam->src_info(), "Слишком много параметров");
        Converter::s_converter()->error().push_only(getType()->src_info(), "См. тип ресурса");
        Converter::s_converter()->error().push_done();
    }
    try
    {
        if (pParam->value().getAsString() == "*")
        {
            if (!(*m_currParam)->getDefault()->defined())
            {
                Converter::s_converter()->error().push_only(pParam->src_info(), "Невозможно использовать '*', к.т. отсутствует значение по умолчанию");
                Converter::s_converter()->error().push_only((*m_currParam)->getType()->src_info(), "См. описание параметра");
                Converter::s_converter()->error().push_done();
            }
            m_paramList.push_back(Param((*m_currParam)->getDefault()));
            m_currParam++;
        }
        else
        {
            m_paramList.push_back(Param((*m_currParam)->getType()->value_cast(pParam)));
            m_currParam++;
        }
    }
    catch(const RDOSyntaxException&)
    {
        Converter::s_converter()->error().modify(rdo::format("Для параметра '%s': ", (*m_currParam)->name().c_str()));
    }
}

bool RDORSSResource::defined() const
{
    return m_currParam == getType()->getParams().end();
}

rdo::runtime::LPRDOCalc RDORSSResource::createCalc() const
{
    std::vector<rdo::runtime::RDOValue> paramList;
    for (const auto& param: params())
    {
        paramList.push_back(param.param()->value());
    }

    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText("Создание ресурса " + src_text());
    pCalc->setSrcInfo(srcInfo);
    return pCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(Converter* pParser, const RDOParserSrcInfo& src_info, const LPRDORTPResType& pResType, std::size_t id)
    : RDORSSResource(pParser, src_info, pResType, id)
{}

rdo::runtime::LPRDOCalc RDOPROCResource::createCalc() const
{
    std::vector<rdo::runtime::RDOValue> paramList;
    for (const auto& param: params())
        paramList.push_back(param.param()->value());

    rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
    ASSERT(pCalc);
    rdo::runtime::RDOSrcInfo srcInfo(src_info());
    srcInfo.setSrcText("Создание ресурса " + src_text());
    pCalc->setSrcInfo(srcInfo);
    return pCalc;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
