#include "stdafx.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_object.h"
#include "rdoprocess_shape_action.h"
#include "rdoprocess_shape_if.h"
#include "rdoprocess_app.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCFlowChartObject
// ----------------------------------------------------------------------------
RDOPROCFlowChartObject::RDOPROCFlowChartObject( RDOPROCObject* _parent, RDOPROCChartObject* _chart_parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCChartObject( _parent, _chart_parent, _flowchart )
{
	rpapp.connect( this, RP_FLOWSTATE_CHANGED );
}

void RDOPROCFlowChartObject::notify( RDOPROCObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == RP_FLOWSTATE_CHANGED ) {
		flowchart->updateFlowState();
		RDOPROCProject::FlowState flow_state = static_cast<RDOPROCProject::FlowState>(wParam);
		switch ( flow_state ) {
			case RDOPROCProject::flow_select   : break;
			case RDOPROCProject::flow_connector: break;
			case RDOPROCProject::flow_rotate   : break;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCFlowChart
// ----------------------------------------------------------------------------
const int grid_dx_plus = 1;
const int grid_dy_plus = 2;
const int grid_timer_id = 1;

#ifdef TEST_SPEED // =====================================
const int sec_timer_id = 2;
const int base_speed = 10000;
#endif // ================================================

BEGIN_MESSAGE_MAP( RDOPROCFlowChart,CWnd )
	//{{AFX_MSG_MAP(RDOPROCFlowChart)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPROCFlowChart::RDOPROCFlowChart():
	CWnd(),
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
	mem_bmp( NULL ),
	saved_dc( 0 ),
	saved_mem_dc( 0 ),
	font_first( NULL ),
	bmp_first( NULL ),
	pen_black( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	pen_shape_color( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	pen_selected_line( PS_DOT, 1, RGB(0x00, 0xFF, 0x00) ),
	pen_selected_box( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) ),
	brush_selected_box( RGB(0x00, 0xFF, 0x00) ),
	paper_border_color( RGB(0xC0, 0xC0, 0xC0) ),
	paper_shadow_color( RGB(0x80, 0x80, 0x80) ),
	paper_bg_color( RGB(0xFF, 0xFF, 0xFF) ),
	chobj( NULL ),
	global_old_x( 0 ),
	global_old_y( 0 )
#ifdef TEST_SPEED // =====================================
	,
	sec_cnt( 0 ),
	sec_timer( 0 ),
	makepixmap_cnt( 0 ),
	makegrid_cnt( 0 ),
	makegridempty_cnt( 0 )
#endif // ================================================
{
	chobj = new RDOPROCFlowChartObject( &rpapp.project, NULL, this );
}

RDOPROCFlowChart::~RDOPROCFlowChart()
{
}

BOOL RDOPROCFlowChart::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

RDOPROCShapeAction* shape_action = NULL;
RDOPROCShapeIf*     shape_if     = NULL;

BOOL RDOPROCFlowChart::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	if ( !CWnd::Create( lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext ) ) return false;

	CDC* dc = GetDC();
	if ( !mem_dc.CreateCompatibleDC( dc ) ) {
		ReleaseDC( dc );
		return false;
	}

	// Запомнить состояния DC для восстановления перед удалением
	saved_dc     = dc->SaveDC();
	saved_mem_dc = mem_dc.SaveDC();
	font_first   = mem_dc.GetCurrentFont();
	bmp_first    = mem_dc.GetCurrentBitmap();
	ReleaseDC( dc );
	if ( !saved_dc || !saved_mem_dc ) return false;

	mem_dc.SetBkMode( TRANSPARENT );

//	updateFont();
	updateScrollBars();

	shape_action = new RDOPROCShapeAction( chobj, this );
	shape_if = new RDOPROCShapeIf( chobj, this );
	shape_if->setPosition( 200, 100 );

#ifdef TEST_SPEED // =====================================
	sec_timer = SetTimer( sec_timer_id, 1000, NULL );
#endif // ================================================

	return true;
}

void RDOPROCFlowChart::OnDestroy()
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
	if ( chobj ) {
		delete chobj;
		chobj = NULL;
	}
	CDC* dc = GetDC();
	if ( dc ) {
		dc->RestoreDC( saved_dc );
	}
	ReleaseDC( dc );
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
//	if ( grid_bmp.GetSafeHandle() ) {
//		grid_dc.SelectObject( bmp_first );
//		grid_bmp.DeleteObject();
//	}
	CWnd::OnDestroy();
}

std::list< RDOPROCShape* >::iterator RDOPROCFlowChart::find( const RDOPROCShape* shape )
{
	return std::find( shapes.begin(), shapes.end(), shape );
}

void RDOPROCFlowChart::insertShape( RDOPROCShape* shape )
{
	if ( find( shape ) == shapes.end() ) {
		shapes.push_back( shape );
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

void RDOPROCFlowChart::deleteShape( RDOPROCShape* shape )
{
	std::list< RDOPROCShape* >::iterator it = find( shape );
	if ( it != shapes.end() ) {
/*
		std::list< CBDFlowChartConnector >::iterator connector = connectors.begin();
		while ( connector != connectors.end() ) {
			if ( connector->shape_from == shape || connector->shape_to == shape ) {
				connector = connectors.erase( connector );
			} else {
				connector++;
			}
		}
*/
		shapes.erase( it );
//		if ( movingShape == shape ) {
//			movingShape = NULL;
//			moving_stop();
//		}
		updateScrollBars();
		updateDC();
	}
}

CSize RDOPROCFlowChart::getFlowSize( const std::list< RDOPROCShape* >& list ) const
{
	int max_x = 0;
	int max_y = 0;
	std::list< RDOPROCShape* >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		rp::RPRect rect = (*it)->getBoundingRect();
		if ( rect.getMaxX() > max_x ) {
			max_x = rect.getMaxX();
		}
		if ( rect.getMaxY() > max_y ) {
			max_y = rect.getMaxY();
		}
		it++;
	}
	return CSize( max_x, max_y );
}

void RDOPROCFlowChart::updateScrollBars()
{
	CSize size = getFlowSize();

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;

	int max_x = size.cx + border_w * 2 + paper_border_w * 2 - 1;
	int max_y = size.cy + border_h * 2 + paper_border_h * 2 - 1;
	int scroll_x_pos = chobj->matrix_transform.dx();
	int scroll_y_pos = chobj->matrix_transform.dy();
	if ( scroll_x_pos + pixmap_w_show > max_x ) scroll_x_pos = max_x - pixmap_w_show;
	if ( scroll_y_pos + pixmap_h_show > max_y ) scroll_y_pos = max_y - pixmap_h_show;
	if ( scroll_x_pos < 0 ) scroll_x_pos = 0;
	if ( scroll_y_pos < 0 ) scroll_y_pos = 0;
	chobj->matrix_transform.dx() = scroll_x_pos;
	chobj->matrix_transform.dy() = scroll_y_pos;

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
}

void RDOPROCFlowChart::updateDC()
{
	InvalidateRect( NULL );
	UpdateWindow();
}

void RDOPROCFlowChart::modify()
{
}

void RDOPROCFlowChart::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
//	grid_pa.resize( 0 );
	makeNewPixmap();
	updateDC();
}

void RDOPROCFlowChart::makeNewPixmap()
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
	GetClientRect( &client_rect );
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
		CDC* dc = GetDC();
		mem_bmp->CreateCompatibleBitmap( dc, pixmap_w_real, pixmap_h_real );
		ReleaseDC( dc );
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

	updateScrollBars();

#ifdef TEST_SPEED // =====================================
	}
	::GetSystemTime( &t2 );
	int delay = (t2.wMinute * 1000 * 60 + t2.wSecond * 1000 + t2.wMilliseconds) - (t1.wMinute * 1000 * 60 + t1.wSecond * 1000 + t1.wMilliseconds);
	TRACE( "makepixmap_delay = %d\n", delay );
#endif // ================================================

	if ( pixmap_w_show_old != pixmap_w_show || pixmap_h_show_old != pixmap_h_show ) {
		makeGrid();
	}
}

void RDOPROCFlowChart::makeGrid()
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

void RDOPROCFlowChart::OnPaint() 
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

	CPaintDC dc( this );
	dc.SaveDC();

	CRect client_rect;
	GetClientRect( &client_rect );
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
				std::list< RDOPROCShape* >::iterator it = shapes.begin();
				while ( it != shapes.end() ) {
					it++;
				}
			}
			::GetSystemTime( &t4 );
#endif // ================================================

			std::list< RDOPROCShape* >::iterator it = shapes.begin();
			while ( it != shapes.end() ) {
				RDOPROCShape* shape = *it;
				shape->transformToGlobal();
				it++;
			}

#ifdef TEST_SPEED // =====================================
			for ( int cnt5 = 0; cnt5 <= base_speed; cnt5++ ) {
#endif // ================================================

			it = shapes.begin();
			while ( it != shapes.end() ) {
				int saved = mem_dc.SaveDC();
				(*it)->draw( mem_dc );
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

			it = shapes.begin();
			while ( it != shapes.end() ) {
				RDOPROCShape* shape = *it;
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
				if ( shape->isSelected() ) {
					if ( rpapp.project.getFlowState() == RDOPROCProject::flow_rotate ) {
						// Центр вращения
						CPoint center = shape->getRotateCenter();
						CPen pen_red( PS_SOLID, 1, RGB(0,0,0) );
						CBrush brush_white( RGB(-1,-1,0) );
						mem_dc.SelectObject( pen_red );
						mem_dc.SelectObject( brush_white );
						mem_dc.Ellipse( center.x - 5, center.y - 5, center.x + 5, center.y + 5 );
					}
					// Прямоугольник вокруг фигуры
					rp::RPRect rect = shape->getBoundingRect();
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
					int box_size    = 7;
					int box_size_2  = 4;
					if ( rpapp.project.getFlowState() == RDOPROCProject::flow_rotate ) {
						mem_dc.Ellipse( x0 - 5, y0 - 5, x0 + 5, y0 + 5 );
						mem_dc.Ellipse( x1 - 5, y1 - 5, x1 + 5, y1 + 5 );
						mem_dc.Ellipse( x2 - 5, y2 - 5, x2 + 5, y2 + 5 );
						mem_dc.Ellipse( x3 - 5, y3 - 5, x3 + 5, y3 + 5 );
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
	dc.RestoreDC( -1 );

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

void RDOPROCFlowChart::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_PAGEUP:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			chobj->matrix_transform.dx() += si.nPage;
			break; 

		case SB_PAGEDOWN:
			GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
			chobj->matrix_transform.dx() -= si.nPage;
			break;

		case SB_LINEUP:
			chobj->matrix_transform.dx()++;
			break;

		case SB_LINEDOWN:
			chobj->matrix_transform.dx()--;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			chobj->matrix_transform.dx() -= si.nTrackPos - chobj->matrix_transform.dx();
			break;
		}
	}
	CSize size = getFlowSize();
	int width = size.cx - pixmap_w_show + paper_border_w * 2;
	if ( chobj->matrix_transform.dx() > width ) chobj->matrix_transform.dx() = width;
	if ( chobj->matrix_transform.dx() < 0 ) chobj->matrix_transform.dx() = 0;
	si.fMask = SIF_POS;
	si.nPos  = chobj->matrix_transform.dx();
	SetScrollInfo( SB_HORZ, &si, TRUE );
	updateDC();
}

void RDOPROCFlowChart::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	switch( nSBCode ) {
		case SB_PAGEUP:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			chobj->matrix_transform.dy() += si.nPage;
			break; 

		case SB_PAGEDOWN:
			GetScrollInfo( SB_VERT, &si, SIF_PAGE );
			chobj->matrix_transform.dy() -= si.nPage;
			break;

		case SB_LINEUP:
			chobj->matrix_transform.dy()++;
			break;

		case SB_LINEDOWN:
			chobj->matrix_transform.dy()--;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			chobj->matrix_transform.dy() -= si.nTrackPos - chobj->matrix_transform.dy();
			break;
		}
	}
	CSize size = getFlowSize();
	int height = size.cy - pixmap_h_show + paper_border_h * 2;
	if ( chobj->matrix_transform.dy() > height ) chobj->matrix_transform.dy() = height;
	if ( chobj->matrix_transform.dy() < 0 ) chobj->matrix_transform.dy() = 0;
	si.fMask = SIF_POS;
	si.nPos  = chobj->matrix_transform.dy();
	SetScrollInfo( SB_VERT, &si, TRUE );
	updateDC();
}

void RDOPROCFlowChart::snapToGrid( RDOPROCShape* shape )
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
			double len1 = RDOPROCFlowChart::getDistance( c_pa[1], c_pa[2], center );
			double len2 = RDOPROCFlowChart::getDistance( c_pa[3], c_pa[0], center );
			if ( len1 < len2 ) {
				dx = c_pa[1].x - center.x;
			} else {
				dx = c_pa[3].x - center.x;
			}
			len1 = RDOPROCFlowChart::getDistance( c_pa[0], c_pa[1], center );
			len2 = RDOPROCFlowChart::getDistance( c_pa[2], c_pa[3], center );
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
void RDOPROCFlowChart::snapToGridAllShapes()
{
	std::vector< RDOPROCShape* >::const_iterator it = shapes.begin();
	while ( it != shapes.end() ) {
		snapToGrid( *it );
		it++;
	}
}
*/

RDOPROCShape* RDOPROCFlowChart::findObject( const int _x, const int _y ) const
{
//	return shape_if;
	int x = _x - border_w - paper_border_w;
	int y = _y - border_h - paper_border_h;
	std::list< RDOPROCShape* >::const_iterator it = shapes.begin();
	while ( it != shapes.end() ) {
		RDOPROCShape* shape = *it;
		rp::RPRect rect = shape->getBoundingRect().extendByPerimetr( shape->main_pen_width * sqrt(2) / 2.0 );
		if ( rect.pointInRect( x, y ) ) {
			shape->meshToGlobal();
			bool flag = shape->pointInPolygon( x, y );
//			shape->translate( -dx, -dy );
			if ( flag ) {
				return shape;
			}
		}
		it++;
	}
	return NULL;
}

/*
void RDOPROCFlowChart::buildMovingChild( std::list< RDOPROCShape* >& movingChildShapes, RDOPROCShape* from )
{
	const std::list< RDOPROCShape* >& output = from->getOutputShapes();
	std::list< RDOPROCShape* >::const_iterator it = output.begin();
	while ( it != output.end() ) {
		if ( std::find( movingChildShapes.begin(), movingChildShapes.end(), *it ) == movingChildShapes.end() ) {
			movingChildShapes.push_back( *it );
			buildMovingChild( movingChildShapes, *it );
		}
		it++;
	}
}
*/

void RDOPROCFlowChart::selectShapeOff()
{
	std::list< RDOPROCShape* >::iterator it = shapes.begin();
	while ( it != shapes.end() ) {
		(*it)->set_selected( false );
		it++;
	}
	updateDC();
}

void RDOPROCFlowChart::selectShape( RDOPROCShape* shape, const bool value )
{
//	selectShapeOff();

//	clearSelectedConnector();
//	if ( value && !hasFocus() ) {
//		setFocus();
//	}
	shape->set_selected( value );
	updateDC();
}

void RDOPROCFlowChart::moving_start( RDOPROCShape* shape, const int global_mouse_x, const int global_mouse_y )
{
	shape->setSelected( true );
//	movingShape = shape;
	global_old_x = global_mouse_x;
	global_old_y = global_mouse_y;
	SetCapture();
}

void RDOPROCFlowChart::moving( const int global_mouse_x, const int global_mouse_y )
{
	if ( !movingShapes.empty() ) {
		int dx = global_mouse_x - global_old_x;
		int dy = global_mouse_y - global_old_y;
		// Отсечь выход за границы листа (левый верхний угол)
		std::list< RDOPROCShape* >::const_iterator it = movingShapes.begin();
		while ( it != movingShapes.end() ) {
			RDOPROCShape* shape = *it;
			int delta = shape->getX() + dx;
			if ( delta < 0 ) {
				dx -= delta;
			}
			delta = shape->getY() + dy;
			if ( delta < 0 ) {
				dy -= delta;
			}
			it++;
		}
		// Переместить объекты
		it = movingShapes.begin();
		while ( it != movingShapes.end() ) {
			(*it)->moving( dx, dy );
			it++;
		}
		updateScrollBars();
		makeGrid();
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
		updateDC();

		global_old_x = global_mouse_x;
		global_old_y = global_mouse_y;
//		grid_wasMouseMoving = true;
	}
}

void RDOPROCFlowChart::moving_stop()
{
	::ReleaseCapture();
	movingShapes.clear();
/*
	grid_shapes.clear();
	grid_shapes.assign( movingShapes.begin(), movingShapes.end() );
//	movingShape = NULL;
	movingShapes.clear();
	if ( !grid_shapes.empty() ) {
		if ( grid_timer ) KillTimer( grid_timer );
		grid_timer = SetTimer( grid_timer_id, 100, NULL );
	}
*/
	updateScrollBars();
}

void RDOPROCFlowChart::OnTimer( UINT nIDEvent )
{
#ifdef TEST_SPEED // =====================================
	if ( nIDEvent == sec_timer ) {
		TRACE( "%d. makepixmap = %d, makegrid = %d, makegrid_empty = %d\n", sec_cnt++, makepixmap_cnt, makegrid_cnt, makegridempty_cnt );
		makegrid_cnt      = 0;
		makegridempty_cnt = 0;
		makepixmap_cnt    = 0;
	}
#endif // ================================================
//	if ( nIDEvent == grid_timer ) {
//		if ( grid_mode != gtSnapOff /*&& grid_wasMouseMoving*/ && !grid_shapes.empty() ) {
//			std::list< RDOPROCShape* >::const_iterator it = grid_shapes.begin();
//			while ( it != grid_shapes.end() ) {
//				snapToGrid( *it );
//				it++;
//			}
//			grid_shapes.clear();
////		grid_wasMouseMoving = false;
//		}
//	}
	CWnd::OnTimer( nIDEvent );
}

void RDOPROCFlowChart::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	RDOPROCShape* object = findObject( point.x, point.y );
//	clearSelectedConnector();
	if ( object ) {
		ClientToScreen( &point );
		moving_start( object, point.x, point.y );
		movingShapes.clear();
		if ( nFlags & MK_CONTROL ) {
			std::list< RDOPROCShape* >::const_iterator it = shapes.begin();
			while ( it != shapes.end() ) {
				movingShapes.push_back( *it );
				it++;
			}
//			movingShapes.assign( shapes.begin(), shapes.end() );
		} else if ( nFlags & MK_SHIFT ) {
			movingShapes.push_back( object );
//			buildMovingChild( movingShape );
		} else {
			movingShapes.push_back( object );
		}
	} else {
		selectShapeOff();
/*
		std::list< CBDFlowChartConnector >::iterator connector = connectors.begin();
		std::list< CBDFlowChartConnector >::iterator connector_found = connectors.end();
		int distance = -1;
		QPoint point( event->x() + scrollView->contentsX() - scrollView->paper_border, event->y() + scrollView->contentsY() - scrollView->paper_border );
		while ( connector != connectors.end() ) {
			int dist = connector->minDistance( point );
			if ( dist != -1 && (distance == -1 || distance > dist ) ) {
				distance = dist;
				connector_found = connector;
			}
			connector++;
		}
		if ( connector_found != connectors.end() && distance < 5 ) {
			connector_found->selected = true;
		}
*/
	}
}

void RDOPROCFlowChart::OnLButtonUp( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonUp( nFlags, point );
	moving_stop();
//	resetPreview();
}

void RDOPROCFlowChart::OnMouseMove( UINT nFlags, CPoint point )
{
	updateDC();

	CWnd::OnMouseMove( nFlags, point );
/*
	if ( new_shape ) {
		new_shape->move( event->x() - new_shape->getSize().width()/2 - scrollView->pixmap_x - scrollView->paper_border + scrollView->contentsX(), event->y() - new_shape->getSize().height()/2 - scrollView->pixmap_y - scrollView->paper_border + scrollView->contentsY() );
		updateScrollView();
		updateDC();
	} else if ( new_connector ) {
		new_connector_pos.setX( event->x() - scrollView->pixmap_x + scrollView->contentsX() );
		new_connector_pos.setY( event->y() - scrollView->pixmap_y + scrollView->contentsY() );
		updateDC();
	} else if ( movingShape ) {
*/
	if ( !movingShapes.empty() ) {
		ClientToScreen( &point );
		moving( point.x, point.y );
	}
}

void RDOPROCFlowChart::updateFlowState()
{
	updateDC();
}

BOOL RDOPROCFlowChart::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	switch ( rpapp.project.getFlowState() ) {
		case RDOPROCProject::flow_select   : ::SetCursor( rpapp.cursors[IDC_FLOW_SELECT] ); break;
		case RDOPROCProject::flow_connector: ::SetCursor( rpapp.cursors[IDC_FLOW_CONNECTOR] ); break;
		case RDOPROCProject::flow_rotate   : ::SetCursor( rpapp.cursors[IDC_FLOW_ROTATE] ); break;
	}
	return TRUE;
}
