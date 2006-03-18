#include "stdafx.h"
#include "rdoprocess_chart_object.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPChartObject
// ----------------------------------------------------------------------------
RPChartObject::RPChartObject( RPObject* _parent, const rp::string& _name ):
	RPObject( _parent, _name ),
	rotate_center( 0, 0 ),
	rotate_center_inited( false ),
	main_pen_width( 20 ),
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

RPFlowChartObject* RPChartObject::flowChart() const
{
	RPChartObject* chart_parent = chartParent();
	RPChartObject* prev = chart_parent;
	while ( chart_parent ) {
		prev = chart_parent;
		chart_parent = chart_parent->chartParent();
	}
	return static_cast<RPFlowChartObject*>(prev);
}

void RPChartObject::modify()
{
	RPChartObject* flowchart = flowChart();
	if ( flowchart ) {
		flowchart->modify();
	}
}

void RPChartObject::update()
{
	RPChartObject* flowchart = flowChart();
	if ( flowchart ) {
		flowchart->update();
	}
}

RPProject::Cursor RPChartObject::getCursor( const rp::point& global_chart_pos )
{
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		RPProject::Cursor cursor = (*it)->getCursor( global_chart_pos );
		if ( cursor != RPProject::cursor_flow_select ) return cursor;
		it++;
	}
	return RPProject::cursor_flow_select;
}

RPChartObject* RPChartObject::find( const rp::point& global_chart_pos )
{
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		RPChartObject* obj = (*it)->find( global_chart_pos );
		if ( obj ) return obj;
		it++;
	}
	return (pointInPolygon(global_chart_pos) || pointInNCArea(global_chart_pos)) ? this : NULL;
}

void RPChartObject::draw( CDC& dc )
{
	// Вызов отрисовки потомков
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		int saved = dc.SaveDC();
		(*it)->draw( dc );
		(*it)->draw1( dc );
		dc.RestoreDC( saved );
		it++;
	}
}

void RPChartObject::draw_after( CDC& dc )
{
	// Вызов отрисовки потомков.
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		int saved = dc.SaveDC();
		(*it)->draw_after( dc );
		dc.RestoreDC( saved );
		it++;
	}
	if ( isSelected() ) draw_selected( dc );
}

void RPChartObject::setPosition( double posx, double posy )
{
	matrix_transform.dx() = posx;
	matrix_transform.dy() = posy;
	modify();
}

void RPChartObject::setPositionPost( double posx, double posy )
{
	matrix_transform_post.dx() = posx;
	matrix_transform_post.dy() = posy;
	modify();
}

void RPChartObject::setScale( double sx, double sy ) 
{
	if ( sx >= 0 && sx < 1e-2 ) sx = 1e-2;
	if ( sy >= 0 && sy < 1e-2 ) sy = 1e-2;
	matrix_scale.sx() = sx;
	matrix_scale.sy() = sy;
	modify();
}

void RPChartObject::setRotation( double alpha )
{
	while ( alpha < 0 ) alpha += 360.0;
	while ( alpha >= 360 ) alpha -= 360.0;
	double alpha_delta = alpha - rotation_alpha;
	rotation_alpha = alpha;

	rp::matrix m_rotate;
	RPChartObject::fillRotateMatrix( m_rotate, alpha_delta );

	rp::matrix r_center;
	r_center.dx() = -rotate_center.x;
	r_center.dy() = -rotate_center.y;

	matrix_rotate = r_center.obr() * m_rotate * r_center * matrix_rotate;
	modify();
}

void RPChartObject::setRotateCenterLocalDelta( double dx, double dy )
{
	rp::matrix m_delta;
	m_delta.dx() = dx;
	m_delta.dy() = dy;

	rp::matrix m_rotate;
	RPChartObject::fillRotateMatrix( m_rotate, rotation_alpha );

	rp::matrix m = m_rotate * m_delta * parentMatrix_noScale();
	rp::point point = getRotateCenter();
	point.x += m.dx();
	point.y += m.dy();
	setRotateCenter( point );
}

void RPChartObject::setSelected( bool value )
{
	bool _sel = isSelected();
	RPObject::setSelected( value );
	if ( _sel != value ) {
		update();
	}
}

bool RPChartObject::isRotateCenter( const rp::point& global_chart_pos ) const
{
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate && isSelected() ) {
		return rp::math::getLength( getRotateCenter(), global_chart_pos ) <= RPFlowChartObject::getSensitivity();
	}
	return false;
}
