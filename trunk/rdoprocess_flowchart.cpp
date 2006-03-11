#include "stdafx.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_object.h"
#include "rdoprocess_shape_action.h"
#include "rdoprocess_shape_if.h"
#include "rdoprocess_app.h"
#include "rdoprocess_math.h"
#include "rdoprocess_messages.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef TEST_SPEED // =====================================
const int sec_timer_id = 2;
const int base_speed = 10000;
#endif // ================================================

// ----------------------------------------------------------------------------
// ---------- RPFlowChartObject
// ----------------------------------------------------------------------------
RPFlowChartObject::RPFlowChartObject( RPObject* _parent, RPChartObject* _chart_parent, RPFlowChart* _flowchart ):
	RPChartObject( _parent, _chart_parent, _flowchart, _T("Flowchart") ),
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
	select_box_size2( 4 ),
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
	global_win_pos_prev( 0, 0 ),
	one_object( NULL ),
	one_object_pcmd( RPChartObject::pcmd_none )
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

RPFlowChartObject::~RPFlowChartObject()
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

void RPFlowChartObject::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == rp::msg::RP_FLOWSTATE_CHANGED ) {
		flowchart->updateFlowState();
/*
		RPProject::FlowState flow_state = static_cast<RPProject::FlowState>(wParam);
		switch ( flow_state ) {
			case RPProject::flow_select   : break;
			case RPProject::flow_connector: break;
			case RPProject::flow_rotate   : break;
		}
*/
	}
}

void RPFlowChartObject::draw( CDC& dc )
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

	std::list< RPChartObject* > objects;
	std::list< RPObject* >::const_iterator it = begin();
	while ( it != end() ) {
		objects.push_back( static_cast<RPChartObject*>(*it) );
		it++;
	}

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
				std::list< RPChartObject* >::iterator it = objects.begin();
				while ( it != objects.end() ) {
					it++;
				}
			}
			::GetSystemTime( &t4 );
#endif // ================================================

			std::list< RPChartObject* >::iterator it = objects.begin();
			while ( it != objects.end() ) {
				RPChartObject* object = *it;
				object->transformToGlobal();
				it++;
			}

#ifdef TEST_SPEED // =====================================
			for ( int cnt5 = 0; cnt5 <= base_speed; cnt5++ ) {
#endif // ================================================

			it = objects.begin();
			while ( it != objects.end() ) {
				int saved = mem_dc.SaveDC();
				(*it)->draw( mem_dc );
				RPChartObject* obj = *it;
				obj->transformToGlobal();
				static_cast<RPShape*>(obj)->pa_global.extendByPerimetr( obj->main_pen_width / 2.0 );
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
				RPChartObject* object = *it;
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
				if ( object->isSelected() ) {
					// Прямоугольник вокруг фигуры
					rp::rect rect = object->getBoundingRect();
					int x0 = rect.p0().x;
					int x1 = rect.p1().x;
					int x2 = rect.p2().x;
					int x3 = rect.p3().x;
					int y0 = rect.p0().y;
					int y1 = rect.p1().y;
					int y2 = rect.p2().y;
					int y3 = rect.p3().y;
					mem_dc.SelectObject( pen_selected_line );
					mem_dc.MoveTo( x0, y0 );
					mem_dc.LineTo( x1, y1 );
					mem_dc.LineTo( x2, y2 );
					mem_dc.LineTo( x3, y3 );
					mem_dc.LineTo( x0, y0 );
					mem_dc.SelectObject( pen_selected_box );
					mem_dc.SelectObject( brush_selected_box );
					int box_size    = select_box_size2 * 2 -1;
					int box_size_2  = select_box_size2;
					if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
						int radius = flowchart->getSensitivity();
						mem_dc.Ellipse( x0 - radius, y0 - radius, x0 + radius, y0 + radius );
						mem_dc.Ellipse( x1 - radius, y1 - radius, x1 + radius, y1 + radius );
						mem_dc.Ellipse( x2 - radius, y2 - radius, x2 + radius, y2 + radius );
						mem_dc.Ellipse( x3 - radius, y3 - radius, x3 + radius, y3 + radius );
					} else {
						mem_dc.Rectangle( x0 - box_size_2, y0 - box_size_2, x0 + box_size_2, y0 + box_size_2 );
						mem_dc.Rectangle( x1 - box_size_2, y1 - box_size_2, x1 + box_size_2, y1 + box_size_2 );
						mem_dc.Rectangle( x2 - box_size_2, y2 - box_size_2, x2 + box_size_2, y2 + box_size_2 );
						mem_dc.Rectangle( x3 - box_size_2, y3 - box_size_2, x3 + box_size_2, y3 + box_size_2 );
					}
					mem_dc.Rectangle( (x0 + x1)/2 - box_size_2, (y0 + y1)/2 - box_size_2, (x0 + x1)/2 + box_size_2, (y0 + y1)/2 + box_size_2 );
					mem_dc.Rectangle( (x1 + x2)/2 - box_size_2, (y1 + y2)/2 - box_size_2, (x1 + x2)/2 + box_size_2, (y1 + y2)/2 + box_size_2 );
					mem_dc.Rectangle( (x2 + x3)/2 - box_size_2, (y2 + y3)/2 - box_size_2, (x2 + x3)/2 + box_size_2, (y2 + y3)/2 + box_size_2 );
					mem_dc.Rectangle( (x3 + x0)/2 - box_size_2, (y3 + y0)/2 - box_size_2, (x3 + x0)/2 + box_size_2, (y3 + y0)/2 + box_size_2 );
					if ( rpapp.project().getFlowState() == RPProject::flow_rotate ) {
						// Центр вращения
						rp::point center = object->getRotateCenter();
						CPen pen_red( PS_SOLID, 1, RGB(0,0,0) );
						CBrush brush_white( RGB(-1,-1,0) );
						int radius = flowchart->getSensitivity();
						mem_dc.SelectObject( pen_red );
						mem_dc.SelectObject( brush_white );
						mem_dc.Ellipse( center.x - radius, center.y - radius, center.x + radius, center.y + radius );
					}
				}
				it++;
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

void RPFlowChartObject::makeNewPixmap()
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

void RPFlowChartObject::onLButtonDown( UINT nFlags, CPoint local_win_pos )
{
	rp::point global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );

	one_object = NULL;
	RPChartObject::PossibleCommand pcmd = RPChartObject::pcmd_none;
	if ( rpapp.project().getFlowState() == RPProject::flow_select || rpapp.project().getFlowState() == RPProject::flow_rotate ) {
		std::list< RPObject* >::const_iterator it = begin();
		while ( it != end() ) {
			RPChartObject* obj = static_cast<RPChartObject*>(*it);
			// Проверим, а не попалили в центр вращения какой-либо фигуры.
			// Сам центр может находится вне фигуры, поэтому проверим для начала все центры всех фигур
			if ( obj->isRotateCenter( global_chart_pos ) ) {
				one_object      = obj;
				one_object_pcmd = RPChartObject::pcmd_rotate_center;
				break;
			}
			// Проверяем поворот с перемещением
			if ( obj->getBoundingRect().extendByPerimetr( flowchart->getSensitivity() ).pointInRect( global_chart_pos ) || obj->pointInPolygon( global_chart_pos ) ) {
				obj->transformToGlobal();
				pcmd = obj->getPossibleCommand( global_chart_pos );
				switch ( pcmd ) {
					case RPChartObject::pcmd_rotate_tl    :
					case RPChartObject::pcmd_rotate_tr    : 
					case RPChartObject::pcmd_rotate_bl    :
					case RPChartObject::pcmd_rotate_br    :
					case RPChartObject::pcmd_scale        :
					case RPChartObject::pcmd_move         :
					case RPChartObject::pcmd_scale_l      :
					case RPChartObject::pcmd_scale_r      :
					case RPChartObject::pcmd_scale_t      :
					case RPChartObject::pcmd_scale_b      :
					case RPChartObject::pcmd_scale_tl     :
					case RPChartObject::pcmd_scale_br     :
					case RPChartObject::pcmd_scale_tr     :
					case RPChartObject::pcmd_scale_bl     : one_object = obj; one_object_pcmd = pcmd; break;
				}
			}
			if ( pcmd != RPChartObject::pcmd_none ) break;
			it++;
		}
	}
	// Запомнили глобальные координаты мышки (глобальные в виндах, а не 2D движке ). Пригодятся в других функциях.
	global_win_pos_prev = local_win_pos;
	flowchart->ClientToScreen( &global_win_pos_prev );
	// Монопольно захватили мышку
	flowchart->SetCapture();
	// Нашли фигуру
	if ( one_object ) {
		one_object->onLButtonDown( nFlags, CPoint( global_chart_pos.x, global_chart_pos.y ) );
		// Фигура для перемещения найдена
		if ( pcmd == RPChartObject::pcmd_move ) {
			moving_objects.clear();
			if ( nFlags & MK_CONTROL ) {
//				moving_objects.assign( flowobj->begin(), flowobj->end() );
			} else {
				moving_objects.push_back( one_object );
			}
		}
	} else {
		// Никого не нашли, выбираем лист
		setSelected( true );
	}
}

void RPFlowChartObject::onLButtonUp( UINT nFlags, CPoint local_win_pos )
{
	// Отдали виндам мышку
	::ReleaseCapture();
	// Почистили все списки
	moving_objects.clear();
	if ( one_object ) {
		CPoint global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		one_object->onLButtonUp( nFlags, global_chart_pos );
	}
	one_object = NULL;
	// Обновить скролл
	flowchart->updateScrollBars();
}

void RPFlowChartObject::onLButtonDblClk( UINT nFlags, CPoint local_win_pos )
{
	RPChartObject* obj = find( local_win_pos );
	if ( obj ) {
		CPoint global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		obj->onLButtonDblClk( nFlags, global_chart_pos );
	}
}

void RPFlowChartObject::onRButtonDown( UINT nFlags, CPoint local_win_pos )
{
	RPChartObject* obj = find( local_win_pos );
	if ( obj ) {
		CPoint global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		obj->onRButtonDown( nFlags, global_chart_pos );
	}
}

void RPFlowChartObject::onRButtonUp( UINT nFlags, CPoint local_win_pos )
{
	RPChartObject* obj = find( local_win_pos );
	if ( obj ) {
		CPoint global_chart_pos = local_win_pos;
		clientToZero( global_chart_pos );
		obj->onRButtonUp( nFlags, global_chart_pos );
	}
}

void RPFlowChartObject::onMouseMove( UINT nFlags, CPoint local_win_pos )
{
	// Глобальные коодинаты мышки в винде
	CPoint global_win_pos = local_win_pos;
	flowchart->ClientToScreen( &global_win_pos );

	// Глобальные координаты мышки в 2D движке (на текущем листе)
	rp::point global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );

	RPChartObject* obj = find( local_win_pos );
	if ( obj ) {
		obj->onMouseMove( nFlags, CPoint( global_chart_pos.x, global_chart_pos.y ) );
	}

	// Если выбран один объект, но он имеет приоритет в обработке.
	// Проверается на изменение масштаба, поворота или центра поворота.
	// Эти операции не могут быть групповыми.
	if ( one_object ) {
		RPChartObject::angle90 a90 = one_object->getAngle90();
		bool   horz  = a90 == RPChartObject::angle90_0 || a90 == RPChartObject::angle90_180;
		double alpha = one_object->getRotation();
		double dx = global_win_pos.x - global_win_pos_prev.x;
		double dy = global_win_pos.y - global_win_pos_prev.y;
		switch ( one_object_pcmd ) {
			case RPChartObject::pcmd_rotate_center: {
				one_object->setRotateCenter( global_chart_pos );
				flowchart->updateDC();
				break;
			}
			case RPChartObject::pcmd_rotate       :
			case RPChartObject::pcmd_rotate_tl    :
			case RPChartObject::pcmd_rotate_tr    :
			case RPChartObject::pcmd_rotate_bl    :
			case RPChartObject::pcmd_rotate_br    : {
				CPoint point_old = global_win_pos_prev;
				CPoint point_new = global_win_pos;
				flowchart->ScreenToClient( &point_old );
				flowchart->ScreenToClient( &point_new );
				clientToZero( point_old );
				clientToZero( point_new );
				one_object->setRotation( one_object->getRotation() + rp::math::getAlpha( point_old, one_object->getRotateCenter(), point_new ) );
				flowchart->updateDC();
				break;
			}
			case RPChartObject::pcmd_scale_t      :
			case RPChartObject::pcmd_scale_b      :
			case RPChartObject::pcmd_scale_l      :
			case RPChartObject::pcmd_scale_r      :
			case RPChartObject::pcmd_scale_tl     :
			case RPChartObject::pcmd_scale_tr     :
			case RPChartObject::pcmd_scale_bl     :
			case RPChartObject::pcmd_scale_br     : {
				rp::rect  rect = one_object->getBoundingRect();
				rp::point point_delta( one_object->globalRotate().obr() * rp::point(dx, dy) );
				rp::point len( rp::math::getLength( rect.p_tl(), rect.p_tr() ), rp::math::getLength( rect.p_tl(), rect.p_bl() ) );
				flowchart->getScaleDelta( point_delta, a90, one_object_pcmd );
				if ( len.x + point_delta.x < 0 ) {
					point_delta.x = -len.x;
				}
				if ( len.y + point_delta.y < 0 ) {
					point_delta.y = -len.y;
				}
				one_object->setScaleX( one_object->getScaleX() * (len.x + point_delta.x ) / len.x );
				one_object->setScaleY( one_object->getScaleY() * (len.y + point_delta.y ) / len.y );
				flowchart->getRectDelta( rect, one_object->getBoundingRect(), point_delta, a90, one_object_pcmd );
				point_delta = one_object->globalRotate().obr() * point_delta;
				one_object->setPostX( one_object->getPostX() + point_delta.x );
				one_object->setPostY( one_object->getPostY() + point_delta.y );
				one_object->setRotateCenterLocalDelta( point_delta.x, point_delta.y );
				flowchart->updateDC();
				break;
			}
		}
	}
	// Перемещение делаем отдельно. Перемещать можно много фигур.
	if ( one_object_pcmd == RPChartObject::pcmd_move ) {
		int dx = global_win_pos.x - global_win_pos_prev.x;
		int dy = global_win_pos.y - global_win_pos_prev.y;
/*
		// Отсечь выход за границы листа (левый верхний угол)
		std::list< RPChartObject* >::iterator it = moving_objects.begin();
		while ( it != moving_objects.end() ) {
			RPChartObject* object = *it;
			int delta = object->getX() + dx;
			if ( delta < 0 ) {
				dx -= delta;
			}
			delta = object->getY() + dy;
			if ( delta < 0 ) {
				dy -= delta;
			}
			it++;
		}
*/
		std::list< RPChartObject* >::iterator it = moving_objects.begin();
		while ( it != moving_objects.end() ) {
			(*it)->moving( dx, dy );
			it++;
		}
		flowchart->updateScrollBars();
		flowchart->makeGrid();
/*
//		if ( dx > 0 ) {
			CSize size = getFlowSize( movingShapes );
			int max_x = size.cx + border_w + paper_border_w;
			int width = pixmap_w_show + scroll_x_pos;
//			TRACE( "dx = %d, max_x = %d, width = %d\n", dx, max_x, width );
			if ( max_x > width ) {
				SCROLLINFO si;
				si.cbSize = sizeof( si );
				si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;

				scroll_x_pos += dx;
				si.nMin   = 0;
				si.nMax   = max_x;
				si.nPos   = scroll_x_pos;
				si.nPage  = client_width;
				SetScrollInfo( SB_HORZ, &si, TRUE );
			}
			makeGrid();
//		}
*/
		flowchart->updateDC();
	}
	global_win_pos_prev = global_win_pos;
}

// ----------------------------------------------------------------------------
// ---------- RPFlowChart
// ----------------------------------------------------------------------------
const int grid_dx_plus = 1;
const int grid_dy_plus = 2;
const int grid_timer_id = 1;

BEGIN_MESSAGE_MAP( RPFlowChart,CWnd )
	//{{AFX_MSG_MAP(RPFlowChart)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPFlowChart::RPFlowChart():
	CWnd(),
	saved_dc( 0 ),
	scroll_bar_size( CRect(0,0,0,0) ),
	flowobj( NULL )
#ifdef TEST_SPEED // =====================================
	,
	sec_cnt( 0 ),
	sec_timer( 0 ),
	makegrid_cnt( 0 ),
	makegridempty_cnt( 0 )
#endif // ================================================
{
}

RPFlowChart::~RPFlowChart()
{
}

BOOL RPFlowChart::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

RPShapeAction* shape_action = NULL;
RPShapeIf*     shape_if     = NULL;

BOOL RPFlowChart::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	if ( !CWnd::Create( lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext ) ) return false;

	// Запомнить состояния DC для восстановления перед удалением
	CDC* dc  = GetDC();
	saved_dc = dc->SaveDC();

	flowobj  = new RPFlowChartObject( &rpapp.project(), NULL, this );

//	updateFont();
	updateScrollBars();

	shape_action = new RPShapeAction( flowobj, flowobj, this );
//	shape_action->setPosition( 200, 100 );
	shape_if = new RPShapeIf( flowobj, flowobj, this );
	shape_if->setPosition( 200, 100 );

#ifdef TEST_SPEED // =====================================
	sec_timer = SetTimer( sec_timer_id, 1000, NULL );
#endif // ================================================

	return true;
}

void RPFlowChart::OnDestroy()
{
#ifdef TEST_SPEED // =====================================
	if ( sec_timer ) {
		KillTimer( sec_timer );
		sec_timer = 0;
	}
#endif // ================================================
//	if ( grid_timer ) {
//		KillTimer( grid_timer );
//		grid_timer = 0;
//	}
	if ( flowobj ) {
		delete flowobj;
		flowobj = NULL;
	}
	CDC* dc = GetDC();
	if ( dc ) {
		dc->RestoreDC( saved_dc );
	}
	ReleaseDC( dc );
//	if ( grid_bmp.GetSafeHandle() ) {
//		grid_dc.SelectObject( bmp_first );
//		grid_bmp.DeleteObject();
//	}
	CWnd::OnDestroy();
}

void RPFlowChart::insertShape( RPShape* shape )
{
	if ( flowobj->find_child( shape ) == flowobj->end() ) {
//		objects.push_back( shape );
/*
		if ( !connectors_before_create.empty() ) {
			std::list< CBDFlowChartConnector >::iterator it = connectors_before_create.begin();
			while ( it != connectors_before_create.end() ) {
				if ( it->shape_from == new_shape ) {
					it->shape_from = shape;
				} else if ( it->shape_to == new_shape ) {
					it->shape_to = shape;
				}
				connectors.push_back( *it );
				it++;
			}
			connectors_before_create.clear();
		}
*/
	}
	updateDC();
}

void RPFlowChart::deleteShape( RPShape* shape )
{
	std::list< RPObject* >::const_iterator it = flowobj->find_child( shape );
	if ( it != flowobj->end() ) {
//		objects.erase( it );

//		if ( movingShape == shape ) {
//			movingShape = NULL;
//			moving_stop();
//		}

		updateScrollBars();
		updateDC();
	}
}

rp::rect RPFlowChart::getFlowSize( const std::list< RPChartObject* >& list ) const
{
	double max_x = 0;
	double max_y = 0;
	double min_x = 0;
	double min_y = 0;
	std::list< RPChartObject* >::const_iterator it = list.begin();
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
	min_x -= flowobj->matrix_transform.dx();
	max_x -= flowobj->matrix_transform.dx();
	min_y -= flowobj->matrix_transform.dy();
	max_y -= flowobj->matrix_transform.dy();
	return rp::rect( min_x, min_y, max_x, max_y );
}

void RPFlowChart::updateScrollBars()
{
	rp::rect rect = getFlowSize();
	if ( scroll_bar_size.left   > rect.getMinX() ) scroll_bar_size.left   = rect.getMinX();
	if ( scroll_bar_size.top    > rect.getMinY() ) scroll_bar_size.top    = rect.getMinY();
	if ( scroll_bar_size.right  < rect.getMaxX() ) scroll_bar_size.right  = rect.getMaxX();
	if ( scroll_bar_size.bottom < rect.getMaxY() ) scroll_bar_size.bottom = rect.getMaxY();
	if ( scroll_bar_size.left   > -flowobj->pixmap_w_show ) scroll_bar_size.left   = -flowobj->pixmap_w_show;
	if ( scroll_bar_size.top    > -flowobj->pixmap_h_show ) scroll_bar_size.top    = -flowobj->pixmap_h_show;
	if ( scroll_bar_size.right  <  flowobj->pixmap_w_show ) scroll_bar_size.right  = flowobj->pixmap_w_show;
	if ( scroll_bar_size.bottom <  flowobj->pixmap_h_show ) scroll_bar_size.bottom = flowobj->pixmap_h_show;

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;

	si.nMin   = scroll_bar_size.left;
	si.nMax   = scroll_bar_size.right - flowobj->pixmap_w_show;
	si.nPos   = -flowobj->matrix_transform.dx();
	si.nPage  = flowobj->pixmap_w_show;
	TRACE( "min = %d, max = %d, page = %d\n", si.nMin, si.nMax, si.nPage );
	SetScrollInfo( SB_HORZ, &si, TRUE );

	si.nMin   = scroll_bar_size.top;
	si.nMax   = scroll_bar_size.bottom;
	si.nPos   = -flowobj->matrix_transform.dy();
	si.nPage  = flowobj->pixmap_h_show;
	SetScrollInfo( SB_VERT, &si, TRUE );

/*
	int max_x = rect.p_tr().x - rect.p_tl().x + border_w * 2 + paper_border_w * 2 - 1;
	int max_y = rect.p_bl().y - rect.p_tl().y + border_h * 2 + paper_border_h * 2 - 1;
	int scroll_x_pos = flowobj->matrix_transform.dx();
	int scroll_y_pos = flowobj->matrix_transform.dy();
	if ( scroll_x_pos + pixmap_w_show > max_x ) scroll_x_pos = max_x - pixmap_w_show;
	if ( scroll_y_pos + pixmap_h_show > max_y ) scroll_y_pos = max_y - pixmap_h_show;
	if ( scroll_x_pos < 0 ) scroll_x_pos = 0;
	if ( scroll_y_pos < 0 ) scroll_y_pos = 0;
	flowobj->matrix_transform.dx() = scroll_x_pos;
	flowobj->matrix_transform.dy() = scroll_y_pos;

	si.nMin   = 0;
	si.nMax   = max_x;
	si.nPos   = scroll_x_pos;
	si.nPage  = client_width;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	si.nMin   = 0;
	si.nMax   = max_y;
	si.nPos   = scroll_y_pos;
	si.nPage  = client_height;
	SetScrollInfo( SB_VERT, &si, TRUE );
*/
}

void RPFlowChart::updateDC()
{
	InvalidateRect( NULL );
	UpdateWindow();
}

void RPFlowChart::modify()
{
}

void RPFlowChart::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
//	grid_pa.resize( 0 );
	if ( flowobj ) flowobj->makeNewPixmap();
	updateDC();
}

void RPFlowChart::makeGrid()
{
/*
#ifdef TEST_SPEED // =====================================
	makegridempty_cnt++;
#endif // ================================================

	if ( pixmap_w_show <= paper_border_w * 2 || pixmap_h_show <= paper_border_h * 2 ) {
		grid_cnt_x = 0;
		grid_cnt_y = 0;
		grid_pa.clear();
		return;
	}

#ifdef TEST_SPEED // =====================================
	makegrid_cnt++;
	SYSTEMTIME t1;
	SYSTEMTIME t2;
	::GetSystemTime( &t1 );
	for ( int cnt = 0; cnt <= base_speed * 10; cnt++ ) {
#endif // ================================================

	CSize size = getFlowSize();
	grid_cnt_x = (max(size.cx, scroll_x_pos + pixmap_w_show - paper_border_w * 2) / grid_step) + 1;
	grid_cnt_y = (max(size.cy, scroll_y_pos + pixmap_h_show - paper_border_h * 2) / grid_step) + 1;
	if ( grid_pa.empty() && grid_bmp.m_hObject ) {
		grid_dc.SelectObject( bmp_first );
		grid_bmp.Detach();
		grid_bmp.DeleteObject();
		grid_bmp.m_hObject = NULL;
	}
	CDC* dc = GetDC();
	if ( grid_type == gtPoints ) {
		if ( !grid_bmp.m_hObject ) {
			// brush
			if ( grid_step < 10 ) {
				grid_bmp_width = grid_step * 50;
			} else if ( grid_step < 25 ) {
				grid_bmp_width = grid_step * 25;
			} else {
				grid_bmp_width = grid_step * 10;
			}
			if ( !grid_dc.m_hDC ) {
				grid_dc.CreateCompatibleDC( dc );
			}
			grid_bmp.CreateCompatibleBitmap( dc, grid_bmp_width, grid_bmp_width );
			grid_dc.SelectObject( &grid_bmp );
			grid_dc.FillSolidRect( 0, 0, grid_bmp_width, grid_bmp_width, paper_bg_color );
			for ( int i = 0; i < grid_bmp_width / grid_step; i++ ) {
				for ( int j = 0; j < grid_bmp_width / grid_step; j++ ) {
					grid_dc.SetPixelV( i * grid_step, j * grid_step, grid_color );
				}
			}
		}
		// points array
		grid_pa.resize( grid_cnt_x * grid_cnt_y );
		for ( int i = 0; i < grid_cnt_x; i++ ) {
			for ( int j = 0; j < grid_cnt_y; j++ ) {
				grid_pa[ i + j * grid_cnt_x ]. x = i * grid_step + paper_border_w;
				grid_pa[ i + j * grid_cnt_x ]. y = j * grid_step + paper_border_h;
			}
		}
	} else {
		if ( !grid_bmp.m_hObject ) {
			// brush
			if ( grid_step < 100 ) {
				grid_bmp_width = grid_step * 4;
			} else if ( grid_step < 300 ) {
				grid_bmp_width = grid_step * 2;
			} else {
				grid_bmp_width = grid_step;
			}
			if ( !grid_dc.m_hDC ) {
				grid_dc.CreateCompatibleDC( dc );
			}
			grid_bmp.CreateCompatibleBitmap( dc, grid_bmp_width, grid_bmp_width );
			grid_dc.SelectObject( &grid_bmp );
			grid_dc.FillSolidRect( 0, 0, grid_bmp_width, grid_bmp_width, paper_bg_color );
			CPen pen;
			if ( grid_type == gtSolidLines ) {
				pen.CreatePen( PS_SOLID, 1, grid_color );
			} else {
				pen.CreatePen( PS_DOT, 1, grid_color );
			}
			grid_dc.SelectObject( pen );
			int y1 = 0;
			int y2 = grid_bmp_width;
			for ( int i = 0; i < grid_bmp_width / grid_step; i++ ) {
				int x = i * grid_step;
				grid_dc.MoveTo( x, y1 );
				grid_dc.LineTo( x, y2 );
			}
			int x1 = 0;
			int x2 = grid_bmp_width;
			for ( int j = 0; j < grid_bmp_width / grid_step; j++ ) {
				int y = j * grid_step;
				grid_dc.MoveTo( x1, y );
				grid_dc.LineTo( x2, y );
			}
		}
		// points array
		grid_pa.resize( grid_cnt_x * 2 + grid_cnt_y * 2 );
		int index = 0;
		int y1 = paper_border_h;
		int y2 = pixmap_h_show - paper_border_h;
		for ( int i = 0; i < grid_cnt_x; i++ ) {
			int x = i * grid_step + paper_border_w;
			grid_pa[ index++ ] = CPoint( x, y1 );
			grid_pa[ index++ ] = CPoint( x, y2 );
		}
		int x1 = paper_border_w;
		int x2 = pixmap_w_show - paper_border_w;
		for ( int j = 0; j < grid_cnt_y; j++ ) {
			int y = j * grid_step + paper_border_h;
			grid_pa[ index++ ] = CPoint( x1, y );
			grid_pa[ index++ ] = CPoint( x2, y );
		}
	}
	ReleaseDC( dc );

#ifdef TEST_SPEED // =====================================
	}
	::GetSystemTime( &t2 );
	int delay = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	TRACE( "makegrid_delay = %d\n", delay );
#endif // ================================================
*/
}

void RPFlowChart::OnPaint() 
{
	CPaintDC dc( this );
	dc.SaveDC();
	flowobj->draw( dc );
	dc.RestoreDC( -1 );
}

void RPFlowChart::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	switch ( nSBCode ) {
		case SB_PAGEUP: {
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			flowobj->matrix_transform.dx() += si.nPage;
			break; 
		}
		case SB_PAGEDOWN: {
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			flowobj->matrix_transform.dx() -= si.nPage;
			break;
		}
		case SB_LINEUP: {
			flowobj->matrix_transform.dx() += 50;
			break;
		}
		case SB_LINEDOWN: {
			flowobj->matrix_transform.dx() -= 50;
			break;
		}
		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			flowobj->matrix_transform.dx() = -si.nTrackPos;
			break;
		}
	}

	if ( scroll_bar_size.left  > -flowobj->matrix_transform.dx() ) scroll_bar_size.left  = -flowobj->matrix_transform.dx();
	if ( scroll_bar_size.right < -flowobj->matrix_transform.dx() ) {
		scroll_bar_size.right = -flowobj->matrix_transform.dx();
		TRACE( "new right = %d\n", scroll_bar_size.right );
	}

	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin   = scroll_bar_size.left;
	si.nMax   = scroll_bar_size.right;
	si.nPos   = -flowobj->matrix_transform.dx();
	si.nPage  = flowobj->pixmap_w_show;
	SetScrollInfo( SB_HORZ, &si, TRUE );
	updateDC();
}

void RPFlowChart::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	switch ( nSBCode ) {
		case SB_PAGEUP: {
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			flowobj->matrix_transform.dy() += si.nPage;
			break; 
		}
		case SB_PAGEDOWN: {
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			flowobj->matrix_transform.dy() -= si.nPage;
			break;
		}
		case SB_LINEUP: {
			flowobj->matrix_transform.dy() += 50;
			break;
		}
		case SB_LINEDOWN: {
			flowobj->matrix_transform.dy() -= 50;
			break;
		}
		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			flowobj->matrix_transform.dy() = -si.nTrackPos;
			break;
		}
	}

	if ( scroll_bar_size.top    > -flowobj->matrix_transform.dy() ) scroll_bar_size.top    = -flowobj->matrix_transform.dy();
	if ( scroll_bar_size.bottom < -flowobj->matrix_transform.dy() ) scroll_bar_size.bottom = -flowobj->matrix_transform.dy();

	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin   = scroll_bar_size.top;
	si.nMax   = scroll_bar_size.bottom;
	si.nPos   = -flowobj->matrix_transform.dy();
	si.nPage  = flowobj->pixmap_h_show;
	SetScrollInfo( SB_VERT, &si, TRUE );
	updateDC();
}

void RPFlowChart::snapToGrid( RPShape* shape )
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
/*
void RPFlowChart::snapToGridAllShapes()
{
	std::vector< RPShape* >::const_iterator it = objects.begin();
	while ( it != objects.end() ) {
		snapToGrid( *it );
		it++;
	}
}
*/

void RPFlowChart::OnTimer( UINT nIDEvent )
{
#ifdef TEST_SPEED // =====================================
	if ( nIDEvent == sec_timer ) {
		TRACE( "%d. makepixmap = %d, makegrid = %d, makegrid_empty = %d\n", sec_cnt++, flowobj->makepixmap_cnt, makegrid_cnt, makegridempty_cnt );
		makegrid_cnt      = 0;
		makegridempty_cnt = 0;
		flowobj->makepixmap_cnt = 0;
	}
#endif // ================================================
//	if ( nIDEvent == grid_timer ) {
//		if ( grid_mode != gtSnapOff && !grid_objects.empty() ) {
//			std::list< RPShape* >::const_iterator it = grid_objects.begin();
//			while ( it != grid_objects.end() ) {
//				snapToGrid( *it );
//				it++;
//			}
//			grid_objects.clear();
//		}
//	}
	CWnd::OnTimer( nIDEvent );
}

void RPFlowChart::OnLButtonDown( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnLButtonDown( nFlags, local_win_pos );
	flowobj->onLButtonDown( nFlags, local_win_pos );
}

void RPFlowChart::OnLButtonUp( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnLButtonUp( nFlags, local_win_pos );
	flowobj->onLButtonUp( nFlags, local_win_pos );
/*
	grid_objects.clear();
	grid_objects.assign( moving_objects.begin(), moving_objects.end() );
//	movingShape = NULL;
	moving_objects.clear();
	if ( !grid_objects.empty() ) {
		if ( grid_timer ) KillTimer( grid_timer );
		grid_timer = SetTimer( grid_timer_id, 100, NULL );
	}
*/
//	resetPreview();
}

void RPFlowChart::OnRButtonDown( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnRButtonDown( nFlags, local_win_pos );
	flowobj->onRButtonDown( nFlags, local_win_pos );
}

void RPFlowChart::OnRButtonUp( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnRButtonUp( nFlags, local_win_pos );
	flowobj->onRButtonUp( nFlags, local_win_pos );
}

void RPFlowChart::OnLButtonDblClk( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnLButtonDblClk( nFlags, local_win_pos );
	flowobj->onLButtonDblClk( nFlags, local_win_pos );
}

void RPFlowChart::getScaleDelta( rp::point& delta, RPChartObject::angle90 a90, RPChartObject::PossibleCommand pcmd ) const
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

void RPFlowChart::getRectDelta( rp::rect& rect_old, rp::rect& rect_new, rp::point& delta, RPChartObject::angle90 a90, RPChartObject::PossibleCommand pcmd ) const
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

void RPFlowChart::OnMouseMove( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnMouseMove( nFlags, local_win_pos );
	flowobj->onMouseMove( nFlags, local_win_pos );
}

void RPFlowChart::updateFlowState()
{
	updateDC();
}

// Функция вызывается при перемещении мышки над окном и при клике, но не вызывается,
// когда мышка перемещается над окном с зажатой клавишей
BOOL RPFlowChart::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	flowobj->clientToZero( point );
	HCURSOR cursor        = NULL;
	RPChartObject* object = NULL;
	flowobj->one_object_pcmd = RPChartObject::pcmd_none;
	std::list< RPObject* >::const_iterator it = flowobj->begin();
	while ( it != flowobj->end() ) {
		RPChartObject* obj = static_cast<RPChartObject*>(*it);
		if ( obj->isRotateCenter( point ) ) {
			object = obj;
			break;
		}
		if ( obj->getBoundingRect().extendByPerimetr( getSensitivity() ).pointInRect( point ) || obj->pointInPolygon( point ) ) {
			object = obj;
			break;
		}
		it++;
	}
	if ( object ) {
		object->transformToGlobal();
		RPChartObject::PossibleCommand _pcmd = object->getPossibleCommand( point, true );
		if ( object->isSelected() ) {
			switch ( _pcmd ) {
				case RPChartObject::pcmd_none         : break;
				case RPChartObject::pcmd_scale        : cursor = rpapp.cursors[IDC_FLOW_SELECT]; break;
				case RPChartObject::pcmd_move         : flowobj->one_object_pcmd = _pcmd; cursor = rpapp.cursors[IDC_FLOW_MOVE]; break;
				case RPChartObject::pcmd_rotate       :
				case RPChartObject::pcmd_rotate_tl    :
				case RPChartObject::pcmd_rotate_tr    :
				case RPChartObject::pcmd_rotate_bl    :
				case RPChartObject::pcmd_rotate_br    : cursor = rpapp.cursors[IDC_FLOW_ROTATE_TL]; break;
				case RPChartObject::pcmd_rotate_center: cursor = rpapp.cursors[IDC_FLOW_ROTATE_CENTER]; break;
				case RPChartObject::pcmd_scale_l      :
				case RPChartObject::pcmd_scale_r      : cursor = rpapp.cursors[IDC_FLOW_SCALE_LR]; break;
				case RPChartObject::pcmd_scale_t      :
				case RPChartObject::pcmd_scale_b      : cursor = rpapp.cursors[IDC_FLOW_SCALE_TB]; break;
				case RPChartObject::pcmd_scale_tl     :
				case RPChartObject::pcmd_scale_br     : cursor = rpapp.cursors[IDC_FLOW_SCALE_TLBR]; break;
				case RPChartObject::pcmd_scale_tr     :
				case RPChartObject::pcmd_scale_bl     : cursor = rpapp.cursors[IDC_FLOW_SCALE_TRBL]; break;
			}
		} else {
			switch ( _pcmd ) {
				case RPChartObject::pcmd_move         : flowobj->one_object_pcmd = _pcmd; cursor = rpapp.cursors[IDC_FLOW_MOVE]; break;
			}
		}
	}
	if ( !cursor ) {
		switch ( rpapp.project().getFlowState() ) {
			case RPProject::flow_select   : cursor = rpapp.cursors[IDC_FLOW_SELECT]; break;
			case RPProject::flow_connector: cursor = rpapp.cursors[IDC_FLOW_CONNECTOR]; break;
			case RPProject::flow_rotate   : cursor = rpapp.cursors[IDC_FLOW_ROTATE]; break;
		}
	}
	if ( cursor ) {
		::SetCursor( cursor );
		return true;
	}
	return false;
}
