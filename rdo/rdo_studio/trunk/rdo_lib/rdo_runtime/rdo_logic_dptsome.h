#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"
#include "rdo_priority.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogic, public RDOPatternPrior
{
DEFINE_FACTORY(RDODPTSome);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTSome (RDOSimulator* sim);
	virtual ~RDODPTSome();
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
