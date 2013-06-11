/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_operation.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_operation.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- OperationBase
// --------------------------------------------------------------------------------
OperationBase::OperationBase(CREF(LPPattern) pPattern, CREF(QString) name)
	: Serie(SK_OPERATION)
	, m_pPattern(pPattern)
	, m_name(name)
{
	setTitle(m_name);
}

OperationBase::~OperationBase()
{}

CREF(QString) OperationBase::getName() const
{
	return m_name;
}

LPPattern OperationBase::getPattern() const
{
	return m_pPattern;
}

void OperationBase::incOperationsCount(Time* const pTime, const int eventIndex)
{
	Value* pPrevValue;
	getLastValue(pPrevValue);

	Value* pNewValue = new Value(pTime, eventIndex, pPrevValue ? pPrevValue->getValue() + 1 : 1);
	addValue(pNewValue);
}

void OperationBase::getCaptions(std::vector<tstring> &captions, const int valueCount) const
{
	Serie::getCaptionsInt(captions, valueCount);
}

void OperationBase::monitorTime(Time* const, const int)
{}

// --------------------------------------------------------------------------------
// -------------------- Operation
// --------------------------------------------------------------------------------
Operation::Operation(CREF(LPPattern) pPattern, CREF(QString) name)
	: OperationBase(pPattern, name)
{}

Operation::~Operation()
{}

void Operation::start(Time* const pTime, const int eventIndex)
{
	OperationBase::incOperationsCount(pTime, eventIndex);
}

void Operation::accomplish(Time* const pTime, const int eventIndex)
{
	Value* pLastValue;
	getLastValue(pLastValue);
	if (pLastValue)
	{
		Value* pNewValue = new Value(pTime, eventIndex, pLastValue->getValue() - 1);
		addValue(pNewValue);
	}
}

// --------------------------------------------------------------------------------
// -------------------- Event
// --------------------------------------------------------------------------------
Event::Event(CREF(LPPattern) pPattern, CREF(QString) name)
	: OperationBase(pPattern, name)
{}

Event::~Event()
{}

void Event::occurs(Time* const pTime, const int eventIndex)
{
	OperationBase::incOperationsCount(pTime, eventIndex);
}

void Event::monitorTime(Time* const pTime, const int eventIndex)
{
	Value* pPrevValue;
	getLastValue(pPrevValue);
	Value* pNewValue = NULL;
	if (pPrevValue && pPrevValue->getValue() != 0)
	{
		if (*pPrevValue->getModelTime() != *pTime)
		{
			pNewValue = new Value(pPrevValue->getModelTime(), pPrevValue->getModelTime()->eventCount, 0);
		}
	}
	if (!pPrevValue)
	{
		pNewValue = new Value(pTime, eventIndex, 0);
	}
	if (pNewValue)
	{
		addValue(pNewValue);
	}
}
