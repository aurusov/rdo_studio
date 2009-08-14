#ifndef RDO_LOGIC_DPTFREE_H
#define RDO_LOGIC_DPTFREE_H

#include "rdo_logic.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
class RDODPTFree: public RDOLogic
{
DEFINE_FACTORY(RDODPTFree)

private:
	RDODPTFree( RDOSimulator* sim );
	virtual ~RDODPTFree();
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTFREE_H
