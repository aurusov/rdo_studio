// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_resource.h"
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/compiler/parser/rdorss.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
RDORTPParam::RDORTPParam(const LPTypeInfo& pType, const LPRDOValue& pDefault, const RDOParserSrcInfo& src_info)
	: RDOParam(src_info, pType, pDefault)
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
	dynamic_cast<IModelStructure*>(getTypeInfo()->type().get())->writeModelStructure(stream);
}

CLOSE_RDO_PARSER_NAMESPACE
