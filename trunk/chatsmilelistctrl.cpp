#include "stdafx.h"
#include "chatsmilelistctrl.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "chatsplash.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSmileListCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatSmileListCtrl, CWnd )
	//{{AFX_MSG_MAP(CChatSmileListCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatSmileListCtrl::CChatSmileListCtrl():
	CWnd(),
	smile_max_width( 0 ),
	text_max_width( 0 ),
	text_focus_height( 0 ),
	prev_line_from( -1 ),
	prev_line_to( -1 ),
	xPos( 0 ),
	yPos( 0 ),
	xMax( 0 ),
	yMax( 0 ),
	xPageSize( 0 ),
	yPageSize( 0 ),
	newClientRect(0, 0, 0, 0 ),
	hasFocus( false ),
	selectedLine( -1 ),
	horzScrollBarVisible( false ),
	vertScrollBarVisible( false )
{
}

CChatSmileListCtrl::~CChatSmileListCtrl()
{
	std::vector< Item* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete (*it)->smile;
		delete *it++;
	}
	list.clear();
}

BOOL CChatSmileListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}

int CChatSmileListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	bool flag = false;
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );
			flag = font.CreateFontIndirect( &lf ) ? true : false;
		}
	}
	if ( !flag ) {
		LOGFONT lf;
		GetFont()->GetLogFont( &lf );
		font.CreateFontIndirect( &lf );
	}

	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &font );
	text_focus_height = dc->DrawText( ":)", CRect( 0, 0, 1, 1 ), DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
	if ( text_focus_height <= 0 ) {
		text_focus_height = 10;
	}
	text_focus_height += 2;

	if ( chatApp.splash ) {
		chatApp.splash->setProgress( CChatSmile::smile, CChatSmile::tomato + 1 );
	}
	for ( int i = CChatSmile::smile; i <= CChatSmile::tomato; i++ ) {
//		if ( i == 43 ) {
//			CString s;
//			s.Format( "%d, %s", i, CChatSmileList::getStr( static_cast<CChatSmile::Type>(i) ).c_str() );
//			AfxMessageBox( s );
//		}
		Item* item  = new Item;
		item->smile = new CChatSmile( static_cast<CChatSmile::Type>(i), this );
		item->smile->showAnimation( false );
		item->smile->setBgColor( ::GetSysColor( COLOR_WINDOW ) );
		item->info = CChatSmileList::getInfo( item->smile->getType() );
		CSize size = item->smile->GetSize();
		size.cx += 2;
		size.cy += 2;
		if ( size.cx > smile_max_width ) {
			smile_max_width = size.cx;
		}
//		if ( i == 43 ) {
//			AfxMessageBox( "1" );
//		}
		CRect rect( 0, 0, 1, 1 );
		dc->DrawText( item->info.c_str(), -1, rect, DT_CENTER | DT_CALCRECT );
		if ( rect.Width() + 2 > text_max_width ) {
			text_max_width = rect.Width() + 2;
		}
		item->height = rect.Height() + 2 > size.cy ? rect.Height() + 2 : size.cy;
		list.push_back( item );
		if ( chatApp.splash ) {
			chatApp.splash->stepProgress();
		}
	}
	if ( chatApp.splash ) {
		chatApp.splash->hideProgress();
	}
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return 0;
}

void CChatSmileListCtrl::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect( &newClientRect );
	InvalidateRect( &newClientRect );
	updateScrollBars();
}

void CChatSmileListCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	switch( nSBCode ) {
		case SB_PAGEUP:
			inc = -xPageSize;
			break;

		case SB_PAGEDOWN:
			inc = xPageSize;
			break;

		case SB_LINEUP:
			inc = -10;
			break;

		case SB_LINEDOWN:
			inc = 10;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			inc = si.nTrackPos - xPos;
			break;
		}
		default:
			inc = 0;
	}

	scrollHorizontally( inc );
}

void CChatSmileListCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	switch( nSBCode ) {
		case SB_TOP:
			inc = -yPos;
			break;
		case SB_PAGEUP:
			inc = min( -text_focus_height, -yPageSize );
			break; 

		case SB_PAGEDOWN:
			inc = max( text_focus_height, yPageSize );
			break;

		case SB_LINEUP:
			inc = -text_focus_height;
			break;

		case SB_LINEDOWN:
			inc = text_focus_height;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			inc = si.nTrackPos - yPos;
			break;
		}
		case SB_BOTTOM:
			inc = yMax - yPos;
			break;
		default:
			inc = 0;
	}

	scrollVertically( inc );
}

BOOL CChatSmileListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	WORD scrollNotify = 0xFFFF;

	if ( zDelta < 0 ) {
		if ( isLastLineVisible() ) return TRUE;
		scrollNotify = SB_LINEDOWN;
	} else {
		if ( isFirstLineVisible() ) return TRUE;
		scrollNotify = SB_LINEUP;
	}

	::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG( scrollNotify, 0 ), NULL );

	return TRUE;
}

bool CChatSmileListCtrl::updateScrollBars()
{
	yMax      = max( getStringsSumHeight(), newClientRect.bottom - 1 );
	yPageSize = yMax >= newClientRect.bottom ? newClientRect.bottom + 1 : newClientRect.bottom;
	yPos      = min( yPos, max( 0, yMax - newClientRect.bottom ) );

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = yMax;
	si.nPage  = yPageSize;
	si.nPos   = yPos;
	SetScrollInfo( SB_VERT, &si, TRUE );

	xMax      = smile_max_width + text_max_width;
	xPageSize = xMax >= newClientRect.right ? newClientRect.right + 1 : newClientRect.right;
	xPos      = min( xPos, max( 0, xMax - newClientRect.right ) );

	si.nMax   = xMax;
	si.nPage  = xPageSize;
	si.nPos   = xPos;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	bool new_horzScrollBarVisible = xMax > newClientRect.Width();
	bool new_vertScrollBarVisible = !(yPos == 0 && yMax < newClientRect.Height());
	bool flag = new_horzScrollBarVisible != horzScrollBarVisible || new_vertScrollBarVisible != vertScrollBarVisible;
	horzScrollBarVisible = new_horzScrollBarVisible;
	vertScrollBarVisible = new_vertScrollBarVisible;
	return flag;
}

bool CChatSmileListCtrl::scrollVertically( int inc )
{
	if ( !inc ) return false;

	if ( yPos + inc < 0 ) {
		inc = -yPos;
	}
	if ( yMax < newClientRect.bottom ) {
		inc = -yPos;
	} else {
		if ( yPos + inc + newClientRect.bottom >= yMax ) {
			inc = yMax - yPos - newClientRect.bottom;
		}
	}

	yPos += inc;

	ScrollWindowEx( 0, -inc, NULL, NULL, NULL, NULL, SW_INVALIDATE );

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = yPos;
	SetScrollInfo( SB_VERT, &si, TRUE );

	Invalidate();
	UpdateWindow();

	return true;
}

bool CChatSmileListCtrl::scrollHorizontally( int inc )
{
	if ( !inc ) return false;

	if ( xPos + inc < 0 ) {
		inc = -xPos;
	}
	if ( xPos + inc + newClientRect.right >= xMax ) {
		inc = xMax - xPos - newClientRect.right;
	}

	xPos += inc;
	
	ScrollWindowEx( -inc, 0, NULL, NULL, NULL, NULL, SW_INVALIDATE );

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = xPos;
	SetScrollInfo( SB_HORZ, &si, TRUE );

	Invalidate();
	UpdateWindow();

	return true;
}

bool CChatSmileListCtrl::isVisible( const int index )
{
	if ( index != -1 ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = list[index]->height;
		return height_before + line_height > yPos && height_before < yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

bool CChatSmileListCtrl::isFirstLineVisible()
{
	int index = 0;
	if ( !list.empty() ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = list[index]->height;
		return height_before >= yPos && height_before < yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

bool CChatSmileListCtrl::isLastLineVisible()
{
	int index = list.size() - 1;
	if ( !list.empty() ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = list[index]->height;
		return height_before >= yPos && height_before + line_height <= yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

int CChatSmileListCtrl::getStringsSumHeight()
{
	if ( !list.size() ) {
		return 0;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &font );
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		height += list[i]->height;
	};
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return height;
}

int CChatSmileListCtrl::getHeightBeforeLine( const int index )
{
	if ( index < 0 || !list.size() ) {
		return -1;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &font );
	for ( int i = 0; i < index; i++ ) {
		height += list[i]->height;
	}
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return height;
}

void CChatSmileListCtrl::OnPaint()
{
	CPaintDC dc( this );

	dc.SetBkMode( TRANSPARENT );

	if ( !IsRectEmpty( &(dc.m_ps.rcPaint) ) ) {

		CFont* prev_font = dc.SelectObject( &font );

		COLORREF bgcolor = ::GetSysColor( COLOR_WINDOW );
		CBrush brush( bgcolor );
		CPen pen( PS_SOLID, 0, bgcolor );
		CPen* prev_pen     = dc.SelectObject( &pen );
		CBrush* prev_brush = dc.SelectObject( &brush );

		int line_from = findLine( yPos );
		int line_to   = findLine( yPos + newClientRect.bottom );
		if ( line_to == -1 ) line_to = list.size() - 1;
		if ( prev_line_from != -1 && prev_line_to != -1 && line_from != -1 ) {
			int i;
			for ( i = prev_line_from; i < line_from; i++ ) {
				list[i]->smile->showAnimation( false );
			}
			for ( i = line_to + 1; i <= prev_line_to; i++ ) {
				list[i]->smile->showAnimation( false );
			}
		}
		prev_line_from = line_from;
		prev_line_to   = line_to;

		CRect rect( -xPos, -yPos, newClientRect.right, 0 );
		int cnt;
		if ( line_from != - 1) {
			cnt = line_to - line_from + 1;
			rect.top += getHeightBeforeLine( line_from );
		} else {
			cnt = 0;
		}
		for ( int i = 0; i < cnt; i++ ) {

			CChatSmile* smile = list[ i + line_from ]->smile;
			smile->showAnimation( true );

			int h = list[i + line_from]->height;
			rect.bottom = rect.top + h;

			CRect smile_rect = rect;
			CSize size = smile->GetSize();
			int delta_x = ( smile_max_width - size.cx ) / 2;
			int delta_y = ( h - size.cy ) / 2;
			smile_rect.left   += delta_x;
			smile_rect.right  += delta_x;
			smile_rect.top    += delta_y;
			smile_rect.bottom += delta_y;
			smile->MoveWindow( smile_rect );

			CRect bg_rect( rect.left, rect.top, rect.right, rect.top + delta_y );
			dc.Rectangle( &bg_rect );
			bg_rect.top   += delta_y + size.cy;
			bg_rect.bottom = rect.bottom;
			dc.Rectangle( &bg_rect );
			bg_rect.right  = smile_rect.left;
			bg_rect.top    = rect.top + 1;
			bg_rect.bottom = rect.bottom - 1;
			dc.Rectangle( &bg_rect );
			bg_rect.left  = smile_rect.left + size.cx;
			bg_rect.right = rect.right;
			dc.Rectangle( &bg_rect );

			rect.left += smile_max_width;

			if ( line_from + i == selectedLine && hasFocus ) {
//				dc.SetTextColor( ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
				dc.SetTextColor( ::GetSysColor( COLOR_WINDOWTEXT ) );
//				CRect bg_rect( smile_max_width - xPos, rect.top, rect.right, rect.bottom );
//				dc.FillSolidRect( bg_rect, ::GetSysColor( COLOR_HIGHLIGHT ) );
				dc.DrawFocusRect( CRect( -xPos, rect.top, smile_max_width + text_max_width - xPos, rect.bottom ) );
			} else {
				dc.SetTextColor( ::GetSysColor( COLOR_WINDOWTEXT ) );
			}
			dc.SetBkMode( TRANSPARENT );
			CRect textRect( 0, 0, 1, 1 );
			dc.DrawText( list[ i + line_from ]->info.c_str(), -1, &textRect, DT_CENTER | DT_CALCRECT );
			textRect.left   = smile_max_width - xPos;
			textRect.right  = smile_max_width + text_max_width - xPos;
			textRect.top    = rect.top + ( h - textRect.Height() ) / 2 - 1;
			textRect.bottom = rect.bottom;
			dc.DrawText( list[ i + line_from ]->info.c_str(), -1, &textRect, DT_CENTER );

			rect.left -= smile_max_width;
			rect.top  += h;

		}
		dc.SelectObject( prev_pen );
		dc.SelectObject( prev_brush );

		dc.FillSolidRect( dc.m_ps.rcPaint.left, rect.bottom, dc.m_ps.rcPaint.right, dc.m_ps.rcPaint.bottom, ::GetSysColor( COLOR_WINDOW ) );

		dc.SelectObject( prev_font );
	}
}

void CChatSmileListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus( pOldWnd );
	hasFocus = true;
	if ( selectedLine == -1 && !list.empty() ) {
		selectedLine = 0;
	}
	repaintLine( selectedLine );
}

void CChatSmileListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus( pNewWnd );
	hasFocus = false;
	repaintLine( selectedLine );
}

void CChatSmileListCtrl::selectLine( const int index )
{
	if ( index < 0 || index > list.size() - 1 || index == selectedLine ) {
		return;
	}
	int height_before = getHeightBeforeLine( index );
	int line_height   = list[index]->height;
	if ( height_before >= yPos && height_before + line_height > yPos + newClientRect.bottom ) {
		scrollVertically( height_before - yPos );
//		scrollVertically( line_height );
	} else if ( !( height_before >= yPos && height_before + line_height <= yPos + newClientRect.bottom ) ) {
		scrollVertically( height_before - yPos );
	}
	if ( hasFocus ) {
		hasFocus = false;
//		if ( selectedLine != -1 ) {
//			list[selectedLine]->smile->setBgColor( ::GetSysColor( COLOR_WINDOW ) );
//		}
		repaintLine( selectedLine );
		selectedLine = index;
		hasFocus = true;
//		list[selectedLine]->smile->setBgColor( ::GetSysColor( COLOR_HIGHLIGHT ) );
		repaintLine( selectedLine );
	}
}

void CChatSmileListCtrl::repaintLine( const int index )
{
	if ( isVisible( index ) ) {
		CRect rect( newClientRect );
		rect.top    = getHeightBeforeLine( index ) - yPos;
		rect.bottom = rect.top + list[index]->height;
		InvalidateRect( &rect );
		UpdateWindow();
	}
}

int CChatSmileListCtrl::findLine( const int _yPos )
{
	if ( list.empty() ) {
		return -1;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		height += list[i]->height;
		if ( height > _yPos ) {
			dc->SelectObject( prev_font );
			ReleaseDC( dc );
			return i;
		}
	};
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return -1;
}

void CChatSmileListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	SetFocus();
	selectLine( findLine( yPos + point.y ) );
}

void CChatSmileListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	chatApp.mainFrame->restoreStatusMode();

	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;

	switch ( nChar ) {

		case VK_UP:
			selectLine( selectedLine - 1 );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( list.size() - 1 );
			break;

		case VK_RETURN:
			copy();
			break;

		default:
			break;
	}
}

UINT CChatSmileListCtrl::OnGetDlgCode() 
{
	return CWnd::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

void CChatSmileListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk( nFlags, point );
	copy();
}

void CChatSmileListCtrl::copy()
{
	if ( selectedLine != -1 && OpenClipboard() ) {
		::EmptyClipboard();
		std::string str = CChatSmileList::getStr( list[selectedLine]->smile->getType() );
		HGLOBAL global_data = ::GlobalAlloc( GHND, str.length() + 1 );
		char* mem = static_cast<char*>(::GlobalLock( global_data ));
		memcpy( mem, str.c_str(), str.length() );
		::SetClipboardData( CF_TEXT, mem );
		HGLOBAL global_lcid = ::GlobalAlloc( GMEM_MOVEABLE, sizeof(DWORD) );
		LCID* lcid = static_cast<LCID*>(::GlobalLock( global_lcid ));
		*lcid = MAKELCID( MAKELANGID(LANG_RUSSIAN, SUBLANG_NEUTRAL), SORT_DEFAULT );
		SetClipboardData( CF_LOCALE, lcid );
		CloseClipboard();
		::GlobalUnlock( global_data );
		::GlobalUnlock( global_lcid );
		chatApp.mainFrame->childView.edit.paste();
		chatApp.mainFrame->childView.edit.SetFocus();
	}
}
