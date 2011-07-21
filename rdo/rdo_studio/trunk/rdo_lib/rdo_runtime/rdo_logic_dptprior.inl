/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptprior.inl
 * @authors   Лущан Дмитрий
 * @date      04.11.2009
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOOrderDPTPrior
// ********************************************************************************
inline LPIBaseOperation RDOOrderDPTPrior::sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container)
{
	BaseOperationList priorContainer;
	STL_FOR_ALL_CONST(container, it)
	{
		if (it->query_cast<IBaseOperation>()->onCheckCondition(pRuntime))
		{
			priorContainer.push_back(*it);
		}
	}

	if (priorContainer.empty())
		return NULL;

	STL_FOR_ALL_CONST(priorContainer, it)
	{
		LPIPriority pattern = *it;
		if (pattern)
		{
			LPRDOCalc prior = pattern->getPrior();
			if (prior)
			{
				RDOValue value = prior->calcValue(pRuntime);
				if (value < 0 || value > 1)
					pRuntime->error(rdo::format(_T("Приоритет активности вышел за пределы диапазона [0..1]: %s"), value.getAsString().c_str()), prior);
			}
		}
	}
	std::sort(priorContainer.begin(), priorContainer.end(), RDODPTActivityCompare(pRuntime));
	return priorContainer.front();
}

// ********************************************************************************
// ******************** RDODPTPrior
// ********************************************************************************
inline RDODPTPrior::RDODPTPrior( CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent ):
	RDOLogicDPTPrior(pRuntime, parent)
{
	pRuntime->getFreeDPTId();
}

inline RDODPTPrior::~RDODPTPrior()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
