/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptprior.inl
 * author   : ƒмитрий Ћущан
 * date     : 
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
// ---------- RDODPTPriorOrder
// ----------------------------------------------------------------------------
inline LPIBaseOperation RDODPTPriorOrder::sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container)
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

	return BaseOperationPriorOrder::sort(sim, priorContainer);
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
