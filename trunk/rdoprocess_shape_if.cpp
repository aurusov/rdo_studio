#include "stdafx.h"
#include "rdoprocess_shape_if.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShapeIf
// ----------------------------------------------------------------------------
RPShapeIf::RPShapeIf( RPObject* _parent ):
	RPShape( _parent, _T("If") )
{
	pa_src.push_back( rp::point(0, -35) );
	pa_src.push_back( rp::point(35, 0) );
	pa_src.push_back( rp::point(0, 35) );
	pa_src.push_back( rp::point(-35, 0) );
	pa_src.push_back( rp::point(0, -35) );

	docks.push_back( RPConnectorDock( this, RPConnectorDock::in,  rp::point( -35, 0  ) ) );
	docks.push_back( RPConnectorDock( this, RPConnectorDock::out, rp::point(  35, 0  ) ) );
	docks.push_back( RPConnectorDock( this, RPConnectorDock::out, rp::point(   0, 35 ) ) );
}

RPShapeIf::~RPShapeIf()
{
}
