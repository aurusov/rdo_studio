// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdortp_param.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
RDORTPParam::RDORTPParam(const LPRDOTypeParam& pParamType, const LPRDOValue& pDefault, const RDOParserSrcInfo& src_info)
	: RDOParam(src_info, pParamType, pDefault)
{}

RDORTPParam::~RDORTPParam()
{}

const std::string& RDORTPParam::name() const
{
	return RDOParam::name();
}

void RDORTPParam::writeModelStructure(std::ostream& stream) const
{
	stream << name() << " ";
	getType()->writeModelStructure(stream);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
