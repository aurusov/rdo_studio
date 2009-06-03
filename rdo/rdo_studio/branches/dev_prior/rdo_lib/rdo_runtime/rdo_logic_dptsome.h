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
	bool operator() ( const RDOBaseOperation* opr1, const RDOBaseOperation* opr2 )
	{
		RDOActivityPatternPrior* proir1 = const_cast<RDOActivityPatternPrior*>(reinterpret_cast<const RDOActivityPatternPrior*>(opr1));
		RDOActivityPatternPrior* proir2 = const_cast<RDOActivityPatternPrior*>(reinterpret_cast<const RDOActivityPatternPrior*>(opr2));
		return proir1->getPrior()->calcValue(m_runtime) < proir2->getPrior()->calcValue(m_runtime);
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
	virtual bool newCheckCondition( RDOSimulator* sim )
	{
		std::sort( begin(), end(), RDODPTActivityCompare(reinterpret_cast<RDORuntime*>(sim)) );
		Iterator it = begin();
		while ( it != end() )
		{
			if ( (*it)->onCheckCondition(sim) )
			{
				m_first = *it;
				return true;
			}
			it++;
		}
		m_first = NULL;
		return false;
	}
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
