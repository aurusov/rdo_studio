#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_object.h"
#include "simulator/runtime/rdo_model_i.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParam
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOTypeParam);
class RDOTypeParam
    : public virtual rdo::counter_reference
    , public RDOParserSrcInfo
    , public IModelStructure
{
DECLARE_FACTORY(RDOTypeParam);
public:
    LPRDOType   type      ()                         const;
    LPRDOValue  value_cast(const LPRDOValue& pValue) const;

    DECLARE_IModelStructure;

protected:
    RDOTypeParam(const LPRDOType& pType, const RDOParserSrcInfo& src_info);
    virtual ~RDOTypeParam();

private:
    LPRDOType m_pType;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
