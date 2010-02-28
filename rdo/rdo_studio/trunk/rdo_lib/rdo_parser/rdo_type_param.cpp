/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.cpp
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_type          (type    )
{
	ASSERT(m_type);
}

RDOTypeParam::~RDOTypeParam()
{}

void RDOTypeParam::checkValue(CREF(RDOValue) value)
{
	if (value.typeID() != m_type->type().typeID())
	{
		rdoParse::g_error().error(value.src_info(), rdo::format(_T("Не получается преобразовать переменную '%s' в тип %s"), value->getAsString().c_str(), m_type->name().c_str()));
	}
}

void RDOTypeParam::writeModelStructure(REF(std::ostream) stream) const
{
	m_type->writeModelStructure(stream);
}

RDOValue RDOTypeParam::value_cast(CREF(RDOValue) value) const
{
	NEVER_REACH_HERE;
	return value;
}

CLOSE_RDO_PARSER_NAMESPACE
