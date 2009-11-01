/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptprior.inl
 * author   : Дмитрий Лущан
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_runtime.h"
#include "rdo_logic.cpp"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- OrderPrior
// ----------------------------------------------------------------------------
inline LPIBaseOperation RDODPTPriorOrder::sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container)
{
	BaseOperationList priorContainer;
	for (BaseOperationList::const_iterator it = container.begin(); it != container.end(); it++)
	{
		if (it->query_cast<IBaseOperation>()->onCheckCondition(sim))
		{
			priorContainer.push_back(*it);
		}
	}
	if (priorContainer.empty())
		return NULL;

	PTR(RDORuntime) runtime = static_cast<PTR(RDORuntime)>(sim);
	for (BaseOperationList::const_iterator it = priorContainer.begin(); it != priorContainer.end(); it++)
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
	RDOLogicPrior()
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

inline RDODPTPrior::~RDODPTPrior()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
