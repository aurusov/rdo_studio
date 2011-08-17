/*
 * copyright: (c) RDO-Team, 2011
 * file     : rdo_type_param.cpp
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type_param.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_type          (type    )
{
	ASSERT(m_type);
	setSrcText(m_type->name());

	switch (m_type->type()->typeID())
	{
	case rdoRuntime::RDOType::t_enum :
	case rdoRuntime::RDOType::t_fuzzy: Converter::s_converter()->insertPreCastType(this); break;
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
