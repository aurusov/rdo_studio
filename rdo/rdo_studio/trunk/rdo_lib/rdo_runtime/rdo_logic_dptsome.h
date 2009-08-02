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
public:
	RDODPTSome (RDOSimulator* sim);
	virtual ~RDODPTSome();
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
