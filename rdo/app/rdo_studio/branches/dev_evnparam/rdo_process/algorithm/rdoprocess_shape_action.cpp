#include "stdafx.h"
#include "rdoprocess_shape_action.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_connector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShapeAction
// ----------------------------------------------------------------------------
RPShapeAction::RPShapeAction( RPObject* _parent ):
	RPShape( _parent, "Action" )
{
	bg_brush.DeleteObject();
	LOGBRUSH lb;
	lb.lbStyle = BS_NULL;
	bg_brush.CreateBrushIndirect( &lb );

	pa_src.push_back( rp::point(-40, -25) );
	pa_src.push_back( rp::point(40, -25) );
	pa_src.push_back( rp::point(40, 25) );
	pa_src.push_back( rp::point(-40, 25) );
	pa_src.push_back( rp::point(-40, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point( 0, -25 ), 90 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::out, rp::point( 0, 25 ), 270 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point( -40, 0 ), 180 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::out, rp::point(  40, 0 ), 0   ) );
//	docks.push_back( new RPConnectorDockResource( this, RPConnectorDock::out,  rp::point( 0, 25 ), 270 ) );

//	matrix_scale.data[0][0] = 2;
}

RPShapeAction::~RPShapeAction()
{
}

RPObject* RPShapeAction::newObject( RPObject* parent )
{
	return new RPShapeAction( parent );
}
