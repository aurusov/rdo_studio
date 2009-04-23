#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"
#include <algorithm>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogic
{
public:
	RDODPTSome( RDOSimulator* sim );
	virtual ~RDODPTSome();

protected:
	// Для приоритетов надо перекрыть (переписать) этот виртуальный метод метод.
	// Сейчас вызывается стандартный обработчик RDOLogic::onCheckCondition,
	// который находит первую возможную операцию.
	// virtual bool onCheckCondition( RDOSimulator* sim );
//	inline bool compareActivity( const RDOActivity* act1, const RDOActivity* act2 )
//	{
//		return ( act1->m_prior_runtime->m_value < act2->m_prior_runtime->m_value );
//	}
	virtual bool newCheckCondition( RDOSimulator* sim )
	{
		std::sort( begin(), end() );
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
