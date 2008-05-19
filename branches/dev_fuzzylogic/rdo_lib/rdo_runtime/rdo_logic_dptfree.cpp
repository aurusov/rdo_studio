#include "pch.h"
#include "rdo_logic_dptfree.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree( RDOSimulator* sim ):
	RDOLogic( sim )
{
}

RDODPTFree::~RDODPTFree()
{
}

} // namespace rdoRuntime
