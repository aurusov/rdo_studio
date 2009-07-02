#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"
#include "rdo_activity.h"
#include "rdo_runtime.h"

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
		RDOActivityPatternPrior* prior1 = dynamic_cast<RDOActivityPatternPrior*>(opr1);
		RDOActivityPatternPrior* prior2 = dynamic_cast<RDOActivityPatternPrior*>(opr2);
		if (prior1 && prior2)
		{
			return prior1->getPrior()->calcValue(m_runtime) > prior2->getPrior()->calcValue(m_runtime);
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

private:
	virtual void actionWithRDOOprContainer( RDOSimulator* sim )
	{
		RDORuntime* runtime = reinterpret_cast<RDORuntime*>(sim);
		Iterator it = begin();
		while ( it != end() )
		{
			RDOActivityPatternPrior* prior = dynamic_cast<RDOActivityPatternPrior*>(*it);
			if ( prior->getPrior()->calcValue(runtime) > 1 || prior->getPrior()->calcValue(runtime) < 0 )
			{
				runtime->error("� �������� ������ ������ ��������� ��������� ���������� ����� �� ������� ��������� [0;1]");
			}
			it++;
		}
		std::sort( begin(), end(), RDODPTActivityCompare(reinterpret_cast<RDORuntime*>(sim)) );
	}
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
