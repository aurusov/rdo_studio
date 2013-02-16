/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_array.cpp
  \author    Чирков Михаил
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include <boost/foreach.hpp>
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

//----------------------------------------------------------------------------
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pArrayType)
	: m_pArrayType(pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insertItem(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);

	m_pArrayType->getItemType()->type()->type_cast(pValue->typeInfo()->type(), pValue->src_info(), m_pArrayType->src_info(), pValue->src_info());
	LPRDOValue pItemValue = m_pArrayType->getItemType()->type()->value_cast(pValue, m_pArrayType->src_info(), pValue->src_info());
	ASSERT(pItemValue);
	m_container.push_back(pItemValue);
}

CREF(LPRDOArrayType) RDOArrayValue::getArrayType() const
{
	return m_pArrayType;
}

REF(LPRDOArrayType) RDOArrayValue::getArrayType()
{
	return m_pArrayType;
}

rdo::runtime::RDOValue RDOArrayValue::getRArray() const
{
	return rdo::runtime::RDOValue(m_pArrayType->getRuntimeArrayType(), createRuntimeValue());
}

rdo::runtime::LPRDOArrayValue RDOArrayValue::createRuntimeValue() const
{
	rdo::runtime::LPRDOArrayValue pArrayValue = rdo::Factory<rdo::runtime::RDOArrayValue>::create(m_pArrayType->getRuntimeArrayType());
	ASSERT(pArrayValue);
	STL_FOR_ALL_CONST(m_container, it)
	{
		pArrayValue->push_back((*it)->value());
	}
	return pArrayValue;
}

tstring RDOArrayValue::getAsString() const
{
	tstring arrayValue;
	STL_FOR_ALL_CONST(m_container, it)
	{
		if (it == m_container.begin())
		{
			arrayValue = rdo::format(_T("[%s"), (*it)->value().getAsString().c_str());
		}
		else
		{
			arrayValue = rdo::format(_T("%s, %s"), arrayValue.c_str(), (*it)->value().getAsString().c_str());
		}
	}
	return rdo::format(_T("%s]"), arrayValue.c_str());
}

CREF(RDOArrayValue::Container) RDOArrayValue::getContainer() const
{
	return m_container;
}

void RDOArrayValue::serializeInDB(REF(IDB) db) const
{
	db.insertRow("array_rv","DEFAULT");
	int array_id = db.queryExecIndex("array_rv");

	BOOST_FOREACH(LPRDOValue arrayItem, getContainer())
	{
		arrayItem->serializeInDB(db);
		db.insertRow("array_value",QString("%1,DEFAULT,%2")
			.arg(array_id)
			.arg(boost::any_cast<int>(db.popContext())));
	}

}

CLOSE_RDO_PARSER_NAMESPACE
