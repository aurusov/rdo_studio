#include "stdafx.h"
#include "rdoprocess_chart_object.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_app.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPChartObject
// ----------------------------------------------------------------------------
RPChartObject::RPChartObject( RPObject* _parent, RPChartObject* _chart_parent, RPFlowChart* _flowchart, const rp::string& _name ):
	RPObject( _parent, _name ),
	rotate_center( 0, 0 ),
	rotate_center_inited( false ),
	chart_parent( _chart_parent ),
	flowchart( _flowchart ),
	main_pen_width( 2 ),
	rotation_alpha( 0 )
{
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0x00, 0x00, 0x00);
	lb.lbHatch = 0;
	main_pen.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, &lb );
}

RPChartObject::~RPChartObject()
{
}

void RPChartObject::setPosition( double posx, double posy )
{
	matrix_transform.dx() = posx;
	matrix_transform.dy() = posy;
	flowchart->modify();
}

void RPChartObject::moving( int dx, int dy )
{
	matrix_transform.dx() += dx;
	matrix_transform.dy() += dy;
	flowchart->modify();
}

void RPChartObject::setRotation( double alpha )
{
	while ( alpha < 0 ) alpha += 360.0;
	while ( alpha > 360 ) alpha -= 360.0;
	double delta = alpha - rotation_alpha;
	rotation_alpha = alpha;
	TRACE( "alpha = %f\n", alpha );
	delta *= rp::math::pi / 180.0;
	double cos_a = cos( delta );
	double sin_a = sin( delta );
//	if ( fabs(cos_a) < 1e-10 ) cos_a = 0;
//	if ( fabs(sin_a) < 1e-10 ) sin_a = 0;
	rp::matrix m_rotate;
	m_rotate.data[0][0] = cos_a;
	m_rotate.data[1][1] = cos_a;
	m_rotate.data[0][1] = sin_a;
	m_rotate.data[1][0] = -sin_a;
	rp::matrix r_center;
	r_center.dx() = -rotate_center.x;
	r_center.dy() = -rotate_center.y;
	matrix_rotate = r_center.obr() * m_rotate * r_center * matrix_rotate;
	flowchart->modify();
}

void RPChartObject::setSelected( bool value )
{
	bool _sel = isSelected();
	RPObject::setSelected( value );
	if ( _sel != value && flowchart ) {
		flowchart->updateDC();
	}
}

bool RPChartObject::isRotateCenter( const CPoint& point ) const
{
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		return rp::math::getLength( getRotateCenter(), point ) <= flowchart->getSensitivity();
	}
	return false;
}
