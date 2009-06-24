#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"
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
	bool operator() ( RDOBaseOperation* opr1, RDOBaseOperation* opr2 )
	{
		RDOActivityPatternPrior* proir1 = dynamic_cast<RDOActivityPatternPrior*>(opr1);
		RDOActivityPatternPrior* proir2 = dynamic_cast<RDOActivityPatternPrior*>(opr2);
		if (proir1 && proir2)
		{
			return proir1->getPrior()->calcValue(m_runtime) > proir2->getPrior()->calcValue(m_runtime);
		}
		return false;
	}

private:
	RDORuntime* m_runtime;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogic
{
public:
	RDODPTSome( RDOSimulator* sim );
	virtual ~RDODPTSome();

protected:
	virtual void newCheckCondition( RDOSimulator* sim )
	{
		std::sort( begin(), end(), RDODPTActivityCompare(reinterpret_cast<RDORuntime*>(sim)) );
	}
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
