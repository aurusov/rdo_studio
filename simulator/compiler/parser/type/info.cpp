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
	: m_pType(pTypeInfo->m_pType)
	, m_srcInfo(pTypeInfo->m_srcInfo)
{
	init();
}

TypeInfo::TypeInfo(const LPIType& pType, const RDOParserSrcInfo& srcInfo)
	: m_pType (pType)
	, m_srcInfo(srcInfo)
{
	if (m_srcInfo->src_text().empty())
	{
		m_srcInfo->setSrcText(itype()->name());
	}
	init();
}

TypeInfo::~TypeInfo()
{}

RDOParserSrcInfo TypeInfo::src_info() const
{
	return m_srcInfo
		? m_srcInfo.get()
		: RDOParserSrcInfo();
}

const RDOParserSrcInfo& TypeInfo::src_info(const RDOParserSrcInfo& srcInfo) const
{
	return m_srcInfo
		? m_srcInfo.get()
		: srcInfo;
}

void TypeInfo::init()
{
	if (m_pType.object_dynamic_cast<RDOEnumType>())
	{
		RDOParser::s_parser()->insertPreCastType(this);
	}
}

rdo::runtime::LPRDOType TypeInfo::type() const
{
	return m_pType.object_dynamic_cast<rdo::runtime::RDOType>();
}

rdo::runtime::RDOType::TypeID TypeInfo::typeID() const
{
	return type()->typeID();
}

const LPIType& TypeInfo::itype() const
{
	return m_pType;
}

LPTypeInfo TypeInfo::type_cast(const LPTypeInfo& pFrom, const RDOParserSrcInfo& src_info) const
{
	// TODO TypeInfo убрать параметр из src_info()
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
