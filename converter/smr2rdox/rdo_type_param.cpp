// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
#include "converter/smr2rdox/rdo_type_param.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParam
// --------------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(const LPRDOType& pType, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info)
    , m_pType         (pType   )
{
    ASSERT(m_pType);
    setSrcText(m_pType->name());

    if (m_pType->type()->typeID() == rdo::runtime::RDOType::t_enum)
    {
        Converter::s_converter()->insertPreCastType(this);
    }
}

RDOTypeParam::~RDOTypeParam()
{}

LPRDOType RDOTypeParam::type() const
{
    return m_pType;
}

void RDOTypeParam::writeModelStructure(std::ostream& stream) const
{
    m_pType->writeModelStructure(stream);
}

LPRDOValue RDOTypeParam::value_cast(const LPRDOValue& pValue) const
{
    ASSERT(pValue);
    return m_pType->value_cast(pValue, src_info(), pValue->src_info());
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
