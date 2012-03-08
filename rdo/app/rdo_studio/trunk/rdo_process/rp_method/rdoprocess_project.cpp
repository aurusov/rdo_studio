#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_messages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rpMethod {
RPProject* project = NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RPProject
// --------------------------------------------------------------------------------
RPProject::RPProject():
	RPObject( NULL ),
	last_button_id( 20000 ),
	flow_state( flow_none ),
	message( NULL ),
	activeObject( NULL ),
	activeFlowChart( NULL )
{
	rpMethod::project = this;
	message = new rp::msg();
	message->setName( "__msg" );
	setName( "project" );
	msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
}

RPProject::~RPProject()
{
	rpMethod::project = NULL;
	if ( message ) {
		rp::msg* _message = message;
		message = NULL;
		delete _message;
	}
}

RPObject* RPProject::findObject( rp::string fullname )
{
	if ( fullname.empty() ) return NULL;
	tstring::size_type pos = fullname.find( "/project" );
	if ( pos != 0 ) return NULL;
	fullname.erase( 0, 8 );
	if ( fullname.empty() ) return this;
	RPObject* obj = this;
	while ( true ) {
		rp::string obj_name = "";
		fullname.erase( 0, 1 );
		pos = fullname.find( '/' );
		bool last = false;
		if ( pos != tstring::npos ) {
			obj_name = fullname.substr( 0, pos );
			fullname.erase( 0, pos );
		} else {
			obj_name = fullname;
			last = true;
		}
		obj = obj->find_child( obj_name );
		if ( obj ) {
			if ( last ) return obj;
		} else {
			return NULL;
		}
	}
	return NULL;
}

void RPProject::setFlowState( FlowState value )
{
	if ( flow_state != value ) {
		flow_state = value;
		sendMessage( this, rp::msg::RP_FLOWSTATE_CHANGED, &flow_state );
	}
}

void RPProject::notify( RPObject* from, UINT message, void* param )
{
	if ( message == rp::msg::RP_OBJ_SELCHANGED ) {
		msg().disconnect( this, rp::msg::RP_OBJ_SELCHANGED );
		activeObject = param ? from : NULL;
		selectChildOff( activeObject );
		msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	}
}

void RPProject::setActiveFlowChart( RPObjectFlowChart* flowChart )
{
	activeFlowChart = flowChart;
	flow_state = activeFlowChart ? flow_select : flow_none;
	sendMessage( this, rp::msg::RP_FLOWSTATE_CHANGED, &flow_state );
}
