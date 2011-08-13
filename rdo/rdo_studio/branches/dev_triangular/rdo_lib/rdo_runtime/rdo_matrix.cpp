/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      rdo_matrix.cpp
 * @authors   Чирков Михаил
 * @date      01.10.2010
 * @brief     Матрицы
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_matrix.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOMatrixValue
// ********************************************************************************
RDOMatrixValue::RDOMatrixValue(CREF(LPRDOMatrixType) pType)
	: m_pMatrixType(pType)
{}

RDOMatrixValue::RDOMatrixValue(CREF(RDOMatrixValue) value)
	: m_container  (value.m_container  )
	, m_pMatrixType(value.m_pMatrixType)
{}

RDOMatrixValue::~RDOMatrixValue()
{}

CREF(LPRDOMatrixType) RDOMatrixValue::type() const
{
	ASSERT(m_pMatrixType);
	return m_pMatrixType;
}

void RDOMatrixValue::insertItem(CREF(RDOValue) pMatrix)
{
	m_container.push_back(pMatrix);
}

RDOMatrixValue::Container::iterator RDOMatrixValue::containerBegin()
{
	return m_container.begin();
}

RDOMatrixValue::Container::iterator RDOMatrixValue::containerEnd()
{
	return m_container.end();
}

void RDOMatrixValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

void RDOMatrixValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

tstring RDOMatrixValue::getAsString() const
{
	tstring MatrixName = _T("[");
	STL_FOR_ALL_CONST(m_container, it)
	{
		MatrixName += it->getAsString();
		if(it != --m_container.end())
			MatrixName += _T(", ");
	}
	return MatrixName += _T("]");
}

CREF(RDOValue) RDOMatrixValue::operator[] (CREF(RDOValue) ind)
{
	return m_container[ind.getInt()];
}

// ********************************************************************************
// ******************** RDOMatrixIterator
// ********************************************************************************
RDOMatrixIterator::RDOMatrixIterator(CREF(RDOMatrixIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

RDOMatrixIterator::Iterator RDOMatrixIterator::getIterator() const
{
	return m_iterator;
}

RDOMatrixIterator::Iterator RDOMatrixIterator::operator+ (rsint num)
{
	return m_iterator + num;
}

RDOMatrixIterator::Iterator RDOMatrixIterator::operator- (rsint num)
{
	return m_iterator - num;
}

RDOValue RDOMatrixIterator::getValue() const
{
	return *m_iterator;
}

rbool RDOMatrixIterator::operator== (CREF(RDOMatrixIterator) iterator) const
{
	return m_iterator == iterator.m_iterator;
}

// ********************************************************************************
// ******************** RDOMatrixType
// ********************************************************************************
RDOMatrixType::RDOMatrixType(CREF(LPMatrixType) pMatrixType)
	: RDOType      (RDOType::t_matrix)
	, m_pMatrixType(pMatrixType      )
{}

RDOMatrixType::LPMatrixType RDOMatrixType::getMatrixType() const
{
	return m_pMatrixType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
