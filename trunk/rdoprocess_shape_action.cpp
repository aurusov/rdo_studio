#include "stdafx.h"
#include "rdoprocess_shape_action.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShapeAction
// ----------------------------------------------------------------------------
RPShapeAction::RPShapeAction( RPObject* _parent ):
	RPShape( _parent, _T("Action") )
{
	pa_src.push_back( rp::point(-40, -25) );
	pa_src.push_back( rp::point(40, -25) );
	pa_src.push_back( rp::point(40, 25) );
	pa_src.push_back( rp::point(-40, 25) );
	pa_src.push_back( rp::point(-40, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::inout, rp::point( 0, -25 ), 90 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::inout, rp::point( 0, 25 ), 270 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::inout, rp::point( -40, 0 ), 180 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::inout, rp::point(  40, 0 ), 0   ) );
//	docks.push_back( new RPConnectorDockResource( this, RPConnectorDock::out,  rp::point( 0, 25 ), 270 ) );

//	matrix_scale.data[0][0] = 2;
}

RPShapeAction::~RPShapeAction()
{
}
