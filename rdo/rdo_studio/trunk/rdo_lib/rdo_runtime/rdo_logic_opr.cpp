#include "pch.h"
#include "rdo_logic_opr.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations(PTR(RDOSimulator) sim)
	: RDOLogicFIFO()
{}

RDOOperations::~RDOOperations()
{}

} // namespace rdoRuntime
