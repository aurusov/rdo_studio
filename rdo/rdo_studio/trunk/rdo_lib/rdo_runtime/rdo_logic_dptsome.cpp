#include "pch.h"
#include "rdo_logic_dptsome.h"
#include "rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome(PTR(RDOSimulator) sim)
	: RDOLogicFIFO()
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

RDODPTSome::~RDODPTSome()
{}

void RDODPTSome::actionWithRDOOprContainer(PTR(RDOSimulator) sim)
{
}

} // namespace rdoRuntime
