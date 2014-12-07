// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_event.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEvent
// --------------------------------------------------------------------------------
RDOEvent::RDOEvent(const LPRDORuntime& pRuntime, const LPRDOPatternEvent& pPattern, bool trace, const std::string& name)
    : RDOActivityPattern<RDOPatternEvent>(pPattern, trace, name)
{
    setTrace  (trace);
    setTraceID(pRuntime->getFreeEventId());
}

void RDOEvent::onStart(const LPRDORuntime& /*pRuntime*/)
{}

void RDOEvent::onStop(const LPRDORuntime& /*pRuntime*/)
{}

bool RDOEvent::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
    return false;
}

IBaseOperation::BOResult RDOEvent::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::BOR_cant_run;
}

void RDOEvent::onMakePlaned(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params)
{
    pRuntime->inc_cnt_events();
    onBeforeEvent(pRuntime, params);
    convertEvent (pRuntime);
    onAfterEvent (pRuntime);
}

void RDOEvent::convertEvent(const LPRDORuntime& pRuntime)
{
    m_pPattern->convertEvent(pRuntime);
}

void RDOEvent::onBeforeEvent(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params)
{
    pRuntime->setCurrentActivity(this);
    setPatternParameters(pRuntime, params);
}

void RDOEvent::onAfterEvent(const LPRDORuntime& pRuntime)
{
    updateConvertStatus(pRuntime, m_pPattern->m_convertorStatus);
    pRuntime->getTracer()->writeEvent(this, pRuntime);
    m_pPattern->convertErase(pRuntime);
    updateRelRes(pRuntime);
}

IBaseOperation::BOResult RDOEvent::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
