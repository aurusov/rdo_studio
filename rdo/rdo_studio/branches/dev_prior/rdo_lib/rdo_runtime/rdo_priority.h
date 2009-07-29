#ifndef RDO_PRIORITY_H
#define RDO_PRIORITY_H

#include "rdo.h"
#include "rdocalc.h"
#include "rdo_runtime.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPatternPrior
// ----------------------------------------------------------------------------
class RDOPatternPrior
{
public:
	RDOPatternPrior()
		: m_prior(NULL)
	{}
	virtual ~RDOPatternPrior()
	{
		if (m_prior)
		{
			delete m_prior;
			m_prior = NULL;
		}
	}

	RDOCalc* getPrior()
	{
		return m_prior; 
	}
	bool setPrior(RDOCalc* prior)
	{
		m_prior = prior;
		return true;
	}

private:
	RDOCalc* m_prior;
};

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
		RDOPatternPrior* pattern1 = dynamic_cast<RDOPatternPrior*>(opr1);
		RDOPatternPrior* pattern2 = dynamic_cast<RDOPatternPrior*>(opr2);
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

} // namespace rdoRuntime

#endif // RDO_PRIORITY_H
