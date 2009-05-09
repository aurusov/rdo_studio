#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityCompare
// ----------------------------------------------------------------------------
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare() {}
	bool operator() ( const RDOBaseOperation& bo1, const RDOBaseOperation& bo2 )
	{
		return bo1.m_prior_runtime->calcValue(  ) < bo2.m_prior_runtime->calcValue(  );
	}
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
		std::sort( begin(), end(), RDODPTActivityCompare() );
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
