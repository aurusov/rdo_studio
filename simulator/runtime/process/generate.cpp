// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/generate.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate(LPIPROCProcess process, const LPRDOCalc& pTime, const LPRDOCalc& pCreateAndGoOnTransactCalc, boost::optional<std::size_t> maxCreateTransactCount)
    : RDOPROCBlock                (process                   )
    , timeNext                    (0.0                       )
    , m_pTimeCalc                 (pTime                     )
    , m_pCreateAndGoOnTransactCalc(pCreateAndGoOnTransactCalc)
    , m_maxCreateTransactCount    (maxCreateTransactCount    )
    , m_createdTransactCount      (0                         )
{}

void RDOPROCGenerate::onStart(const LPRDORuntime& pRuntime)
{
    calcNextTimeInterval(pRuntime);
}

bool RDOPROCGenerate::onCheckCondition(const LPRDORuntime& pRuntime)
{
    if (m_maxCreateTransactCount && m_createdTransactCount >= m_maxCreateTransactCount.get())
    {
        return false;
    }

    return pRuntime->getCurrentTime() >= timeNext ? true : false;
}

IBaseOperation::ResultCode RDOPROCGenerate::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::DONE;
}

void RDOPROCGenerate::calcNextTimeInterval(const LPRDORuntime& pRuntime)
{
    timeNext = m_pTimeCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime();
    pRuntime->addTimePoint(
        timeNext,
        this,
        boost::bind(&RDOPROCGenerate::onMakePlaned, this, pRuntime)
    );
}

void RDOPROCGenerate::onStop(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCGenerate::onMakePlaned(const LPRDORuntime& pRuntime)
{
    ++m_createdTransactCount;

    if (m_pStatistics)
        m_pStatistics->setTransCount(m_createdTransactCount);

    LPRDOPROCTransact pTransact = m_pCreateAndGoOnTransactCalc->calcValue(pRuntime).
            getPointerByType<RDOPROCTransact, RDOResourceTypeList>();
    ASSERT(pTransact);

    pTransact->setBlock(this);
    pTransact->next();

    RDOTrace* tracer = pRuntime->getTracer();
    if (!tracer->isNull())
    {
        tracer->getOStream() << pTransact->traceResourceState('\0', pRuntime) << tracer->getEOL();
    }

    calcNextTimeInterval(pRuntime);
}

IBaseOperation::ResultCode RDOPROCGenerate::onContinue(const LPRDORuntime& /*pRuntime*/)
{
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

void RDOPROCGenerate::setStatistics(const LPIInternalStatistics& pStatistics)
{
    m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
