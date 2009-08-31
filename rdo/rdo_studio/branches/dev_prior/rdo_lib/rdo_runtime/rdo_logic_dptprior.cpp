#include "pch.h"
#include "rdo_logic_dptprior.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior( RDOSimulator* sim ):
	RDOLogic( sim )
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

RDODPTPrior::~RDODPTPrior()
{
}

void RDODPTPrior::actionWithRDOOprContainer(PTR(RDOSimulator) sim)
{
	PTR(RDORuntime) runtime = static_cast<PTR(RDORuntime)>(sim);
	for (CIterator it = begin(); it != end(); ++it)
	{
		LPIPriority pattern = *it;
		if (pattern)
		{
			PTR(RDOCalc) prior = pattern->getPrior();
			if (prior)
			{
				RDOValue value = prior->calcValue(runtime);
				if (value < 0 || value > 1)
					runtime->error(rdo::format(_T("ѕриоритет активности вышел за пределы диапазона [0..1]: %s"), value.getAsString().c_str()), prior);
			}
		}
	}
	std::sort(begin(), end(), RDODPTActivityCompare(static_cast<PTR(RDORuntime)>(sim)));
}

} // namespace rdoRuntime
