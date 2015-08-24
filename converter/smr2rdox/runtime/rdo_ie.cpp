// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/runtime/rdo_ie.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent(bool trace)
    : RDOPattern(trace)
    , m_timeCalc(NULL )
{}

double RDOPatternIrregEvent::getNextTimeInterval(const LPRDORuntime& pRuntime)
{
    double time_next = m_timeCalc->calcValue(pRuntime).getDouble();
    if (time_next >= 0)
    {
        return time_next;
    }
    pRuntime->error().push(rdo::format("Попытка запланировать событие в прошлом. Выражение времени для $Time имеет отрицательное значение: %f", time_next), m_timeCalc->srcInfo());
    return 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOIrregEvent
// --------------------------------------------------------------------------------
RDOIrregEvent::RDOIrregEvent(RDOPatternIrregEvent* pPattern, bool trace, const std::string& name)
    : RDOActivityPattern<RDOPatternIrregEvent>(pPattern, trace, name)
{
    setTrace(trace);
    setTraceID(std::size_t(~0));
}

void RDOIrregEvent::onStart(const LPRDORuntime& pRuntime)
{
    onBeforeIrregularEvent(pRuntime);
}

void RDOIrregEvent::onStop(const LPRDORuntime& pRuntime)
{
    pRuntime->removeTimePoint(this);
}

bool RDOIrregEvent::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
    return false;
}

IBaseOperation::ResultCode RDOIrregEvent::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

void RDOIrregEvent::convertEvent(const LPRDORuntime& /*pRuntime*/) 
{ 
    NEVER_REACH_HERE;
}

void RDOIrregEvent::onBeforeIrregularEvent(const LPRDORuntime& /*pRuntime*/)
{
    NEVER_REACH_HERE;
}

void RDOIrregEvent::onAfterIrregularEvent(const LPRDORuntime& /*pRuntime*/)
{
    NEVER_REACH_HERE;
}

double RDOIrregEvent::getNextTimeInterval(const LPRDORuntime& pRuntime) 
{ 
    pRuntime->setCurrentActivity(this);
    return m_pPattern->getNextTimeInterval(pRuntime); 
}

IBaseOperation::ResultCode RDOIrregEvent::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

CLOSE_RDO_RUNTIME_NAMESPACE
