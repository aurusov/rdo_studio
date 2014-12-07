// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

//----------------------------------------------------------------------------
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(const LPRDOArrayType& pArrayType)
	: m_pArrayType(pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insertItem(const LPRDOValue& pValue)
{
	ASSERT(pValue);

	m_pArrayType->getItemType()->itype()->type_cast(pValue->typeInfo()->itype(), pValue->src_info(), m_pArrayType->src_info(), pValue->src_info());
	LPRDOValue pItemValue = m_pArrayType->getItemType()->itype()->value_cast(pValue, m_pArrayType->src_info(), pValue->src_info());
	ASSERT(pItemValue);
	m_container.push_back(pItemValue);
}

const LPRDOArrayType& RDOArrayValue::getArrayType() const
{
	return m_pArrayType;
}

LPRDOArrayType& RDOArrayValue::getArrayType()
{
	return m_pArrayType;
}

rdo::runtime::RDOValue RDOArrayValue::getRArray() const
{
	return rdo::runtime::RDOValue(m_pArrayType, createRuntimeValue());
}

rdo::runtime::LPRDOArrayValue RDOArrayValue::createRuntimeValue() const
{
	rdo::runtime::LPRDOArrayValue pArrayValue = rdo::Factory<rdo::runtime::RDOArrayValue>::create(
			m_pArrayType.object_dynamic_cast<rdo::runtime::RDOArrayType>());
	ASSERT(pArrayValue);
	for (const auto& item: m_container)
	{
		pArrayValue->push_back(item->value());
	}
	return pArrayValue;
}

std::string RDOArrayValue::getAsString() const
{
	std::string arrayValue;

	for (Container::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
	{
		if (it == m_container.begin())
		{
			arrayValue = rdo::format("[%s", (*it)->value().getAsString().c_str());
		}
		else
		{
			arrayValue = rdo::format("%s, %s", arrayValue.c_str(), (*it)->value().getAsString().c_str());
		}
	}
	return rdo::format("%s]", arrayValue.c_str());
}

const RDOArrayValue::Container& RDOArrayValue::getContainer() const
{
	return m_container;
}

CLOSE_RDO_PARSER_NAMESPACE
