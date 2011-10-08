#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_messages.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_pixmap.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPObjectFlowChart
// --------------------------------------------------------------------------------
const int RPObjectFlowChart::select_box_size2 = 4;

RPObjectFlowChart::RPObjectFlowChart( RPObject* _parent ):
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
	one_selected( NULL ),
	one_connector( NULL ),
	dock_begin( NULL ),
	ct_wanted( ctw_non ),
	flowchart( NULL ),
	drag_and_drop_shape( NULL ),
	drag_and_drop_connector( NULL ),
	trash_show( false ),
	trash_over( false ),
	trash_bmp( NULL )
#ifdef TEST_SPEED
	,
	makepixmap_cnt( 0 )
#endif
{
	rpMethod::project->msg().connect( this, rp::msg::RP_FLOWSTATE_CHANGED );
	rpMethod::project->msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	trash_rect.left   = 0;
	trash_rect.top    = 0;
	trash_rect.right  = 32;
	trash_rect.bottom = 32;
	trash_bmp = new RPObjectPixmap( this, new RPPixmap( IDB_TRASH, RGB(0xFF,0x00,0xFF) ) );
}

RPObjectFlowChart::~RPObjectFlowChart()
{
	if ( drag_and_drop_shape ) {
		delete drag_and_drop_shape;
		drag_and_drop_shape = NULL;
	}
	if ( trash_bmp ) {
		delete trash_bmp;
		trash_bmp = NULL;
	}
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

void RPObjectFlowChart::init()
{
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

	setActive( true );

	makeNewPixmap();
	update();
}

void RPObjectFlowChart::after_constructor()
{
	rpMethod::project->makeFlowChartWnd( this );
}

void RPObjectFlowChart::load( rp::RPXMLNode* node )
{
	setCorrectName( node->getAttribute("name") );
	can_update = false;
	rp::RPXMLNode* shape_node = node->nextChild( NULL );
	while (shape_node)
	{
		if ( shape_node->getName() == "shape" ) {
			RPObject* shape = rpMethod::factory->getNewObject( shape_node->getAttribute("class"), this );
			shape->load( shape_node );
		} else if ( shape_node->getName() == "connectors" ) {
			rp::RPXMLNode* connector_node = shape_node->nextChild( NULL );
			while (connector_node)
			{
				RPObject* connector = rpMethod::factory->getNewObject( connector_node->getAttribute("class"), this );
				connector->load( connector_node );
				connector_node = shape_node->nextChild( connector_node );
			}
			break;
		}
		shape_node = node->nextChild( shape_node );
	}
	can_update = true;
	update();
}

rp::RPXMLNode* RPObjectFlowChart::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = parent_node->makeChild( "flowchart" );
	obj_node->insertAttribute( "name", name );
	obj_node->insertAttribute( "class", getClassName() );
//	rp::RPXMLNode* obj_node = RPObjectMatrix::save( parent_node );
	// Записать графические объекты за вычетом конекторов
	std::list< RPObject* > all_child;
	getAllChildNotByClass( all_child, "RPConnector" );
	std::list< RPObject* >::const_iterator it = all_child.begin();
	while ( it != all_child.end() ) {
		(*it)->save( obj_node );
		it++;
	}
	// Записать конекторы
	rp::RPXMLNode* connectors_node = obj_node->makeChild( "connectors" );
	all_child.clear();
	getAllChildByClass( all_child, "RPConnector" );
	it = all_child.begin();
	while ( it != all_child.end() ) {
		(*it)->save( connectors_node );
		it++;
	}
	return obj_node;
}

void RPObjectFlowChart::notify( RPObject* from, UINT message, void* param )
{
	if ( message == rp::msg::RP_FLOWSTATE_CHANGED ) {
		if ( *static_cast<RPProject::FlowState*>(param) == RPProject::flow_connector ) {
			if ( one_selected ) one_selected->setSelected( false );
			ct_wanted = ctw_begin;
		} else {
			ct_wanted = ctw_non;
		}
		update();
	}
	if ( message == rp::msg::RP_OBJ_SELCHANGED && from && from->isChartObject() && static_cast<RPObjectFlowChart*>(from)->flowChart() == this && from->isSelected() ) {
		one_selected = static_cast<RPObjectFlowChart*>(from);
	}
}

bool RPObjectFlowChart::setName( const rp::string& value )
{
	bool result = RPObjectMatrix::setName( value );
	if ( result ) {
		flowchart->setName( value );
	}
	return result;
}

void RPObjectFlowChart::findAutoConnector()
{
	if ( drag_and_drop_shape ) {
		RPConnectorDock* backup_dock_begin = NULL;
		RPConnectorDock* backup_dock_end   = NULL;
		if ( drag_and_drop_connector ) {
			backup_dock_begin = drag_and_drop_connector->dock_begin;
			backup_dock_end   = drag_and_drop_connector->dock_end;
			if ( backup_dock_begin ) backup_dock_begin->connectors.remove( drag_and_drop_connector );
			if ( backup_dock_end )   backup_dock_end->connectors.remove( drag_and_drop_connector );
		}
		double           min_length = 300;
		RPConnectorDock* min_dock_begin = NULL;
		RPConnectorDock* min_dock_end   = NULL;
		std::list< RPObject* > shapes;
		getAllChildByClass( shapes, "RPShape", true );
		// От перетаскиваемой к остальным фигурам
		std::vector< RPConnectorDock* >::const_iterator drag_dock_it = drag_and_drop_shape->getDocks().begin();
		while ( drag_dock_it != drag_and_drop_shape->getDocks().end() ) {
			if ( (*drag_dock_it)->isOut() && (*drag_dock_it)->can_connect() ) {
				std::list< RPObject* >::const_iterator shape_it = shapes.begin();
				while ( shape_it != shapes.end() ) {
					RPShape* shape = static_cast<RPShape*>(*shape_it);
					if ( shape != drag_and_drop_shape ) {
						std::vector< RPConnectorDock* >::const_iterator dock_it = shape->getDocks().begin();
						while ( dock_it != shape->getDocks().end() ) {
							if ( (*dock_it)->isIn() && (*dock_it)->can_connect( *drag_dock_it ) ) {
								double length = rp::math::getLength( (*drag_dock_it)->getPosition(), (*dock_it)->getPosition() );
								if ( length < min_length ) {
									min_length     = length;
									min_dock_begin = *drag_dock_it;
									min_dock_end   = *dock_it;
								}
							}
							dock_it++;
						}
					}
					shape_it++;
				}
			}
			drag_dock_it++;
		}
		// От всех фигур к перетаскиваемой
		std::list< RPObject* >::const_iterator shape_it = shapes.begin();
		while ( shape_it != shapes.end() ) {
			RPShape* shape = static_cast<RPShape*>(*shape_it);
			if ( shape != drag_and_drop_shape ) {
				std::vector< RPConnectorDock* >::const_iterator drag_dock_it = drag_and_drop_shape->getDocks().begin();
				while ( drag_dock_it != drag_and_drop_shape->getDocks().end() ) {
					if ( (*drag_dock_it)->isIn() ) {
						std::vector< RPConnectorDock* >::const_iterator dock_it = shape->getDocks().begin();
						while ( dock_it != shape->getDocks().end() ) {
							if ( (*dock_it)->isOut() && (*dock_it)->can_connect() && (*drag_dock_it)->can_connect( *dock_it ) ) {
								double length = rp::math::getLength( (*dock_it)->getPosition(), (*drag_dock_it)->getPosition() );
								if ( length < min_length ) {
									min_length     = length;
									min_dock_begin = *dock_it;
									min_dock_end   = *drag_dock_it;
								}
							}
							dock_it++;
						}
					}
					drag_dock_it++;
				}
			}
			shape_it++;
		}
		if ( min_dock_begin ) {
			if ( drag_and_drop_connector && backup_dock_begin == min_dock_begin && backup_dock_end == min_dock_end ) {
				drag_and_drop_connector->dock_begin = backup_dock_begin;
				drag_and_drop_connector->dock_end   = backup_dock_end;
				backup_dock_begin->connectors.push_back( drag_and_drop_connector );
				backup_dock_end->connectors.push_back( drag_and_drop_connector );
			} else {
				if ( drag_and_drop_connector ) delete drag_and_drop_connector;
				drag_and_drop_connector = min_dock_begin->make_connector( this );
				drag_and_drop_connector->dock_begin = min_dock_begin;
				drag_and_drop_connector->dock_end   = min_dock_end;
				min_dock_begin->connectors.push_back( drag_and_drop_connector );
				min_dock_end->connectors.push_back( drag_and_drop_connector );
			}
		} else if ( drag_and_drop_connector ) {
			delete drag_and_drop_connector;
			drag_and_drop_connector = NULL;
		}
	}
}

void RPObjectFlowChart::onDragEnter( const RPObjectClassInfo* classInfo, const rp::point& point )
{
	drag_and_drop_shape = static_cast<RPShape*>(rpMethod::factory->getNewObject( classInfo->getClassName(), this ));
	drag_and_drop_shape->setPosition( point.x, point.y );
	findAutoConnector();
	update();
}
/*
int timeDelta( const SYSTEMTIME& time1, const SYSTEMTIME& time2 )
{
	if ( time1.wYear != time2.wYear || time1.wMonth != time2.wMonth || time1.wDay != time2.wDay ) return 1000;
	int _time1 = time1.wMilliseconds + time1.wSecond * 1000 + time1.wMinute * 1000 * 60 + time1.wHour * 1000 * 60 * 60;
	int _time2 = time2.wMilliseconds + time2.wSecond * 1000 + time2.wMinute * 1000 * 60 + time2.wHour * 1000 * 60 * 60;
	return _time1 - _time2;
}
*/
void RPObjectFlowChart::onDragOver( const rp::point& point )
{
	if ( drag_and_drop_shape ) {
		drag_and_drop_shape->setPosition( point.x, point.y );
		findAutoConnector();
		update();
	}
}

void RPObjectFlowChart::onDragLeave()
{
	if ( drag_and_drop_shape ) {
		delete drag_and_drop_shape;
		drag_and_drop_shape = NULL;
		if ( drag_and_drop_connector ) {
			delete drag_and_drop_connector;
			drag_and_drop_connector = NULL;
		}
		update();
	}
}

void RPObjectFlowChart::onDrop( const rp::point& point )
{
	UNUSED(point);

	if ( drag_and_drop_shape ) {
		if ( rpMethod::project->getFlowState() == RPProject::flow_select || rpMethod::project->getFlowState() == RPProject::flow_rotate ) {
			drag_and_drop_shape->setSelected( true );
		}
		drag_and_drop_shape     = NULL;
		drag_and_drop_connector = NULL;
		update();
	}
}

rp::point RPObjectFlowChart::mouse_current() const
{
	CPoint point = global_win_pos_current;
	flowchart->ScreenToClient( &point );
	clientToZero( point );
	return point;
}

rp::point RPObjectFlowChart::mouse_prev() const
{
	CPoint point = global_win_pos_prev;
	flowchart->ScreenToClient( &point );
	clientToZero( point );
	return point;
}

void RPObjectFlowChart::modify()
{
}

void RPObjectFlowChart::update()
{
	if ( flowchart && can_update ) {
		flowchart->InvalidateRect( NULL );
		flowchart->UpdateWindow();
	}
}

void RPObjectFlowChart::setActive( bool value )
{
	if ( value ) setSelected( true );
	rpMethod::project->setActiveFlowChart( value ? this : NULL );
}

RPProject::Cursor RPObjectFlowChart::getCursor( const rp::point& global_chart_pos )
{
	// Вышли за пределы листа
	if ( pointInNCArea( global_chart_pos ) ) return RPProject::cursor_flow_select;

	// Если есть выделенный объект, то проверим сначала его
	if ( one_selected && one_selected->pointInShape(global_chart_pos) ) {
		RPProject::Cursor cursor = one_selected->getCursor( global_chart_pos );
		if ( cursor != RPProject::cursor_flow_select ) return cursor;
	}

	// Проверили все объекты на листе
	RPProject::Cursor cursor = RPObjectMatrix::getCursor( global_chart_pos );
	if ( cursor != RPProject::cursor_flow_select ) return cursor;

	// Объектов не нашли, выставляем курсор для листа
	switch ( rpMethod::project->getFlowState() ) {
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

rp::rect RPObjectFlowChart::getBoundingRect( bool global ) const
{
	UNUSED(global);

	rp::rect rect = const_cast<RPObjectFlowChart*>(this)->getMaxRect();
	return rp::rect( rect.getMinX() + 100, rect.getMinY() + 100, rect.getMaxX() - 100, rect.getMaxY() - 100 );
}

rp::rect RPObjectFlowChart::getMaxRect()
{
	rp::rect rect = RPObjectMatrix::getMaxRect();
	return rp::rect( rect.getMinX() - matrix_transform.dx_const() - 100, rect.getMinY() - matrix_transform.dy_const() - 100, rect.getMaxX() - matrix_transform.dx_const() + 100, rect.getMaxY() - matrix_transform.dy_const() + 100 );
}

RPPixmap* bmp = NULL;

void RPObjectFlowChart::draw( CDC& dc )
{
#ifdef TEST_SPEED // -------------------------------------
	SYSTEMTIME t0;
	SYSTEMTIME t1;
	SYSTEMTIME t2;
	SYSTEMTIME t3;
	SYSTEMTIME t4;
	SYSTEMTIME t5;
	SYSTEMTIME t6;
	SYSTEMTIME t7;
	::GetSystemTime( &t0 );
#endif // ------------------------------------------------

	std::list< RPObjectChart* > objects;
	getChartObjects( objects );

	CRect client_rect;
	flowchart->GetClientRect( &client_rect );
	client_width  = client_rect.Width();
	client_height = client_rect.Height();

	if ( client_width < border_w * 2 || client_height < border_h * 2 ) {

#ifdef TEST_SPEED // -------------------------------------
		for ( int cnt1 = 0; cnt1 <= base_speed * 10; cnt1++ ) {
#endif // ------------------------------------------------

		dc.FillSolidRect( 0, 0, client_width, client_height, paper_border_color );

#ifdef TEST_SPEED // -------------------------------------
		}
		::GetSystemTime( &t1 );
#endif // ------------------------------------------------

	} else {

#ifdef TEST_SPEED // -------------------------------------
		for ( int cnt1 = 0; cnt1 <= base_speed * 10; cnt1++ ) {
#endif // ------------------------------------------------

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

#ifdef TEST_SPEED // -------------------------------------
		}
		::GetSystemTime( &t1 );
		for ( int cnt2 = 0; cnt2 <= base_speed * 10; cnt2++ ) {
#endif // ------------------------------------------------

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

#ifdef TEST_SPEED // -------------------------------------
		}
		::GetSystemTime( &t2 );
#endif // ------------------------------------------------

		if ( pixmap_w_show > 0 && pixmap_h_show > 0 ) {

#ifdef TEST_SPEED // -------------------------------------
			for ( int cnt3 = 0; cnt3 <= base_speed; cnt3++ ) {
#endif // ------------------------------------------------
/*
			if ( grid_show ) {
				int x_start = scroll_x_pos / grid_bmp_width;
				int y_start = scroll_y_pos / grid_bmp_width;
				int x_stop  = x_start + pixmap_w_show / grid_bmp_width + 1;
				int y_stop  = y_start + pixmap_h_show / grid_bmp_width + 1;
#ifdef TEST_SPEED // -------------------------------------
				if ( cnt3 == 0 ) {
#endif // ------------------------------------------------
				TRACE( "x_start = %d, y_start = %d, x_stop = %d, y_stop = %d\n", x_start, y_start, x_stop, y_stop );
#ifdef TEST_SPEED // -------------------------------------
				}
#endif // ------------------------------------------------
				for ( int i = x_start; i <= x_stop; i++ ) {
					for ( int j = y_start; j <= y_stop; j++ ) {
						mem_dc.BitBlt( -scroll_x_pos + paper_border_w + i * grid_bmp_width, -scroll_y_pos + paper_border_h + j * grid_bmp_width, grid_bmp_width, grid_bmp_width, &grid_dc, 0, 0, SRCCOPY );
					}
				}
			}
*/
			mem_dc.FillSolidRect( 0, 0, pixmap_w_show, pixmap_h_show, paper_bg_color );

#ifdef TEST_SPEED // -------------------------------------
			}
			::GetSystemTime( &t3 );

			for ( int cnt4 = 0; cnt4 <= base_speed * 10; cnt4++ ) {
				std::list< RPObjectChart* >::iterator it = objects.begin();
				while ( it != objects.end() ) {
					it++;
				}
			}
			::GetSystemTime( &t4 );
#endif // ------------------------------------------------

//			std::list< RPObjectChart* >::iterator it = objects.begin();
//			while ( it != objects.end() ) {
//				RPObjectChart* object = *it;
//				object->transformToGlobal();
//				it++;
//			}

#ifdef TEST_SPEED // -------------------------------------
			for ( int cnt5 = 0; cnt5 <= base_speed; cnt5++ ) {
#endif // ------------------------------------------------
/*
			if ( !bmp ) bmp = rpMethod::project->createBitmap( red );
			CDC dc2;
			dc2.CreateCompatibleDC( &dc );
			HGDIOBJ bmp_old = dc2.SelectObject( bmp->getBitmap() );
			mem_dc.BitBlt( getX(), getY(), bmp->getWidth(), bmp->getHeight(), &dc2, 0, 0, SRCCOPY );
			dc2.SelectObject( bmp_old );
*/
			RPObjectMatrix::draw( mem_dc );

#ifdef TEST_SPEED // -------------------------------------
			}
			::GetSystemTime( &t5 );
#endif // ------------------------------------------------

#ifdef TEST_SPEED // -------------------------------------
			for ( int cnt6 = 0; cnt6 <= base_speed * 10; cnt6++ ) {
#endif // ------------------------------------------------
#ifdef TEST_SPEED // -------------------------------------
			}
			::GetSystemTime( &t6 );
#endif // ------------------------------------------------

			std::list< RPObjectChart* >::iterator it = objects.begin();
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
			if ( trash_show ) {
//				trash_bmp->Draw( mem_dc.m_hDC, trash_rect.left, trash_rect.top, trash_rect.Width() );
			}

#ifdef TEST_SPEED // -------------------------------------
			for ( int cnt7 = 0; cnt7 <= base_speed * 5; cnt7++ ) {
#endif // ------------------------------------------------

			dc.BitBlt( border_w + paper_border_w, border_h + paper_border_h, pixmap_w_show, pixmap_h_show, &mem_dc, 0, 0, SRCCOPY );
			
#ifdef TEST_SPEED // -------------------------------------
			}
			::GetSystemTime( &t7 );
#endif // ------------------------------------------------
		}
	}

#ifdef TEST_SPEED // -------------------------------------
	int delay1 = (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds) - (t0.wMinute * 1000 * 60 + t0.wSecond * 1000 + t0.wMilliseconds);
	int delay2 = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	int delay3 = ((t3.wMinute * 1000 * 60 + t3.wSecond * 1000 + t3.wMilliseconds) - (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds)) * 10;
	int delay4 = (t4.wMinute * 1000 * 60 + t4.wSecond * 1000 + t4.wMilliseconds) - (t3.wMinute * 1000 * 60 + t3.wSecond * 1000 + t3.wMilliseconds);
	int delay5 = ((t5.wMinute * 1000 * 60 + t5.wSecond * 1000 + t5.wMilliseconds) - (t4.wMinute * 1000 * 60 + t4.wSecond * 1000 + t4.wMilliseconds)) * 10;
	int delay6 = (t6.wMinute * 1000 * 60 + t6.wSecond * 1000 + t6.wMilliseconds) - (t5.wMinute * 1000 * 60 + t5.wSecond * 1000 + t5.wMilliseconds);
	int delay7 = ((t7.wMinute * 1000 * 60 + t7.wSecond * 1000 + t7.wMilliseconds) - (t6.wMinute * 1000 * 60 + t6.wSecond * 1000 + t6.wMilliseconds)) * 2;
	int delay0 = delay1 + delay2 + delay3 + delay4 + delay5 + delay6 + delay7;
	TRACE( "paint_delay = %d: %d, %d, %d, %d, %d, %d, %d\n", delay0, delay1, delay2, delay3, delay4, delay5, delay6, delay7 );
#endif // ------------------------------------------------

}

void RPObjectFlowChart::makeNewPixmap()
{
#ifdef TEST_SPEED // -------------------------------------
	makepixmap_cnt++;

	SYSTEMTIME t1;
	SYSTEMTIME t2;
	::GetSystemTime( &t1 );
#endif // ------------------------------------------------

	int pixmap_w_show_old = pixmap_w_show;
	int pixmap_h_show_old = pixmap_h_show;

#ifdef TEST_SPEED // -------------------------------------
	for ( int cnt = 0; cnt <= base_speed * 10; cnt++ ) {
#endif // ------------------------------------------------

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

#ifdef TEST_SPEED // -------------------------------------
	}
	::GetSystemTime( &t2 );
	int delay = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	TRACE( "makepixmap_delay = %d\n", delay );
#endif // ------------------------------------------------

	if ( pixmap_w_show_old != pixmap_w_show || pixmap_h_show_old != pixmap_h_show ) {
		flowchart->makeGrid();
	}
	trash_rect.top    = client_height - border_w * 2 - paper_border_w * 2 - trash_rect.Height();
	trash_rect.bottom = trash_rect.top + 32;
	trash_show        = trash_rect.top >= 0;
}

void RPObjectFlowChart::snapToGrid( RPObjectMatrix* shape )
{
	UNUSED(shape);

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
	RPObjectChart* obj = RPObjectMatrix::find( global_chart_pos );
	return obj != this ? obj : NULL;
}

void RPObjectFlowChart::insert_connector( RPConnectorDock* dock )
{
	if ( !one_connector && dock ) {
		dock_begin    = dock;
		one_connector = dock->make_connector( this );
		one_connector->dock_begin = dock;
		dock->connectors.push_back( one_connector );
		ct_wanted = ctw_end;
		update();
		return;
	}
	if ( one_connector && dock ) {
		one_connector->dock_end = dock;
		dock->connectors.push_back( one_connector );
		one_connector = NULL;
		dock_begin    = NULL;
		ct_wanted = ctw_begin;
		update();
		return;
	}
}

void RPObjectFlowChart::onLButtonDown( UINT nFlags, CPoint local_win_pos )
{
	// Запомнили глобальные координаты мышки (глобальные в виндах, а не 2D-движке). Пригодятся в других функциях.
	global_win_pos_prev = local_win_pos;
	flowchart->ClientToScreen( &global_win_pos_prev );
	global_win_pos_current = global_win_pos_prev;
	// Глобальные координаты в 2D-движке
	rp::point global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	// Монопольно захватили мышку
	flowchart->SetCapture();
	// Нашли объект под мышкой
	if ( one_selected && one_selected->pointInShape(global_chart_pos) ) {
		one_object = one_selected;
	} else {
		one_object = find( global_chart_pos );
	}
	if ( one_object ) {
		// Нашли фигуру
		bool selected = one_object->isSelected();
		if ( rpMethod::project->getFlowState() == RPProject::flow_select || rpMethod::project->getFlowState() == RPProject::flow_rotate ) {
			one_object->onLButtonDown( nFlags, CPoint( static_cast<int>(global_chart_pos.x), static_cast<int>(global_chart_pos.y) ) );
		}
		one_object->command_before( global_chart_pos, !selected );
		return;
	}
	// Никого не нашли, выбираем лист
	one_selected = NULL;
	RPObjectMatrix::onLButtonDown( nFlags, CPoint( static_cast<int>(global_chart_pos.x), static_cast<int>(global_chart_pos.y) ) );
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
	if ( obj ) obj->onMouseMove( nFlags, CPoint( static_cast<int>(global_chart_pos.x), static_cast<int>(global_chart_pos.y) ) );
	// Выполнение команды над выделенным объектом
	if ( one_object ) {
		one_object->command_make( global_chart_pos );
		// А не над корзиной ли ?
		if ( one_object->can_delete() ) {
			trash_over = is_trash_over( global_chart_pos );
			// Приходится выставлять курсор прямо тут, т.к. getCursor не вызывается, если мышка зажата
			HCURSOR cur = rpMethod::project->cursors[ trash_over ? RPProject::cursor_flow_trash : RPProject::cursor_flow_move ];
			if ( cur ) {
				::SetCursor( cur );
			}
		}
	}

	if ( one_connector ) one_connector->update();

	global_win_pos_prev = global_win_pos_current;
}

void RPObjectFlowChart::onLButtonUp( UINT nFlags, CPoint local_win_pos )
{
	// Отдали виндам мышку
	::ReleaseCapture();

	if ( one_object ) {
		if ( trash_over ) {
			// Будем удалять объект
			if ( one_selected == one_object ) one_selected = NULL;
			delete one_object;
			one_object = NULL;
			update();
		} else {
			CPoint global_chart_pos = local_win_pos;
			clientToZero( global_chart_pos );
			one_object->onLButtonUp( nFlags, global_chart_pos );
			one_object = NULL;
		}
	}

	// Обновить скролл
	flowchart->updateScrollBars();
}

void RPObjectFlowChart::onLButtonDblClk( UINT nFlags, CPoint local_win_pos )
{
	CPoint global_chart_pos = local_win_pos;
	clientToZero( global_chart_pos );
	// Проверим корзину
	if ( is_trash_over(global_chart_pos) ) {
	} else {
		RPObjectChart* obj = find( global_chart_pos );
		if ( obj ) obj->onLButtonDblClk( nFlags, global_chart_pos );
	}
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

void RPObjectFlowChart::setBgBrush( const CBrush& brush )
{
	LOGBRUSH lb;
	const_cast<CBrush&>(brush).GetLogBrush( &lb );
	paper_bg_color = lb.lbColor;
	update();
}
