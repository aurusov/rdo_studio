#include "pch.h"
#include "rdo_logic_opr.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations( RDOSimulator* sim ):
	RDOLogic( sim )
{
}

RDOOperations::~RDOOperations()
{
}

} // namespace rdoRuntime
