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
	rotation_alpha = alpha;
	TRACE( "alpha = %f\n", alpha );
	alpha *= rp::math::pi / 180.0;
	double cos_a = cos( alpha );
	double sin_a = sin( alpha );
//	if ( fabs(cos_a) < 1e-10 ) cos_a = 0;
//	if ( fabs(sin_a) < 1e-10 ) sin_a = 0;
	matrix_rotate.data[0][0] = cos_a;
	matrix_rotate.data[1][1] = cos_a;
	matrix_rotate.data[0][1] = -sin_a;
	matrix_rotate.data[1][0] = sin_a;
	flowchart->modify();
}

void RPChartObject::setRotateCenter( const CPoint& point )
{
	CPoint rotate_center_prev = rotate_center;
	rotate_center = rotateCenterMatrix().obr() * point;
	p0 = CPoint( 0, 0 );
	p1 = rotate_center_prev;
	p2 = rotate_center;
	if ( p1 != p2 ) {
		double len01 = rp::math::getLength( p0, p1 );
		double len02 = rp::math::getLength( p0, p2 );
		double len12 = rp::math::getLength( p1, p2 );
		double cos_a = (len01*len01 + len02*len02 - len12*len12) / (2*len01*len02);
		double alpha = acos(cos_a) * 180.0 / rp::math::pi;
		TRACE( "delta = %f\n", alpha );
//		setRotation( rotation_alpha + alpha );
	}
}

void RPChartObject::draw( CDC& dc )
{
/*
	rp::matrix gm = globalMatrix();
	CPen pen1( PS_SOLID, 1, RGB(-1,0,-1) );
	dc.SelectObject( pen1 );
	dc.MoveTo( gm * p0 );
	dc.LineTo( gm * p1 );
	dc.LineTo( gm * p2 );
	dc.LineTo( gm * p0 );
*/
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
