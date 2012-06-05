/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptprior.inl
 * author   : Лущан Дмитрий
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_runtime.h"
#include "rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOrderDPTPrior
// ----------------------------------------------------------------------------
inline LPIBaseOperation RDOOrderDPTPrior::sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container)
{
	BaseOperationList priorContainer;
	STL_FOR_ALL_CONST(BaseOperationList, container, it)
	{
		if (it->query_cast<IBaseOperation>()->onCheckCondition(sim))
		{
			priorContainer.push_back(*it);
		}
	}

	if (priorContainer.empty())
		return NULL;

	PTR(RDORuntime) runtime = static_cast<PTR(RDORuntime)>(sim);
	STL_FOR_ALL_CONST(BaseOperationList, priorContainer, it)
	{
		LPIPriority pattern = *it;
		if (pattern)
		{
			PTR(RDOCalc) prior = pattern->getPrior();
			if (prior)
			{
				RDOValue value = prior->calcValue(runtime);
				if (value < 0 || value > 1)
					runtime->error(rdo::format(_T("Приоритет активности вышел за пределы диапазона [0..1]: %s"), value.getAsString().c_str()), prior);
			}
		}
	}
	std::sort(priorContainer.begin(), priorContainer.end(), RDODPTActivityCompare(static_cast<PTR(RDORuntime)>(sim)));
	return priorContainer.front();
}

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
inline RDODPTPrior::RDODPTPrior( RDOSimulator* sim ):
	RDOLogicDPTPrior()
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

inline RDODPTPrior::~RDODPTPrior()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
