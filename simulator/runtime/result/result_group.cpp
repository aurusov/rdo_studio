// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "simulator/runtime/result/result_group.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_model_i.h"
#include "repository/rdorepository.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResultGroup::RDOPMDResultGroup(const std::string& name)
    : m_name (name                               )
    , m_state(name.empty() ? RGS_START : RGS_STOP)
{}

RDOPMDResultGroup::~RDOPMDResultGroup()
{}

void RDOPMDResultGroup::resetResult(const LPRDORuntime& pRuntime)
{
    if (m_state == RGS_STOP)
        return;

    ASSERT(pRuntime);

    m_timeStart = pRuntime->getCurrentTime();

    if (!m_name.empty())
    {
        if (!m_streamFull.is_open())
        {
            LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
            ASSERT(pThreadProxy);
            rdo::repository::RDOThreadRepository::CreateFileInfo file(
                rdo::locale::convertToWStr(boost::str(boost::format("- %1% - full.txt") % m_name)),
                m_streamFull
            );
            pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
        }
        if (!m_streamTable.is_open())
        {
            LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
            ASSERT(pThreadProxy);
            rdo::repository::RDOThreadRepository::CreateFileInfo file(
                rdo::locale::convertToWStr(boost::str(boost::format("- %1% - table.txt") % m_name)),
                m_streamTable
            );
            pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
            if (m_streamTable.is_open())
            {
                for (const LPIResult& pResult: m_resultList)
                {
                    LPIResultGetValue pGetValue = pResult.object_dynamic_cast<IResultGetValue>();
                    if (pGetValue)
                    {
                        LPIName pName = pGetValue.object_dynamic_cast<IName>();
                        ASSERT(pName);
                        m_streamTable << pName->name() << "\t";
                    }
                }
                m_streamTable << std::endl;
            }
        }
    }

    for (LPIResult& pResult: m_resultList)
    {
        pResult->resetResult(pRuntime);
    }
}

void RDOPMDResultGroup::checkResult(const LPRDORuntime& pRuntime)
{
    if (m_state == RGS_STOP)
        return;

    for (LPIResult& pResult: m_resultList)
    {
        pResult->checkResult(pRuntime);
    }
}

void RDOPMDResultGroup::calcStat(const LPRDORuntime& pRuntime, std::ostream& stream)
{
    if (m_state == RGS_STOP)
        return;

    if (!m_name.empty())
    {
        double timeStop = pRuntime->getCurrentTime();
        boost::format header("---> %1%, %2$1.6f -> %3$1.6f = %4$1.6f\n");
        header
            % m_name
            % m_timeStart
            % timeStop
            % (timeStop - m_timeStart);

        stream << header.str();

        if (m_streamFull.is_open())
        {
            m_streamFull << header.str();
        }
    }

    bool tableWrite = false;
    for (LPIResult& pResult: m_resultList)
    {
        std::stringstream textStream;

        pResult->calcStat(pRuntime, textStream);

        stream << textStream.str();
        if (m_streamFull.is_open())
        {
            m_streamFull << textStream.str();
        }

        LPIResultGetValue pGetValue = pResult.object_dynamic_cast<IResultGetValue>();
        if (pGetValue)
        {
            if (pGetValue->getValue().typeID() != RDOType::t_real)
            {
                m_streamTable << pGetValue->getValue().getAsString() << "\t";
            }
            else
            {
                m_streamTable << rdo::format("%f", pGetValue->getValue().getDouble()) << "\t";
            }
            tableWrite = true;
        }
    }

    if (tableWrite)
    {
        m_streamTable << std::endl;
    }

    if (!m_name.empty())
    {
        boost::format footer("<--- %1%\n");
        footer % m_name;
        stream << footer.str();
        if (m_streamFull.is_open())
        {
            m_streamFull << footer.str();
        }
    }
    pRuntime->getResults().flush();
}

void RDOPMDResultGroup::onStart(const LPRDORuntime& pRuntime)
{
    m_state = RGS_START;
    resetResult(pRuntime);
}

void RDOPMDResultGroup::onStop(const LPRDORuntime& pRuntime)
{
    calcStat(pRuntime, pRuntime->getResults().getOStream());
    m_state = RGS_STOP;
}

void RDOPMDResultGroup::onAppend(const LPIResult& pResult)
{
    ASSERT(pResult);

    m_resultList.push_back(pResult);
}

CLOSE_RDO_RUNTIME_NAMESPACE
