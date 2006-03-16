#include "stdafx.h"
#include "rdoprocess_shape.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_app.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShape
// ----------------------------------------------------------------------------
RPShape::RPShape( RPObject* _parent, const rp::string& _name ):
	RPChartObject( _parent, _name )
//	snap_to_point( 0, 0 )
{
}

RPShape::~RPShape()
{
}

RPProject::Cursor RPShape::getCursor( const rp::point& global_pos )
{
	RPProject::Cursor cursor = RPChartObject::getCursor( global_pos );
	if ( cursor != RPProject::cursor_flow_select ) return cursor;

	if ( isRotateCenter( global_pos ) ) return RPProject::cursor_flow_rotate_center;
	if ( getBoundingRect().extendByPerimetr( RPFlowChartObject::getSensitivity() ).pointInRect( global_pos ) || pointInPolygon( global_pos ) ) {
		transformToGlobal();
		RPChartObject::PossibleCommand pcmd = getPossibleCommand( global_pos, true );
		if ( isSelected() ) {
			switch ( pcmd ) {
				case RPChartObject::pcmd_move         : return RPProject::cursor_flow_move;
				case RPChartObject::pcmd_rotate_tl    : return RPProject::cursor_flow_rotate_tl;
				case RPChartObject::pcmd_rotate_tr    : return RPProject::cursor_flow_rotate_tr;
				case RPChartObject::pcmd_rotate_bl    : return RPProject::cursor_flow_rotate_bl;
				case RPChartObject::pcmd_rotate_br    : return RPProject::cursor_flow_rotate_br;
				case RPChartObject::pcmd_scale_l      :
				case RPChartObject::pcmd_scale_r      : return RPProject::cursor_flow_scale_lr;
				case RPChartObject::pcmd_scale_t      :
				case RPChartObject::pcmd_scale_b      : return RPProject::cursor_flow_scale_tb;
				case RPChartObject::pcmd_scale_tl     :
				case RPChartObject::pcmd_scale_br     : return RPProject::cursor_flow_scale_tlbr;
				case RPChartObject::pcmd_scale_tr     :
				case RPChartObject::pcmd_scale_bl     : return RPProject::cursor_flow_scale_trbl;
			}
		} else {
			switch ( pcmd ) {
				case RPChartObject::pcmd_move         :
				case RPChartObject::pcmd_rotate_tl    :
				case RPChartObject::pcmd_rotate_tr    :
				case RPChartObject::pcmd_rotate_bl    :
				case RPChartObject::pcmd_rotate_br    :
				case RPChartObject::pcmd_scale_l      :
				case RPChartObject::pcmd_scale_r      :
				case RPChartObject::pcmd_scale_t      :
				case RPChartObject::pcmd_scale_b      :
				case RPChartObject::pcmd_scale_tl     :
				case RPChartObject::pcmd_scale_br     :
				case RPChartObject::pcmd_scale_tr     :
				case RPChartObject::pcmd_scale_bl     : return RPProject::cursor_flow_move;
			}
		}
	}
	return RPProject::cursor_flow_select;
}

void RPShape::setPosition( int x, int y )
{
	RPChartObject::setPosition( x, y );
	if (flowChart()) flowChart()->snapToGrid( this );
}

void RPShape::transformToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

rp::rect RPShape::getBoundingRect( bool global ) const
{
	rp::rect bound_rect;
	if ( !pa_src.empty() ) {
		int x_min = pa_src[0].x;
		int y_min = pa_src[0].y;
		int x_max = pa_src[0].x;
		int y_max = pa_src[0].y;
		std::vector< rp::point >::const_iterator it = pa_src.begin() + 1;
		while ( it != pa_src.end() ) {
			if ( x_min > it->x ) x_min = it->x;
			if ( y_min > it->y ) y_min = it->y;
			if ( x_max < it->x ) x_max = it->x;
			if ( y_max < it->y ) y_max = it->y;
			it++;
		}
		rp::rect rect( x_min, y_min, x_max, y_max );
		if ( global ) rect.transform( globalMatrix() );
		bound_rect = rect;
	}
	return bound_rect;
}

void RPShape::drawPolyline( CDC& dc )
{
	if ( pa_global.size() < 2 ) return;
	dc.SelectObject( main_pen );
	dc.BeginPath();
	if ( pa_global.isPolygon() ) {
		dc.Polygon( &pa_global.getWinPolyline()[0], pa_global.size() );
	} else {
		dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
	}
	dc.EndPath();
	dc.StrokePath();

	rp::matrix gm = globalMatrix();
	CPen pen1( PS_SOLID, 1, RGB(-1,0,0) );
	dc.SelectObject( pen1 );
	dc.MoveTo( gm * CPoint(-70,0) );
	dc.LineTo( gm * CPoint(70,0) );
	CPen pen2( PS_SOLID, 1, RGB(0,-1,0) );
	dc.SelectObject( pen2 );
	dc.MoveTo( gm * CPoint(0,-70) );
	dc.LineTo( gm * CPoint(0,70) );
	dc.DrawText( rp::string::format( "alpha = %f", rotation_alpha ).c_str(), CRect( gm * CPoint(0,70), CSize(100,100)), DT_SINGLELINE );
}

/*
void RPShape::drawConnectorsInput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conI.size(); i++ ) {
			painter.drawEllipse( conI.point(i).x() - 3, conI.point(i).y() - 3, 7, 7 );
		}
	}
}

void RPShape::drawConnectorsOutput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conO.size(); i++ ) {
			painter.drawEllipse( conO.point(i).x() - 3, conO.point(i).y() - 3, 7, 7 );
		}
	}
}
*/

void RPShape::draw( CDC& dc )
{
	RPChartObject::draw( dc );
	drawPolyline( dc );

/*
	if ( flowChart->getShowShapeName() ) {
		painter.setPen( flowChart->getShapeColor() );
		int name_x = getX() + flowChart->getPaperBorder();
		int name_y = getY() + flowChart->getPaperBorder();
		painter.drawText( name_x, name_y, size.width(), size.height()-1, Qt::AlignHCenter | Qt::AlignVCenter | Qt::SingleLine, name );
	}
	drawConnectorsInput( painter );
	drawConnectorsOutput( painter );
*/
}

void RPShape::draw_selected( CDC& dc )
{
	RPChartObject::draw_selected( dc );

	// Прямоугольник вокруг фигуры
	rp::rect rect = getBoundingRect();
	int x0 = rect.p0().x;
	int x1 = rect.p1().x;
	int x2 = rect.p2().x;
	int x3 = rect.p3().x;
	int y0 = rect.p0().y;
	int y1 = rect.p1().y;
	int y2 = rect.p2().y;
	int y3 = rect.p3().y;
	RPFlowChartObject* flowchart = flowChart();
	dc.SelectObject( flowchart->getPenSelectedLine() );
	dc.MoveTo( x0, y0 );
	dc.LineTo( x1, y1 );
	dc.LineTo( x2, y2 );
	dc.LineTo( x3, y3 );
	dc.LineTo( x0, y0 );
	dc.SelectObject( flowchart->getPenSelectedBox() );
	dc.SelectObject( flowchart->getBrushSelectedBox() );
	int box_size    = flowchart->getSelectBoxSize2() * 2 -1;
	int box_size_2  = flowchart->getSelectBoxSize2();
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		int radius = RPFlowChartObject::getSensitivity();
		dc.Ellipse( x0 - radius, y0 - radius, x0 + radius, y0 + radius );
		dc.Ellipse( x1 - radius, y1 - radius, x1 + radius, y1 + radius );
		dc.Ellipse( x2 - radius, y2 - radius, x2 + radius, y2 + radius );
		dc.Ellipse( x3 - radius, y3 - radius, x3 + radius, y3 + radius );
	} else {
		dc.Rectangle( x0 - box_size_2, y0 - box_size_2, x0 + box_size_2, y0 + box_size_2 );
		dc.Rectangle( x1 - box_size_2, y1 - box_size_2, x1 + box_size_2, y1 + box_size_2 );
		dc.Rectangle( x2 - box_size_2, y2 - box_size_2, x2 + box_size_2, y2 + box_size_2 );
		dc.Rectangle( x3 - box_size_2, y3 - box_size_2, x3 + box_size_2, y3 + box_size_2 );
	}
	dc.Rectangle( (x0 + x1)/2 - box_size_2, (y0 + y1)/2 - box_size_2, (x0 + x1)/2 + box_size_2, (y0 + y1)/2 + box_size_2 );
	dc.Rectangle( (x1 + x2)/2 - box_size_2, (y1 + y2)/2 - box_size_2, (x1 + x2)/2 + box_size_2, (y1 + y2)/2 + box_size_2 );
	dc.Rectangle( (x2 + x3)/2 - box_size_2, (y2 + y3)/2 - box_size_2, (x2 + x3)/2 + box_size_2, (y2 + y3)/2 + box_size_2 );
	dc.Rectangle( (x3 + x0)/2 - box_size_2, (y3 + y0)/2 - box_size_2, (x3 + x0)/2 + box_size_2, (y3 + y0)/2 + box_size_2 );
	if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		// Центр вращения
		rp::point center = getRotateCenter();
		CPen pen_red( PS_SOLID, 1, RGB(0,0,0) );
		CBrush brush_white( RGB(-1,-1,0) );
		int radius = RPFlowChartObject::getSensitivity();
		dc.SelectObject( pen_red );
		dc.SelectObject( brush_white );
		dc.Ellipse( center.x - radius, center.y - radius, center.x + radius, center.y + radius );
	}
}

void RPShape::draw1( CDC& dc )
{
	transformToGlobal();
	pa_global.extendByPerimetr( main_pen_width / 2.0 );
	CPen pen( PS_SOLID, 1, RGB(-1,0,0) );
	dc.SelectObject( pen );
	dc.BeginPath();
	if ( pa_global.isPolygon() ) {
		dc.Polygon( &pa_global.getWinPolyline()[0], pa_global.size() );
	} else {
//		dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
	}
	dc.EndPath();
	dc.StrokePath();
}

RPChartObject::PossibleCommand RPShape::getPossibleCommand( const rp::point& global_pos, bool for_cursor )
{
	// Отдельно проверим на перемещение центра вращения. Он отрисовывается поверх выделения, значит и проверяться должен первым.
	if ( isRotateCenter( global_pos ) ) return RPChartObject::pcmd_rotate_center;
	rp::rect rect = getBoundingRect();
	int sensitivity = RPFlowChartObject::getSensitivity();
	angle90 a90 = getAngle90();
	angle45 a45 = getAngle45();
	double alpha = getRotation();
	// Отдельно проверим на растяжение за правый нижний угол, т.к. фигуру, сжатую в ноль, лучше растягивать из него
	if ( rpapp.project().getFlowState() == RPProject::flow_select ) {
		if ( rp::math::getLength( rect.p_tl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPChartObject::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPChartObject::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPChartObject::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPChartObject::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPChartObject::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPChartObject::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_br;
				}
			}
		}
		// Отдельно проверим на растяжение за левый нижний угол, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать именно из него, а не в лево-вверх
		if ( rp::math::getLength( rect.p_tl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPChartObject::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPChartObject::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPChartObject::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPChartObject::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPChartObject::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPChartObject::pcmd_scale_bl;
				}
			}
		}
	}
	bool any = rpapp.project().getFlowState() == RPProject::flow_select || rpapp.project().getFlowState() == RPProject::flow_rotate;
	// Общая часть и для перемещения и для вращения
	if ( any ) {
		// Отдельно проверим на растяжение за нижний центр, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать вниз за него, а не вверх
		if ( rp::math::getLength( rect.p_r(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPChartObject::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPChartObject::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPChartObject::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPChartObject::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPChartObject::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPChartObject::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_b;
				}
			}
		}
		// Отдельно проверим на растяжение за правый центр, т.к. фигуру, сжатую в вертикальную линию, лучше растягивать вправо за него, а не влево
		if ( rp::math::getLength( rect.p_r(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPChartObject::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPChartObject::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPChartObject::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPChartObject::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPChartObject::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPChartObject::pcmd_scale_r;
				}
			}
		}
		// Для остальных (нижний центр тоже проверяется)
		if ( rp::math::getLength( rect.p_r(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_r;
					case angle45_90 : return RPChartObject::pcmd_scale_t;
					case angle45_180: return RPChartObject::pcmd_scale_l;
					case angle45_270: return RPChartObject::pcmd_scale_b;
					case angle45_45 : return RPChartObject::pcmd_scale_tr;
					case angle45_135: return RPChartObject::pcmd_scale_tl;
					case angle45_225: return RPChartObject::pcmd_scale_bl;
					case angle45_315: return RPChartObject::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_r;
					case angle90_90 : return RPChartObject::pcmd_scale_t;
					case angle90_180: return RPChartObject::pcmd_scale_l;
					case angle90_270: return RPChartObject::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_l;
					case angle45_90 : return RPChartObject::pcmd_scale_b;
					case angle45_180: return RPChartObject::pcmd_scale_r;
					case angle45_270: return RPChartObject::pcmd_scale_t;
					case angle45_45 : return RPChartObject::pcmd_scale_bl;
					case angle45_135: return RPChartObject::pcmd_scale_br;
					case angle45_225: return RPChartObject::pcmd_scale_tr;
					case angle45_315: return RPChartObject::pcmd_scale_tl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_l;
					case angle90_90 : return RPChartObject::pcmd_scale_b;
					case angle90_180: return RPChartObject::pcmd_scale_r;
					case angle90_270: return RPChartObject::pcmd_scale_t;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_t;
					case angle45_90 : return RPChartObject::pcmd_scale_l;
					case angle45_180: return RPChartObject::pcmd_scale_b;
					case angle45_270: return RPChartObject::pcmd_scale_r;
					case angle45_45 : return RPChartObject::pcmd_scale_tl;
					case angle45_135: return RPChartObject::pcmd_scale_bl;
					case angle45_225: return RPChartObject::pcmd_scale_br;
					case angle45_315: return RPChartObject::pcmd_scale_tr;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_t;
					case angle90_90 : return RPChartObject::pcmd_scale_l;
					case angle90_180: return RPChartObject::pcmd_scale_b;
					case angle90_270: return RPChartObject::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_b;
					case angle45_90 : return RPChartObject::pcmd_scale_r;
					case angle45_180: return RPChartObject::pcmd_scale_t;
					case angle45_270: return RPChartObject::pcmd_scale_l;
					case angle45_45 : return RPChartObject::pcmd_scale_br;
					case angle45_135: return RPChartObject::pcmd_scale_tr;
					case angle45_225: return RPChartObject::pcmd_scale_tl;
					case angle45_315: return RPChartObject::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_b;
					case angle90_90 : return RPChartObject::pcmd_scale_r;
					case angle90_180: return RPChartObject::pcmd_scale_t;
					case angle90_270: return RPChartObject::pcmd_scale_l;
				}
			}
		}
	}
	if ( rpapp.project().getFlowState() == RPProject::flow_select ) {
		// Только при перемещении
		if ( rp::math::getLength( rect.p_tl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_tl;
					case angle45_90 : return RPChartObject::pcmd_scale_bl;
					case angle45_180: return RPChartObject::pcmd_scale_br;
					case angle45_270: return RPChartObject::pcmd_scale_tr;
					case angle45_45 : return RPChartObject::pcmd_scale_l;
					case angle45_135: return RPChartObject::pcmd_scale_b;
					case angle45_225: return RPChartObject::pcmd_scale_r;
					case angle45_315: return RPChartObject::pcmd_scale_t;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_tl;
					case angle90_90 : return RPChartObject::pcmd_scale_bl;
					case angle90_180: return RPChartObject::pcmd_scale_br;
					case angle90_270: return RPChartObject::pcmd_scale_tr;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_tr;
					case angle45_90 : return RPChartObject::pcmd_scale_tl;
					case angle45_180: return RPChartObject::pcmd_scale_bl;
					case angle45_270: return RPChartObject::pcmd_scale_br;
					case angle45_45 : return RPChartObject::pcmd_scale_t;
					case angle45_135: return RPChartObject::pcmd_scale_l;
					case angle45_225: return RPChartObject::pcmd_scale_b;
					case angle45_315: return RPChartObject::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_tr;
					case angle90_90 : return RPChartObject::pcmd_scale_tl;
					case angle90_180: return RPChartObject::pcmd_scale_bl;
					case angle90_270: return RPChartObject::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_bl;
					case angle45_90 : return RPChartObject::pcmd_scale_br;
					case angle45_180: return RPChartObject::pcmd_scale_tr;
					case angle45_270: return RPChartObject::pcmd_scale_tl;
					case angle45_45 : return RPChartObject::pcmd_scale_b;
					case angle45_135: return RPChartObject::pcmd_scale_r;
					case angle45_225: return RPChartObject::pcmd_scale_t;
					case angle45_315: return RPChartObject::pcmd_scale_l;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_bl;
					case angle90_90 : return RPChartObject::pcmd_scale_br;
					case angle90_180: return RPChartObject::pcmd_scale_tr;
					case angle90_270: return RPChartObject::pcmd_scale_tl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPChartObject::pcmd_scale_br;
					case angle45_90 : return RPChartObject::pcmd_scale_tr;
					case angle45_180: return RPChartObject::pcmd_scale_tl;
					case angle45_270: return RPChartObject::pcmd_scale_bl;
					case angle45_45 : return RPChartObject::pcmd_scale_r;
					case angle45_135: return RPChartObject::pcmd_scale_t;
					case angle45_225: return RPChartObject::pcmd_scale_l;
					case angle45_315: return RPChartObject::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPChartObject::pcmd_scale_br;
					case angle90_90 : return RPChartObject::pcmd_scale_tr;
					case angle90_180: return RPChartObject::pcmd_scale_tl;
					case angle90_270: return RPChartObject::pcmd_scale_bl;
				}
			}
		}
	} else if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		// Только при вращении
		if ( rp::math::getLength( rect.p_tl(), global_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPChartObject::pcmd_rotate_tl;
				case angle90_90 : return RPChartObject::pcmd_rotate_bl;
				case angle90_180: return RPChartObject::pcmd_rotate_br;
				case angle90_270: return RPChartObject::pcmd_rotate_tr;
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPChartObject::pcmd_rotate_tr;
				case angle90_90 : return RPChartObject::pcmd_rotate_tl;
				case angle90_180: return RPChartObject::pcmd_rotate_bl;
				case angle90_270: return RPChartObject::pcmd_rotate_br;
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPChartObject::pcmd_rotate_bl;
				case angle90_90 : return RPChartObject::pcmd_rotate_br;
				case angle90_180: return RPChartObject::pcmd_rotate_tr;
				case angle90_270: return RPChartObject::pcmd_rotate_tl;
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPChartObject::pcmd_rotate_br;
				case angle90_90 : return RPChartObject::pcmd_rotate_tr;
				case angle90_180: return RPChartObject::pcmd_rotate_tl;
				case angle90_270: return RPChartObject::pcmd_rotate_bl;
			}
		}
	}
	// Общая часть и для перемещения и для вращения
	if ( any ) {
		if ( pointInPolygon( global_pos ) ) {
			return RPChartObject::pcmd_move;
		}
	}
	return RPChartObject::pcmd_none;
}

void RPShape::onLButtonDown( UINT nFlags, CPoint flowchart_mouse_pos )
{
	RPChartObject::onLButtonDown( nFlags, flowchart_mouse_pos );
	TRACE( "onLButtonDown\n" );
}

void RPShape::onLButtonUp( UINT nFlags, CPoint flowchart_mouse_pos )
{
	TRACE( "onLButtonUp\n" );
}

void RPShape::onLButtonDblClk( UINT nFlags, CPoint flowchart_mouse_pos )
{
	TRACE( "onLButtonDblClk\n" );
}

void RPShape::onRButtonDown( UINT nFlags, CPoint flowchart_mouse_pos )
{
	TRACE( "onRButtonDown\n" );
}

void RPShape::onRButtonUp( UINT nFlags, CPoint flowchart_mouse_pos )
{
	TRACE( "onRButtonUp\n" );
}

void RPShape::onMouseMove( UINT nFlags, CPoint flowchart_mouse_pos )
{
	TRACE( "onMouseMove\n" );
}
