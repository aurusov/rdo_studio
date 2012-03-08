#include "app/rdo_studio_mfc/plugins/mfc/stdafx.h"
#include "app/rdo_studio_mfc/plugins/mfc/rdopluginmfcmainframe.h"
#include "app/rdo_studio_mfc/plugins/mfc/rdopluginmfc.h"
#include "app/rdo_studio_mfc/plugins/mfc/resource.h"

#include <limits>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::string format( const char* str, ... )
{
	std::vector< char > s;
	s.resize( 256 );
	va_list paramList;
	int size = -1;
	while ( size == -1 ) {
		va_start( paramList, str );
		size = vsnprintf_s( &s[0], s.size(), s.size(), str, paramList );
		va_end( paramList );
		if ( size == -1 ) {
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return std::string( s.begin(), s.end() );
}

void replace_with( std::string& src, const std::string& what, const std::string& with )
{
	if ( what != with ) {
		std::string temp;
		std::string::size_type prev_pos = 0;
		std::string::size_type pos = src.find( what, 0 );
		while ( std::string::npos != pos ) {
			temp += std::string( src.begin() + prev_pos, src.begin() + pos ) + with;
			prev_pos = pos + what.size();
			pos = src.find( what, prev_pos );
		}
		if ( !temp.empty() ) {
			src = temp + std::string( src.begin() + prev_pos, src.end() );
			if ( std::string::npos == with.find( what ) ) {
				replace_with( src, what, with );
			}
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPluginMFCLogCtrl
// --------------------------------------------------------------------------------
static int vertBorder = 1;
static int horzBorder = 1;

BEGIN_MESSAGE_MAP( RDOPluginMFCLogCtrl, CWnd )
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

RDOPluginMFCLogCtrl::RDOPluginMFCLogCtrl():
	CWnd(),
	lineHeight( 0 ),
	charWidth( 0 ),
	maxStrWidth( 0 ),
	xPos( 0 ),
	yPos( 0 ),
	xMax( 0 ),
	yMax( 0 ),
	xPageSize( 0 ),
	yPageSize( 0 ),
	clipRect( 0, 0, 0, 0 ),
	prevClientRect( 0, 0, 0, 0 ),
	newClientRect(0, 0, 0, 0 ),
	prevWindowRect( 0, 0, 0, 0 ),
	lastViewableLine( 0 ),
	hasFocus( false ),
	selectedLine( -1 ),
	fullRepaintLines( 0 ),
	focusOnly( false ),
	stringsCount( 0 ),
	drawLog( true ),
	hdc( NULL ),
	saved_hdc( 0 ),
	hwnd( NULL ),
	font( NULL )
{
}

RDOPluginMFCLogCtrl::~RDOPluginMFCLogCtrl()
{
	clear();
}

BOOL RDOPluginMFCLogCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//Setting class style CS_OWNDC to avoid DC releasing
	//and selecting previous objects into it
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int RDOPluginMFCLogCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;
	//Remembering handle to the window in hwnd member
	hwnd = GetSafeHwnd();
	//Remembering handle to the private device context in hdc member
	if ( hwnd )
		hdc = ::GetDC( hwnd );
	if ( !hdc ) return -1;
	//Setting background mode one time in initialization.
	//We have private DC, so we needn't to reset it each time
	//we paint
	::SetBkMode( hdc, TRANSPARENT );
	//Remembering default font to select it into DC
	//when setting new font
	font = static_cast<HFONT>(::GetStockObject( DEFAULT_GUI_FONT ));
	TEXTMETRIC tm;
	if ( hwnd ) {
		//Select new font into device context and recalculate
		//text lineHeight and charWidth
		::SelectObject( hdc, font );
		::GetTextMetrics( hdc, &tm );
		lineHeight = tm.tmHeight + 2 * vertBorder;
		charWidth  = tm.tmAveCharWidth/*tm.tmMaxCharWidth*/;
	}

	//Saving the own DC state to restore it before destroying
	//a window
	saved_hdc = ::SaveDC( hdc );
	updateScrollBars();
	return 0;
}

void RDOPluginMFCLogCtrl::OnSize( UINT nType, int cx, int cy )
{
	UNUSED(nType);
	UNUSED(cx   );
	UNUSED(cy   );

	TRACE( "S. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
		//In our case OnSize() invalidates all needed rectangles.
		//Default handler invalidates all client area.
//		CWnd::OnSize(  nType, cx, cy );

		GetClientRect( &newClientRect );

		CRect newWindowRect;
		GetWindowRect( &newWindowRect );

		int prevYPos = yPos;
		int prevXPos = xPos;
		//updateScrollBars() uses newClientRect so call it
		//after setting up newClientRect
		updateScrollBars();

		//isFullyVisible() uses newClientRect so call it
		//after setting up newClientRect
		bool lastLineVisible = isFullyVisible( stringsCount - 1 );
		bool lastCharVisible = maxStrWidth == xPos + newClientRect.Width() / charWidth;
		
		bool fullVisibleVert = !yPos && lastLineVisible;
		bool fullVisibleHorz = !xPos && lastCharVisible;
		
		bool needShiftVert = yPos < prevYPos && !fullVisibleVert;
		bool needShiftHorz = xPos < prevXPos && !fullVisibleHorz;
		
		bool topChanged = prevWindowRect.top != newWindowRect.top;
		int dx = newClientRect.right - prevClientRect.right;
		int dy = newClientRect.bottom - prevClientRect.bottom;
		
		CRect prevClRectBackup;
		prevClRectBackup.CopyRect( &prevClientRect );
		
		int mul = newClientRect.Height() / lineHeight;
		if ( mul * lineHeight < newClientRect.Height() )
			mul++;
		lastViewableLine = yPos + mul - 1;

		prevClientRect = newClientRect;
		prevWindowRect = newWindowRect;

		//If top of the window changed repainting all window
		//because default OnSize() handler invalidates all
		//client rectangle.
		//If top is the same validating all client rectangle
		//and calculating invalid rectangle.
		if ( !topChanged ) {

			ValidateRect( &newClientRect );

			//If new window size is smaller than the previous
			//no repainting is needed.
			if ( dx < 0 && dy < 0 )
				return;
			
			CRgn bottomRgn;
			if ( dy )
				bottomRgn.CreateRectRgn( newClientRect.left, prevClRectBackup.bottom - 1, newClientRect.right, newClientRect.bottom );
			else
				bottomRgn.CreateRectRgn( 0, 0, 0, 0 );

			//Substracting 1 pixel to remove old focus rectangle.
			CRgn rightRgn;
			if ( dx )
				rightRgn.CreateRectRgn( prevClRectBackup.right - 1, newClientRect.top, newClientRect.right, newClientRect.bottom );
			else
				rightRgn.CreateRectRgn( 0, 0, 0, 0 );
			
			CRgn invalidRgn;
			invalidRgn.CreateRectRgn( 0, 0, 0, 0 );
			int res = invalidRgn.CombineRgn( &bottomRgn, &rightRgn, RGN_OR );

			if ( res == NULLREGION || res == ERROR ) {
				
				invalidRgn.DeleteObject();
				invalidRgn.CreateRectRgn( newClientRect.left, newClientRect.top, newClientRect.right, newClientRect.bottom );
			
			} else if ( needShiftVert || needShiftHorz ) {
				
				//If scrolled vertically to the end of log
				//and resizing forces to appear new line
				//at the top of the log.
				if ( needShiftVert ) {

					ScrollWindowEx( 0, lineHeight * ( prevYPos - yPos ),
						(CONST CRect *) NULL, NULL,
						(CRgn*) NULL, (CRect*) NULL, SW_INVALIDATE );
					if ( dx )
						InvalidateRgn( &rightRgn );
				}
				
				//If scrolled horizontally to the end of log
				//and resizing forces to appear new char
				//at the left of the log.
				if ( needShiftHorz ) {

					ScrollWindowEx( charWidth * ( prevXPos - xPos ), 0,
						(CONST CRect*)NULL, (CONST CRect*)NULL,
						(CRgn*)NULL, (CRect*)NULL, SW_INVALIDATE );
			
					if ( isVisible( selectedLine) ) {
						CRect rect;
						getLineRect( selectedLine, &rect );
						InvalidateRect( &rect );
					}
					
					if ( dy )
						InvalidateRgn( &bottomRgn );
				}
				//All needed rectangles invalidated
				//by ScrollWindowEx() so RETURN.
				return;
			}
			
			InvalidateRgn( &invalidRgn );
			
			bottomRgn.DeleteObject();
			rightRgn.DeleteObject();
			invalidRgn.DeleteObject();
		} else {
			InvalidateRect( &newClientRect );
		}
}

void RDOPluginMFCLogCtrl::OnPaint()
{
	TRACE( "P. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );

	mutex.Lock();

	PAINTSTRUCT ps;
	::BeginPaint( hwnd, &ps );

	if ( drawLog ) {

		if ( !IsRectEmpty( &(ps.rcPaint) ) ) {

			int firstLine = max ( 0, yPos + ps.rcPaint.top / lineHeight );
			int mul = ps.rcPaint.bottom / lineHeight;
			if ( ps.rcPaint.bottom > mul * lineHeight ) mul++;
			int lastLine = min ( stringsCount - 1, yPos + mul - 1 );

			int y = lineHeight * ( -yPos + firstLine - 1 );
			CRect rect( charWidth * ( -xPos ), y, ps.rcPaint.right, y + lineHeight );
			stringList::const_iterator it = const_findString( firstLine );
			for ( int i = firstLine; i < lastLine + 1; i++ ) {

				DWORD foregroundColor;
				DWORD backgroundColor;
				if ( i != selectedLine || focusOnly ) {
					foregroundColor = ::GetSysColor( COLOR_WINDOWTEXT );
					backgroundColor = ::GetSysColor( COLOR_WINDOW );
				} else {
					foregroundColor = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
					backgroundColor = ::GetSysColor( COLOR_HIGHLIGHT );
				}

				rect.OffsetRect( 0, lineHeight );

				//Main drawing cycle
				::SetBkColor( hdc, backgroundColor );
				::SetTextColor( hdc, foregroundColor );
				::ExtTextOut( hdc, rect.left + horzBorder, rect.top + vertBorder, ETO_OPAQUE, rect, (*it).c_str(), (*it).length(), NULL );
				//End of main drawing cycle :)

				if ( i == selectedLine && hasFocus ) {
					CRect focusRect;
					focusRect.CopyRect( &newClientRect );
					focusRect.top = rect.top;
					focusRect.bottom = rect.bottom;
					::DrawFocusRect( hdc, focusRect );
				}

				it++;
			}

			//MFC's FillSolidRect do the same thing
			::SetBkColor( hdc, ::GetSysColor( COLOR_WINDOW ) );
			::ExtTextOut( hdc, 0, 0, ETO_OPAQUE, CRect( ps.rcPaint.left, rect.bottom, ps.rcPaint.right, ps.rcPaint.bottom ), NULL, 0, NULL );
		}
	} else {
		//MFC's FillSolidRect do the same thing
		::SetBkColor( hdc, ::GetSysColor( COLOR_WINDOW ) );
		::ExtTextOut( hdc, 0, 0, ETO_OPAQUE, newClientRect, NULL, 0, NULL );
	}

	::EndPaint( hwnd, &ps );

	mutex.Unlock();
}

void RDOPluginMFCLogCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof( si );

	switch( nSBCode ) {
		case SB_PAGEUP:
			inc = -xPageSize;
			break; 

		case SB_PAGEDOWN:
			inc = xPageSize;
			break;

		case SB_LINEUP:
			inc = -1;
			break;

		case SB_LINEDOWN:
			inc = 1;
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

void RDOPluginMFCLogCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof( si );

	switch( nSBCode ) {
		case SB_TOP:
			inc = -yPos;
			break;
		case SB_PAGEUP:
			inc = min( -1, -yPageSize );
			break; 

		case SB_PAGEDOWN:
			inc = max( 1, yPageSize );
			break;

		case SB_LINEUP:
			inc = -1;
			break;

		case SB_LINEDOWN:
			inc = 1;
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

BOOL RDOPluginMFCLogCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED(pDC);
	return TRUE;
}

void RDOPluginMFCLogCtrl::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	hasFocus = true;
	repaintLine( selectedLine );
}

void RDOPluginMFCLogCtrl::OnKillFocus( CWnd* pNewWnd )
{
	CWnd::OnKillFocus( pNewWnd );
	hasFocus = false;
	repaintLine( selectedLine );
}

void RDOPluginMFCLogCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	UNUSED(nRepCnt);
	UNUSED(nFlags );

	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;
	
	switch ( nChar ) {
	
		case VK_UP:
			selectLine( selectedLine - 1 );
			break;

		case VK_PRIOR:
			selectLine( max ( selectedLine - yPageSize, 0 ) );
			break;

		case VK_NEXT:
			selectLine( min ( selectedLine + yPageSize, stringsCount - 1 ) );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( stringsCount - 1 );
			break;

		case VK_LEFT: {
			scrollNotify = SB_LINEUP;
			msg = WM_HSCROLL;
			break;
		}
		
		case VK_RIGHT: {
			scrollNotify = SB_LINEDOWN;
			msg = WM_HSCROLL;
			break;
		}
		default:
			break;
	}

	if (scrollNotify != -1) 
		SendNotifyMessage( msg, MAKELONG(scrollNotify, 0), NULL );
	
}

BOOL RDOPluginMFCLogCtrl::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	UNUSED(nFlags);
	UNUSED(pt    );

	WORD scrollNotify = 0xFFFF;
	
	if ( zDelta < 0 )
		scrollNotify = SB_LINEDOWN;
	else
		scrollNotify = SB_LINEUP;
	
	SendNotifyMessage( WM_VSCROLL, MAKELONG(scrollNotify, 0), NULL );
	
	return TRUE;
}

void RDOPluginMFCLogCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	SetFocus();
	selectLine( min( yPos + point.y / lineHeight, stringsCount - 1 ) );
}

void RDOPluginMFCLogCtrl::recalcWidth( const int newMaxStrWidth )
{
	if ( maxStrWidth != newMaxStrWidth ) {
		int width = maxStrWidth * charWidth;
		int newwidth = newMaxStrWidth * charWidth + 2 * horzBorder;

		if ( newwidth > width ) {
			maxStrWidth = newwidth / charWidth;
			if ( maxStrWidth * charWidth < newwidth )
				maxStrWidth++;
		}
	}
}

void RDOPluginMFCLogCtrl::updateScrollBars()
{
	xPageSize = newClientRect.Width() / charWidth;
	yPageSize = newClientRect.Height() / lineHeight;

	yMax = max ( 0, stringsCount - yPageSize );
	int prev_ypos = yPos;
	yPos = min ( yPos, yMax );
	setYPosIterator( prev_ypos );
	int mul = yPageSize;
	if ( mul * lineHeight < newClientRect.Height() )
		mul++;
	lastViewableLine = yPos + mul - 1;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;

	if ( drawLog ) { 
		
		si.nMax   = stringsCount - 1;
		si.nPage  = yPageSize; 
		si.nPos   = yPos; 
		SetScrollInfo( SB_VERT, &si, TRUE );
		
		xMax = max ( 0, maxStrWidth - xPageSize );
		xPos = min ( xPos, xMax ); 
		
		si.nMax   = maxStrWidth - 1; 
		si.nPage  = xPageSize; 
		si.nPos   = xPos; 
		SetScrollInfo( SB_HORZ, &si, TRUE );
	} else {
		si.nMax   = 0;
		si.nPage  = 0; 
		si.nPos   = 0; 
		SetScrollInfo( SB_VERT, &si, FALSE );
		
		xMax = max ( 0, maxStrWidth - xPageSize );
		xPos = min ( xPos, xMax ); 
		
		si.nMax   = 0; 
		si.nPage  = 0; 
		si.nPos   = 0; 
		SetScrollInfo( SB_HORZ, &si, FALSE );
	}
}

bool RDOPluginMFCLogCtrl::scrollVertically( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the vertical scrolling increment does not
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc == max ( -yPos, min ( inc, yMax - yPos ) ) ) {
		int prev_ypos = yPos;
		yPos += inc;
		setYPosIterator( prev_ypos );
		lastViewableLine += inc;
		
		CRect rect;
		rect.CopyRect( &newClientRect );

		if ( fullRepaintLines )
			if ( inc > 0 )
				rect.bottom = ( rect.Height() / lineHeight - fullRepaintLines + 1 ) * lineHeight;
			else
				rect.top = ( fullRepaintLines - 1 )  * lineHeight;
		
		ScrollWindowEx( 0, -lineHeight * inc,
			(CONST CRect *) NULL, &rect,
			(CRgn*) NULL, NULL, SW_INVALIDATE );
		/*ScrollWindowEx( 0, -lineHeight * inc,
			(CONST CRect *) NULL, NULL,
			(CRgn*) NULL, (CRect*) NULL, 0 );*/

		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = yPos;
		SetScrollInfo( SB_VERT, &si, TRUE );

		if ( fullRepaintLines ) {
			rect.CopyRect( &newClientRect );
			if ( inc > 0 ) {
				rect.top = ( rect.Height() / lineHeight - fullRepaintLines + 1 ) * lineHeight;
			} else {
				rect.bottom = ( fullRepaintLines - 1 ) * lineHeight;
			}
			InvalidateRect( &rect );
		}
		
		/*rect.CopyRect( &newClientRect );
		if ( inc > 0 ) {
			rect.top = rect.Height() - inc * lineHeight;
			if ( fullRepaintLines )
				rect.top = ( rect.top / lineHeight - fullRepaintLines + 1) * lineHeight;
		} else {
			rect.bottom = (-inc) * lineHeight;
			if ( fullRepaintLines )
				rect.bottom = ( rect.bottom / lineHeight + fullRepaintLines - 1) * lineHeight;
		}
		
		InvalidateRect( &rect );*/
		updateWindow();
		res = true;
	}
	return res;
}

bool RDOPluginMFCLogCtrl::scrollHorizontally( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the horizontal scrolling increment does not 
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc == max ( -xPos, min ( inc, xMax - xPos ) ) ) {
		xPos += inc;
		
		ScrollWindowEx( -charWidth * inc, 0,
			(CONST CRect*)NULL, (CONST CRect*)NULL,
			(CRgn*)NULL, (CRect*)NULL, SW_INVALIDATE );
		
		if ( isVisible( selectedLine) ) {
			CRect rect;
			getLineRect( selectedLine, &rect );
			InvalidateRect( &rect );
		}
		
		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		updateWindow();
		res = true;
	}
	return res;
}

bool RDOPluginMFCLogCtrl::isVisible( const int index ) const
{
	return index <= lastViewableLine && index >= yPos;
}

bool RDOPluginMFCLogCtrl::isFullyVisible( const int index ) const
{
	int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOPluginMFCLogCtrl::selectLine( const int index )
{
	if ( index < 0 || index > stringsCount - 1 || index == selectedLine )
		return;
	int prevSel = selectedLine;
	int inc = max ( - prevSel, min ( index - prevSel, stringsCount - 1 - prevSel ) );

	if ( inc ) {
		selectedLine += inc;
		
		//makeLineVisible() scrolls to the line and repaints
		//it and nearby line if scrolling occurs.
		//If no scrolling is done repaint line
		bool needrepaint = !makeLineVisible( selectedLine );
		if ( needrepaint )
			repaintLine( selectedLine );

		//repaintLine() repaints line only if it's visible
		repaintLine( prevSel );
	}
}

void RDOPluginMFCLogCtrl::getLineRect( const int index, CRect* rect ) const
{
	rect->CopyRect( &newClientRect );
	rect->top = ( index - yPos ) * lineHeight;
	rect->bottom = min( rect->top + lineHeight, rect->bottom );
}

void RDOPluginMFCLogCtrl::repaintLine ( const int index )
{
	if ( isVisible( index ) ) {
		CRect rect;
		getLineRect( index, &rect );
		InvalidateRect( &rect );
		updateWindow();
	}
}

void RDOPluginMFCLogCtrl::updateWindow()
{
	CRgn updateRgn;
	int rgn_type = GetUpdateRgn( &updateRgn );
	if ( rgn_type != NULLREGION && rgn_type != ERROR )
		SendNotifyMessage( WM_PAINT, 0, 0 );
}

bool RDOPluginMFCLogCtrl::makeLineVisible( const int index )
{
	bool res = false;
	
	if ( isFullyVisible( index ) )
		return res;

	int inc;
	if ( yPos < index ) {
		int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
		inc = index - lastVisible;
	} else
		inc = index - yPos;
	
	//Repainting nearby lines after scrolling
	fullRepaintLines = 2;
	
	res = scrollVertically( inc );
	
	fullRepaintLines = 0;
	
	return res;
}

void RDOPluginMFCLogCtrl::addStringToLog( std::string logStr )
{
	if ( hwnd ) {

		TRACE( "8. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );

		mutex.Lock();

		replace_with( logStr, "\t", "    " );

		bool prevVisible = isVisible( stringsCount - 1 );

		strings.push_back( logStr );
		if ( !stringsCount )
			yPos_iterator = strings.begin();
		stringsCount ++;

		recalcWidth( logStr.length() );

		int lastString = stringsCount - 1;

		mutex.Unlock();

		if ( drawLog ) {

			updateScrollBars();


			fullRepaintLines = 1;

			if (  selectedLine != -1 && selectedLine == lastString - 1 ) {
				selectedLine = lastString;
				fullRepaintLines ++;
			}

			if ( !isFullyVisible( lastString ) && prevVisible && ( !isVisible( selectedLine ) || selectedLine == lastString ) )
				//::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL );
				scrollVertically( yMax - yPos );
			else if ( isVisible( lastString ) ) {
				repaintLine( lastString );
				if ( fullRepaintLines == 2 )
					repaintLine( lastString - 1 );
			}

			fullRepaintLines = 0;
		} else {
			if (  selectedLine != -1 && selectedLine == lastString - 1 )
				selectedLine = lastString;
		}
	}
}

void RDOPluginMFCLogCtrl::getString( const int index, std::string& str ) const
{
	const_cast<CMutex&>(mutex).Lock();

	std::string res = "";

	if ( index >= 0 && index < stringsCount )
		str.assign( *const_findString( index ) );

	const_cast<CMutex&>(mutex).Unlock();
}

int RDOPluginMFCLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

void RDOPluginMFCLogCtrl::getSelected( std::string& str ) const
{
	getString( selectedLine, str );
}

void RDOPluginMFCLogCtrl::clear()
{
	mutex.Lock();
	
	strings.clear();

	stringsCount      = 0;
	maxStrWidth       = 0;
	lastViewableLine  = 0;
	selectedLine = -1;
	if ( ::IsWindow( m_hWnd ) ) {
		updateScrollBars();
		Invalidate();
		updateWindow();
	}
	
	mutex.Unlock();
}

void RDOPluginMFCLogCtrl::setYPosIterator( const int prev_yPos )
{
	if ( yPos != prev_yPos ) {
		if ( yPos == 0 ) {
			yPos_iterator = strings.begin();
		} else if ( yPos == yMax ) {
			yPos_iterator = strings.end();
			for ( int i = stringsCount; i > yMax; i-- )
				yPos_iterator--;
		} else {
			int delta = yPos - prev_yPos;
			if ( delta > 0 ) {
				for ( int i = 0; i < delta; i++ )
					yPos_iterator++;
			} else {
				for ( int i = delta; i < 0; i++ )
					yPos_iterator--;
			}
		}
	}
}

stringList::iterator RDOPluginMFCLogCtrl::findString( int index )
{
	stringList::iterator res;
	
	if ( index == 0 ) {
		res = strings.begin();
	} else if ( index == yPos ) {
		res = yPos_iterator;
	} else if ( index == stringsCount - 1 ) {
		res = strings.end();
		res --;
	} else {
		int deltaPos = index - yPos;
		int deltaEnd = index - ( stringsCount - 1 );
		int mod_deltaPos = deltaPos >= 0 ? deltaPos : -1 * deltaPos;
		int mod_deltaEnd = deltaEnd >= 0 ? deltaEnd : -1 * deltaEnd;
		int delta = min( index, mod_deltaPos );
		delta = min( delta, mod_deltaEnd );
		if ( delta == index ) {
			res = strings.begin();
		} else if ( delta == mod_deltaPos ) {
			res = yPos_iterator;
			delta = deltaPos;
		} else if ( delta == mod_deltaEnd ) {
			res = strings.end();
			res --;
			delta = deltaEnd;
		}
		if ( delta > 0 ) {
			for ( int i = 0; i < delta; i++ )
				res++;
		} else {
			for ( int i = delta; i < 0; i++ )
				res--;
		}
	}

	return res;
}

stringList::const_iterator RDOPluginMFCLogCtrl::const_findString( int index ) const
{
	stringList::const_iterator res;
	
	if ( index == 0 ) {
		res = strings.begin();
	} else if ( index == yPos ) {
		res = yPos_iterator;
	} else if ( index == stringsCount - 1 ) {
		res = strings.end();
		res --;
	} else {
		int deltaPos = index - yPos;
		int deltaEnd = index - ( stringsCount - 1 );
		int mod_deltaPos = deltaPos >= 0 ? deltaPos : -1 * deltaPos;
		int mod_deltaEnd = deltaEnd >= 0 ? deltaEnd : -1 * deltaEnd;
		int delta = min( index, mod_deltaPos );
		delta = min( delta, mod_deltaEnd );
		if ( delta == index ) {
			res = strings.begin();
		} else if ( delta == mod_deltaPos ) {
			res = yPos_iterator;
			delta = deltaPos;
		} else if ( delta == mod_deltaEnd ) {
			res = strings.end();
			res --;
			delta = deltaEnd;
		}
		if ( delta > 0 ) {
			for ( int i = 0; i < delta; i++ )
				res++;
		} else {
			for ( int i = delta; i < 0; i++ )
				res--;
		}
	}

	return res;
}

void RDOPluginMFCLogCtrl::OnDestroy() 
{
	if ( hdc ) {
		::RestoreDC( hdc, saved_hdc );
	}
	CWnd ::OnDestroy();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPluginMFCMainFrame
// --------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOPluginMFCMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(RDOPluginMFCMainFrame, CFrameWnd)
	ON_COMMAND(ID_PLUGIN_CLOSE, OnPluginClose)
	ON_COMMAND(ID_MODEL_BUILD, OnModelBuild)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_COMMAND(ID_MODEL_STOP, OnModelStop)
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD, OnUpdateModelBuild)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP, OnUpdateModelStop)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODEL_RUN_MAXSPEED, OnModelRunMaxSpeed)
	ON_COMMAND(ID_MODEL_RUNTIME_JUMP, OnModelRunJump)
	ON_COMMAND(ID_MODEL_RUN_SYNC, OnModelRunSync)
	ON_COMMAND(ID_MODEL_RUN_PAUSE, OnModelRunPause)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_MAXSPEED, OnUpdateModelRunMaxSpeed)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_JUMP, OnUpdateModelRunJump)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_SYNC, OnUpdateModelRunSync)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN_PAUSE, OnUpdateModelRunPause)
	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowRateInc)
	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowRateIncFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowRateDecFour)
	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowRateDec)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowRateInc)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowRateIncFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowRateDecFour)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowRateDec)
	ON_COMMAND(ID_MODEL_FRAME_NEXT, OnModelFrameNext)
	ON_COMMAND(ID_MODEL_FRAME_PREV, OnModelFramePrev)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_NEXT, OnUpdateModelFrameNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_PREV, OnUpdateModelFramePrev)
	ON_COMMAND(ID_MODEL_NEW, OnModelNew)
	ON_COMMAND(ID_MODEL_OPEN, OnModelOpen)
	ON_COMMAND(ID_MODEL_SAVE, OnModelSave)
	ON_COMMAND(ID_MODEL_CLOSE, OnModelClose)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SAVE, OnUpdateModelSave)
	ON_UPDATE_COMMAND_UI(ID_MODEL_CLOSE, OnUpdateModelClose)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

RDOPluginMFCMainFrame::RDOPluginMFCMainFrame():
	CFrameWnd()
{
}

RDOPluginMFCMainFrame::~RDOPluginMFCMainFrame()
{
}

BOOL RDOPluginMFCMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle |= WS_EX_TOPMOST;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, pluginMFCApp.LoadCursor( IDC_ARROW ), NULL, pluginMFCApp.LoadIcon( IDR_MAINFRAME ) );
	return TRUE;
}

int RDOPluginMFCMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetWindowPos( &CWnd::wndTopMost, 0, 0, 300, 200, SWP_NOMOVE );

	log.CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_HSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect( 0, 0, 1, 1 ), this, 0 );

	return 0;
}

BOOL RDOPluginMFCMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( log.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void RDOPluginMFCMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize( nType, cx, cy );
	if ( log.GetSafeHwnd() ) {
		CRect r;
		GetClientRect( r );
		log.MoveWindow( r );
	}
}

void RDOPluginMFCMainFrame::insertLine( const char* line )
{
	log.addStringToLog( format( "%d. %s", log.getStringsCount(), line ) );
}

void RDOPluginMFCMainFrame::OnPluginClose()
{
	OnClose();
}

void RDOPluginMFCMainFrame::OnModelBuild()
{
	pluginMFCApp.studio.model.build();
}

void RDOPluginMFCMainFrame::OnModelRun()
{
	pluginMFCApp.studio.model.run();
}

void RDOPluginMFCMainFrame::OnModelStop()
{
	pluginMFCApp.studio.model.stop();
}

void RDOPluginMFCMainFrame::OnUpdateModelBuild( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelRun( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() && !pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnUpdateModelStop( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() );
}

void RDOPluginMFCMainFrame::OnModelRunMaxSpeed() 
{
	pluginMFCApp.studio.model.setRuntimeMode( rdoPlugin::MRTM_MaxSpeed );
}

void RDOPluginMFCMainFrame::OnModelRunJump() 
{
	pluginMFCApp.studio.model.setRuntimeMode( rdoPlugin::MRTM_Jump );
}

void RDOPluginMFCMainFrame::OnModelRunSync() 
{
	pluginMFCApp.studio.model.setRuntimeMode( rdoPlugin::MRTM_Sync );
}

void RDOPluginMFCMainFrame::OnModelRunPause() 
{
	pluginMFCApp.studio.model.setRuntimeMode( rdoPlugin::MRTM_Pause );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunMaxSpeed(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getRuntimeMode() == rdoPlugin::MRTM_MaxSpeed : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunJump(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getRuntimeMode() == rdoPlugin::MRTM_Jump : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunSync(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getRuntimeMode() == rdoPlugin::MRTM_Sync : 0 );
}

void RDOPluginMFCMainFrame::OnUpdateModelRunPause(CCmdUI* pCmdUI) 
{
	bool flag = pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.frame.isDescribed();
	pCmdUI->Enable( flag );
	pCmdUI->SetCheck( flag ? pluginMFCApp.studio.model.getRuntimeMode() == rdoPlugin::MRTM_Pause || pluginMFCApp.studio.model.getRuntimeMode() == rdoPlugin::MRTM_BreakPoint : 0 );
}

void RDOPluginMFCMainFrame::OnModelShowRateInc() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate *= 1.5;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateIncFour() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate *= 4;
	if ( showRate <= DBL_MAX ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateDecFour() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate /= 4;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnModelShowRateDec() 
{
	double showRate = pluginMFCApp.studio.frame.getShowRate();
	showRate /= 1.5;
	if ( showRate >= DBL_MIN ) {
		pluginMFCApp.studio.frame.setShowRate( showRate );
	}
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateInc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getRuntimeMode() != rdoPlugin::MRTM_MaxSpeed && pluginMFCApp.studio.frame.getShowRate() * 1.5 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getRuntimeMode() != rdoPlugin::MRTM_MaxSpeed && pluginMFCApp.studio.frame.getShowRate() * 4 <= DBL_MAX );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getRuntimeMode() != rdoPlugin::MRTM_MaxSpeed && pluginMFCApp.studio.frame.getShowRate() / 4 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isRunning() && pluginMFCApp.studio.model.getRuntimeMode() != rdoPlugin::MRTM_MaxSpeed && pluginMFCApp.studio.frame.getShowRate() / 1.5 >= DBL_MIN );
}

void RDOPluginMFCMainFrame::OnModelFrameNext() 
{
	pluginMFCApp.studio.frame.showNext();
}

void RDOPluginMFCMainFrame::OnModelFramePrev() 
{
	pluginMFCApp.studio.frame.showPrev();
}

void RDOPluginMFCMainFrame::OnUpdateModelFrameNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.frame.canShowNext() );
}

void RDOPluginMFCMainFrame::OnUpdateModelFramePrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( pluginMFCApp.studio.frame.canShowPrev() );
}

void RDOPluginMFCMainFrame::OnModelNew()
{
	pluginMFCApp.studio.model.create( "", "" );
}

void RDOPluginMFCMainFrame::OnModelOpen()
{
	pluginMFCApp.studio.model.open( NULL );
}

void RDOPluginMFCMainFrame::OnModelSave()
{
	pluginMFCApp.studio.model.save();
}

void RDOPluginMFCMainFrame::OnModelClose()
{
	pluginMFCApp.studio.model.close();
}

void RDOPluginMFCMainFrame::OnUpdateModelSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( pluginMFCApp.studio.model.isModify() );
}

void RDOPluginMFCMainFrame::OnUpdateModelClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( pluginMFCApp.studio.model.hasModel() );
}

void RDOPluginMFCMainFrame::OnClose() 
{
	pluginMFCApp.studio.plugin.stop( AfxGetInstanceHandle() );
	CFrameWnd::OnClose();
}
