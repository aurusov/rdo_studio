/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : Чирков Михаил, Урусов Андрей
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
	: RDOType         (&rdoRuntime::g_unknow)
	, RDOParserSrcInfo(src_info             )
	, m_pItemType     (pItemType            )
{
	ASSERT(m_pItemType);
	m_type = new rdoRuntime::RDOArrayType(RDOParser::s_parser()->runtime(), m_pItemType->type());
	setSrcText(name());
	rdoParse::RDOParser::s_parser()->insertArrayType(this);
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
			//! Это один и тот же тип
			if (pArray == from)
			{
				return pArray;
			}
			else
			{
				rdoParse::g_error().push_only(src_info,     _T("Несоответствие массивов"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("и"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdoParse::g_error().push_done();
			break;
		}
	default:
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Ожидается значение типа массив, найдено: %s"), from_src_info.src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
		break;
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOArrayType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
	try
	{
		if (from.typeID() == rdoRuntime::RDOType::t_array)
			if (m_type == &from.type()->type())
				toValue = from;
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Неверное значение параметра для массива: %s"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T(": %s"), name().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

PTR(rdoRuntime::RDOCalc) RDOArrayType::calc_cast(PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

RDOValue RDOArrayType::get_default() const
{
	NEVER_REACH_HERE;
	return RDOValue();
}

void RDOArrayType::writeModelStructure(REF(std::ostream) stream) const
{
	CPTR(RDOArrayType) pArray(const_cast<PTR(RDOArrayType)>(this));
	stream << (*pArray).name()<< std::endl;
}

CREF(LPRDOType) RDOArrayType::itemType() const
{
	return m_pItemType;
}

//----------------------------------------------------------------------------
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(LPRDOArrayType pArrayType)
	: m_pArrayType(pArrayType)
{
	ASSERT(m_pArrayType);
}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insertItem(CREF(RDOValue) value)
{
	m_pArrayType->itemType()->type_cast(value.type(), value.src_info(), m_pArrayType->src_info(), value.src_info());
	RDOValue itemValue = m_pArrayType->itemType()->value_cast(value, m_pArrayType->src_info(), value.src_info());
	m_container.push_back(itemValue);
}

CREF(LPRDOArrayType) RDOArrayValue::getArrayType() const
{
	return m_pArrayType;
}

rdoRuntime::RDOValue RDOArrayValue::getRValue() const
{
	rdoRuntime::RDOArrayValue arrayValue(*m_pArrayType->__array());
	STL_FOR_ALL_CONST(Container, m_container, it)
	{
		arrayValue.insertItem(it->value());
	}
	rdoRuntime::RDOValue value(arrayValue);
	return value;
}

CLOSE_RDO_PARSER_NAMESPACE
