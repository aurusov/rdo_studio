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
	flow_state( flow_none ),
	activeFlowChart( NULL )
{
	rpapp.msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );

	generate_time_MJ = 100;
}

RPProject::~RPProject()
{
}

void RPProject::setFlowState( FlowState value )
{
	if ( flow_state != value ) {
		flow_state = value;
		rpapp.sendMessage( this, rp::msg::RP_FLOWSTATE_CHANGED, flow_state );
	}
}

void RPProject::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == rp::msg::RP_OBJ_SELCHANGED ) {
		rpapp.msg().disconnect( this, rp::msg::RP_OBJ_SELCHANGED );
		selectChildOff( wParam ? from : NULL );
		rpapp.msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	}
}

void RPProject::setActiveFlowChart( RPObjectFlowChart* flowChart )
{
	activeFlowChart = flowChart;
	flow_state = activeFlowChart ? flow_select : flow_none;
	rpapp.sendMessage( this, rp::msg::RP_FLOWSTATE_CHANGED, flow_state );
}
