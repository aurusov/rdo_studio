// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type_param_suchas.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
RDOTypeParamSuchAs::RDOTypeParamSuchAs(const LPRDOParam& pParam)
    : RDOTypeParam(pParam->getType()->type(), pParam->getType()->src_info())
    , m_pParam(pParam)
{}

RDOTypeParamSuchAs::~RDOTypeParamSuchAs()
{}

const LPRDOParam& RDOTypeParamSuchAs::getParam() const
{
    return m_pParam;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
