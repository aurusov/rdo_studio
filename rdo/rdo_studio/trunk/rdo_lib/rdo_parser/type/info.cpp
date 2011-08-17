/*
 * copyright: (c) RDO-Team, 2011
 * file     : info.cpp
 * author   : Урусов Андрей
 * date     : 09.04.2011
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/info.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------
TypeInfo::TypeInfo(CREF(LPTypeInfo) pTypeInfo)
	: m_pType  (pTypeInfo->m_pType  )
	, m_srcInfo(pTypeInfo->m_srcInfo)
{
	init();
}

TypeInfo::TypeInfo(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) srcInfo)
	: m_pType  (pType  )
	, m_srcInfo(srcInfo)
{
	init();
}

void TypeInfo::init()
{
	switch (m_pType->type()->typeID())
	{
	case rdoRuntime::RDOType::t_enum :
	case rdoRuntime::RDOType::t_fuzzy: RDOParser::s_parser()->insertPreCastType(this); break;
	}
}

LPTypeInfo TypeInfo::type_cast(CREF(LPTypeInfo) pFrom, CREF(RDOParserSrcInfo) src_info) const
{
	/// @todo TypeInfo убрать параметр из src_info()
	LPRDOType pType = type()->type_cast(pFrom->type(), pFrom->src_info(src_info), this->src_info(src_info), src_info);
	ASSERT(pType);
	LPTypeInfo pTypeInfo = rdo::Factory<TypeInfo>::create(pType, this->src_info(src_info));
	ASSERT(pTypeInfo);
	return pTypeInfo;
}

RDOValue TypeInfo::value_cast(CREF(RDOValue) value) const
{
	return m_pType->value_cast(value, m_srcInfo.get(), value.src_info());
}

CLOSE_RDO_PARSER_NAMESPACE
