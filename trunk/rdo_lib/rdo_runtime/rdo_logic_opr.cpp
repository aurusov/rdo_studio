#include "pch.h"
#include "rdo_logic_opr.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
