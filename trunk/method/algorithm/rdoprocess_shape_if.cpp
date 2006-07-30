#include "stdafx.h"
#include "rdoprocess_shape_if.h"
#include "../../rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPConnectorDockGreen
// ----------------------------------------------------------------------------
class RPConnectorDockGreen: public RPConnectorDockOne
{
public:
	RPConnectorDockGreen( RPObjectMatrix* __object_matrix, Type _type, const rp::point& _point, double _norm ): RPConnectorDockOne( __object_matrix, _type, _point, _norm ) {};
	virtual ~RPConnectorDockGreen() {};

	virtual COLORREF color() const { return RGB(0x80, 0xFF, 0x00); }
};

// ----------------------------------------------------------------------------
// ---------- RPConnectorDockRed
// ----------------------------------------------------------------------------
class RPConnectorDockRed: public RPConnectorDockOne
{
public:
	RPConnectorDockRed( RPObjectMatrix* __object_matrix, Type _type, const rp::point& _point, double _norm ): RPConnectorDockOne( __object_matrix, _type, _point, _norm ) {};
	virtual ~RPConnectorDockRed() {};

	virtual COLORREF color() const { return RGB(0xFF, 0x80, 0x00); }
};

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

	docks.push_back( new RPConnectorDock     ( this, RPConnectorDock::in,  rp::point( -35, 0  ), 180 ) );
	docks.push_back( new RPConnectorDockGreen( this, RPConnectorDock::out, rp::point(  35, 0  ), 0   ) );
	docks.push_back( new RPConnectorDockRed  ( this, RPConnectorDock::out, rp::point(   0, 35 ), 270 ) );
}

RPShapeIf::~RPShapeIf()
{
}
