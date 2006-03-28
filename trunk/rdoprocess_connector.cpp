#include "stdafx.h"
#include "rdoprocess_connector.h"
#include "rdoprocess_object_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPConnector
// ----------------------------------------------------------------------------
RPConnector::RPConnector( RPObject* _parent, const rp::string& _name ):
	RPObjectChart( _parent, _name ),
	dock_begin( NULL ),
	dock_end( NULL )
{
	main_pen_width = 1;
	main_pen.DeleteObject();
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0x00, 0x00, 0x00);
	lb.lbHatch = 0;
	main_pen.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, &lb );
}

RPConnector::~RPConnector()
{
}

void RPConnector::draw( CDC& dc )
{
	if ( dock_begin && !dock_end ) {
		rp::point p1 = dock_begin->getPosition();
		rp::point p2 = flowChart()->mouse_current();
		CPen* old_pen = dc.SelectObject( &main_pen );
		dc.MoveTo( p1.x, p1.y );
		dc.LineTo( p2.x, p2.y );
		dc.SelectObject( old_pen );
	} else if ( dock_begin && dock_end ) {
		rp::point p1 = dock_begin->getPosition();
		rp::point p2 = dock_end->getPosition();
		CPen* old_pen = dc.SelectObject( &main_pen );
		dc.MoveTo( p1.x, p1.y );
		dc.LineTo( p2.x, p2.y );
		dc.SelectObject( old_pen );
	}
}

RPConnectorDock* RPConnector::getConnectedDock( const RPConnectorDock& dock ) const
{
	if ( &dock == dock_begin ) return dock_end;
	if ( &dock == dock_end ) return dock_begin;
	return NULL;
}
