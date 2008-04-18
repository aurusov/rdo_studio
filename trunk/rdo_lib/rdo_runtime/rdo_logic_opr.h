#ifndef RDO_LOGIC_OPR_H
#define RDO_LOGIC_OPR_H

#include "rdo.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
class RDOOperations: public RDOBaseLogic
{
public:
	static RDOOperations* getInstance( RDOSimulator* sim );
	static void           destroy    ( RDOSimulator* sim );

private:
	RDOOperations( RDOSimulator* sim );
	virtual ~RDOOperations();

	static RDOOperations* s_instance;
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_OPR_H
