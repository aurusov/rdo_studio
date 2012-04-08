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
RDOTypeParam::RDOTypeParam(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_pType         (pType   )
{
	ASSERT(m_pType);
	setSrcText(m_pType->name());

	if (m_pType->type()->typeID() == rdo::runtime::RDOType::t_enum ||
		m_pType->type().object_dynamic_cast<rdo::runtime::RDOFuzzyType>())
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

void RDOTypeParam::writeModelStructure(REF(std::ostream) stream) const
{
	m_pType->writeModelStructure(stream);
}

LPRDOValue RDOTypeParam::value_cast(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);
	return m_pType->value_cast(pValue, src_info(), pValue->src_info());
}

CLOSE_RDO_CONVERTER_NAMESPACE
