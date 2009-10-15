#include "pch.h"
#include "rdo_logic_dptsome.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOSimulator* sim ):
	RDOLogic( sim )
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

RDODPTSome::~RDODPTSome()
{
}

} // namespace rdoRuntime
