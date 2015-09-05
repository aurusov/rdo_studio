#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/param.h"
#include "simulator/runtime/rdo_model_i.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDORTPResType);

class RDORTPParam
    : public RDOParam
    , public IModelStructure
    , public IName
    , public virtual rdo::counter_reference
{
DECLARE_FACTORY(RDORTPParam);
public:
    DECLARE_IModelStructure;
    DECLARE_IName;

private:
    RDORTPParam(const LPRDOTypeParam& pParamType, const LPRDOValue& pDefault, const RDOParserSrcInfo& src_info);
    virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
