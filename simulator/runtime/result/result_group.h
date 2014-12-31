#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem/fstream.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/result/result_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPMDResultGroup: public IResult, public IResultGroup
{
DECLARE_FACTORY(RDOPMDResultGroup);
private:
    RDOPMDResultGroup(const std::string& name);
    virtual ~RDOPMDResultGroup();

    enum class State
    {
        STOP,
        START
    };

    typedef std::vector<LPIResult> ResultList;
    ResultList m_resultList;
    std::string m_name;
    State m_state;
    double m_timeStart;
    boost::filesystem::ofstream m_streamFull;
    boost::filesystem::ofstream m_streamTable;

    DECLARE_IResult;
    DECLARE_IResultGroup;
};

CLOSE_RDO_RUNTIME_NAMESPACE
