// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/such_as.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
RDOTypeParamSuchAs::RDOTypeParamSuchAs(const LPRDOParam& pParam)
	: TypeInfo(pParam->getTypeInfo())
	, m_pParam(pParam               )
{}

RDOTypeParamSuchAs::~RDOTypeParamSuchAs()
{}

const LPRDOParam& RDOTypeParamSuchAs::getParam() const
{
	return m_pParam;
}

CLOSE_RDO_PARSER_NAMESPACE
