#ifndef RDO_LOGIC_OPR_H
#define RDO_LOGIC_OPR_H

#include "rdo_logic.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
class RDOOperations: public RDOLogic
{
DEFINE_FACTORY(RDOOperations);

private:
	RDOOperations( RDOSimulator* sim );
	virtual ~RDOOperations();
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_OPR_H
