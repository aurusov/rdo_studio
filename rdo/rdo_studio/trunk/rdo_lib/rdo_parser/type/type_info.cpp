/*
 * copyright: (c) RDO-Team, 2011
 * filename : type_info.cpp
 * author   : Урусов Андрей
 * date     : 09.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type_info.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------
TypeInfo::TypeInfo(CREF(LPTypeInfo) pTypeInfo)
	: m_pType  (pTypeInfo->m_pType  )
	, m_srcInfo(pTypeInfo->m_srcInfo)
{}

TypeInfo::TypeInfo(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) srcInfo)
	: m_pType  (pType  )
	, m_srcInfo(srcInfo)
{}

TypeInfo::~TypeInfo()
{}

CREF(LPRDOType) TypeInfo::type() const
{
	return m_pType;
}

CREF(RDOParserSrcInfo) TypeInfo::src_info(CREF(RDOParserSrcInfo) srcInfo) const
{
	return m_srcInfo ? m_srcInfo.get() : srcInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
