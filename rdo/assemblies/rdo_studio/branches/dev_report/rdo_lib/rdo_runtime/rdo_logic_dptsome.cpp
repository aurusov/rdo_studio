#include "pch.h"
#include "rdo_logic_dptsome.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
