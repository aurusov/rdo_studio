#include "rdo_ctrls_pch.h"
#include "rdologctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStringList
// ----------------------------------------------------------------------------
RDOStringList::RDOStringList( int blockSize )
	: CStringList( blockSize )
{
}

RDOStringList::~RDOStringList()
{
}

bool RDOStringList::scan( char*& wildCards, char*&str ) const
{
	// remove the '?' and '*'
	for( wildCards ++; *str != '\0' && ( *wildCards == '?' || *wildCards == '*' ); wildCards ++ )
		if ( *wildCards == '?') str ++;
	while ( *wildCards == '*') wildCards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if ( *str == '\0' && *wildCards != '\0' ) return false;
	if ( *str == '\0' && *wildCards == '\0' ) return true; 
	// else search substring
	else
	{
		char* wdsCopy = wildCards;
		char* strCopy = str;
		bool  res     = 1;
		do 
		{
			if ( !match( wildCards, str, true, false ) )	strCopy ++;
			wildCards = wdsCopy;
			str		  = strCopy;
			while ( ( *wildCards != *str ) && ( *str != '\0' ) ) str ++;
			wdsCopy = wildCards;
			strCopy = str;
		} while ( ( *str != '\0' ) ? !match( wildCards, str, true, false ) : ( res = false ) != false );

		if ( *str == '\0' && *wildCards == '\0' ) return true;

		return res;
	}
}

bool RDOStringList::match( const CString& wildCards, const CString& str, const bool matchCase, const bool matchWholeWord ) const
{
	bool res = true;
	
	CString strWild = wildCards;
	CString strComp = str;
	
	if ( !matchCase ) {
		strWild.MakeLower();
		strComp.MakeLower();
	}
	
	if ( matchWholeWord )
		return strWild == strComp;
	
	char* wildcards = strWild.GetBuffer( strWild.GetLength() + 1 );
	char* strcomp   = strComp.GetBuffer( strComp.GetLength() + 1 );

	//iterate and delete '?' and '*' one by one
	while( *wildcards != '\0' && res && *strcomp != '\0' )
	{
		if ( *wildcards == '?' )
			strcomp ++;
		else if ( *wildcards == '*' )
		{
			res = scan( wildcards, strcomp );
			wildcards --;
		}
		else
		{
			res = ( *wildcards == *strcomp );
			strcomp ++;
		}
		wildcards ++;
	}
	while ( *wildcards == '*' && res )  wildcards ++;

	return res && *strcomp == '\0' && *wildcards == '\0';
}

int RDOStringList::findNext( const CString& findWhat, const int findFrom, const int findTo, const bool searchDown, const bool matchCase, const bool matchWholeWord ) const
{
	ASSERT_VALID( this );
	
	if ( findFrom < 0 || findFrom >= m_nCount || findTo < 0 || findTo >= m_nCount )
		return -1;

	CString findStr = findWhat;
	
	if ( !matchWholeWord && findStr.FindOneOf( "*?") == -1 ) {
		findStr.Insert( 0, "*");
		findStr += "*";
	}

	CNode* nodeFrom = (CNode*)FindIndex( findFrom );
	CNode* nodeTo   = (CNode*)FindIndex( findTo );
	int found = -1;

	if ( !searchDown ) {
		for ( ; nodeFrom != nodeTo->pPrev; nodeFrom = nodeFrom->pPrev ) {
			found++;
			if ( match( findStr, nodeFrom->data, matchCase, matchWholeWord ) )
				return findFrom - found;
		}
	} else {
		for ( ; nodeFrom != nodeTo->pNext; nodeFrom = nodeFrom->pNext ) {
			found++;
			if ( match( findStr, nodeFrom->data, matchCase, matchWholeWord ) )
				return findFrom + found;
		}
	}
	
	return -1;
}

// ----------------------------------------------------------------------------
// ---------- RDOLogCtrl
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( RDOLogCtrl, CWnd )

BEGIN_MESSAGE_MAP( RDOLogCtrl, CWnd )
	//{{AFX_MSG_MAP(RDOLogCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOLogCtrl::RDOLogCtrl( RDOLogStyle* style ):
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
	logStyle( style ),
	firstFoundLine( -1 ),
	bHaveFound( false ),
	bSearchDown( true ),
	bMatchCase( false ),
	bMatchWholeWord( false )
{
	//if no style specified default style will be used
	if ( !logStyle ) logStyle = new RDOLogStyle();
	logStyle->load();
}

RDOLogCtrl::~RDOLogCtrl()
{
	if ( logStyle ) delete logStyle;
}

BOOL RDOLogCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int RDOLogCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;
	setFont( logStyle->font, false );
	updateScrollBars();
	return 0;
}

void RDOLogCtrl::OnSize( UINT nType, int cx, int cy )
{
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
		bool lastLineVisible = isFullyVisible( logStrings.GetCount() - 1 );
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

bool RDOLogCtrl::getItemColors( const int index,COLORREF& textColor, COLORREF& backColor ) const
{
	return logStyle->getItemColors( index, textColor, backColor );
}

bool RDOLogCtrl::getItemColors( const CString& item, COLORREF& textColor, COLORREF& backColor ) const
{
	return logStyle->getItemColors( item, textColor, backColor );
}

void RDOLogCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* dc = BeginPaint( &ps );

	int prevDC = dc->SaveDC();

	dc->SetBkMode( TRANSPARENT );

	if ( !IsRectEmpty( &(ps.rcPaint) ) ) {
	
		CFont*  prevFont  = dc->SelectObject( &fontLog );
		
		int firstLine = max ( 0, yPos + ps.rcPaint.top / lineHeight );
		int mul = ps.rcPaint.bottom / lineHeight;
		if ( ps.rcPaint.bottom > mul * lineHeight ) mul++;
		int lastLine = min ( logStrings.GetCount() - 1, yPos + mul - 1 );

		COLORREF back;
		COLORREF front;

		int y = lineHeight * ( -yPos + firstLine - 1 );
		CRect rect( charWidth * ( -xPos ), y, ps.rcPaint.right, y + lineHeight );
		POSITION pos = logStrings.FindIndex( firstLine );

		for ( int i = firstLine; i < lastLine + 1; i++ ) {

			CString str;

			str = logStrings.GetNext( pos );
			
			if ( i != selectedLine || focusOnly ) {
				if ( !( getItemColors( str, front, back ) || getItemColors( i, front, back ) ) ) {
					front = dc->GetTextColor();
					back  = dc->GetBkColor();
				}
			} else {
				front = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
				back  = ::GetSysColor( COLOR_HIGHLIGHT );
			}

			int backdc = dc->SaveDC();

			CBrush brush ( back );
			CPen pen( PS_SOLID, 1, back );
			dc->SelectObject( &pen );
			dc->SelectObject( &brush );

			rect.OffsetRect( 0, lineHeight );

			dc->Rectangle( &rect );
			
			dc->SetTextColor( front );

			rect.left += logStyle->horzBorder;

			dc->DrawText( str, rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

			rect.left -= logStyle->horzBorder;
			
			dc->RestoreDC( backdc );

			if ( i == selectedLine && hasFocus ) {
				CRect focusRect;
				focusRect.CopyRect( &newClientRect );
				focusRect.top = rect.top;
				focusRect.bottom = rect.bottom;
				dc->DrawFocusRect( &focusRect );
			}
		}

		dc->FillSolidRect( ps.rcPaint.left, rect.bottom, ps.rcPaint.right, ps.rcPaint.bottom, dc->GetBkColor() );
	}
	
	dc->RestoreDC( prevDC );

	EndPaint( &ps );
}

void RDOLogCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
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

void RDOLogCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
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

BOOL RDOLogCtrl::OnEraseBkgnd(CDC* pDC) 
{
  return TRUE;
}

void RDOLogCtrl::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	hasFocus = true;
	repaintLine( selectedLine );
}

void RDOLogCtrl::OnKillFocus( CWnd* pNewWnd )
{
	CWnd::OnKillFocus( pNewWnd );
	hasFocus = false;
	repaintLine( selectedLine );
}

void RDOLogCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
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
			selectLine( min ( selectedLine + yPageSize, logStrings.GetCount() - 1 ) );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( logStrings.GetCount() - 1 );
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
		::SendMessage( m_hWnd, msg, MAKELONG(scrollNotify, 0), NULL );
	
}

BOOL RDOLogCtrl::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	WORD scrollNotify = 0xFFFF;
	
	if ( zDelta < 0 )
		scrollNotify = SB_LINEDOWN;
	else
		scrollNotify = SB_LINEUP;
	
	::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG(scrollNotify, 0), NULL );
	
	return TRUE;
}

void RDOLogCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	SetFocus();
	selectLine( min( yPos + point.y / lineHeight, logStrings.GetCount() - 1 ) );
}

void RDOLogCtrl::recalcWidth( const int newMaxStrWidth )
{
	if ( maxStrWidth != newMaxStrWidth ) {
		int width = maxStrWidth * charWidth;
		int newwidth = newMaxStrWidth * charWidth + 2 * logStyle->horzBorder;

		if ( newwidth > width ) {
			maxStrWidth = newwidth / charWidth;
			if ( maxStrWidth * charWidth < newwidth )
				maxStrWidth++;
		}
	}
}

void RDOLogCtrl::updateScrollBars()
{
	xPageSize = newClientRect.Width() / charWidth;
	yPageSize = newClientRect.Height() / lineHeight;
	int count = logStrings.GetCount();

	yMax = max ( 0, count - yPageSize );
	yPos = min ( yPos, yMax );
	int mul = yPageSize;
	if ( mul * lineHeight < newClientRect.Height() )
		mul++;
	lastViewableLine = yPos + mul - 1;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	si.nMin   = 0; 
	
	si.nMax   = logStrings.GetCount() - 1; 
	si.nPage  = yPageSize; 
	si.nPos   = yPos; 
	SetScrollInfo( SB_VERT, &si, TRUE );
	
	xMax = max ( 0, maxStrWidth - xPageSize );
	xPos = min ( xPos, xMax ); 
	
	si.nMax   = maxStrWidth - 1; 
	si.nPage  = xPageSize; 
	si.nPos   = xPos; 
	SetScrollInfo( SB_HORZ, &si, TRUE );
}

bool RDOLogCtrl::scrollVertically( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the vertical scrolling increment does not
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc = max ( -yPos, min ( inc, yMax - yPos ) ) ) {
		yPos += inc;
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
		si.cbSize = sizeof(si);
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
		UpdateWindow ();
		res = true;
	}
	return res;
}

bool RDOLogCtrl::scrollHorizontally( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the horizontal scrolling increment does not 
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc = max ( -xPos, min ( inc, xMax - xPos ) ) ) {
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
		si.cbSize = sizeof(si);
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		UpdateWindow();
		res = true;
	}
	return res;
}

bool RDOLogCtrl::isVisible( const int index ) const
{
	return index <= lastViewableLine && index >= yPos;
}

bool RDOLogCtrl::isFullyVisible( const int index ) const
{
	int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOLogCtrl::selectLine( const int index )
{
	if ( index < 0 || index > logStrings.GetCount() - 1 || index == selectedLine )
		return;
	int prevSel = selectedLine;
	int inc = max ( - prevSel, min ( index - prevSel, logStrings.GetCount() - 1 - prevSel ) );

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
	CWnd* parent = GetParent();
	if ( parent )
		::SendMessage( parent->m_hWnd, WM_LOGSELCHANGE, (WPARAM)prevSel, (LPARAM)selectedLine );
}

void RDOLogCtrl::getLineRect( const int index, CRect* rect ) const
{
	rect->CopyRect( &newClientRect );
	rect->top = ( index - yPos ) * lineHeight;
	rect->bottom = min( rect->top + lineHeight, rect->bottom );
}

void RDOLogCtrl::repaintLine ( const int index )
{
	if ( isVisible( index ) ) {
		CRect rect;
		getLineRect( index, &rect );
		InvalidateRect( &rect );
		UpdateWindow();
	}
}

bool RDOLogCtrl::makeLineVisible( const int index )
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

void RDOLogCtrl::addStringToLog( const CString logStr )
{
	if ( GetSafeHwnd() ) {
		bool prevVisible = isVisible( logStrings.GetCount() - 1 );

		logStrings.AddTail( logStr );

		recalcWidth( logStr.GetLength() );

		updateScrollBars();

		int lastString = logStrings.GetCount() - 1;

		fullRepaintLines = 1;

		if (  selectedLine != -1 && selectedLine == lastString - 1 ) {
			selectedLine = lastString;
			fullRepaintLines ++;
		}


		if ( !isFullyVisible( lastString ) && prevVisible && ( !isVisible( selectedLine ) || selectedLine == lastString ) )
			::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL );
		else if ( isVisible( lastString ) ) {
			repaintLine( lastString );
			if ( fullRepaintLines == 2 )
				repaintLine( lastString - 1 );
		}

		fullRepaintLines = 0;
	}
}

const RDOStringList& RDOLogCtrl::getLogStrings() const
{
	return logStrings;
}

const RDOLogStyle& RDOLogCtrl::getStyle() const
{
	return (*logStyle);
}

void RDOLogCtrl::setStyle( const RDOLogStyle& style, const bool needRedraw )
{
	(*logStyle) = style;
	setFont( logStyle->font, false );
	
	recalcWidth( maxStrWidth );
	updateScrollBars();
	
	if ( needRedraw ) {
		Invalidate();
		UpdateWindow();
	}
}

void RDOLogCtrl::setFont( const RDOFont& font, const bool needRedraw )
{
	if ( fontLog.m_hObject )
		if ( !fontLog.DeleteObject() )
			return;
	
	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	// The negative is to allow for leading
	lf.lfHeight    = -MulDiv( font.size, ::GetDeviceCaps( GetDC()->m_hDC, LOGPIXELSY ), 72 );
	lf.lfWeight    = font.style & RDOFS_BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = font.style & RDOFS_ITALIC;
	lf.lfUnderline = font.style & RDOFS_UNDERLINE;
	lf.lfCharSet   = font.characterSet;
	strcpy( lf.lfFaceName, font.name );

	if ( !fontLog.CreateFontIndirect( &lf ) )
		return;
	
	TEXTMETRIC tm;
	if ( m_hWnd ) {
		CDC* dc = GetDC();
		CFont* oldFont = dc->SelectObject( &fontLog );
		dc->GetTextMetrics( &tm );
		lineHeight = tm.tmHeight + 2 * logStyle->vertBorder;
		charWidth  = tm.tmAveCharWidth/*tm.tmMaxCharWidth*/;
		dc->SelectObject( oldFont );
	}

	
	if ( needRedraw ) {
		Invalidate();   
		UpdateWindow();
	}
}

/*void RDOLogCtrl::storeWindowRect()
{
	GetWindowRect( &prevWindowRect );
}*/

CString RDOLogCtrl::getString( const int index ) const
{
	if ( index >= 0 && index < logStrings.GetCount() ) {
		POSITION pos = logStrings.FindIndex( index );
		return logStrings.GetAt( pos );
	} else {
		return "";
	}
}

int RDOLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

CString RDOLogCtrl::getSelected() const
{
	return getString( selectedLine );
}

void RDOLogCtrl::copy()
{
	if ( canCopy() ) {
		if ( !OpenClipboard() || !::EmptyClipboard() )
			return;
		CString str = getSelected();
		char* ptr = (char*)::LocalAlloc( LMEM_FIXED, str.GetLength() + 1 );
		strcpy( ptr, str );
		::SetClipboardData( CF_TEXT, ptr );
		CloseClipboard();
	}
}

void RDOLogCtrl::clear()
{
	logStrings.RemoveAll();
	maxStrWidth       = 0;
	lastViewableLine  = 0;
	selectedLine = -1;
	updateScrollBars();
	Invalidate();
	UpdateWindow();
}

int RDOLogCtrl::find( const bool searchDown, const bool matchCase, const bool matchWholeWord )
{
	int startPosition = selectedLine + 1;
	int endPosition = logStrings.GetCount() - 1;
	if ( !searchDown ) {
		startPosition = selectedLine - 1;
		endPosition   = 0;
	}
	int posFind = logStrings.findNext( findStr, startPosition, endPosition, searchDown, matchCase, matchWholeWord );
	if ( posFind == -1 ) {
		if ( !searchDown ) {
			startPosition = logStrings.GetCount() - 1;
			endPosition   = 0;
		} else {
			startPosition = 0;
			endPosition   = logStrings.GetCount() - 1;
		}
		posFind = logStrings.findNext( findStr, startPosition, endPosition, searchDown, matchCase, matchWholeWord );
	}
	if ( posFind == -1 ) {
		firstFoundLine = -1;
		bHaveFound    = false;
		return -1;
	} else {
		if ( firstFoundLine == -1 ) {
			firstFoundLine = posFind;
		} else if ( posFind == firstFoundLine ) {
			firstFoundLine = -1;
			bHaveFound    = false;
			return -1;
		}
		bHaveFound = true;
		return posFind;
	}
	return -1;
}
