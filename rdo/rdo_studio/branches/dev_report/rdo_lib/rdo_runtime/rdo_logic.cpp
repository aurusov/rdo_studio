#include "pch.h"
#include "rdo_logic.h"
#include "rdo_simulator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

RDOLogic::RDOLogic( RDOSimulator* sim ):
	RDOOprContainer< RDOBaseOperation >( sim ),
	m_childLogic( NULL ),
	m_condition( NULL ),
	m_lastCondition( false )
{
	m_childLogic.reparent( this );

	if ( sim )
	{
		sim->appendLogic( this );
	}
}

} // namespace rdoRuntime
