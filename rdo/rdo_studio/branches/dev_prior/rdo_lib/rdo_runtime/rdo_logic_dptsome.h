#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"
#include "rdo_runtime.h"
#include "rdo_activity.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityCompare
// ----------------------------------------------------------------------------
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(RDORuntime* runtime)
		: m_runtime(runtime)
	{}
	bool operator() (RDOBaseOperation* opr1, RDOBaseOperation* opr2 )
	{
		RDOActivityPatternPrior* pattern1 = dynamic_cast<RDOActivityPatternPrior*>(opr1);
		RDOActivityPatternPrior* pattern2 = dynamic_cast<RDOActivityPatternPrior*>(opr2);
		if (pattern1 && pattern2)
		{
			RDOCalc* prior1 = pattern1->getPrior();
			RDOCalc* prior2 = pattern2->getPrior();
			RDOValue value1 = prior1 ? prior1->calcValue(m_runtime) : RDOValue(0.0);
			RDOValue value2 = prior2 ? prior2->calcValue(m_runtime) : RDOValue(0.0);
			return value1 > value2;
		}
		return false;
	}

private:
	RDORuntime* m_runtime;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogic, public RDOActivityPatternPrior
{
public:
	RDODPTSome (RDOSimulator* sim);
	virtual ~RDODPTSome();

private:
	virtual void actionWithRDOOprContainer(RDOSimulator* sim)
	{
		RDORuntime* runtime = reinterpret_cast<RDORuntime*>(sim);
		for (CIterator it = begin(); it != end(); ++it)
		{
			RDOActivityPatternPrior* pattern = dynamic_cast<RDOActivityPatternPrior*>(*it);
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
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
