#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPROCTerminate: public RDOPROCBlock, public IInternalStatisticsManager, public boost::noncopyable
{
DECLARE_FACTORY(RDOPROCTerminate);
private:
	RDOPROCTerminate(const LPIPROCProcess& pProcess, const LPRDOCalc& pCalc);

	std::size_t m_terminatedTransactCount;
	LPRDOCalc m_pTermCalc;
	LPIInternalStatistics m_pStatistics;

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE
