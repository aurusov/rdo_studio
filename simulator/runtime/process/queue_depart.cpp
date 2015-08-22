// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/queue_depart.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par)
    : RDOPROCBlock(process )
    , fromParser  (From_Par)
{}

void RDOPROCBlockForQueue::_onStart(const LPRDORuntime& pRuntime)
{
    int Id_res = fromParser.Id_res;
    int Id_param = fromParser.Id_param;
    LPRDOResource res = pRuntime->getResourceByID(Id_res);
    forRes.Id_param = Id_param;
    forRes.rss = res.object_static_cast<RDOPROCResource>();
    forRes.defaultValue = RDOValue(RDOPROCQueue::getDefaultValue());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par)
    : RDOPROCBlockForQueue(process, From_Par)
{}

std::size_t RDOPROCQueue::getDefaultValue()
{
    return 0;
}

std::string RDOPROCQueue::getQueueParamName()
{
    return "длина_очереди";
}

void RDOPROCQueue::onStart(const LPRDORuntime& pRuntime)
{
    _onStart(pRuntime);
}

bool RDOPROCQueue::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
    if (!m_transacts.empty())
    {
        RDOValue i = forRes.rss->getParam(forRes.Id_param);
        RDOValue j = RDOValue(int (1));
        forRes.rss->setParam(forRes.Id_param, i + j);
        return true;
    }
    else
    {
        return false;
    }
}

IBaseOperation::ResultCode RDOPROCQueue::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
    m_transacts.front()->next();
    return IBaseOperation::ResultCode::DONE;
}

void RDOPROCQueue::onStop (const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::ResultCode RDOPROCQueue::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
RDOPROCDepart::RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par)
    : RDOPROCBlockForQueue(process, From_Par)
{}

std::size_t RDOPROCDepart::getDefaultValue()
{
    return 0;
}

std::string RDOPROCDepart::getDepartParamName()
{
    return "длина_очереди";
}

void RDOPROCDepart::onStart(const LPRDORuntime& pRuntime)
{
    _onStart(pRuntime);
}

bool RDOPROCDepart::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
    if (!m_transacts.empty())
    {
        RDOValue i = forRes.rss->getParam(forRes.Id_param);
        RDOValue j = RDOValue(int (1));
        forRes.rss->setParam(forRes.Id_param, i - j);
        return true;
    }
    else
    {
        return false;
    }
}

IBaseOperation::ResultCode RDOPROCDepart::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
    m_transacts.front()->next();
    return IBaseOperation::ResultCode::DONE;
}

void RDOPROCDepart::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::ResultCode RDOPROCDepart::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

CLOSE_RDO_RUNTIME_NAMESPACE
