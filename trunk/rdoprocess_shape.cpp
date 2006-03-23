#include "stdafx.h"
#include "rdoprocess_shape.h"
#include "rdoprocess_object_flowchart.h"
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
	RPObjectMatrix( _parent, _name ),
	pcmd( pcmd_none )
//	snap_to_point( 0, 0 )
{
}

RPShape::~RPShape()
{
}

RPObjectChart* RPShape::find( const rp::point& global_chart_pos )
{
	if ( pointInPolygon(global_chart_pos) || pointInNCArea(global_chart_pos) ) {
		return this;
	}
	return RPObjectMatrix::find( global_chart_pos );
}

rp::rect RPShape::getMaxRect()
{
	rp::rect rect = RPObjectMatrix::getMaxRect();
	double max_x = rect.getMaxX();
	double max_y = rect.getMaxY();
	double min_x = rect.getMinX();
	double min_y = rect.getMinY();
	transformToGlobal();
	pa_global.extendByPerimetr( static_cast<double>(main_pen_width) / 2.0 );
	if ( pa_global.getMaxX() > max_x ) max_x = pa_global.getMaxX();
	if ( pa_global.getMaxY() > max_y ) max_y = pa_global.getMaxY();
	if ( pa_global.getMinX() < min_x ) min_x = pa_global.getMinX();
	if ( pa_global.getMinY() < min_y ) min_y = pa_global.getMinY();
	return rp::rect( min_x, min_y, max_x, max_y );
}

RPProject::Cursor RPShape::getCursor( const rp::point& global_chart_pos )
{
	RPProject::Cursor cursor = RPObjectMatrix::getCursor( global_chart_pos );
	if ( cursor != RPProject::cursor_flow_select ) return cursor;

	if ( isRotateCenter( global_chart_pos ) ) return RPProject::cursor_flow_rotate_center;
	if ( pointInPolygon(global_chart_pos) || pointInNCArea(global_chart_pos) ) {
		RPShape::PossibleCommand pcmd = getPossibleCommand( global_chart_pos, true );
		if ( isSelected() ) {
			switch ( pcmd ) {
				case RPShape::pcmd_move         : return RPProject::cursor_flow_move;
				case RPShape::pcmd_rotate_tl    : return RPProject::cursor_flow_rotate_tl;
				case RPShape::pcmd_rotate_tr    : return RPProject::cursor_flow_rotate_tr;
				case RPShape::pcmd_rotate_bl    : return RPProject::cursor_flow_rotate_bl;
				case RPShape::pcmd_rotate_br    : return RPProject::cursor_flow_rotate_br;
				case RPShape::pcmd_scale_l      :
				case RPShape::pcmd_scale_r      : return RPProject::cursor_flow_scale_lr;
				case RPShape::pcmd_scale_t      :
				case RPShape::pcmd_scale_b      : return RPProject::cursor_flow_scale_tb;
				case RPShape::pcmd_scale_tl     :
				case RPShape::pcmd_scale_br     : return RPProject::cursor_flow_scale_tlbr;
				case RPShape::pcmd_scale_tr     :
				case RPShape::pcmd_scale_bl     : return RPProject::cursor_flow_scale_trbl;
			}
		} else {
			switch ( pcmd ) {
				case RPShape::pcmd_move         :
				case RPShape::pcmd_rotate_tl    :
				case RPShape::pcmd_rotate_tr    :
				case RPShape::pcmd_rotate_bl    :
				case RPShape::pcmd_rotate_br    :
				case RPShape::pcmd_scale_l      :
				case RPShape::pcmd_scale_r      :
				case RPShape::pcmd_scale_t      :
				case RPShape::pcmd_scale_b      :
				case RPShape::pcmd_scale_tl     :
				case RPShape::pcmd_scale_br     :
				case RPShape::pcmd_scale_tr     :
				case RPShape::pcmd_scale_bl     : return RPProject::cursor_flow_move;
			}
		}
	}
	return RPProject::cursor_flow_select;
}

bool RPShape::pointInNCArea( const rp::point& global_chart_pos )
{
	if ( !isSelected() ) return false;
	switch ( getPossibleCommand( global_chart_pos ) ) {
		case RPShape::pcmd_rotate_center:
		case RPShape::pcmd_rotate_tl    :
		case RPShape::pcmd_rotate_tr    :
		case RPShape::pcmd_rotate_bl    :
		case RPShape::pcmd_rotate_br    :
		case RPShape::pcmd_scale_l      :
		case RPShape::pcmd_scale_r      :
		case RPShape::pcmd_scale_t      :
		case RPShape::pcmd_scale_b      :
		case RPShape::pcmd_scale_tl     :
		case RPShape::pcmd_scale_br     :
		case RPShape::pcmd_scale_tr     :
		case RPShape::pcmd_scale_bl     : return true;
	}
	return false;
}

void RPShape::setPosition( int x, int y )
{
	RPObjectMatrix::setPosition( x, y );
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
	CBrush brush( RGB(0xFF, 0xFF, 0xA0) );
	CBrush* old_brush = dc.SelectObject( &brush );
//	dc.BeginPath();
	if ( pa_global.isPolygon() ) {
		dc.Polygon( &pa_global.getWinPolyline()[0], pa_global.size() );
	} else {
		dc.Polyline( &pa_global.getWinPolyline()[0], pa_global.size() );
	}
//	dc.EndPath();
//	dc.StrokePath();
	dc.SelectObject( old_brush );
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
	RPObjectMatrix::draw( dc );

	// Перевод фигуры в глобальные координаты
	transformToGlobal();

	// Отрисовка полигона
	drawPolyline( dc );
/*
	rp::matrix gm = globalMatrix();
	CPen pen1( PS_SOLID, 1, RGB(-1,0,0) );
	dc.SelectObject( pen1 );
	dc.MoveTo( gm * CPoint(-70,0) );
	dc.LineTo( gm * CPoint(70,0) );
	CPen pen2( PS_SOLID, 1, RGB(0,-1,0) );
	dc.SelectObject( pen2 );
	dc.MoveTo( gm * CPoint(0,-70) );
	dc.LineTo( gm * CPoint(0,70) );
	dc.DrawText( rp::string::format( "alpha = %f", getRotationGlobal() ).c_str(), CRect( gm * CPoint(0,70), CSize(100,100)), DT_SINGLELINE );
//	dc.DrawText( rp::string::format( "alpha = %f", rotation_alpha ).c_str(), CRect( gm * CPoint(0,70), CSize(100,100)), DT_SINGLELINE );

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
*/
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
	RPObjectMatrix::draw_selected( dc );

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
	RPObjectFlowChart* flowchart = flowChart();
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
		int radius = RPObjectFlowChart::getSensitivity();
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
		int radius = RPObjectFlowChart::getSensitivity();
		dc.SelectObject( pen_red );
		dc.SelectObject( brush_white );
		dc.Ellipse( center.x - radius, center.y - radius, center.x + radius, center.y + radius );
	}
}

RPShape::PossibleCommand RPShape::getPossibleCommand( const rp::point& global_chart_pos, bool for_cursor )
{
	// Отдельно проверим на перемещение центра вращения. Он отрисовывается поверх выделения, значит и проверяться должен первым.
	if ( isRotateCenter( global_chart_pos ) ) return RPShape::pcmd_rotate_center;
	rp::rect rect = getBoundingRect();
	int sensitivity = RPObjectFlowChart::getSensitivity();
	angle90 a90 = getAngle90();
	angle45 a45 = getAngle45();
	// Отдельно проверим на растяжение за правый нижний угол, т.к. фигуру, сжатую в ноль, лучше растягивать из него
	if ( rpapp.project().getFlowState() == RPProject::flow_select ) {
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_br;
				}
			}
		}
		// Отдельно проверим на растяжение за левый нижний угол, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать именно из него, а не в лево-вверх
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_bl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_bl;
				}
			}
		}
	}
	bool any = rpapp.project().getFlowState() == RPProject::flow_select || rpapp.project().getFlowState() == RPProject::flow_rotate;
	// Общая часть и для перемещения и для вращения
	if ( any ) {
		// Отдельно проверим на растяжение за нижний центр, т.к. фигуру, сжатую в горизонтальную линию, лучше растягивать вниз за него, а не вверх
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_b;
				}
			}
		}
		// Отдельно проверим на растяжение за правый центр, т.к. фигуру, сжатую в вертикальную линию, лучше растягивать вправо за него, а не влево
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_180: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_180: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_270: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_270: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_90 : return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_90 : return RPShape::pcmd_scale_r;
				}
			}
		}
		// Для остальных (нижний центр тоже проверяется)
		if ( rp::math::getLength( rect.p_r(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_r;
					case angle45_90 : return RPShape::pcmd_scale_t;
					case angle45_180: return RPShape::pcmd_scale_l;
					case angle45_270: return RPShape::pcmd_scale_b;
					case angle45_45 : return RPShape::pcmd_scale_tr;
					case angle45_135: return RPShape::pcmd_scale_tl;
					case angle45_225: return RPShape::pcmd_scale_bl;
					case angle45_315: return RPShape::pcmd_scale_br;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_r;
					case angle90_90 : return RPShape::pcmd_scale_t;
					case angle90_180: return RPShape::pcmd_scale_l;
					case angle90_270: return RPShape::pcmd_scale_b;
				}
			}
		}
		if ( rp::math::getLength( rect.p_l(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_l;
					case angle45_90 : return RPShape::pcmd_scale_b;
					case angle45_180: return RPShape::pcmd_scale_r;
					case angle45_270: return RPShape::pcmd_scale_t;
					case angle45_45 : return RPShape::pcmd_scale_bl;
					case angle45_135: return RPShape::pcmd_scale_br;
					case angle45_225: return RPShape::pcmd_scale_tr;
					case angle45_315: return RPShape::pcmd_scale_tl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_l;
					case angle90_90 : return RPShape::pcmd_scale_b;
					case angle90_180: return RPShape::pcmd_scale_r;
					case angle90_270: return RPShape::pcmd_scale_t;
				}
			}
		}
		if ( rp::math::getLength( rect.p_t(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_t;
					case angle45_90 : return RPShape::pcmd_scale_l;
					case angle45_180: return RPShape::pcmd_scale_b;
					case angle45_270: return RPShape::pcmd_scale_r;
					case angle45_45 : return RPShape::pcmd_scale_tl;
					case angle45_135: return RPShape::pcmd_scale_bl;
					case angle45_225: return RPShape::pcmd_scale_br;
					case angle45_315: return RPShape::pcmd_scale_tr;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_t;
					case angle90_90 : return RPShape::pcmd_scale_l;
					case angle90_180: return RPShape::pcmd_scale_b;
					case angle90_270: return RPShape::pcmd_scale_r;
				}
			}
		}
		if ( rp::math::getLength( rect.p_b(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_b;
					case angle45_90 : return RPShape::pcmd_scale_r;
					case angle45_180: return RPShape::pcmd_scale_t;
					case angle45_270: return RPShape::pcmd_scale_l;
					case angle45_45 : return RPShape::pcmd_scale_br;
					case angle45_135: return RPShape::pcmd_scale_tr;
					case angle45_225: return RPShape::pcmd_scale_tl;
					case angle45_315: return RPShape::pcmd_scale_bl;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_b;
					case angle90_90 : return RPShape::pcmd_scale_r;
					case angle90_180: return RPShape::pcmd_scale_t;
					case angle90_270: return RPShape::pcmd_scale_l;
				}
			}
		}
	}
	if ( rpapp.project().getFlowState() == RPProject::flow_select ) {
		// Только при перемещении
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_tl;
					case angle45_90 : return RPShape::pcmd_scale_bl;
					case angle45_180: return RPShape::pcmd_scale_br;
					case angle45_270: return RPShape::pcmd_scale_tr;
					case angle45_45 : return RPShape::pcmd_scale_l;
					case angle45_135: return RPShape::pcmd_scale_b;
					case angle45_225: return RPShape::pcmd_scale_r;
					case angle45_315: return RPShape::pcmd_scale_t;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_tl;
					case angle90_90 : return RPShape::pcmd_scale_bl;
					case angle90_180: return RPShape::pcmd_scale_br;
					case angle90_270: return RPShape::pcmd_scale_tr;
				}
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_tr;
					case angle45_90 : return RPShape::pcmd_scale_tl;
					case angle45_180: return RPShape::pcmd_scale_bl;
					case angle45_270: return RPShape::pcmd_scale_br;
					case angle45_45 : return RPShape::pcmd_scale_t;
					case angle45_135: return RPShape::pcmd_scale_l;
					case angle45_225: return RPShape::pcmd_scale_b;
					case angle45_315: return RPShape::pcmd_scale_r;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_tr;
					case angle90_90 : return RPShape::pcmd_scale_tl;
					case angle90_180: return RPShape::pcmd_scale_bl;
					case angle90_270: return RPShape::pcmd_scale_br;
				}
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_bl;
					case angle45_90 : return RPShape::pcmd_scale_br;
					case angle45_180: return RPShape::pcmd_scale_tr;
					case angle45_270: return RPShape::pcmd_scale_tl;
					case angle45_45 : return RPShape::pcmd_scale_b;
					case angle45_135: return RPShape::pcmd_scale_r;
					case angle45_225: return RPShape::pcmd_scale_t;
					case angle45_315: return RPShape::pcmd_scale_l;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_bl;
					case angle90_90 : return RPShape::pcmd_scale_br;
					case angle90_180: return RPShape::pcmd_scale_tr;
					case angle90_270: return RPShape::pcmd_scale_tl;
				}
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			if ( for_cursor ) {
				switch ( a45 ) {
					case angle45_0  : return RPShape::pcmd_scale_br;
					case angle45_90 : return RPShape::pcmd_scale_tr;
					case angle45_180: return RPShape::pcmd_scale_tl;
					case angle45_270: return RPShape::pcmd_scale_bl;
					case angle45_45 : return RPShape::pcmd_scale_r;
					case angle45_135: return RPShape::pcmd_scale_t;
					case angle45_225: return RPShape::pcmd_scale_l;
					case angle45_315: return RPShape::pcmd_scale_b;
				}
			} else {
				switch ( a90 ) {
					case angle90_0  : return RPShape::pcmd_scale_br;
					case angle90_90 : return RPShape::pcmd_scale_tr;
					case angle90_180: return RPShape::pcmd_scale_tl;
					case angle90_270: return RPShape::pcmd_scale_bl;
				}
			}
		}
	} else if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		// Только при вращении
		if ( rp::math::getLength( rect.p_tl(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_tl;
				case angle90_90 : return RPShape::pcmd_rotate_bl;
				case angle90_180: return RPShape::pcmd_rotate_br;
				case angle90_270: return RPShape::pcmd_rotate_tr;
			}
		}
		if ( rp::math::getLength( rect.p_tr(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_tr;
				case angle90_90 : return RPShape::pcmd_rotate_tl;
				case angle90_180: return RPShape::pcmd_rotate_bl;
				case angle90_270: return RPShape::pcmd_rotate_br;
			}
		}
		if ( rp::math::getLength( rect.p_bl(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_bl;
				case angle90_90 : return RPShape::pcmd_rotate_br;
				case angle90_180: return RPShape::pcmd_rotate_tr;
				case angle90_270: return RPShape::pcmd_rotate_tl;
			}
		}
		if ( rp::math::getLength( rect.p_br(), global_chart_pos ) <= sensitivity ) {
			switch ( a90 ) {
				case angle90_0  : return RPShape::pcmd_rotate_br;
				case angle90_90 : return RPShape::pcmd_rotate_tr;
				case angle90_180: return RPShape::pcmd_rotate_tl;
				case angle90_270: return RPShape::pcmd_rotate_bl;
			}
		}
	}
	// Общая часть и для перемещения и для вращения
	if ( any ) {
		if ( pointInPolygon( global_chart_pos ) ) {
			return RPShape::pcmd_move;
		}
	}
	return RPShape::pcmd_none;
}

void RPShape::getScaleDelta( rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd )
{
	switch ( pcmd ) {
		case RPShape::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_90 : delta.y = -delta.y; break;
				case RPShape::angle90_180: break;
				case RPShape::angle90_270: delta.x = -delta.x; break;
			}
			break;
		}
		case RPShape::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = -delta.y; break;
				case RPShape::angle90_90 : break;
				case RPShape::angle90_180: delta.x = -delta.x; break;
				case RPShape::angle90_270: delta.x = -delta.x; delta.y = -delta.y; break;
			}
			break;
		}
		case RPShape::pcmd_scale_br: {
			switch ( a90 ) {
				case RPShape::angle90_0  : break;
				case RPShape::angle90_90 : delta.x = -delta.x; break;
				case RPShape::angle90_180: delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_270: delta.y = -delta.y; break;
			}
			break;
		}
		case RPShape::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; break;
				case RPShape::angle90_90 : delta.x = -delta.x; delta.y = -delta.y; break;
				case RPShape::angle90_180: delta.y = -delta.y; break;
				case RPShape::angle90_270: break;
			}
			break;
		}
		case RPShape::pcmd_scale_r: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_90 : delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_180: delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_270: delta.y = -delta.y; delta.x = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_l: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_90 : delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_180: delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_270: delta.y = delta.y ; delta.x = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_t: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_90 : delta.x = delta.x ; delta.y = 0; break;
				case RPShape::angle90_180: delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_270: delta.x = -delta.x; delta.y = 0; break;
			}
			break;
		}
		case RPShape::pcmd_scale_b: {
			switch ( a90 ) {
				case RPShape::angle90_0  : delta.y = delta.y ; delta.x = 0; break;
				case RPShape::angle90_90 : delta.x = -delta.x; delta.y = 0; break;
				case RPShape::angle90_180: delta.y = -delta.y; delta.x = 0; break;
				case RPShape::angle90_270: delta.x = delta.x ; delta.y = 0; break;
			}
			break;
		}
	}
}

void RPShape::getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPShape::angle90 a90, RPShape::PossibleCommand pcmd )
{
	switch ( pcmd ) {
		case RPShape::pcmd_scale_tl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_tr: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_br: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_bl: {
			switch ( a90 ) {
				case RPShape::angle90_0  : {
					delta.x = rect_old.p_tr().x - rect_new.p_tr().x;
					delta.y = rect_old.p_tr().y - rect_new.p_tr().y;
					break;
				}
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_br().x - rect_new.p_br().x;
					delta.y = rect_old.p_br().y - rect_new.p_br().y;
					break;
				}
				case RPShape::angle90_180: {
					delta.x = rect_old.p_bl().x - rect_new.p_bl().x;
					delta.y = rect_old.p_bl().y - rect_new.p_bl().y;
					break;
				}
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_r: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_90 : {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_180:
				case RPShape::angle90_270: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_l: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_90 : {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
				case RPShape::angle90_180:
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_t: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_270: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
				case RPShape::angle90_90 :
				case RPShape::angle90_180: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
			}
			break;
		}
		case RPShape::pcmd_scale_b: {
			switch ( a90 ) {
				case RPShape::angle90_0  :
				case RPShape::angle90_270: {
					delta.x = rect_old.p_tl().x - rect_new.p_tl().x;
					delta.y = rect_old.p_tl().y - rect_new.p_tl().y;
					break;
				}
				case RPShape::angle90_90 :
				case RPShape::angle90_180: {
					delta.x = rect_new.p_tl().x - rect_old.p_tl().x;
					delta.y = rect_new.p_tl().y - rect_old.p_tl().y;
					break;
				}
			}
			break;
		}
	}
}

void RPShape::command_before( const rp::point& global_chart_pos )
{
	RPObjectMatrix::command_before( global_chart_pos );
	pcmd = getPossibleCommand( global_chart_pos );
}

void RPShape::setPositionPostDelta( double posx, double posy )
{
	std::list< RPObjectChart* > objects;
	getChartObjects( objects );
	std::list< RPObjectChart* >::iterator it = objects.begin();
	while ( it != objects.end() ) {
		if ( (*it)->isShape() ) {
			RPShape* obj = static_cast<RPShape*>(*it);
//			obj->setPositionPostDelta( posx, posy );
//			obj->RPObjectMatrix::setPositionPost( posx, posy );
		}
		it++;
	}
	setPositionPost( posx, posy );
}

void RPShape::setPositionPost( double posx, double posy )
{
	RPObjectMatrix::setPositionPost( posx, posy );
}

void RPShape::command_make( const rp::point& global_chart_pos )
{
	RPObjectMatrix::command_make( global_chart_pos );

	RPObjectFlowChart* flowchart = flowChart();
	RPShape::angle90 a90 = getAngle90();
	bool horz = a90 == RPShape::angle90_0 || a90 == RPShape::angle90_180;
	rp::point mouse_delta = flowchart->mouse_delta();
	switch ( pcmd ) {
		case RPShape::pcmd_move: {
			mouse_delta = parentRotate().obr() * mouse_delta;
			matrix_transform.dx() += mouse_delta.x;
			matrix_transform.dy() += mouse_delta.y;
			modify();
			update();
			break;
		}
		case RPShape::pcmd_rotate_center: {
			setRotateCenter( global_chart_pos );
			update();
			break;
		}
		case RPShape::pcmd_rotate_tl    :
		case RPShape::pcmd_rotate_tr    :
		case RPShape::pcmd_rotate_bl    :
		case RPShape::pcmd_rotate_br    : {
			setRotation( getRotation() + rp::math::getAlpha( flowchart->mouse_prev(), getRotateCenter(), flowchart->mouse_current() ) );
			update();
			break;
		}
		case RPShape::pcmd_scale_t      :
		case RPShape::pcmd_scale_b      :
		case RPShape::pcmd_scale_l      :
		case RPShape::pcmd_scale_r      :
		case RPShape::pcmd_scale_tl     :
		case RPShape::pcmd_scale_tr     :
		case RPShape::pcmd_scale_bl     :
		case RPShape::pcmd_scale_br     : {
			rp::rect  rect = getBoundingRect();
			rp::point point_delta( globalRotate().obr() * mouse_delta );
			rp::point len( rp::math::getLength( rect.p_tl(), rect.p_tr() ), rp::math::getLength( rect.p_tl(), rect.p_bl() ) );
			RPShape::getScaleDelta( point_delta, a90, pcmd );
			if ( len.x + point_delta.x < 0 ) {
				point_delta.x = -len.x;
			}
			if ( len.y + point_delta.y < 0 ) {
				point_delta.y = -len.y;
			}
			setScaleX( getScaleX() * (len.x + point_delta.x ) / len.x );
			setScaleY( getScaleY() * (len.y + point_delta.y ) / len.y );
			RPShape::getRectDelta( rect, getBoundingRect(), point_delta, a90, pcmd );
			point_delta = globalRotate().obr() * point_delta;
//			setPositionPostDelta( getPostX() + point_delta.x, getPostY() + point_delta.y );
			setPostX( getPostX() + point_delta.x );
			setPostY( getPostY() + point_delta.y );
			setRotateCenterLocalDelta( point_delta.x, point_delta.y );
			update();
			break;
		}
	}
}
