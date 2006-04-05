#include "stdafx.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_object_flowchart.h"
#include "rdoprocess_object.h"
#include "rdoprocess_shape_action.h"
#include "rdoprocess_shape_if.h"

// MJ start
#include "rdoprocess_shape_process_MJ.h" 
#include "rdoprocess_shape_create_MJ.h"
#include "rdoprocess_shape_terminate_MJ.h"
#include "rdoprocess_shape_resource.h"

// MJ end

#include "rdoprocess_app.h"

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
	scroll_inited( false ),
	scroll_delta( 0, 0 ),
	scroll_size( 0, 0 ),
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

RPShapeAction*      shape_action  = NULL;
RPShapeIf*          shape_if      = NULL;
RPShapeCreateMJ*    shape_create  = NULL; // MJ
RPShapeProcessMJ*   shape_process = NULL; // MJ
RPShapeTerminateMJ* shape_terminate = NULL; // MJ
RPShapeResource_MJ*    shape_resource = NULL; // MJ 1.04.06

BOOL RPFlowChart::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	if ( !CWnd::Create( lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext ) ) return false;

	// «апомнить состо€ни€ DC дл€ восстановлени€ перед удалением
	CDC* dc  = GetDC();
	saved_dc = dc->SaveDC();

	flowobj  = new RPObjectFlowChart( &rpapp.project(), this );

//	updateFont();
	updateScrollBars();
/*
	RPShapeAction* action = new RPShapeAction( flowobj );
	action->setPosition( 100, 45 );
	action = new RPShapeAction( flowobj );
	action->setPosition( 250, 145 );
*/

	shape_action = new RPShapeAction( flowobj );
	shape_action->setPosition( 100, 45 );
	shape_if = new RPShapeIf( flowobj );
	shape_if->setPosition( 300, 100 );

	// MJ start
	shape_create = new RPShapeCreateMJ( flowobj );
	shape_create->setPosition( 100,100 );

	shape_process = new RPShapeProcessMJ( flowobj );
	shape_process->setPosition( 100,150 );

	shape_terminate = new RPShapeTerminateMJ( flowobj );
	shape_terminate->setPosition( 100,200 );

	shape_resource = new RPShapeResource_MJ( flowobj );
	shape_resource->setPosition( 100,250 );

	shape_resource = new RPShapeResource_MJ( flowobj );
	shape_resource->setPosition( 100,300 );
	// MJ end


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

void RPFlowChart::updateScrollBars()
{
	rp::rect rect = flowobj->getMaxRect();
	if ( scroll_delta.x < -rect.getMinX() ) scroll_delta.x = -rect.getMinX();
	if ( scroll_delta.y < -rect.getMinY() ) scroll_delta.y = -rect.getMinY();
//	if ( scroll_delta.x < flowobj->pixmap_w_show ) scroll_delta.x = flowobj->pixmap_w_show;
//	if ( scroll_delta.y < flowobj->pixmap_h_show ) scroll_delta.y = flowobj->pixmap_h_show;

	if ( scroll_size.x  < rect.getMaxX() ) scroll_size.x = rect.getMaxX();
	if ( scroll_size.y  < rect.getMaxY() ) scroll_size.y = rect.getMaxY();
	if ( scroll_size.x  < flowobj->pixmap_w_show ) scroll_size.x = flowobj->pixmap_w_show;
	if ( scroll_size.y  < flowobj->pixmap_h_show ) scroll_size.y = flowobj->pixmap_h_show;

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;

	si.nMin   = 0;
	si.nMax   = scroll_size.x + scroll_delta.x;
	si.nPos   = scroll_delta.x - flowobj->matrix_transform.dx();
	si.nPage  = flowobj->pixmap_w_show;
	SetScrollInfo( SB_HORZ, &si );

	si.nMin   = 0;
	si.nMax   = scroll_size.y + scroll_delta.y;
	si.nPos   = scroll_delta.y - flowobj->matrix_transform.dy();
	si.nPage  = flowobj->pixmap_h_show;
	SetScrollInfo( SB_VERT, &si );
}

void RPFlowChart::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
//	scroll_inited = false;
//	grid_pa.resize( 0 );
	if ( flowobj ) {
		flowobj->makeNewPixmap();
		flowobj->update();
	}
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
			flowobj->matrix_transform.dx() = scroll_delta.x - si.nTrackPos;
			break;
		}
	}
	if ( scroll_delta.x < flowobj->matrix_transform.dx_const() ) {
		int delta      = flowobj->matrix_transform.dx_const() - scroll_delta.x;
		scroll_delta.x = flowobj->matrix_transform.dx_const();
		scroll_size.x += delta;
	}
	if ( scroll_size.x < -flowobj->matrix_transform.dx_const() ) {
		int delta = -flowobj->matrix_transform.dx_const() - scroll_size.x;
		if ( delta > 1 ) {
			scroll_size.x += delta;
		} else {
			flowobj->matrix_transform.dx() += 1;
		}
	}
	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = scroll_size.x + scroll_delta.x;
	si.nPos   = scroll_delta.x - flowobj->matrix_transform.dx();
	si.nPage  = flowobj->pixmap_w_show;
	SetScrollInfo( SB_HORZ, &si );
	if ( flowobj ) flowobj->update();
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
			flowobj->matrix_transform.dy() = scroll_delta.y - si.nTrackPos;
			break;
		}
	}
	if ( scroll_delta.y < flowobj->matrix_transform.dy_const() ) {
		int delta      = flowobj->matrix_transform.dy_const() - scroll_delta.y;
		scroll_delta.y = flowobj->matrix_transform.dy_const();
		scroll_size.y += delta;
	}
	if ( scroll_size.y < -flowobj->matrix_transform.dy_const() ) {
		int delta = -flowobj->matrix_transform.dy_const() - scroll_size.y;
		if ( delta > 1 ) {
			scroll_size.y += delta;
		} else {
			flowobj->matrix_transform.dy() += 1;
		}
	}
	si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = scroll_size.y + scroll_delta.y;
	si.nPos   = scroll_delta.y - flowobj->matrix_transform.dy();
	si.nPage  = flowobj->pixmap_h_show;
	SetScrollInfo( SB_VERT, &si );
	if ( flowobj ) flowobj->update();
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

void RPFlowChart::OnMouseMove( UINT nFlags, CPoint local_win_pos )
{
	CWnd::OnMouseMove( nFlags, local_win_pos );
	flowobj->onMouseMove( nFlags, local_win_pos );
}

// ‘ункци€ вызываетс€ при перемещении мышки над окном и при клике, но не вызываетс€,
// когда мышка перемещаетс€ над окном с зажатой клавишей
BOOL RPFlowChart::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	flowobj->clientToZero( point );
	HCURSOR cur = NULL;
	RPProject::Cursor cursor = flowobj->getCursor( point );
	switch ( cursor ) {
		case RPProject::cursor_flow_select       : cur = rpapp.cursors[IDC_FLOW_SELECT]; break;
		case RPProject::cursor_flow_move         : cur = rpapp.cursors[IDC_FLOW_MOVE]; break;
		case RPProject::cursor_flow_connector    : cur = rpapp.cursors[IDC_FLOW_CONNECTOR]; break;
		case RPProject::cursor_flow_rotate       : cur = rpapp.cursors[IDC_FLOW_ROTATE]; break;
		case RPProject::cursor_flow_rotate_center: cur = rpapp.cursors[IDC_FLOW_ROTATE_CENTER]; break;
		case RPProject::cursor_flow_rotate_tl    :
		case RPProject::cursor_flow_rotate_tr    :
		case RPProject::cursor_flow_rotate_bl    :
		case RPProject::cursor_flow_rotate_br    : cur = rpapp.cursors[IDC_FLOW_ROTATE_TL]; break;
		case RPProject::cursor_flow_scale_lr     : cur = rpapp.cursors[IDC_FLOW_SCALE_LR]; break;
		case RPProject::cursor_flow_scale_tb     : cur = rpapp.cursors[IDC_FLOW_SCALE_TB]; break;
		case RPProject::cursor_flow_scale_tlbr   : cur = rpapp.cursors[IDC_FLOW_SCALE_TLBR]; break;
		case RPProject::cursor_flow_scale_trbl   : cur = rpapp.cursors[IDC_FLOW_SCALE_TRBL]; break;
		case RPProject::cursor_flow_dock_in      :
		case RPProject::cursor_flow_dock_out     :
		case RPProject::cursor_flow_dock_inout   :
		case RPProject::cursor_flow_dock_fly     : cur = rpapp.cursors[IDC_FLOW_DOCK_IN]; break;
		case RPProject::cursor_flow_dock_not     : cur = rpapp.cursors[IDC_FLOW_DOCK_NOT]; break;
	}
	if ( cur ) {
		::SetCursor( cur );
		return true;
	}
	return false;
}
