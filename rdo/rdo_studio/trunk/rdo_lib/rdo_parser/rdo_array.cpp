/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPRDOType) pItemType, CREF(RDOParserSrcInfo) src_info)
	: RDOType          (rdoRuntime::g_unknow)
	, RDOParserSrcInfo (src_info            )
	, m_pItemType      (pItemType           )
{
	ASSERT(pItemType);
	setSrcText(name());
	m_pType = rdo::Factory<rdoRuntime::RDOArrayType>::create(pItemType->type());
	ASSERT(m_pType);
}

RDOArrayType::~RDOArrayType()
{}

inline tstring RDOArrayType::name() const
{
	return rdo::format(_T("array<%s>"), m_pItemType->name().c_str());
}

LPRDOType RDOArrayType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
			LPRDOArrayType from(const_cast<PTR(RDOArrayType)>(static_cast<CPTR(RDOArrayType)>(from.get())));
			//! Это один и тот же тип
			if (pArray->getItemType()->type_cast(from->getItemType(), from->src_info(), pArray->src_info(), from->src_info()))
			{
				return pArray;
			}
			break;
		}
	default:
		{
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("Несоответствие размерности массива")));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
		}
	}
	return NULL;
}

RDOValue RDOArrayType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch(from.typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArrayT(const_cast<PTR(RDOArrayType)>(this));
			rdoRuntime::RDOArrayValue rArrayV = from->getArray();
			for(rdoRuntime::RDOArrayValue::Container::iterator it = rArrayV.containerBegin(); it != rArrayV.containerEnd(); ++it)
			{
				*it = pArrayT->getItemType()->value_cast(RDOValue(*it, from.type(), src_info), to_src_info, src_info).value();
			}
			return from;
			break;
		}
	default:
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Несоответствие размерности массива")));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
		break;
	}
	}
	return from;
}

rdoRuntime::LPRDOCalc RDOArrayType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

RDOValue RDOArrayType::get_default() const
{
	LPRDOArrayType pArrayType(const_cast<PTR(RDOArrayType)>(this));
	return RDOValue(rdoRuntime::RDOArrayValue(getRuntimeArrayType()), pArrayType, RDOParserSrcInfo());
}

void RDOArrayType::writeModelStructure(REF(std::ostream) stream) const
{
	CPTR(RDOArrayType) pArray(const_cast<PTR(RDOArrayType)>(this));
	stream << (*pArray).name()<< std::endl;
}

CREF(LPRDOType) RDOArrayType::getItemType() const
{
	return m_pItemType;
}

rdoRuntime::LPRDOArrayType RDOArrayType::getRuntimeArrayType() const
{
	return m_pType.object_static_cast<rdoRuntime::RDOArrayType>();
}

//----------------------------------------------------------------------------
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pArrayType)
	: m_pArrayType(pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insertItem(CREF(RDOValue) value)
{
	m_pArrayType->getItemType()->type_cast(value.type(), value.src_info(), m_pArrayType->src_info(), value.src_info());
	RDOValue ItemValue = m_pArrayType->getItemType()->value_cast(value, m_pArrayType->src_info(), value.src_info());
	m_container.push_back(ItemValue);
}

CREF(LPRDOArrayType) RDOArrayValue::getArrayType() const
{
	return m_pArrayType;
}

rdoRuntime::RDOValue RDOArrayValue::getRArray() const
{
	rdoRuntime::RDOArrayValue arrayValue(m_pArrayType->getRuntimeArrayType());
	STL_FOR_ALL_CONST(m_container, it)
	{
		arrayValue.insertItem(it->value());
	}
	rdoRuntime::RDOValue value(arrayValue);
	return value;
}

CLOSE_RDO_PARSER_NAMESPACE
