/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotraceroperation.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerOperationBase
// --------------------------------------------------------------------------------
TracerOperationBase::TracerOperationBase(CREF(LPTracerPattern) pPattern, CREF(QString) name)
	: TracerSerie(SK_OPERATION)
	, m_pPattern(pPattern)
	, m_name(name)
{
	setTitle(m_name);
}

TracerOperationBase::~TracerOperationBase()
{}

CREF(QString) TracerOperationBase::getName() const
{
	return m_name;
}

LPTracerPattern TracerOperationBase::getPattern() const
{
	return m_pPattern;
}

void TracerOperationBase::incOperationsCount(TracerTimeNow* const pTime, const int eventIndex)
{
	TracerValue* pNewValue = new TracerValue(pTime, eventIndex);
	TracerValue* pPrevValue;
	getLastValue(pPrevValue);
	pNewValue->value = pPrevValue ? pPrevValue->value + 1 : 1;
	addValue(pNewValue);
}

void TracerOperationBase::getCaptions(std::vector<tstring> &captions, const int valueCount) const
{
	TracerSerie::getCaptionsInt(captions, valueCount);
}

void TracerOperationBase::monitorTime(TracerTimeNow* const, const int)
{}

// --------------------------------------------------------------------------------
// -------------------- TracerOperation
// --------------------------------------------------------------------------------
TracerOperation::TracerOperation(CREF(LPTracerPattern) pPattern, CREF(QString) name)
	: TracerOperationBase(pPattern, name)
{}

TracerOperation::~TracerOperation()
{}

void TracerOperation::start(TracerTimeNow* const pTime, const int eventIndex)
{
	TracerOperationBase::incOperationsCount(pTime, eventIndex);
}

void TracerOperation::accomplish(TracerTimeNow* const pTime, const int eventIndex)
{
	TracerValue* lastval;
	getLastValue(lastval);
	if (lastval)
	{
		TracerValue* newval = new TracerValue(pTime, eventIndex);
		newval->value = lastval->value - 1;
		addValue(newval);
	}
}

// --------------------------------------------------------------------------------
// -------------------- TracerIrregularEvent
// --------------------------------------------------------------------------------
TracerEvent::TracerEvent(CREF(LPTracerPattern) pPattern, CREF(QString) name)
	: TracerOperationBase(pPattern, name)
{}

TracerEvent::~TracerEvent()
{}

void TracerEvent::occurs(TracerTimeNow* const pTime, const int eventIndex)
{
	TracerOperationBase::incOperationsCount(pTime, eventIndex);
}

void TracerEvent::monitorTime(TracerTimeNow* const pTime, const int eventIndex)
{
	TracerValue* prevval;
	getLastValue(prevval);
	TracerValue* newval = NULL;
	if (prevval && prevval->value != 0)
	{
		if (*prevval->getModelTime() != *pTime)
		{
			newval = new TracerValue(prevval->getModelTime(), prevval->getModelTime()->eventCount);
			newval->value = 0;
		}
	}
	if (!prevval)
	{
		newval = new TracerValue(pTime, eventIndex);
		newval->value = 0;
	}
	if (newval)
	{
		addValue(newval);
	}
}