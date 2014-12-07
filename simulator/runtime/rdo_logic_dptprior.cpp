// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptprior.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOOrderDPTPrior
// --------------------------------------------------------------------------------
LPIBaseOperation RDOOrderDPTPrior::sort(const LPRDORuntime& pRuntime, BaseOperationList& container)
{
	BaseOperationList priorContainer;
	for (const auto& operation: container)
	{
		if (operation.object_dynamic_cast<IBaseOperation>()->onCheckCondition(pRuntime))
		{
			priorContainer.push_back(operation);
		}
	}

	if (priorContainer.empty())
		return NULL;

	for (const auto& operation: container)
	{
		LPIPriority pPattern = operation.object_dynamic_cast<IPriority>();
		if (pPattern)
		{
			LPRDOCalc pPriorCalc = pPattern->getPrior();
			if (pPriorCalc)
			{
				RDOValue value = pPriorCalc->calcValue(pRuntime);
				if (value < 0.0 || value > 1.0)
				{
					pRuntime->error().push(rdo::format("Приоритет активности вышел за пределы диапазона [0..1]: %s", value.getAsString().c_str()), pPriorCalc->srcInfo());
				}
			}
		}
	}
	std::sort(priorContainer.begin(), priorContainer.end(), RDODPTActivityCompare(pRuntime));
	return priorContainer.front();
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent)
	: RDOLogicDPTPrior(pRuntime, pParent)
{
	pRuntime->getFreeDPTId();
}

RDODPTPrior::~RDODPTPrior()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
