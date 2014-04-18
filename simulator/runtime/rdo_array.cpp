/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_array.cpp
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2009
  \brief     Массив
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayValue
// --------------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(const LPRDOArrayType& pType)
	: m_pArrayType(pType)
{}

RDOArrayValue::RDOArrayValue(const LPRDOArrayValue& pValue)
	: m_container (pValue->m_container )
	, m_pArrayType(pValue->m_pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

const LPRDOArrayType& RDOArrayValue::type() const
{
	ASSERT(m_pArrayType);
	return m_pArrayType;
}

void RDOArrayValue::push_back(const RDOValue& item)
{
	m_container.push_back(item);
}

LPRDOArrayIterator RDOArrayValue::begin()
{
	return rdo::Factory<RDOArrayIterator>::create(m_container.begin());
}

LPRDOArrayIterator RDOArrayValue::end()
{
	return rdo::Factory<RDOArrayIterator>::create(m_container.end());
}

void RDOArrayValue::insert(const LPRDOArrayIterator& pWhere, const LPRDOArrayIterator& pFromFirst, const LPRDOArrayIterator& pFromLast)
{
	ASSERT(pWhere    );
	ASSERT(pFromFirst);
	ASSERT(pFromLast );

	m_container.insert(pWhere->getIterator(), pFromFirst->getIterator(), pFromLast->getIterator());
}

void RDOArrayValue::erase(const LPRDOArrayIterator& pFirst, const LPRDOArrayIterator& pLast)
{
	ASSERT(pFirst)
	ASSERT(pLast );

	m_container.erase(pFirst->getIterator(), pLast->getIterator());
}

std::string RDOArrayValue::getAsString() const
{
	std::string result("[");
	for (Container::const_iterator item = m_container.begin(); item != m_container.end(); ++item)
	{
		if (item == m_container.begin())
		{
			result = rdo::format("%s%s", result.c_str(), item->getAsString().c_str());
		}
		else
		{
			result = rdo::format("%s, %s", result.c_str(), item->getAsString().c_str());
		}
	}
	return rdo::format("%s]", result.c_str());
}

std::size_t RDOArrayValue::size() const
{
	return m_container.size();
}

const RDOValue& RDOArrayValue::getItem(const RDOValue& index) const
{
	const std::size_t ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException("Выход за пределы массива");
	}
	return m_container[ind];
}

void RDOArrayValue::setItem(const RDOValue& index, const RDOValue& item)
{
	const std::size_t ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException("Выход за пределы массива");
	}
	m_container[ind] = item;
}

LPRDOArrayValue RDOArrayValue::clone() const
{
	LPRDOArrayValue pClone = rdo::Factory<RDOArrayValue>::create(type());
	ASSERT(pClone);

	BOOST_FOREACH(const RDOValue& value, m_container)
	{
		pClone->push_back(value.clone());
	}

	return pClone;
}

// --------------------------------------------------------------------------------
// -------------------- RDOArrayIterator
// --------------------------------------------------------------------------------
RDOArrayIterator::RDOArrayIterator(const LPRDOArrayIterator& pIterator)
	: RDOType   (RDOType::t_pointer   )
	, m_iterator(pIterator->m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(const RDOArrayIterator& iterator)
	: RDOType   (RDOType::t_pointer )
	, m_iterator(iterator.m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(const Iterator& iterator)
	: RDOType   (RDOType::t_pointer)
	, m_iterator(iterator          )
{}

RDOArrayIterator::~RDOArrayIterator()
{}

RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

const RDOValue& RDOArrayIterator::getValue() const
{
	return *m_iterator;
}

LPRDOArrayIterator RDOArrayIterator::preInc(int delta)
{
	m_iterator += delta;
	return LPRDOArrayIterator(this);
}

LPRDOArrayIterator RDOArrayIterator::postInc(int delta)
{
	LPRDOArrayIterator pPrev = rdo::Factory<RDOArrayIterator>::create(m_iterator);
	ASSERT(pPrev);
	m_iterator += delta;
	return pPrev;
}

LPRDOArrayIterator RDOArrayIterator::next()
{
	return preInc(1);
}

bool RDOArrayIterator::equal(const LPRDOArrayIterator& pIterator) const
{
	ASSERT(pIterator);
	return m_iterator == pIterator->m_iterator;
}

LPRDOArrayIterator RDOArrayIterator::clone() const
{
	return rdo::Factory<RDOArrayIterator>::create(*this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOArrayType
// --------------------------------------------------------------------------------
RDOArrayType::RDOArrayType(const LPItemType& pItemType)
	: RDOType    (RDOType::t_pointer)
	, m_pItemType(pItemType         )
{}

RDOArrayType::~RDOArrayType()
{}

const RDOArrayType::LPItemType& RDOArrayType::getItemType() const
{
	return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
