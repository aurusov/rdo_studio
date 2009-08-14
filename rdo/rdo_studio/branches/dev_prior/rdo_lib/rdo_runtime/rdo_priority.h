#ifndef RDO_PRIORITY_H
#define RDO_PRIORITY_H

#include "rdo.h"
#include "rdocalc.h"
#include "rdo_runtime.h"
#include "rdo_priority_interface.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPatternPrior
// ----------------------------------------------------------------------------
class RDOPatternPrior: public IPriority
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPriority)
QUERY_INTERFACE_END

protected:
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

private:
	PTR(RDOCalc) m_prior;

	virtual PTR(RDOCalc) getPrior()
	{
		return m_prior; 
	}
	virtual rbool setPrior(PTR(RDOCalc) prior)
	{
		m_prior = prior;
		return true;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityCompare
// ----------------------------------------------------------------------------
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(PTR(RDORuntime) runtime)
		: m_runtime(runtime)
	{}
	rbool operator() (CREF(LPIBaseOperation) opr1, CREF(LPIBaseOperation) opr2)
	{
		LPIPriority pattern1 = opr1;
		LPIPriority pattern2 = opr2;
		if (pattern1 && pattern2)
		{
			PTR(RDOCalc) prior1 = pattern1->getPrior();
			PTR(RDOCalc) prior2 = pattern2->getPrior();
			RDOValue value1 = prior1 ? prior1->calcValue(m_runtime) : RDOValue(0.0);
			RDOValue value2 = prior2 ? prior2->calcValue(m_runtime) : RDOValue(0.0);
			return value1 > value2;
		}
		return false;
	}

private:
	PTR(RDORuntime) m_runtime;
};

} // namespace rdoRuntime

#endif // RDO_PRIORITY_H
