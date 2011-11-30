/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_param.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.02.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
#include "converter/smr2rdox/rdo_type_param.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParam
// --------------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_type          (type    )
{
	ASSERT(m_type);
	setSrcText(m_type->name());

	if (m_type->type()->typeID() == rdoRuntime::RDOType::t_enum ||
		m_type->type().object_dynamic_cast<rdoRuntime::RDOFuzzyType>())
	{
		Converter::s_converter()->insertPreCastType(this);
	}
}

RDOTypeParam::~RDOTypeParam()
{}

LPRDOType RDOTypeParam::type() const
{
	return m_type;
}

void RDOTypeParam::writeModelStructure(REF(std::ostream) stream) const
{
	m_type->writeModelStructure(stream);
}

RDOValue RDOTypeParam::value_cast(CREF(RDOValue) value) const
{
	return m_type->value_cast(value, src_info(), value.src_info());
}

CLOSE_RDO_CONVERTER_NAMESPACE
