// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/advance.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance(LPIPROCProcess process, const LPRDOCalc& _pDelayCalc)
    : RDOPROCBlock(process    )
    , pDelayCalc  (_pDelayCalc)
{}

RDOPROCAdvance::LeaveTr::LeaveTr(const LPRDOPROCTransact& _transact, double _timeLeave)
    : transact (_transact )
    , timeLeave(_timeLeave)
{}

bool RDOPROCAdvance::onCheckCondition(const LPRDORuntime& pRuntime)
{
    if (!m_transacts.empty())
    {
        return true;
    } 
    else
    {
        if (!leave_list.empty())
        {
            double tnow = pRuntime->getCurrentTime();
            std::list< LeaveTr >::iterator it = leave_list.begin();
            while (it != leave_list.end())
            {
                if (tnow >= it->timeLeave)
                {
                    return true;
                }
                ++it;
            }
        }
    }
    return false;
}

IBaseOperation::ResultCode RDOPROCAdvance::onDoOperation(const LPRDORuntime& pRuntime)
{
    if (m_transacts.empty())
        return IBaseOperation::ResultCode::CANNOT_RUN;

    //        TRACE1("%7.1f ADVANCE BEGIN\n", pRuntime->getCurrentTime());
    double timeLeave = pDelayCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime();
    leave_list.push_back(LeaveTr(m_transacts.front(), timeLeave));
    m_transacts.erase(m_transacts.begin());

    pRuntime->addTimePoint(
        timeLeave,
        this,
        boost::bind(&RDOPROCAdvance::onMakePlaned, this, pRuntime)
    );

    if (m_pStatistics)
        m_pStatistics->setTransCount(m_transacts.size());

    return IBaseOperation::ResultCode::DONE;
}

void RDOPROCAdvance::onStart(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCAdvance::onStop(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCAdvance::onMakePlaned(const LPRDORuntime& pRuntime)
{
    if (leave_list.empty())
        return;

    double tnow = pRuntime->getCurrentTime();
    std::list<LeaveTr>::iterator it = leave_list.begin();
    while (it != leave_list.end())
    {
        if (tnow >= it->timeLeave)
        {
            // TRACE1("%7.1f ADVANCE END\n", it->timeLeave);
            it->transact->next();
            leave_list.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

IBaseOperation::ResultCode RDOPROCAdvance::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

void RDOPROCAdvance::setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics)
{
    m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
