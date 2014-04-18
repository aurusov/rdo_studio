/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

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
