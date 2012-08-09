/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_matrix.cpp
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.10.2010
  \brief     Матрица
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_matrix.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixValue
// --------------------------------------------------------------------------------
RDOMatrixValue::RDOMatrixValue(CREF(LPRDOMatrixType) pType)
	: m_pMatrixType(pType)
{}

RDOMatrixValue::RDOMatrixValue(CREF(LPRDOMatrixValue) pValue)
	: m_container  (pValue->m_container  )
	, m_pMatrixType(pValue->m_pMatrixType)
{}

RDOMatrixValue::~RDOMatrixValue()
{}

CREF(LPRDOMatrixType) RDOMatrixValue::type() const
{
	ASSERT(m_pMatrixType);
	return m_pMatrixType;
}

void RDOMatrixValue::push_back(CREF(RDOValue) item)
{
	m_container.push_back(item);
}

LPRDOMatrixIterator RDOMatrixValue::begin()
{
	return rdo::Factory<RDOMatrixIterator>::create(m_container.begin());
}

LPRDOMatrixIterator RDOMatrixValue::end()
{
	return rdo::Factory<RDOMatrixIterator>::create(m_container.end());
}

void RDOMatrixValue::insert(CREF(LPRDOMatrixIterator) pWhere, CREF(LPRDOMatrixIterator) pFromFirst, CREF(LPRDOMatrixIterator) pFromLast)
{
	ASSERT(pWhere    );
	ASSERT(pFromFirst);
	ASSERT(pFromLast );

	m_container.insert(pWhere->getIterator(), pFromFirst->getIterator(), pFromLast->getIterator());
}

void RDOMatrixValue::erase(CREF(LPRDOMatrixIterator) pFirst, CREF(LPRDOMatrixIterator) pLast)
{
	ASSERT(pFirst)
	ASSERT(pLast );

	m_container.erase(pFirst->getIterator(), pLast->getIterator());
}

tstring RDOMatrixValue::getAsString() const
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

ruint RDOMatrixValue::size() const
{
	return m_container.size();
}

CREF(RDOValue) RDOMatrixValue::getItem(CREF(RDOValue) index) const
{
	ruint ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException(_T("Выход за пределы матрицы"));
	}
	return m_container[ind];
}

void RDOMatrixValue::setItem(CREF(RDOValue) index, CREF(RDOValue) item)
{
	ruint ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException(_T("Выход за пределы матрицы"));
	}
	m_container[ind] = item;
}

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixIterator
// --------------------------------------------------------------------------------
RDOMatrixIterator::RDOMatrixIterator(CREF(LPRDOMatrixIterator) pIterator)
	: RDOType   (RDOType::t_pointer   )
	, m_iterator(pIterator->m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(CREF(RDOMatrixIterator) iterator)
	: RDOType   (RDOType::t_pointer )
	, m_iterator(iterator.m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(CREF(Iterator) iterator)
	: RDOType   (RDOType::t_pointer)
	, m_iterator(iterator          )
{}

RDOMatrixIterator::~RDOMatrixIterator()
{}

RDOMatrixIterator::Iterator RDOMatrixIterator::getIterator() const
{
	return m_iterator;
}

CREF(RDOValue) RDOMatrixIterator::getValue() const
{
	return *m_iterator;
}

LPRDOMatrixIterator RDOMatrixIterator::preInc(rsint delta)
{
	m_iterator += delta;
	return LPRDOMatrixIterator(this);
}

LPRDOMatrixIterator RDOMatrixIterator::postInc(rsint delta)
{
	LPRDOMatrixIterator pPrev = rdo::Factory<RDOMatrixIterator>::create(m_iterator);
	ASSERT(pPrev);
	m_iterator += delta;
	return pPrev;
}

LPRDOMatrixIterator RDOMatrixIterator::next()
{
	return preInc(1);
}

rbool RDOMatrixIterator::equal(CREF(LPRDOMatrixIterator) pIterator) const
{
	ASSERT(pIterator);
	return m_iterator == pIterator->m_iterator;
}

LPRDOMatrixIterator RDOMatrixIterator::clone() const
{
	return rdo::Factory<RDOMatrixIterator>::create(*this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixType
// --------------------------------------------------------------------------------
RDOMatrixType::RDOMatrixType(CREF(LPItemType) pItemType)
	: RDOType    (RDOType::t_pointer)
	, m_pItemType(pItemType         )
{}

RDOMatrixType::~RDOMatrixType()
{}

CREF(RDOMatrixType::LPItemType) RDOMatrixType::getItemType() const
{
	return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
