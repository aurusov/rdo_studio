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
RDOOperations* RDOOperations::s_instance = NULL;

RDOOperations::RDOOperations( RDOSimulator* sim ):
	RDOBaseLogic( sim )
{
	sim->addBaseLogic( this );
}

RDOOperations::~RDOOperations()
{
}

RDOOperations* RDOOperations::getInstance( RDOSimulator* sim )
{
	if ( s_instance == NULL )
	{
		s_instance = new RDOOperations( sim );
	}
	return s_instance;
}

void RDOOperations::destroy( RDOSimulator* sim )
{
	if ( s_instance )
	{
		delete s_instance;
		s_instance = NULL;
	}
}

} // namespace rdoRuntime
