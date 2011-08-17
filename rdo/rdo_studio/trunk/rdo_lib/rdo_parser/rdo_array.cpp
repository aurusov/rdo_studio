/*
 * copyright: (c) RDO-Team, 2011
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
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pArrayType)
	: m_pArrayType(pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insertItem(CREF(RDOValue) value)
{
	m_pArrayType->getItemType()->type()->type_cast(value.typeInfo()->type(), value.src_info(), m_pArrayType->src_info(), value.src_info());
	RDOValue ItemValue = m_pArrayType->getItemType()->type()->value_cast(value, m_pArrayType->src_info(), value.src_info());
	m_container.push_back(ItemValue);
}

CREF(LPRDOArrayType) RDOArrayValue::getArrayType() const
{
	return m_pArrayType;
}

REF(LPRDOArrayType) RDOArrayValue::getArrayType()
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

tstring RDOArrayValue::getAsString() const
{
	tstring arrayValue;
	STL_FOR_ALL_CONST(m_container, it)
	{
		if (it == m_container.begin())
		{
			arrayValue = rdo::format(_T("[%s"), it->value().getAsString().c_str());
		}
		else
		{
			arrayValue = rdo::format(_T("%s, %s"), arrayValue.c_str(), it->value().getAsString().c_str());
		}
	}
	return rdo::format(_T("%s]"), arrayValue.c_str());
}

CREF(RDOArrayValue::Container) RDOArrayValue::getContainer() const
{
	return m_container;
}

CLOSE_RDO_PARSER_NAMESPACE
