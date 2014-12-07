#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayType
// --------------------------------------------------------------------------------
class RDOArrayType
    : public rdo::runtime::RDOArrayType
    , public IType
    , public IModelStructure
    , public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
public:
    const LPTypeInfo& getItemType() const;
    LPTypeInfo typeInfo() const;

private:
    RDOArrayType(const LPTypeInfo& pItemType, const RDOParserSrcInfo& src_info);
    virtual ~RDOArrayType();

    LPTypeInfo m_pItemType;

    DECLARE_IType;
    DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

CLOSE_RDO_PARSER_NAMESPACE
