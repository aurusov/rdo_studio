#include "stdafx.h"
#include "chatviewer.h"
#include "chatapp.h"
#include "chatmainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatViewer
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatViewer, CWnd )
	//{{AFX_MSG_MAP(CChatViewer)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatViewer::CChatViewer():
	CWnd(),
	xPos( 0 ),
	yPos( 0 ),
	xMax( 0 ),
	yMax( 0 ),
	xPageSize( 0 ),
	yPageSize( 0 ),
	maxStrWidth( 0 ),
	newClientRect(0, 0, 0, 0 ),
	hasFocus( false ),
	selectedLine( -1 ),
	horzScrollBarVisible( false ),
	vertScrollBarVisible( false ),
	prev_line_from( -1 ),
	prev_line_to( -1 )
{
}

CChatViewer::~CChatViewer()
{
}

BOOL CChatViewer::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}

int CChatViewer::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;
	return 0;
}

void CChatViewer::OnSize( UINT nType, int cx, int cy )
{
	strings.recalculateSize();
	GetClientRect( &newClientRect );
	InvalidateRect( &newClientRect );
	updateScrollBars();
}

void CChatViewer::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
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
			inc = -chatApp.style.charWidth;
			break;

		case SB_LINEDOWN:
			inc = chatApp.style.charWidth;
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

void CChatViewer::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	switch( nSBCode ) {
		case SB_TOP:
			inc = -yPos;
			break;
		case SB_PAGEUP:
			inc = min( -chatApp.style.charHeight, -yPageSize );
			break; 

		case SB_PAGEDOWN:
			inc = max( chatApp.style.charHeight, yPageSize );
			break;

		case SB_LINEUP:
			inc = -chatApp.style.charHeight;
			break;

		case SB_LINEDOWN:
			inc = chatApp.style.charHeight;
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

BOOL CChatViewer::OnEraseBkgnd( CDC* pDC ) 
{
	return TRUE;
}

void CChatViewer::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	hasFocus = true;
	strings.setSelected( selectedLine );
	repaintLine( selectedLine );
}

void CChatViewer::OnKillFocus( CWnd* pNewWnd )
{
	CWnd::OnKillFocus( pNewWnd );
	hasFocus = false;
	strings.clearSelected( selectedLine );
	repaintLine( selectedLine );
}

void CChatViewer::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CWnd::OnKeyDown( nChar, nRepCnt, nFlags );

	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;

	switch ( nChar ) {

		case VK_UP:
			selectLine( selectedLine - 1 );
			break;

		case VK_PRIOR:
//			selectLine( max ( selectedLine - yPageSize, 0 ) );
			break;

		case VK_NEXT:
//			selectLine( min ( selectedLine + yPageSize, strings.count() - 1 ) );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( strings.count() - 1 );
			break;

		case VK_LEFT: {
//			scrollNotify = SB_LINEUP;
//			msg = WM_HSCROLL;
			break;
		}

		case VK_RIGHT: {
//			scrollNotify = SB_LINEDOWN;
//			msg = WM_HSCROLL;
			break;
		}
		default:
			break;
	}

//	if ( scrollNotify != -1 ) {
//		::SendMessage( m_hWnd, msg, MAKELONG( scrollNotify, 0 ), NULL );
//	}

	chatApp.mainFrame->restoreStatusMode();
}

BOOL CChatViewer::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
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

void CChatViewer::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	SetFocus();
	selectLine( findLine( yPos + point.y ) );
}

void CChatViewer::recalcWidth()
{
	maxStrWidth = getStringsMaxWidth();
}

bool CChatViewer::updateScrollBars()
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

	recalcWidth();
	xMax      = maxStrWidth;
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

bool CChatViewer::scrollVertically( int inc )
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

bool CChatViewer::scrollHorizontally( int inc )
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

bool CChatViewer::isVisible( const int index )
{
	if ( index != -1 ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = getStrHeight( index );
		return height_before + line_height > yPos && height_before < yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

bool CChatViewer::isFirstLineVisible()
{
	int index = 0;
	if ( strings.count() ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = getStrHeight( index );
		return height_before >= yPos && height_before < yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

bool CChatViewer::isLastLineVisible()
{
	int index = strings.count() - 1;
	if ( strings.count() ) {
		int height_before = getHeightBeforeLine( index );
		int line_height   = getStrHeight( index );
		return height_before >= yPos && height_before + line_height <= yPos + newClientRect.bottom;
	} else {
		return false;
	}
}

void CChatViewer::selectLine( const int index )
{
	if ( index < 0 || index > strings.count() - 1 || index == selectedLine ) {
		return;
	}
	int height_before = getHeightBeforeLine( index );
	int line_height   = getStrHeight( index );
	if ( height_before >= yPos && height_before + line_height > yPos + newClientRect.bottom ) {
		scrollVertically( height_before - yPos );
//		scrollVertically( line_height );
	} else if ( !( height_before >= yPos && height_before + line_height <= yPos + newClientRect.bottom ) ) {
		scrollVertically( height_before - yPos );
	}
	if ( hasFocus ) {
		hasFocus = false;
		strings.clearSelected( selectedLine );
		repaintLine( selectedLine );
		selectedLine = index;
		hasFocus = true;
		strings.setSelected( selectedLine );
		repaintLine( selectedLine );
	}
}

void CChatViewer::repaintLine( const int index )
{
	if ( isVisible( index ) ) {
		CRect rect( newClientRect );
		rect.top    = getHeightBeforeLine( index ) - yPos;
		rect.bottom = rect.top + getStrHeight( index );
		InvalidateRect( &rect );
		UpdateWindow();
	}
}

void CChatViewer::addString( const std::string& userName, const std::string& message, CChatStringType type, const std::string& toUserName )
{
	if ( GetSafeHwnd() ) {

		CChatString* str = new CChatString( getStringCount(), userName, message, type, toUserName );
		strings.addString( str );

		updateScrollBars();

		int lastString = strings.count() - 1;
		if ( lastString <= 0 ) lastString = 1;

		if ( isVisible( lastString - 1 ) ) {
			selectedLine = lastString;
			::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG( SB_BOTTOM, 0 ), NULL );
		}
	}
}

void CChatViewer::setStyle( const CChatViewerStyle& _style, const bool needRedraw )
{
//	style = _style;

	updateScrollBars();

	strings.recalculateSize();

	if ( needRedraw ) {
		Invalidate();
		UpdateWindow();
	}
}

CChatString* CChatViewer::getString( const int index ) const
{
	if ( index >= 0 && index < strings.count() ) {
		return strings[index];
	}
	return NULL;
}

int CChatViewer::findLine( const int _yPos )
{
	if ( !strings.count() ) {
		return -1;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	int cnt = strings.count();
	for ( int i = 0; i < cnt; i++ ) {
		height += strings[i]->getHeight( dc, newClientRect.right );
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

int CChatViewer::getStringsSumHeight()
{
	if ( !strings.count() ) {
		return 0;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	int cnt = strings.count();
	for ( int i = 0; i < cnt; i++ ) {
		height += strings[i]->getHeight( dc, newClientRect.right );
	};
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return height;
}

int CChatViewer::getHeightBeforeLine( const int index )
{
	if ( index < 0 || !strings.count() ) {
		return -1;
	}

	int height = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	for ( int i = 0; i < index; i++ ) {
		height += strings[i]->getHeight( dc, newClientRect.right );
	}
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return height;
}

bool CChatViewer::sizeChanged()
{
	if ( !strings.count() ) {
		return true;
	}

	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	int cnt = strings.count();
	for ( int i = 0; i < cnt; i++ ) {
		if ( strings[i]->sizeChanged( dc, newClientRect.right ) ) {
			dc->SelectObject( prev_font );
			ReleaseDC( dc );
			return true;
		}
	};
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return false;
}

int CChatViewer::getStringsMaxWidth()
{
	if ( !strings.count() ) {
		return 0;
	}

	int max_width = 0;
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );
	int cnt = strings.count();
	for ( int i = 0; i < cnt; i++ ) {
		int str_width = strings[i]->getMaxWidth( dc, newClientRect.right );
		if ( str_width > max_width ) {
			max_width = str_width;
		}
	};
	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return max_width;
}

int CChatViewer::getStrHeight( const int index )
{
	CDC* dc = GetDC();
	CFont* prev_font = dc->SelectObject( &chatApp.style.fontFSN );

	int height = 0;
	CChatString* str = getString( index );
	if ( str ) {
		height = str->getHeight( dc, newClientRect.right );
	}

	dc->SelectObject( prev_font );
	ReleaseDC( dc );

	return height;
}

void CChatViewer::OnPaint()
{
	CPaintDC dc( this );

	dc.SetBkMode( TRANSPARENT );

	if ( !IsRectEmpty( &(dc.m_ps.rcPaint) ) ) {

		CFont* prev_font = dc.SelectObject( &chatApp.style.fontFSN );

		COLORREF back;

		int line_from = findLine( yPos );
		int line_to   = findLine( yPos + newClientRect.bottom );
		if ( line_to == -1 ) line_to = strings.count() - 1;
		if ( prev_line_from != -1 && prev_line_to != -1 && line_from != -1 ) {
			int i;
			for ( i = prev_line_from; i < line_from; i++ ) {
				strings[i]->showAnimation( false );
			}
			for ( i = line_to + 1; i <= prev_line_to; i++ ) {
				strings[i]->showAnimation( false );
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

			CChatString* str = strings[ i + line_from ];
			str->showAnimation( true );
			if ( line_from + i == selectedLine && hasFocus ) {
				back = chatApp.style.theme.selectedBgColor;
			} else {
				back = str->getBgColor();
			}
			CBrush brush( back );
			CPen pen( PS_SOLID, 0, back );
			CPen* prev_pen     = dc.SelectObject( &pen );
			CBrush* prev_brush = dc.SelectObject( &brush );

			dc.SetTextColor( str->getTextColor() );

			int h = str->getHeight( &dc, newClientRect.right );
			rect.bottom = rect.top + h;

			dc.Rectangle( &rect );

			rect.left  += chatApp.style.horzBorder;
			rect.right -= xPos;

			str->drawText( &dc, rect );

			rect.left  -= chatApp.style.horzBorder;
			rect.right += xPos;
			rect.top   += h;

			dc.SelectObject( prev_pen );
			dc.SelectObject( prev_brush );
		}

		dc.FillSolidRect( dc.m_ps.rcPaint.left, rect.bottom, dc.m_ps.rcPaint.right, dc.m_ps.rcPaint.bottom, chatApp.style.theme.viewerBgColor );

		dc.SelectObject( prev_font );
	}
}

bool CChatViewer::isSelected() const
{
	return selectedLine >= 0 && selectedLine < strings.count();
}

void CChatViewer::copy()
{
	if ( isSelected() && OpenClipboard() ) {
		::EmptyClipboard();
		HGLOBAL global_data = ::GlobalAlloc( GHND, strings[selectedLine]->getString().length() + 1 );
		char* mem = static_cast<char*>(::GlobalLock( global_data ));
		memcpy( mem, strings[selectedLine]->getString().c_str(), strings[selectedLine]->getString().length() );
		::SetClipboardData( CF_TEXT, mem );
		HGLOBAL global_lcid = ::GlobalAlloc( GMEM_MOVEABLE, sizeof(DWORD) );
		LCID* lcid = static_cast<LCID*>(::GlobalLock( global_lcid ));
		*lcid = MAKELCID( MAKELANGID(LANG_RUSSIAN, SUBLANG_NEUTRAL), SORT_DEFAULT );
		SetClipboardData( CF_LOCALE, lcid );
		CloseClipboard();
		::GlobalUnlock( global_data );
		::GlobalUnlock( global_lcid );
	}
}
