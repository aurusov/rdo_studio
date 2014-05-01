/*!
  \copyright (c) RDO-Team, 2011
  \file      info.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeInfo
// --------------------------------------------------------------------------------
TypeInfo::TypeInfo(const LPTypeInfo& pTypeInfo)
	: m_pType  (pTypeInfo->m_pType  )
	, m_srcInfo(pTypeInfo->m_srcInfo)
{
	init();
}

TypeInfo::TypeInfo(const LPIType& pType, const RDOParserSrcInfo& srcInfo)
	: m_pType  (pType.object_dynamic_cast<rdo::runtime::RDOType>())
	, m_srcInfo(srcInfo)
{
	if (m_srcInfo->src_text().empty())
	{
		m_srcInfo->setSrcText(itype()->name());
	}
	init();
}

void TypeInfo::init()
{
	if (m_pType.object_dynamic_cast<RDOEnumType>())
	{
		RDOParser::s_parser()->insertPreCastType(this);
	}
}

const rdo::runtime::LPRDOType& TypeInfo::type() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID TypeInfo::typeID() const
{
	return m_pType->typeID();
}

LPIType TypeInfo::itype() const
{
	const LPIType result = m_pType.object_dynamic_cast<IType>();
	ASSERT(result);
	return result;
}

LPTypeInfo TypeInfo::type_cast(const LPTypeInfo& pFrom, const RDOParserSrcInfo& src_info) const
{
	/// @todo TypeInfo убрать параметр из src_info()
	LPIType pType = itype()->type_cast(pFrom->itype(), pFrom->src_info(src_info), this->src_info(src_info), src_info);
	ASSERT(pType);
	LPTypeInfo pTypeInfo = rdo::Factory<TypeInfo>::create(pType, this->src_info(src_info));
	ASSERT(pTypeInfo);
	return pTypeInfo;
}

LPRDOValue TypeInfo::value_cast(const LPRDOValue& pValue) const
{
	return itype()->value_cast(pValue, m_srcInfo.get(), pValue->src_info());
}

CLOSE_RDO_PARSER_NAMESPACE
