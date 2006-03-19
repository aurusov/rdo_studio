#include "stdafx.h"
#include "rdoprocess_object_matrix.h"
#include "rdoprocess_object_flowchart.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPObjectMatrix
// ----------------------------------------------------------------------------
RPObjectMatrix::RPObjectMatrix( RPObject* _parent, const rp::string& _name ):
	RPObjectChart( _parent, _name ),
	rotate_center( 0, 0 ),
	rotate_center_inited( false ),
	rotation_alpha( 0 )
{
}

RPObjectMatrix::~RPObjectMatrix()
{
}

void RPObjectMatrix::setPosition( double posx, double posy )
{
	matrix_transform.dx() = posx;
	matrix_transform.dy() = posy;
	modify();
	if (flowChart()) flowChart()->snapToGrid( this );
}

void RPObjectMatrix::setPositionPost( double posx, double posy )
{
	matrix_transform_post.dx() = posx;
	matrix_transform_post.dy() = posy;
	modify();
}

void RPObjectMatrix::setScale( double sx, double sy ) 
{
	if ( sx >= 0 && sx < 1e-2 ) sx = 1e-2;
	if ( sy >= 0 && sy < 1e-2 ) sy = 1e-2;
	matrix_scale.sx() = sx;
	matrix_scale.sy() = sy;
	modify();
}

void RPObjectMatrix::setRotation( double alpha )
{
	while ( alpha < 0 ) alpha += 360.0;
	while ( alpha >= 360 ) alpha -= 360.0;
	double alpha_delta = alpha - rotation_alpha;
	rotation_alpha = alpha;

	rp::matrix m_rotate;
	RPObjectMatrix::fillRotateMatrix( m_rotate, alpha_delta );

	rp::matrix r_center;
	r_center.dx() = -rotate_center.x;
	r_center.dy() = -rotate_center.y;

	matrix_rotate = r_center.obr() * m_rotate * r_center * matrix_rotate;
	modify();
}

void RPObjectMatrix::setRotateCenterLocalDelta( double dx, double dy )
{
	rp::matrix m_delta;
	m_delta.dx() = dx;
	m_delta.dy() = dy;

	rp::matrix m_rotate;
	RPObjectMatrix::fillRotateMatrix( m_rotate, rotation_alpha );

	rp::matrix m = globalRotate() * m_delta;
	rp::point point = getRotateCenter();
	point.x += m.dx();
	point.y += m.dy();
	setRotateCenter( point );
}

bool RPObjectMatrix::isRotateCenter( const rp::point& global_chart_pos ) const
{
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate && isSelected() ) {
		return rp::math::getLength( getRotateCenter(), global_chart_pos ) <= RPObjectFlowChart::getSensitivity();
	}
	return false;
}
