#include "stdafx.h"
#include "rdoprocess_project.h"
#include "rdoprocess_app.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCProject
// ----------------------------------------------------------------------------
RDOPROCProject::RDOPROCProject():
	RDOPROCObject( NULL ),
	flow_state( flow_select )
{
}

RDOPROCProject::~RDOPROCProject()
{
}

void RDOPROCProject::setFlowState( FlowState value )
{
	if ( flow_state != value ) {
		flow_state = value;
		rpapp.sendMessage( this, RP_FLOWSTATE_CHANGED, flow_state, 0  );
	}
}
