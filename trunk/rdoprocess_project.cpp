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
// ---------- RPProject
// ----------------------------------------------------------------------------
RPProject::RPProject():
	RPObject( NULL ),
	flow_state( flow_select )
{
	rpapp.msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
}

RPProject::~RPProject()
{
}

void RPProject::setFlowState( FlowState value )
{
	if ( flow_state != value ) {
		flow_state = value;
		rpapp.sendMessage( this, rp::msg::RP_FLOWSTATE_CHANGED, flow_state, 0  );
	}
}

void RPProject::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message = rp::msg::RP_OBJ_SELCHANGED ) {
		rpapp.msg().disconnect( this, rp::msg::RP_OBJ_SELCHANGED );
		selectChildOff( wParam ? from : NULL );
		rpapp.msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	}
}
