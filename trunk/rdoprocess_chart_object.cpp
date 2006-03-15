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

RPProject::Cursor RPChartObject::getCursor( const rp::point& global_pos )
{
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		RPProject::Cursor cursor = (*it)->getCursor( global_pos );
		if ( cursor != RPProject::cursor_flow_select ) return cursor;
		it++;
	}
	return RPProject::cursor_flow_select;
}

void RPChartObject::draw( CDC& dc )
{
	// Перевод фигуры в глобальные координаты
	transformToGlobal();

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

void RPChartObject::draw_selected( CDC& dc )
{
	// Вызов отрисовки потомков
	std::list< RPChartObject* > objects;
	getChartObjects( objects );
	std::list< RPChartObject* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		if ( (*it)->isSelected() ) {
			int saved = dc.SaveDC();
			(*it)->draw_selected( dc );
			dc.RestoreDC( saved );
		}
		it++;
	}
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

void RPChartObject::moving( int dx, int dy )
{
	matrix_transform.dx() += dx;
	matrix_transform.dy() += dy;
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

	rp::matrix m = m_rotate * m_delta * parentMatrix();
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

bool RPChartObject::isRotateCenter( const rp::point& point ) const
{
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate && isSelected() ) {
		return rp::math::getLength( getRotateCenter(), point ) <= RPFlowChartObject::getSensitivity();
	}
	return false;
}

void RPChartObject::getScaleDelta( rp::point& delta, RPChartObject::angle90 a90, RPChartObject::PossibleCommand pcmd )
{
	switch ( pcmd ) {
		case RPChartObject::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPChartObject::angle90_90 : delta.y = -delta.y; break;
				case RPChartObject::angle90_180: break;
				case RPChartObject::angle90_270: delta.x = -delta.x; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.y = -delta.y; break;
				case RPChartObject::angle90_90 : break;
				case RPChartObject::angle90_180: delta.x = -delta.x; break;
				case RPChartObject::angle90_270: delta.x = -delta.x; delta.y = -delta.y; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_br: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : break;
				case RPChartObject::angle90_90 : delta.x = -delta.x; break;
				case RPChartObject::angle90_180: delta.x = -delta.x; delta.y = -delta.y; break;
				case RPChartObject::angle90_270: delta.y = -delta.y; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.x = -delta.x; break;
				case RPChartObject::angle90_90 : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPChartObject::angle90_180: delta.y = -delta.y; break;
				case RPChartObject::angle90_270: break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_r: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.x = delta.x ; delta.y = 0; break;
				case RPChartObject::angle90_90 : delta.y = delta.y ; delta.x = 0; break;
				case RPChartObject::angle90_180: delta.x = -delta.x; delta.y = 0; break;
				case RPChartObject::angle90_270: delta.y = -delta.y; delta.x = 0; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_l: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.x = -delta.x; delta.y = 0; break;
				case RPChartObject::angle90_90 : delta.y = -delta.y; delta.x = 0; break;
				case RPChartObject::angle90_180: delta.x = delta.x ; delta.y = 0; break;
				case RPChartObject::angle90_270: delta.y = delta.y ; delta.x = 0; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_t: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.y = -delta.y; delta.x = 0; break;
				case RPChartObject::angle90_90 : delta.x = delta.x ; delta.y = 0; break;
				case RPChartObject::angle90_180: delta.y = delta.y ; delta.x = 0; break;
				case RPChartObject::angle90_270: delta.x = -delta.x; delta.y = 0; break;
			}
			break;
		}
		case RPChartObject::pcmd_scale_b: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : delta.y = delta.y ; delta.x = 0; break;
				case RPChartObject::angle90_90 : delta.x = -delta.x; delta.y = 0; break;
				case RPChartObject::angle90_180: delta.y = -delta.y; delta.x = 0; break;
				case RPChartObject::angle90_270: delta.x = delta.x ; delta.y = 0; break;
			}
			break;
		}
	}
}

void RPChartObject::getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPChartObject::angle90 a90, RPChartObject::PossibleCommand pcmd )
{
	switch ( pcmd ) {
		case RPChartObject::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPChartObject::angle90_90 : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPChartObject::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPChartObject::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPChartObject::angle90_180: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_br: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPChartObject::angle90_90 : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPChartObject::angle90_180: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPChartObject::angle90_90 : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPChartObject::angle90_180: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_r: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  :
				case RPChartObject::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPChartObject::angle90_180:
				case RPChartObject::angle90_270: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_l: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  :
				case RPChartObject::angle90_90 : {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
				case RPChartObject::angle90_180:
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_t: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  :
				case RPChartObject::angle90_270: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
				case RPChartObject::angle90_90 :
				case RPChartObject::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPChartObject::pcmd_scale_b: {
			switch ( a90 ) {
				case RPChartObject::angle90_0  :
				case RPChartObject::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPChartObject::angle90_90 :
				case RPChartObject::angle90_180: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
			}
			break;
		}
	}
}
