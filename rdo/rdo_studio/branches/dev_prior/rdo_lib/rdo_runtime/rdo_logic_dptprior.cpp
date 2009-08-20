#include "pch.h"
#include "rdo_logic_dptprior.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior( RDOSimulator* sim ):
	RDOLogic( sim )
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

RDODPTPrior::~RDODPTPrior()
{
}

} // namespace rdoRuntime
