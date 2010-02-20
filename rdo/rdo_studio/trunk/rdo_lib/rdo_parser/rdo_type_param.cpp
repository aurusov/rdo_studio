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
RDOTypeParam::RDOTypeParam(CREF(rdoRuntime::RDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOType         (rdoRuntime::g_unknow)
	, RDOParserSrcInfo(src_info            )
{}

RDOTypeParam::~RDOTypeParam()
{}

void RDOTypeParam::checkValue(CREF(RDOValue) value)
{
	if (value->typeID() != type().typeID())
	{
		rdoParse::g_error().error(value.src_info(), rdo::format(_T("Не получается преобразовать переменную '%s' в тип %s"), value->getAsString().c_str(), name().c_str()));
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamInt
// ----------------------------------------------------------------------------
RDOTypeParamInt::RDOTypeParamInt(PTR(RDOTypeRange) range, CREF(RDOParserSrcInfo) src_info)
	: RDOTypeParam(rdoRuntime::g_int, src_info)
	, m_range(range)
{}

void RDOTypeParamInt::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE
