/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.inl
 * author   : Урусов Андрей
 * date     : 03.10.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdomacros.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- BaseOperationPriorOrder
// ----------------------------------------------------------------------------
inline LPIBaseOperation BaseOperationPriorOrder::sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container)
{
	if (container.empty())
		return NULL;

	PTR(RDORuntime) runtime = static_cast<PTR(RDORuntime)>(sim);
	STL_FOR_ALL_CONST(BaseOperationList, container, it)
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
	std::sort(container.begin(), container.end(), RDODPTActivityCompare(static_cast<PTR(RDORuntime)>(sim)));
	return container.front();
}

CLOSE_RDO_RUNTIME_NAMESPACE
