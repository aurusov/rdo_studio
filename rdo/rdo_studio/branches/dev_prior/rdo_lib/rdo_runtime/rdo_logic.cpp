#include "pch.h"
#include "rdo_logic.h"
#include "rdo_simulator.h"
#include "rdo_priority.h"

#pragma warning(disable : 4786)  

namespace rdoRuntime {

RDOLogic::RDOLogic( RDOSimulator* sim ):
	RDOOprContainer< RDOBaseOperation >( sim ),
	m_childLogic( NULL ),
	m_condition( NULL ),
	m_lastCondition( false )
{
	m_childLogic.reparent( this );

	if ( sim )
	{
		sim->appendLogic( this );
	}
}

	void RDOLogic::actionWithRDOOprContainer(RDOSimulator* sim)
	{
		RDORuntime* runtime = reinterpret_cast<RDORuntime*>(sim);
		for (CIterator it = begin(); it != end(); ++it)
		{
			RDOPatternPrior* pattern = dynamic_cast<RDOPatternPrior*>(*it);
			if (pattern)
			{
				RDOCalc* prior = pattern->getPrior();
				if (prior)
				{
					RDOValue value = prior->calcValue(runtime);
					if (value < 0 || value > 1)
						runtime->error(rdo::format(_T("ѕриоритет активности вышел за пределы диапазона [0..1]: %s"), value.getAsString().c_str()), prior);
				}
			}
		}
		std::sort(begin(), end(), RDODPTActivityCompare(reinterpret_cast<RDORuntime*>(sim)));
	}

} // namespace rdoRuntime
