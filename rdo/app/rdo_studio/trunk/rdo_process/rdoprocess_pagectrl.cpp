/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_pagectrl.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      20.02.2006
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_pagectrl.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/mctranspblt/McTransparentBlit.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrl
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPPageCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

RPPageCtrl::RPPageCtrl():
	CWnd(),
	label_height( 0 ),
	fly_timer( 0 ),
	over_timer( 0 )
{
	it_current = items.end();
}

RPPageCtrl::~RPPageCtrl()
{
}

BOOL RPPageCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL );
//	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_BTNFACE+1), NULL );

	return TRUE;
}

int RPPageCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	return 0;
}

void RPPageCtrl::OnDestroy()
{
	if ( fly_timer ) {
		KillTimer( fly_timer );
		fly_timer = 0;
	}
	if ( over_timer ) {
		KillTimer( over_timer );
		over_timer = 0;
	}
	std::list< RPPageCtrlItem* >::iterator it = items.begin();
	while ( it != items.end() ) {
		(*it)->DestroyWindow();
		delete *it;
		items.erase( it );
		it = items.begin();
	}
	CWnd::OnDestroy();
}

void RPPageCtrl::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	repaint();
}

void RPPageCtrl::OnPaint()
{
	CPaintDC dc( this );
	if ( items.empty() ) {
		CRect client_rect;
		GetClientRect( client_rect );
		dc.FillSolidRect( client_rect, ::GetSysColor(COLOR_BTNFACE) );
	}
}

void RPPageCtrl::startFly()
{
	if ( !fly_timer ) {
		fly_timer = SetTimer( 2, 100, NULL );
	}
}

void RPPageCtrl::repaint()
{
	if ( items.empty() ) {
		Invalidate();
		RedrawWindow();
	} else {
		CRect client_rect;
		GetClientRect( client_rect );
		std::list< RPPageCtrlItem* >::const_iterator it = items.begin();
		int delta_height_before = 0;
		int delta_height_after  = 0;
		while ( it != it_current && it != items.end() ) {
			RPPageCtrlItem* item = *it;
			item->SetWindowPos( NULL, 0, delta_height_before, client_rect.Width(), label_height, 0 );
			item->show();
			delta_height_before += label_height;
			it++;
		}
		if ( it == it_current ) {
			it = items.end();
			it--;
			while ( it != it_current ) {
				RPPageCtrlItem* item = *it;
				if ( (client_rect.Height() - delta_height_after - label_height) < (delta_height_before + label_height) ) {
					item->hide();
				} else {
					delta_height_after += label_height;
					item->SetWindowPos( NULL, 0, client_rect.Height() - delta_height_after, client_rect.Width(), label_height, 0 );
					item->show();
				}
				it--;
			}
		}
		if ( it_current != items.end() ) {
			(*it_current)->SetWindowPos( NULL, 0, delta_height_before, client_rect.Width(), client_rect.Height() - delta_height_after - delta_height_before, 0 );
			(*it_current)->show();
		}
	}
}

RPPageCtrlItem* RPPageCtrl::prepareNewPage()
{
	if ( !items.empty() ) {
		if ( !items.back()->wnd ) return items.back();
	}
	RPPageCtrlItem* page = new RPPageCtrlItem();
	items.push_back( page );
	page->Create( _T(""), _T(""), 0, CRect(0,0,100,100), this, UINT(~0) );
	return page;
}

RPPageCtrlItem* RPPageCtrl::insertPage( CWnd* wnd, const rp::string& label )
{
	RPPageCtrlItem* page = prepareNewPage();
	it_current = items.end();
	it_current--;
	page->wnd = wnd;
	page->label.SetWindowText( label.c_str() );
	page->show();
	return page;
}

void RPPageCtrl::removePage( RPPageCtrlItem* page )
{
	std::list< RPPageCtrlItem* >::iterator it = std::find( items.begin(), items.end(), page );
	if ( it != items.end() ) {
		rbool current_flag = it_current == it;
		std::list< RPPageCtrlItem* >::iterator it_next = items.erase( it );
		if ( current_flag ) {
			it_current = it_next;
			if ( it_current == items.end() && it_current != items.begin() ) it_current--;
			if ( it_current == items.end()                                ) it_current = items.begin();
		}
		page->DestroyWindow();
		delete page;
		repaint();
	}
}

void RPPageCtrl::selectFirst()
{
	it_current = items.begin();
	repaint();
}

void RPPageCtrl::startOver()
{
	if ( !over_timer ) {
		over_timer = SetTimer( 1, 100, NULL );
	}
}

void RPPageCtrl::OnTimer( UINT nIDEvent )
{
	if ( nIDEvent == fly_timer ) {
		KillTimer( fly_timer );
		fly_timer = 0;
		if ( items.empty() || it_current == items.end() ) {
			repaint();
		} else {
			CRect client_rect;
			GetClientRect( client_rect );
			std::list< RPPageCtrlItem* >::const_iterator it = items.begin();
			std::list< RPPageCtrlItem* >::const_iterator it_last = it_current;
			it_last++;
			int delta_height_before = 0;
			int delta_height_after  = 0;
			while ( it != it_last ) {
				RPPageCtrlItem* item = *it;
				item->fly = true;
				CRect rect;
				item->GetWindowRect( &rect );
				ScreenToClient( rect );
				rect.top -= 40;
				if ( rect.top < delta_height_before ) {
					rect.top  = delta_height_before;
					item->fly = false;
				}
				item->SetWindowPos( NULL, 0, rect.top, client_rect.Width(), label_height, 0 );
				item->show();
				delta_height_before += label_height;
				it++;
			}
			if ( it != items.end() ) {
				it = items.end();
				it--;
				rbool last = true;
				while ( it != it_current ) {
					RPPageCtrlItem* item = *it;
					item->fly = true;
					CRect rect;
					item->GetWindowRect( &rect );
					ScreenToClient( rect );
					if ( (client_rect.Height() - delta_height_after - label_height) < (delta_height_before + label_height) ) {
						item->hide();
					} else {
						delta_height_after += label_height;
						rect.top += 40;
						if ( rect.top > client_rect.Height() - delta_height_after ) {
							rect.top  = client_rect.Height() - delta_height_after;
							item->fly = false;
						}
						item->SetWindowPos( NULL, 0, rect.top, 0, 0, SWP_NOSIZE );
						item->show();
					}
					last = false;
					it--;
				}
			}
			if ( it_current != items.end() ) {
				(*it_current)->SetWindowPos( NULL, 0, 0, client_rect.Width(), client_rect.Height() - delta_height_after - delta_height_before + label_height, SWP_NOMOVE );
				(*it_current)->show();
			}
			it = items.begin();
			while ( it != items.end() ) {
				(*it)->Invalidate();
				(*it)->RedrawWindow();
				it++;
			}
			it = items.begin();
			while ( it != items.end() ) {
				if ( (*it)->fly ) {
					fly_timer = SetTimer( 2, 10, NULL );
					break;
				}
				it++;
			}
			if ( !fly_timer ) {
				repaint();
			}
		}
	}
	if ( nIDEvent == over_timer ) {
		KillTimer( over_timer );
		over_timer = 0;
		CPoint point;
		if ( ::GetCursorPos( &point ) ) {
			std::list< RPPageCtrlItem* >::const_iterator it = items.begin();
			while ( it != items.end() ) {
				CPoint p = point;
				(*it)->label.ScreenToClient( &p );
				(*it)->label.updateOver( p );
				it++;
			}
		}
	}
	CWnd::OnTimer( nIDEvent );
}

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrlItemLabel
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPPageCtrlItemLabel, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

RPPageCtrlItemLabel::RPPageCtrlItemLabel():
	CWnd(),
	over_close_button( false )
{
}

RPPageCtrlItemLabel::~RPPageCtrlItemLabel()
{
}

BOOL RPPageCtrlItemLabel::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL );
//	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL );

	return TRUE;
}

int RPPageCtrlItemLabel::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
///???? по-моему что то не так
	label_bmp.LoadBitmap( IDB_PAGECTRL_PAGE );
	close_bmp.LoadBitmap( IDB_PAGECTRL_CLOSE );
	close_bmp_over.LoadBitmap( IDB_PAGECTRL_CLOSE_OVER );

	return 0;
}

void RPPageCtrlItemLabel::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
}

void RPPageCtrlItemLabel::OnPaint()
{
	CPaintDC dc( this );
	dc.SaveDC();
	CFont font;
	font.CreateStockObject( DEFAULT_GUI_FONT );
	dc.SelectObject( &font );
	CRect client_rect;
	GetClientRect( client_rect );
	dc.FillSolidRect( client_rect, ::GetSysColor(COLOR_BTNFACE) );
	CString str;
	GetWindowText( str );
	CRect label_bmp_rect = getLabelBMPRect();
	CRect close_bmp_rect = getCloseBMPRect();
	CRect text_rect = client_rect;
	text_rect.left  = label_bmp_rect.right + 2;
	text_rect.right = close_bmp_rect.left - 2;
	CRect text_rect_calc = client_rect;
	dc.DrawText( str, text_rect_calc, DT_SINGLELINE | DT_CENTER | DT_CALCRECT | DT_VCENTER );
	if ( text_rect_calc.Width() > text_rect.Width() ) {
		dc.DrawText( str, text_rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );
	} else {
		dc.DrawText( str, text_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	}
	CDC mem_dc;
	mem_dc.CreateCompatibleDC( &dc );
	mem_dc.SelectObject( &label_bmp );
	McTransparentBlt( dc.m_hDC, label_bmp_rect.left, label_bmp_rect.top, label_bmp_rect.Width(), label_bmp_rect.Height(), mem_dc.m_hDC, 0, 0, label_bmp_rect.Width(), label_bmp_rect.Height(), RGB(-1,-1,-1) );
	if ( over_close_button ) {
		mem_dc.SelectObject( &close_bmp_over );
	} else {
		mem_dc.SelectObject( &close_bmp );
	}
	McTransparentBlt( dc.m_hDC, close_bmp_rect.left, close_bmp_rect.top, close_bmp_rect.Width(), close_bmp_rect.Height(), mem_dc.m_hDC, 0, 0, close_bmp_rect.Width(), close_bmp_rect.Height(), RGB(-1,-1,-1) );
	dc.RestoreDC( -1 );
}

CRect RPPageCtrlItemLabel::getLabelBMPRect()
{
	CRect client_rect;
	GetClientRect( client_rect );
	BITMAP bmp;
	label_bmp.GetBitmap( &bmp );
	CRect rect;
	rect.left   = 2;
	rect.top    = 0;
	rect.right  = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	rect.right  = rect.left + 16;
	rect.bottom = rect.top + 16;
	return rect;
}

CRect RPPageCtrlItemLabel::getCloseBMPRect()
{
	CRect client_rect;
	GetClientRect( client_rect );
	BITMAP bmp;
	close_bmp.GetBitmap( &bmp );
	CRect rect;
	rect.left   = client_rect.Width() - bmp.bmWidth - 2;
	rect.top    = 2;
	rect.right  = rect.left + bmp.bmWidth;
	rect.bottom = rect.top + bmp.bmHeight;
	return rect;
}

void RPPageCtrlItemLabel::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	RPPageCtrlItem* page     = static_cast<RPPageCtrlItem*>(GetParent());
	RPPageCtrl*     pagectrl = static_cast<RPPageCtrl*>(page->GetParent());
	std::list< RPPageCtrlItem* >::const_iterator it = std::find( pagectrl->items.begin(), pagectrl->items.end(), page );
	if ( it != pagectrl->items.end() ) {
		CRect close_bmp_rect = getCloseBMPRect();
		if ( close_bmp_rect.PtInRect( point ) ) {
			AfxGetApp()->PostThreadMessage( RP_PAGECTRL_DELETEITEM, reinterpret_cast<WPARAM>(page), 0 );
		} else {
			pagectrl->it_current = it;
			pagectrl->startFly();
//			pagectrl->repaint();
		}
	}
}

void RPPageCtrlItemLabel::OnMouseMove( UINT nFlags, CPoint point )
{
	CWnd::OnMouseMove( nFlags, point );
	updateOver( point );
}

void RPPageCtrlItemLabel::updateOver( const CPoint& point )
{
	rbool prev = over_close_button;
	over_close_button = getCloseBMPRect().PtInRect( point ) ? true : false;
	if ( over_close_button != prev ) {
		Invalidate();
		RedrawWindow();
	}
	if ( over_close_button ) { static_cast<RPPageCtrl*>(GetParent()->GetParent())->startOver(); }
}

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrlItem
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPPageCtrlItem, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_BEGINDRAG, 1, RPPageCtrlItem::OnListCtrlBeginDrag)
END_MESSAGE_MAP()

RPPageCtrlItem::RPPageCtrlItem():
	CWnd(),
	wnd( NULL ),
	fly( false )
{
}

RPPageCtrlItem::~RPPageCtrlItem()
{
}

BOOL RPPageCtrlItem::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL );

	return TRUE;
}

int RPPageCtrlItem::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	label.Create( _T(""), _T(""), WS_DLGFRAME, CRect(0,0,200,200), this, UINT(~0) );

	RPPageCtrl* pagectrl = static_cast<RPPageCtrl*>(GetParent());
	if ( !pagectrl->label_height ) {
		CRect label_client_rect;
		CRect label_window_rect;
		label.GetClientRect( label_client_rect );
		label.GetWindowRect( label_window_rect );
		CFont font;
		font.CreateStockObject( DEFAULT_GUI_FONT );
		CDC* dc = label.GetDC();
		dc->SaveDC();
		dc->SelectObject( &font );
		CSize size = dc->GetTextExtent( "_" );
		dc->RestoreDC( -1 );
		ReleaseDC( dc );
		pagectrl->label_height = label_window_rect.Width() - label_client_rect.Width() + size.cy;
		if ( pagectrl->label_height < 16 + label_window_rect.Height() - label_client_rect.Height() ) pagectrl->label_height = 16 + label_window_rect.Height() - label_client_rect.Height();
	}
	label.SetWindowPos( NULL, 0, 0, 0, pagectrl->label_height, 0 );

	return 0;
}

BOOL RPPageCtrlItem::DestroyWindow()
{
	if ( wnd ) {
		wnd->DestroyWindow();
		delete wnd;
		wnd = NULL;
	}
	return CWnd::DestroyWindow();
}

void RPPageCtrlItem::setPixmap( RPPixmap& bmp )
{
	label.label_bmp.DeleteObject();
	BITMAP bmp_info;
	bmp.getCBitmap().GetBitmap( &bmp_info );
	label.label_bmp.CreateBitmapIndirect( &bmp_info );

	CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
	CDC mem_dc;
	if ( mem_dc.CreateCompatibleDC( desktopDC ) ) {
		HGDIOBJ bmp_old = mem_dc.SelectObject( label.label_bmp );
		mem_dc.PatBlt( 0, 0, bmp.getWidth(), bmp.getHeight(), WHITENESS );
		bmp.Draw( mem_dc.m_hDC, 0, 0, bmp.getWidth() );
		mem_dc.SelectObject( bmp_old );
	}

	CWnd::GetDesktopWindow()->ReleaseDC( desktopDC );
}

void RPPageCtrlItem::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	CRect client_rect;
	GetClientRect( client_rect );
	CRect label_window_rect;
	label.GetWindowRect( label_window_rect );
	label.SetWindowPos( NULL, 0, 0, client_rect.Width(), label_window_rect.Height(), 0 );
	if ( wnd ) {
		wnd->SetWindowPos( NULL, 0, label_window_rect.Height(), client_rect.Width(), client_rect.Height() - label_window_rect.Height(), 0 );
	}
}

void RPPageCtrlItem::hide()
{
	if ( !IsWindowVisible() ) return;
	ShowWindow( SW_HIDE );
	label.ShowWindow( SW_HIDE );
	wnd->ShowWindow( SW_HIDE );
}

void RPPageCtrlItem::show()
{
	if ( IsWindowVisible() ) return;
	label.ShowWindow( SW_SHOW );
	wnd->ShowWindow( SW_SHOW );
	ShowWindow( SW_SHOW );
}

void RPPageCtrlItem::OnListCtrlBeginDrag( NMHDR* pNMHDR, LRESULT* pResult )
{
	UNUSED(pNMHDR);

	// Это уведомление приходит только от CListCtrl, поэтому смело можем приводить тип
	CListCtrl* list_ctrl = static_cast<CListCtrl*>(wnd);
	list_ctrl->SetFocus();
	RPObjectClassInfo* class_info = NULL;
	if ( list_ctrl->GetItemCount() ) {
		int index = list_ctrl->GetNextItem( -1, LVNI_SELECTED );
		if ( index != -1 ) {
			class_info = reinterpret_cast<RPObjectClassInfo*>(list_ctrl->GetItemData( index ));
		}
	}
	if ( class_info ) {
		CSharedFile	sf( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT );
		CString		text( class_info->getClassName().c_str() );
		sf.Write( text, text.GetLength() );
		HGLOBAL hglobal = sf.Detach();
		if ( hglobal ) {
			COleDataSource source;
			source.CacheGlobalData( CF_TEXT, hglobal );
			DROPEFFECT result = source.DoDragDrop();
			if ( result == DROPEFFECT_MOVE ) {
				TRACE( "drag&drop OK\n" );
			} else {
				TRACE( "drag&drop FAILD\n" );
			}
			::GlobalFree( hglobal );
		}
	}
	*pResult = 0;
}

