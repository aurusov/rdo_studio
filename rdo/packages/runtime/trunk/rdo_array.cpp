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
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayValue
// --------------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pType)
	: m_pArrayType(pType)
{}

RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayValue) pValue)
	: m_container (pValue->m_container )
	, m_pArrayType(pValue->m_pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

CREF(LPRDOArrayType) RDOArrayValue::type() const
{
	ASSERT(m_pArrayType);
	return m_pArrayType;
}

void RDOArrayValue::push_back(CREF(RDOValue) item)
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

void RDOArrayValue::insert(CREF(LPRDOArrayIterator) pWhere, CREF(LPRDOArrayIterator) pFromFirst, CREF(LPRDOArrayIterator) pFromLast)
{
	ASSERT(pWhere    );
	ASSERT(pFromFirst);
	ASSERT(pFromLast );

	m_container.insert(pWhere->getIterator(), pFromFirst->getIterator(), pFromLast->getIterator());
}

void RDOArrayValue::erase(CREF(LPRDOArrayIterator) pFirst, CREF(LPRDOArrayIterator) pLast)
{
	ASSERT(pFirst)
	ASSERT(pLast );

	m_container.erase(pFirst->getIterator(), pLast->getIterator());
}

tstring RDOArrayValue::getAsString() const
{
	tstring result(_T("["));
	STL_FOR_ALL_CONST(m_container, it)
	{
		if (it == m_container.begin())
		{
			result = rdo::format(_T("%s%s"), result.c_str(), it->getAsString().c_str());
		}
		else
		{
			result = rdo::format(_T("%s, %s"), result.c_str(), it->getAsString().c_str());
		}
	}
	return rdo::format(_T("%s]"), result.c_str());
}

ruint RDOArrayValue::size() const
{
	return m_container.size();
}

CREF(RDOValue) RDOArrayValue::getItem(CREF(RDOValue) index) const
{
	ruint ind = index.getUInt();
	ASSERT(m_container.size() < ind);
	return m_container[ind];
}

void RDOArrayValue::setItem(CREF(RDOValue) index, CREF(RDOValue) item)
{
	ruint ind = index.getUInt();
	ASSERT(m_container.size() < ind);
	m_container[ind] = item;
}

// --------------------------------------------------------------------------------
// -------------------- RDOArrayIterator
// --------------------------------------------------------------------------------
RDOArrayIterator::RDOArrayIterator(CREF(LPRDOArrayIterator) pIterator)
	: RDOType   (RDOType::t_pointer   )
	, m_iterator(pIterator->m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(RDOArrayIterator) iterator)
	: RDOType   (RDOType::t_pointer )
	, m_iterator(iterator.m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(Iterator) iterator)
	: RDOType   (RDOType::t_pointer)
	, m_iterator(iterator          )
{}

RDOArrayIterator::~RDOArrayIterator()
{}

RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

CREF(RDOValue) RDOArrayIterator::getValue() const
{
	return *m_iterator;
}

LPRDOArrayIterator RDOArrayIterator::preInc(rsint delta)
{
	m_iterator += delta;
	return LPRDOArrayIterator(this);
}

LPRDOArrayIterator RDOArrayIterator::postInc(rsint delta)
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

rbool RDOArrayIterator::equal(CREF(LPRDOArrayIterator) pIterator) const
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
RDOArrayType::RDOArrayType(CREF(LPItemType) pItemType)
	: RDOType    (RDOType::t_pointer)
	, m_pItemType(pItemType         )
{}

RDOArrayType::~RDOArrayType()
{}

CREF(RDOArrayType::LPItemType) RDOArrayType::getItemType() const
{
	return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
