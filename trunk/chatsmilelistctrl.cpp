#include "stdafx.h"
#include "chatsmilelistctrl.h"
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
	text_focus_height = dc->DrawText( ":)", CRect( 0, 0, 1, 1 ), DT_SINGLELINE | DT_CALCRECT );
	if ( text_focus_height <= 0 ) {
		text_focus_height = 10;
	}
	text_focus_height += 2;
	for ( int i = CChatSmile::smile; i <= CChatSmile::tomato; i++ ) {
		Item* item   = new Item;
		item->smile  = NULL;
		item->type   = static_cast<CChatSmile::Type>(i);
		item->info   = CChatSmileList::getStr( item->type );
		item->height = 0;

//		CChatSmile* smile = list.addSmile( static_cast<CChatSmile::Type>(i), this );
//		smile->ShowWindow( SW_HIDE );
//		CSize size = smile->GetSize();
//		size.cx += 2;
//		size.cy += 2;
//		int smile_height = size.cy;
//		heights.push_back( text_focus_height > smile_height ? text_focus_height : smile_height );
//		if ( size.cx > smile_max_width ) {
//			smile_max_width = size.cx;
//		}
		CRect rect( 0, 0, 1, 1 );
		dc->DrawText( CChatSmileList::getStr( static_cast<CChatSmile::Type>(i) ).c_str(), -1, rect, DT_SINGLELINE | DT_CALCRECT );
		if ( rect.Width() > text_max_width ) {
			text_max_width = rect.Width();
		}
		list.push_back( item );
	}
	dc->SelectObject( prev_font );
	ReleaseDC( dc );
//	list.setBgColor( ::GetSysColor( COLOR_WINDOW ) );

	return 0;
}

/*
BEGIN_MESSAGE_MAP( CChatSmileListCtrl, CListBox )
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

CChatSmileListCtrl::CChatSmileListCtrl():
	CListBox(),
	smile_max_width( 0 ),
	text_focus_height( 0 )
{
}

CChatSmileListCtrl::~CChatSmileListCtrl()
{
}

BOOL CChatSmileListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CListBox::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT, LBS_MULTICOLUMN;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL );
	return TRUE;
}

int CChatSmileListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CListBox::OnCreate( lpCreateStruct ) == -1 ) return -1;

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		SetFont( CFont::FromHandle( hf ) );
	}

	CDC* dc = GetDC();
	text_focus_height = dc->DrawText( ":)", CRect( 0, 0, 1, 1 ), DT_SINGLELINE | DT_CALCRECT );
	if ( text_focus_height <= 0 ) {
		text_focus_height = 10;
	}
	text_focus_height += 2;
	ReleaseDC( dc );
	for ( int i = CChatSmile::smile; i <= CChatSmile::tomato; i++ ) {
		CChatSmile* smile = list.addSmile( static_cast<CChatSmile::Type>(i), this );
		smile->ShowWindow( SW_HIDE );
		int index = AddString( CChatSmileList::getStr( static_cast<CChatSmile::Type>(i) ).c_str() );
		SetItemData( index, reinterpret_cast<DWORD>(smile) );
		CSize size = smile->GetSize();
		int smile_height = size.cy + 2;
		SetItemHeight( index, text_focus_height > smile_height ? text_focus_height : smile_height );
		if ( size.cx > smile_max_width ) {
			smile_max_width = size.cx;
		}
	}
	list.setBgColor( ::GetSysColor( COLOR_WINDOW ) );

	return 0;
}

void CChatSmileListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );

	if ( static_cast<int>(lpDrawItemStruct->itemID) < 0 ) {
		return;
		if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS) ) {
			pDC->DrawFocusRect( &lpDrawItemStruct->rcItem );
		} else if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS) ) {
			pDC->DrawFocusRect( &lpDrawItemStruct->rcItem ); 
		}
		return;
	}

	CRect  focusRect( lpDrawItemStruct->rcItem );
	CRect  textRect( focusRect );
	CPoint smilePoint( focusRect.left, focusRect.top );

	CChatSmile* smile = reinterpret_cast<CChatSmile*>(GetItemData( lpDrawItemStruct->itemID ));
	CSize smile_size = smile->GetSize();
	smilePoint.x  += 1 + ( smile_max_width - smile_size.cx ) / 2;
	smilePoint.y  += 1;

	textRect.left += smile_max_width + 1;

//	focusRect.left = textRect.left - 1;
//	focusRect.DeflateRect( 0, (focusRect.Height() - text_focus_height) / 2 );

	if ( (lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)) ) {
		COLORREF color = ::GetSysColor( COLOR_HIGHLIGHT );
		pDC->FillSolidRect( &focusRect, color );
		smile->setBgColor( color );
	} else if ( !(lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & ODA_SELECT) ) {
		COLORREF color = ::GetSysColor( COLOR_WINDOW );
		pDC->FillSolidRect( &focusRect, color );
		smile->setBgColor( color );
	}

	if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	} else if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	}

	smile->MoveWindow( smilePoint.x, smilePoint.y, smile_size.cx, smile_size.cy );
	smile->ShowWindow( SW_SHOW );

	int prev_bkMode = pDC->SetBkMode( TRANSPARENT );
	COLORREF prev_textColor;

	if ( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
	} else if ( lpDrawItemStruct->itemState & ODS_DISABLED ) {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_GRAYTEXT) );
	} else {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );
	}

	CString str;
	GetText( lpDrawItemStruct->itemID, str );

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if ( GetStyle() & LBS_USETABSTOPS ) {
		nFormat |= DT_EXPANDTABS;
	}
	pDC->DrawText( str, -1, &textRect, nFormat );

	pDC->SetTextColor( prev_textColor );
	pDC->SetBkMode( prev_bkMode );
}

void CChatSmileListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}

void CChatSmileListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListBox ::OnVScroll(nSBCode, nPos, pScrollBar);
}
*/

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

bool CChatSmileListCtrl::updateScrollBars()
{
	yMax      = max( getStringsSumHeight(), newClientRect.bottom - 1 );
	yPageSize = newClientRect.bottom;
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
	xPageSize = newClientRect.right;
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

		COLORREF back;

//		int line_from = findLine( yPos );
//		int line_to   = findLine( yPos + newClientRect.bottom );
		int line_from = 0;
		int line_to   = list.size() - 1;
		if ( line_to == -1 ) line_to = list.size() - 1;
		if ( prev_line_from != -1 && prev_line_to != -1 && line_from != -1 ) {
			int i;
			for ( i = prev_line_from; i < line_from; i++ ) {
//				list.showAnimation( false );
			}
			for ( i = line_to + 1; i <= prev_line_to; i++ ) {
//				list.showAnimation( false );
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
			if ( !smile ) {
				smile = new CChatSmile( list[ i + line_from ]->type, this );
				list[ i + line_from ]->smile = smile;
			}
			smile->ShowWindow( SW_SHOW );
			if ( line_from + i == selectedLine && hasFocus ) {
				back = ::GetSysColor( COLOR_HIGHLIGHT );
			} else {
				back = ::GetSysColor( COLOR_WINDOW );
			}
			CBrush brush( back );
			CPen pen( PS_SOLID, 0, back );
			CPen* prev_pen     = dc.SelectObject( &pen );
			CBrush* prev_brush = dc.SelectObject( &brush );

			int h = list[i + line_from]->height;
			rect.bottom = rect.top + h;

			CRect smile_rect = rect;
			CSize size = smile->GetSize();
			int delta = ( smile_max_width - size.cx ) / 2;
			smile_rect.left   += delta;
			smile_rect.right  += delta;
			smile_rect.top    += 1;
			smile_rect.bottom += 1;
			smile->MoveWindow( smile_rect );

			CRect bg_rect( rect.left, rect.top, rect.right, rect.top + 1 );
			dc.Rectangle( &bg_rect );
			bg_rect.top    = rect.bottom - 1;
			bg_rect.bottom = rect.bottom;
			dc.Rectangle( &bg_rect );
			bg_rect.right  = smile_rect.left;
			bg_rect.top    = rect.top + 1;
			bg_rect.bottom = rect.bottom - 1;
			dc.Rectangle( &bg_rect );
			bg_rect.left  = smile_rect.left + size.cx;
			bg_rect.right = rect.right;
			dc.Rectangle( &bg_rect );

			rect.left  += smile_max_width;
//			rect.right -= xPos;

			dc.DrawText( CChatSmileList::getStr( smile->getType() ).c_str(), -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER );

			rect.left  -= smile_max_width;
//			rect.right += xPos;
			rect.top   += h;

			dc.SelectObject( prev_pen );
			dc.SelectObject( prev_brush );
		}

		dc.FillSolidRect( dc.m_ps.rcPaint.left, rect.bottom, dc.m_ps.rcPaint.right, dc.m_ps.rcPaint.bottom, ::GetSysColor( COLOR_WINDOW ) );

		dc.SelectObject( prev_font );
	}
}
