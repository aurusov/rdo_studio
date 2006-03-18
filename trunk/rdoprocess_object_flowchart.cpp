#include "stdafx.h"
#include "rdoprocess_object_flowchart.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPObjectFlowChart
// ----------------------------------------------------------------------------
const int RPObjectFlowChart::select_box_size2 = 4;

RPObjectFlowChart::RPObjectFlowChart( RPObject* _parent, RPFlowChart* _flowchart ):
	RPObjectMatrix( _parent, _T("Flowchart") ),
	init_ok( false ),
	border_w( 7 ),
	border_h( 7 ),
	paper_border_w( 7 ),
	paper_border_h( 7 ),
	paper_border( 1 ),
	paper_shadow( 2 ),
	pixmap_w_real( 0 ),
	pixmap_h_real( 0 ),
	pixmap_w_show( 0 ),
	pixmap_h_show( 0 ),
	client_width( 0 ),
	client_height( 0 ),
	pen_black( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	pen_shape_color( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	pen_selected_line( PS_DOT, 1, RGB(0x00, 0xFF, 0x00) ),
	pen_selected_box( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	brush_selected_box( RGB(0x00, 0xFF, 0x00) ),
	paper_border_color( RGB(0xC0, 0xC0, 0xC0) ),
	paper_shadow_color( RGB(0x80, 0x80, 0x80) ),
	paper_bg_color( RGB(0xFF, 0xFF, 0xFF) ),
	mem_bmp( NULL ),
	bmp_first( NULL ),
	font_first( NULL ),
	saved_mem_dc( 0 ),
	global_win_pos_current( 0, 0 ),
	global_win_pos_prev( 0, 0 ),
	one_object( NULL ),
	flowchart( _flowchart )
#ifdef TEST_SPEED
	,
	makepixmap_cnt( 0 )
#endif
{
	rpapp.msg().connect( this, rp::msg::RP_FLOWSTATE_CHANGED );

	CDC* dc = flowchart->GetDC();
	if ( !mem_dc.CreateCompatibleDC( dc ) ) {
		flowchart->ReleaseDC( dc );
		return;
	}
	flowchart->ReleaseDC( dc );

	// Запомнить состояния DC для восстановления перед удалением
	saved_mem_dc = mem_dc.SaveDC();
	font_first   = mem_dc.GetCurrentFont();
	bmp_first    = mem_dc.GetCurrentBitmap();
	if ( !saved_mem_dc ) return;

	mem_dc.SetBkMode( TRANSPARENT );

	init_ok = true;
}

RPObjectFlowChart::~RPObjectFlowChart()
{
	if ( mem_dc.m_hDC ) {
		mem_dc.SelectObject( font_first );
		mem_dc.SelectObject( bmp_first );
		if ( mem_bmp ) {
			mem_bmp->DeleteObject();
			delete mem_bmp;
		}
		mem_dc.RestoreDC( saved_mem_dc );
		mem_dc.DeleteDC();
	}
}

void RPObjectFlowChart::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == rp::msg::RP_FLOWSTATE_CHANGED ) update();
}

void RPObjectFlowChart::modify()
{
}

void RPObjectFlowChart::update()
{
	flowchart->InvalidateRect( NULL );
	flowchart->UpdateWindow();
}

RPProject::Cursor RPObjectFlowChart::getCursor( const rp::point& global_chart_pos )
{
	if ( pointInNCArea( global_chart_pos ) ) return RPProject::cursor_flow_select;

	RPProject::Cursor cursor = RPObjectMatrix::getCursor( global_chart_pos );
	if ( cursor != RPProject::cursor_flow_select ) return cursor;

	switch ( rpapp.project().getFlowState() ) {
		case RPProject::flow_select   : return RPProject::cursor_flow_select;
		case RPProject::flow_connector: return RPProject::cursor_flow_connector;
		case RPProject::flow_rotate   : return RPProject::cursor_flow_rotate;
	}
	return RPProject::cursor_flow_select;
}

bool RPObjectFlowChart::pointInPolygon( const rp::point& global_chart_pos )
{
	return global_chart_pos.x >= 0 && global_chart_pos.y >= 0 && global_chart_pos.x <= pixmap_w_show && global_chart_pos.y <= pixmap_h_show;
}

rp::rect RPObjectFlowChart::getFlowSize( const std::list< RPObjectChart* >& list ) const
{
	double max_x = 0;
	double max_y = 0;
	double min_x = 0;
	double min_y = 0;
	std::list< RPObjectChart* >::const_iterator it = list.begin();
	if ( it != list.end() ) {
		rp::rect rect = (*it)->getMaxRect();
		max_x = rect.getMaxX();
		max_y = rect.getMaxY();
		min_x = rect.getMinX();
		min_y = rect.getMinY();
		it++;
	}
	while ( it != list.end() ) {
		rp::rect rect = (*it)->getMaxRect();
		double _max_x = rect.getMaxX();
		double _max_y = rect.getMaxY();
		double _min_x = rect.getMinX();
		double _min_y = rect.getMinY();
		if ( _max_x > max_x ) max_x = _max_x;
		if ( _max_y > max_y ) max_y = _max_y;
		if ( _min_x < min_x ) min_x = _min_x;
		if ( _min_y < min_y ) min_y = _min_y;
		it++;
	}
	min_x -= matrix_transform.dx_const();
	max_x -= matrix_transform.dx_const();
	min_y -= matrix_transform.dy_const();
	max_y -= matrix_transform.dy_const();
	return rp::rect( min_x, min_y, max_x, max_y );
}

void RPObjectFlowChart::draw( CDC& dc )
{
#ifdef TEST_SPEED // =====================================
	SYSTEMTIME t0;
	SYSTEMTIME t1;
	SYSTEMTIME t2;
	SYSTEMTIME t3;
	SYSTEMTIME t4;
	SYSTEMTIME t5;
	SYSTEMTIME t6;
	SYSTEMTIME t7;
	::GetSystemTime( &t0 );
#endif // ================================================

	std::list< RPObjectChart* > objects;
	getChartObjects( objects );

	CRect client_rect;
	flowchart->GetClientRect( &client_rect );
	client_width  = client_rect.Width();
	client_height = client_rect.Height();

	if ( client_width < border_w * 2 || client_height < border_h * 2 ) {

#ifdef TEST_SPEED // =====================================
		for ( int cnt1 = 0; cnt1 <= base_speed * 10; cnt1++ ) {
#endif // ================================================

		dc.FillSolidRect( 0, 0, client_width, client_height, paper_border_color );

#ifdef TEST_SPEED // =====================================
		}
		::GetSystemTime( &t1 );
#endif // ================================================

	} else {

#ifdef TEST_SPEED // =====================================
		for ( int cnt1 = 0; cnt1 <= base_speed * 10; cnt1++ ) {
#endif // ================================================

		dc.FillSolidRect( 0, 0, border_w - paper_border, client_height, paper_border_color );
		dc.FillSolidRect( client_width - border_w + paper_border + paper_shadow, 0, border_w - paper_border - paper_shadow, client_height, paper_border_color );
		dc.FillSolidRect( border_w - paper_border, 0, client_width - border_w * 2 + paper_border * 2 + paper_shadow, border_h - paper_border, paper_border_color );
		dc.FillSolidRect( border_w - paper_border, client_height - border_h + paper_border + paper_shadow, client_width - border_w * 2 + paper_border * 2 + paper_shadow, border_h - paper_border - paper_shadow, paper_border_color );
		dc.FillSolidRect( border_w - paper_border, client_height - border_h + paper_border, paper_shadow, paper_shadow, paper_border_color );
		dc.FillSolidRect( client_width - border_w + paper_border, border_h - paper_border, paper_shadow, paper_shadow, paper_border_color );

		if ( client_width > border_w * 2 + paper_border_w * 2 && client_height > border_h * 2 + paper_border_h * 2 ) {
			dc.FillSolidRect( border_w, border_h, paper_border_w, client_height - border_h * 2, paper_bg_color );
			dc.FillSolidRect( border_w + paper_border_w, border_h, client_width - border_w * 2 - paper_border_w, paper_border_h, paper_bg_color );
			dc.FillSolidRect( client_width - border_w - paper_border_w, border_h + paper_border_h, paper_border_w, client_height - border_h * 2 - paper_border_h, paper_bg_color );
			dc.FillSolidRect( border_w + paper_border_w, client_height - border_h - paper_border_h, client_width - border_w * 2 - paper_border_w * 2, paper_border_h, paper_bg_color );
		} else {
			dc.FillSolidRect( border_w, border_h, client_width - border_w * 2, client_height - border_h * 2, paper_bg_color );
		}

#ifdef TEST_SPEED // =====================================
		}
		::GetSystemTime( &t1 );
		for ( int cnt2 = 0; cnt2 <= base_speed * 10; cnt2++ ) {
#endif // ================================================

		const int shadow_border_w = 2;
		const int shadow_border_h = 2;
		dc.FillSolidRect( client_width - border_w + paper_border, border_h + shadow_border_h - paper_border, shadow_border_w, client_height - border_h * 2 + shadow_border_w, paper_shadow_color );
		dc.FillSolidRect( border_w + shadow_border_w - paper_border, client_height - border_h + paper_border, client_width - border_w * 2, shadow_border_h, paper_shadow_color );
		dc.SelectObject( pen_black );
		dc.MoveTo( border_w - 1, border_h - 1 );
		dc.LineTo( client_width - border_w, border_h - 1 );
		dc.LineTo( client_width - border_w, client_height - border_h );
		dc.LineTo( border_w - 1, client_height - border_h );
		dc.LineTo( border_w - 1, border_h - 1 );

#ifdef TEST_SPEED // =====================================
		}
		::GetSystemTime( &t2 );
#endif // ================================================

		if ( pixmap_w_show > 0 && pixmap_h_show > 0 ) {

#ifdef TEST_SPEED // =====================================
			for ( int cnt3 = 0; cnt3 <= base_speed; cnt3++ ) {
#endif // ================================================
/*
			if ( grid_show ) {
				int x_start = scroll_x_pos / grid_bmp_width;
				int y_start = scroll_y_pos / grid_bmp_width;
				int x_stop  = x_start + pixmap_w_show / grid_bmp_width + 1;
				int y_stop  = y_start + pixmap_h_show / grid_bmp_width + 1;
#ifdef TEST_SPEED // =====================================
				if ( cnt3 == 0 ) {
#endif // ================================================
				TRACE( "x_start = %d, y_start = %d, x_stop = %d, y_stop = %d\n", x_start, y_start, x_stop, y_stop );
#ifdef TEST_SPEED // =====================================
				}
#endif // ================================================
				for ( int i = x_start; i <= x_stop; i++ ) {
					for ( int j = y_start; j <= y_stop; j++ ) {
						mem_dc.BitBlt( -scroll_x_pos + paper_border_w + i * grid_bmp_width, -scroll_y_pos + paper_border_h + j * grid_bmp_width, grid_bmp_width, grid_bmp_width, &grid_dc, 0, 0, SRCCOPY );
					}
				}
			}
*/
			mem_dc.FillSolidRect( 0, 0, pixmap_w_show, pixmap_h_show, paper_bg_color );

#ifdef TEST_SPEED // =====================================
			}
			::GetSystemTime( &t3 );

			for ( int cnt4 = 0; cnt4 <= base_speed * 10; cnt4++ ) {
				std::list< RPObjectChart* >::iterator it = objects.begin();
				while ( it != objects.end() ) {
					it++;
				}
			}
			::GetSystemTime( &t4 );
#endif // ================================================

//			std::list< RPObjectChart* >::iterator it = objects.begin();
//			while ( it != objects.end() ) {
//				RPObjectChart* object = *it;
//				object->transformToGlobal();
//				it++;
//			}

#ifdef TEST_SPEED // =====================================
			for ( int cnt5 = 0; cnt5 <= base_speed; cnt5++ ) {
#endif // ================================================

			std::list< RPObjectChart* >::iterator it = objects.begin();
			while ( it != objects.end() ) {
				int saved = mem_dc.SaveDC();
				RPObjectChart* obj = *it;
				obj->draw( mem_dc );
				obj->draw1( mem_dc );
				mem_dc.RestoreDC( saved );
				it++;
			}

#ifdef TEST_SPEED // =====================================
			}
			::GetSystemTime( &t5 );
#endif // ================================================

#ifdef TEST_SPEED // =====================================
			for ( int cnt6 = 0; cnt6 <= base_speed * 10; cnt6++ ) {
#endif // ================================================
#ifdef TEST_SPEED // =====================================
			}
			::GetSystemTime( &t6 );
#endif // ================================================

			it = objects.begin();
			while ( it != objects.end() ) {
				RPObjectChart* object = *it;
				int saved = mem_dc.SaveDC();
				object->draw_after( mem_dc );
				mem_dc.RestoreDC( saved );
				it++;
/*
				CPoint snap_to_point = shape->getCenter();
				if ( snap_to_point.x > paper_border_w && snap_to_point.y > paper_border_h && snap_to_point.x <= pixmap_w_show - paper_border_w && snap_to_point.y <= pixmap_h_show - paper_border_h ) {
					CPen pen_red( PS_SOLID, 1, RGB(-1,0,0) );
					CBrush brush_white( RGB(-1,-1,-1) );
					mem_dc.SelectObject( pen_red );
					mem_dc.SelectObject( brush_white );
					mem_dc.Ellipse( snap_to_point.x - 2, snap_to_point.y - 2, snap_to_point.x + 2, snap_to_point.y + 2 );
				}
*/
			}

#ifdef TEST_SPEED // =====================================
			for ( int cnt7 = 0; cnt7 <= base_speed * 5; cnt7++ ) {
#endif // ================================================

			dc.BitBlt( border_w + paper_border_w, border_h + paper_border_h, pixmap_w_show, pixmap_h_show, &mem_dc, 0, 0, SRCCOPY );

#ifdef TEST_SPEED // =====================================
			}
			::GetSystemTime( &t7 );
#endif // ================================================
		}
	}

#ifdef TEST_SPEED // =====================================
	int delay1 = (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds) - (t0.wMinute * 1000 * 60 + t0.wSecond * 1000 + t0.wMilliseconds);
	int delay2 = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	int delay3 = ((t3.wMinute * 1000 * 60 + t3.wSecond * 1000 + t3.wMilliseconds) - (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds)) * 10;
	int delay4 = (t4.wMinute * 1000 * 60 + t4.wSecond * 1000 + t4.wMilliseconds) - (t3.wMinute * 1000 * 60 + t3.wSecond * 1000 + t3.wMilliseconds);
	int delay5 = ((t5.wMinute * 1000 * 60 + t5.wSecond * 1000 + t5.wMilliseconds) - (t4.wMinute * 1000 * 60 + t4.wSecond * 1000 + t4.wMilliseconds)) * 10;
	int delay6 = (t6.wMinute * 1000 * 60 + t6.wSecond * 1000 + t6.wMilliseconds) - (t5.wMinute * 1000 * 60 + t5.wSecond * 1000 + t5.wMilliseconds);
	int delay7 = ((t7.wMinute * 1000 * 60 + t7.wSecond * 1000 + t7.wMilliseconds) - (t6.wMinute * 1000 * 60 + t6.wSecond * 1000 + t6.wMilliseconds)) * 2;
	int delay0 = delay1 + delay2 + delay3 + delay4 + delay5 + delay6 + delay7;
	TRACE( "paint_delay = %d: %d, %d, %d, %d, %d, %d, %d\n", delay0, delay1, delay2, delay3, delay4, delay5, delay6, delay7 );
#endif // ================================================

}

void RPObjectFlowChart::makeNewPixmap()
{
#ifdef TEST_SPEED // =====================================
	makepixmap_cnt++;

	SYSTEMTIME t1;
	SYSTEMTIME t2;
	::GetSystemTime( &t1 );
#endif // ================================================

	int pixmap_w_show_old = pixmap_w_show;
	int pixmap_h_show_old = pixmap_h_show;

#ifdef TEST_SPEED // =====================================
	for ( int cnt = 0; cnt <= base_speed * 10; cnt++ ) {
#endif // ================================================

	CRect client_rect;
	flowchart->GetClientRect( &client_rect );
	client_width  = client_rect.Width();
	client_height = client_rect.Height();

	int new_pixmap_w_real = client_width - border_w * 2;
	int new_pixmap_h_real = client_height - border_h * 2;
	if ( pixmap_w_real < new_pixmap_w_real || pixmap_h_real < new_pixmap_h_real ) {
		if ( mem_bmp ) {
			mem_dc.SelectObject( bmp_first );
			mem_bmp->DeleteObject();
			delete mem_bmp;
			mem_bmp = NULL;
		}
		pixmap_w_real = new_pixmap_w_real;
		pixmap_h_real = new_pixmap_h_real;
		mem_bmp = new CBitmap();
		CDC* dc = flowchart->GetDC();
		mem_bmp->CreateCompatibleBitmap( dc, pixmap_w_real, pixmap_h_real );
		flowchart->ReleaseDC( dc );
		mem_dc.SelectObject( mem_bmp );
	}
	pixmap_w_show = client_width - border_w * 2 - paper_border_w * 2;
	if ( pixmap_w_show > pixmap_w_real ) {
		pixmap_w_show = pixmap_w_real;
	}
	pixmap_h_show = client_height - border_h * 2 - paper_border_h * 2;
	if ( pixmap_h_show > pixmap_h_real ) {
		pixmap_h_show = pixmap_h_real;
	}

	flowchart->updateScrollBars();

#ifdef TEST_SPEED // =====================================
	}
	::GetSystemTime( &t2 );
	int delay = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	TRACE( "makepixmap_delay = %d\n", delay );
#endif // ================================================

	if ( pixmap_w_show_old != pixmap_w_show || pixmap_h_show_old != pixmap_h_show ) {
		flowchart->makeGrid();
	}
}

void RPObjectFlowChart::snapToGrid( RPObjectMatrix* shape )
{
/*
	if ( shape ) {
		if ( grid_pa.empty() ) return;
		CPoint center;
		if ( grid_mode == gtSnapToPoint ) {
			center = CPoint( shape->getSnapToPoint() );
			center.Offset( shape->getX() + paper_border_w, shape->getY() + paper_border_h );
		} else {
			center = shape->getCenter();
			center += CPoint( shape->getX() + paper_border_w, shape->getY() + paper_border_h );
		}
		int c_x = ((center.x - paper_border_w) / grid_step);
		int c_y = ((center.y - paper_border_h) / grid_step);
		bool makeNewGrid = false;
		const int point_cnt = 4;
		std::vector< CPoint > c_pa;
		c_pa.resize( point_cnt );
		if ( c_x < 0 ) {
			c_x = 0;
		}
		if ( c_y < 0 ) {
			c_y = 0;
		}
		if ( grid_type == gtPoints ) {
			if ( c_x > grid_cnt_x ) {
				c_x = grid_cnt_x;
			}
			if ( c_y > grid_cnt_y ) {
				c_y = grid_cnt_y;
			}
			c_pa[0] = grid_pa[ c_x + c_y * grid_cnt_x ];
			c_pa[1] = grid_pa[ c_x + 1 + c_y * grid_cnt_x ];
			c_pa[2] = grid_pa[ c_x + 1 + (c_y + 1) * grid_cnt_x ];
			c_pa[3] = grid_pa[ c_x + (c_y + 1) * grid_cnt_x ];
		} else {
			int x1 = grid_pa[ c_x * 2 ].x;
			int y1 = grid_pa[ grid_cnt_x * 2 + c_y * 2 ].y;
			int x2 = x1 + grid_step;
			int y2 = y1 + grid_step;
			if ( c_x >= grid_cnt_x - grid_dx_plus || c_y >= grid_cnt_y - grid_dy_plus ) {
				makeNewGrid = true;
			}
			c_pa[0] = CPoint( x1, y1 );
			c_pa[1] = CPoint( x2, y1 );
			c_pa[2] = CPoint( x2, y2 );
			c_pa[3] = CPoint( x1, y2 );
		}
		int dx = 0;
		int dy = 0;
		if ( grid_mode == gtSnapToPoint ) {
			double len1 = RPFlowChart::getDistance( c_pa[1], c_pa[2], center );
			double len2 = RPFlowChart::getDistance( c_pa[3], c_pa[0], center );
			if ( len1 < len2 ) {
				dx = c_pa[1].x - center.x;
			} else {
				dx = c_pa[3].x - center.x;
			}
			len1 = RPFlowChart::getDistance( c_pa[0], c_pa[1], center );
			len2 = RPFlowChart::getDistance( c_pa[2], c_pa[3], center );
			if ( len1 < len2 ) {
				dy = c_pa[1].y - center.y;
			} else {
				dy = c_pa[3].y - center.y;
			}
			if ( abs(dx) > grid_delta ) {
				dx = 0;
			}
			if ( abs(dy) > grid_delta ) {
				dy = 0;
			}
		} else {
			dx = ( c_pa[0].x + (c_pa[1].x - c_pa[0].x)/2 ) - center.x;
			dy = ( c_pa[0].y + (c_pa[3].y - c_pa[0].y)/2 ) - center.y;
		}
		if ( (dx || dy) && grid_mode != gtSnapOff ) {
			shape->move( shape->getX() + dx, shape->getY() + dy );
		}
		if ( dx || dy || makeNewGrid ) {
			if ( makeNewGrid ) {
				grid_pa.resize( 0 );
				makeGrid();
				updateScrollBars();
			}
			updateDC();
		}
	}
*/
}

RPObjectChart* RPObjectFlowChart::find( const rp::point& global_chart_pos )
{
	RPObjectChart* obj = RPObjectChart::find( global_chart_pos );
	return obj != this ? obj : NULL;
}

void RPObjectFlowChart::onLButtonDown( UINT nFlags, CPoint local_win_pos )
{
	// Запомнили глобальные координаты мышки (глобальные в виндах, а не 2D движке). Пригодятся в других функциях.
	global_win_pos_prev = local_win_pos;
	flowchart->ClientToScreen( &global_win_pos_prev );
	global_win_pos_current = global_win_pos_prev;
	// Монопольно захватили мышку
	flowchart->SetCapture();
	// Нашли объект под мышкой
	if ( rpapp.project().getFlowState() == RPProject::flow_select || rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		rp::point global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		one_object = find( global_chart_pos );
		if ( one_object ) {
			// Нашли фигуру
			one_object->onLButtonDown( nFlags, CPoint( global_chart_pos.x, global_chart_pos.y ) );
			one_object->command_before( global_chart_pos );
		}
		return;
	}
	// Никого не нашли, выбираем лист
	setSelected( true );
}

void RPObjectFlowChart::onMouseMove( UINT nFlags, CPoint local_win_pos )
{
	// Глобальные коодинаты мышки в винде
	global_win_pos_current = local_win_pos;
	flowchart->ClientToScreen( &global_win_pos_current );

	// Глобальные координаты мышки в 2D движке (на текущем листе)
	rp::point global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	RPObjectChart* obj = find( global_chart_pos );
	// Движение мышки над найденным объектом
	if ( obj ) obj->onMouseMove( nFlags, CPoint( global_chart_pos.x, global_chart_pos.y ) );
	// Выполнение команды над выделенным объектом
	if ( one_object ) one_object->command_make( global_chart_pos );

	global_win_pos_prev = global_win_pos_current;
}

void RPObjectFlowChart::onLButtonUp( UINT nFlags, CPoint local_win_pos )
{
	// Отдали виндам мышку
	::ReleaseCapture();

	if ( one_object ) {
		CPoint global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		one_object->onLButtonUp( nFlags, global_chart_pos );
		one_object = NULL;
	}

	// Обновить скролл
	flowchart->updateScrollBars();
}

void RPObjectFlowChart::onLButtonDblClk( UINT nFlags, CPoint local_win_pos )
{
	CPoint global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	RPObjectChart* obj = find( global_chart_pos );
	if ( obj ) obj->onLButtonDblClk( nFlags, global_chart_pos );
}

void RPObjectFlowChart::onRButtonDown( UINT nFlags, CPoint local_win_pos )
{
	CPoint global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	RPObjectChart* obj = find( global_chart_pos );
	if ( obj ) obj->onRButtonDown( nFlags, global_chart_pos );
}

void RPObjectFlowChart::onRButtonUp( UINT nFlags, CPoint local_win_pos )
{
	CPoint global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	RPObjectChart* obj = find( global_chart_pos );
	if ( obj ) obj->onRButtonUp( nFlags, global_chart_pos );
}
