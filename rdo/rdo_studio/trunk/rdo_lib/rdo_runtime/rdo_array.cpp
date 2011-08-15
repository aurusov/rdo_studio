/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      rdo_array.cpp
 * @author    Чирков Михаил
 * @date      14.12.2009
 * @brief     Массив
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_array.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOArrayValue
// ********************************************************************************
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pType)
	: m_pArrayType(pType)
{}

RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_container (value.m_container )
	, m_pArrayType(value.m_pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

CREF(LPRDOArrayType) RDOArrayValue::type() const
{
	ASSERT(m_pArrayType);
	return m_pArrayType;
}

void RDOArrayValue::insertItem(CREF(RDOValue) pArray)
{
	m_container.push_back(pArray);
}

RDOArrayValue::Container::iterator RDOArrayValue::containerBegin()
{
	return m_container.begin();
}

RDOArrayValue::Container::iterator RDOArrayValue::containerEnd()
{
	return m_container.end();
}

void RDOArrayValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

void RDOArrayValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

tstring RDOArrayValue::getAsString() const
{
	tstring arrayValue;
	STL_FOR_ALL_CONST(m_container, it)
	{
		if (it == m_container.begin())
		{
			arrayValue = rdo::format(_T("[%s"), it->getAsString().c_str());
		}
		else
		{
			arrayValue = rdo::format(_T("%s, %s"), arrayValue.c_str(), it->getAsString().c_str());
		}
	}
	return rdo::format(_T("%s]"), arrayValue.c_str());
}

CREF(RDOValue) RDOArrayValue::operator[] (CREF(RDOValue) ind) const
{
	return m_container[ind.getInt()];
}

ruint RDOArrayValue::arraySize() const
{
	return m_container.size();
}

void RDOArrayValue::setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item)
{
	m_container[ind.getInt()] = item;
}

// ********************************************************************************
// ******************** RDOArrayIterator
// ********************************************************************************
RDOArrayIterator::RDOArrayIterator(CREF(RDOArrayIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

RDOArrayIterator::Iterator RDOArrayIterator::operator+ (rsint num)
{
	return m_iterator + num;
}

RDOArrayIterator::Iterator RDOArrayIterator::operator- (rsint num)
{
	return m_iterator - num;
}

RDOValue RDOArrayIterator::getValue() const
{
	return *m_iterator;
}

rbool RDOArrayIterator::operator== (CREF(RDOArrayIterator) iterator) const
{
	return m_iterator == iterator.m_iterator;
}

// ********************************************************************************
// ******************** RDOArrayType
// ********************************************************************************
RDOArrayType::RDOArrayType(CREF(LPItemType) pItemType)
	: RDOType    (RDOType::t_array)
	, m_pItemType(pItemType       )
{}

RDOArrayType::LPItemType RDOArrayType::getItemType() const
{
	return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
