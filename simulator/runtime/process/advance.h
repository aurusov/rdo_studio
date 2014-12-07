#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPROCAdvance: public RDOPROCBlock, public IInternalStatisticsManager
{
DECLARE_FACTORY(RDOPROCAdvance);
protected:
	LPRDOCalc pDelayCalc;

		struct LeaveTr
	{
		LPRDOPROCTransact transact;
		double            timeLeave;
		LeaveTr(const LPRDOPROCTransact& _transact, double _timeLeave);
	};
	std::list<LeaveTr> leave_list;

private:
	RDOPROCAdvance(LPIPROCProcess process, const LPRDOCalc& _pDelayCalc);

	LPIInternalStatistics m_pStatistics;

	void onMakePlaned(const LPRDORuntime& pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE
