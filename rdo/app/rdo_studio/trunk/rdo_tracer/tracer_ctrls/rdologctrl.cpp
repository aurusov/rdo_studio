/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologctrl.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace rdoStyle;

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrlFindInList
// --------------------------------------------------------------------------------
class RDOLogCtrlFindInList
{
	RDOLogCtrl* log;
	tstring strToFind;
	rbool matchCase;
	rbool matchWholeWord;
	
	rbool scan( tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend ) const;
	rbool match( tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend ) const;
public:
	RDOLogCtrlFindInList( RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord );
	rbool operator()( tstring nextstr );
};

}; // namespace rdoTracerLog

RDOLogCtrlFindInList::RDOLogCtrlFindInList( RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord )
	: log( _log ),
	strToFind( _strToFind ),
	matchCase( _matchCase ),
	matchWholeWord( _matchWholeWord )
{
}

rbool RDOLogCtrlFindInList::scan( tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend ) const
{
	// remove the '?' and '*'
	for( wildCards ++; str != strend && ( *wildCards == '?' || *wildCards == '*' ); wildCards ++ )
		if ( *wildCards == '?') str ++;
	while ( *wildCards == '*') wildCards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if ( str == strend && wildCards != wildend ) return false;
	if ( str == strend && wildCards == wildend ) return true; 
	// else search substring
	else
	{
		tstring::iterator wdsCopy = wildCards;
		tstring::iterator strCopy = str;
		rbool res = 1;
		do 
		{
			if ( !match( wildCards, wildend, str, strend ) ) strCopy ++;
			wildCards = wdsCopy;
			str		  = strCopy;
			while ( ( *wildCards != *str ) && ( str != strend ) ) str ++;
			wdsCopy = wildCards;
			strCopy = str;
		} while ( ( str != strend ) ? !match( wildCards, wildend, str, strend ) : ( res = false ) != false );

		if ( str == strend && wildCards == wildend ) return true;

		return res;
	}
}

rbool RDOLogCtrlFindInList::match( tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend ) const
{
	rbool res = true;
	
	tstring strWild;
	tstring strComp;
	if ( wildcards != wildend ) {
		strWild.assign( &(*wildcards) );
	}
	if ( strcomp != strend ) {
		strComp.assign( &(*strcomp) );
	}
	tstring::iterator strWildb = strWild.begin();
	tstring::iterator strWilde = strWild.end();
	tstring::iterator strCompb = strComp.begin();
	tstring::iterator strCompe = strComp.end();

	//iterate and delete '?' and '*' one by one
	while( strWildb != strWilde && res && strCompb != strCompe )
	{
		if ( *strWildb == '?' )
			strCompb ++;
		else if ( *strWildb == '*' )
		{
			res = scan( strWildb, strWilde, strCompb, strCompe );
			strWildb --;
		}
		else
		{
			res = ( *strWildb == *strCompb );
			strCompb ++;
		}
		strWildb ++;
	}
	while ( *strWildb && *strWildb == '*' && res )  strWildb ++;

	return res && strCompb == strCompe && strWildb == strWilde;
}

rbool RDOLogCtrlFindInList::operator()( tstring nextstr )
{
	if ( !matchWholeWord && strToFind.find_first_of( "*?" ) == tstring::npos ) {
		strToFind.insert( 0, "*");
		strToFind += "*";
	}

	tstring str = nextstr;
	
	if ( !matchCase ) {
		std::transform( strToFind.begin(), strToFind.end(), strToFind.begin(), tolower );
		std::transform( str.begin(), str.end(), str.begin(), tolower );
	}
	
	log->posFind ++;

	if ( matchWholeWord )
		return strToFind == str;

	tstring::iterator findstrb = strToFind.begin();
	tstring::iterator findstre = strToFind.end();
	tstring::iterator strb = str.begin();
	tstring::iterator stre = str.end();
	return match( findstrb, findstre,  strb, stre );
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrl
// --------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( RDOLogCtrl, CWnd )

BEGIN_MESSAGE_MAP( RDOLogCtrl, CWnd )
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
	stringsCount( 0 ),
	firstFoundLine( -1 ),
	posFind( -1 ),
	bHaveFound( false ),
	bSearchDown( true ),
	bMatchCase( false ),
	bMatchWholeWord( false ),
	drawLog( true ),
	hdc( NULL ),
	saved_hdc( 0 ),
	hwnd( NULL ),
	fontInit( NULL ),
	hfontLog( NULL )
{
	//if no style specified default style will be used
	if ( !logStyle ) {
		logStyle = &studioApp.mainFrame->style_trace;
	}
}

RDOLogCtrl::~RDOLogCtrl()
{
	clear();
}

BOOL RDOLogCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//Setting class style CS_OWNDC to avoid DC releasing
	//and selecting previous objects into it
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int RDOLogCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
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
	fontInit = (HFONT)::GetCurrentObject( hdc, OBJ_FONT );
	//Saving the own DC state to restore it before destroying
	//a window
	saved_hdc = ::SaveDC( hdc );
	setFont( false );
	updateScrollBars();
	return 0;
}

void RDOLogCtrl::OnSize( UINT nType, int cx, int cy )
{
	UNUSED(nType);
	UNUSED(cx   );
	UNUSED(cy   );

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
		rbool lastLineVisible = isFullyVisible( stringsCount - 1 );
		rbool lastCharVisible = maxStrWidth == xPos + newClientRect.Width() / charWidth;
		
		rbool fullVisibleVert = !yPos && lastLineVisible;
		rbool fullVisibleHorz = !xPos && lastCharVisible;
		
		rbool needShiftVert = yPos < prevYPos && !fullVisibleVert;
		rbool needShiftHorz = xPos < prevXPos && !fullVisibleHorz;
		
		rbool topChanged = prevWindowRect.top != newWindowRect.top;
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

rbool RDOLogCtrl::getItemColors( const int index, RDOLogColorPair* &colors ) const
{
	return logStyle->getItemColors( index, colors );
}

rbool RDOLogCtrl::getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const
{
	return logStyle->getItemColors( item, colors );
}

void RDOLogCtrl::OnPaint()
{
	mutex.Lock();

	PAINTSTRUCT ps;
	::BeginPaint( hwnd, &ps );

	if ( drawLog ) {

		if ( !IsRectEmpty( &(ps.rcPaint) ) ) {
		
			int firstLine = max ( 0, yPos + ps.rcPaint.top / lineHeight );
			int mul = ps.rcPaint.bottom / lineHeight;
			if ( ps.rcPaint.bottom > mul * lineHeight ) mul++;
			int lastLine = min ( stringsCount - 1, yPos + mul - 1 );

			RDOLogColorPair* colors = NULL;

			int y = lineHeight * ( -yPos + firstLine - 1 );
			CRect rect( charWidth * ( -xPos ), y, ps.rcPaint.right, y + lineHeight );
			stringList::const_iterator it = const_findString( firstLine );
			for ( int i = firstLine; i < lastLine + 1; i++ ) {

				if ( i != selectedLine || focusOnly ) {
					if ( !getItemColors( (*it), colors ) )
						getItemColors( i, colors );
				} else {
					colors = new RDOLogColorPair();
					colors->foregroundColor = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
					colors->backgroundColor  = ::GetSysColor( COLOR_HIGHLIGHT );
				}

				rect.OffsetRect( 0, lineHeight );

				//Main drawing cycle
				::SetBkColor( hdc, colors->backgroundColor );
				::SetTextColor( hdc, colors->foregroundColor );
				::ExtTextOut( hdc, rect.left + logStyle->borders->horzBorder, rect.top + logStyle->borders->vertBorder, ETO_OPAQUE, rect, (*it).c_str(), (*it).length(), NULL );
				//End of main drawing cycle :)

				if ( i == selectedLine && hasFocus ) {
					CRect focusRect;
					focusRect.CopyRect( &newClientRect );
					focusRect.top = rect.top;
					focusRect.bottom = rect.bottom;
					::DrawFocusRect( hdc, focusRect );
				}

				it++;
				
				if ( i == selectedLine && !focusOnly && colors ) {
					delete colors; colors = NULL;
				}
			}

			getItemColors( "", colors );
			
			//MFC's FillSolidRect do the same thing
			::SetBkColor( hdc, colors->backgroundColor );
			::ExtTextOut( hdc, 0, 0, ETO_OPAQUE, CRect( ps.rcPaint.left, rect.bottom, ps.rcPaint.right, ps.rcPaint.bottom ), NULL, 0, NULL );
		}
	} else {
		RDOLogColorPair* colors = NULL;
		getItemColors( "", colors );

		//MFC's FillSolidRect do the same thing
		::SetBkColor( hdc, colors->backgroundColor );
		::ExtTextOut( hdc, 0, 0, ETO_OPAQUE, newClientRect, NULL, 0, NULL );
	}
	
	::EndPaint( hwnd, &ps );

	mutex.Unlock();
}

void RDOLogCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
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

void RDOLogCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
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

BOOL RDOLogCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED(pDC);
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

BOOL RDOLogCtrl::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
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

void RDOLogCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	SetFocus();
	selectLine( min( yPos + point.y / lineHeight, stringsCount - 1 ) );
}

void RDOLogCtrl::recalcWidth( const int newMaxStrWidth )
{
	if ( maxStrWidth != newMaxStrWidth ) {
		int width = maxStrWidth * charWidth;
		int newwidth = newMaxStrWidth * charWidth + 2 * logStyle->borders->horzBorder;

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

rbool RDOLogCtrl::scrollVertically( int inc )
{
	rbool res = false;
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

rbool RDOLogCtrl::scrollHorizontally( int inc )
{
	rbool res = false;
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

rbool RDOLogCtrl::isVisible( const int index ) const
{
	return index <= lastViewableLine && index >= yPos;
}

rbool RDOLogCtrl::isFullyVisible( const int index ) const
{
	int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOLogCtrl::selectLine( const int index )
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
		rbool needrepaint = !makeLineVisible( selectedLine );
		if ( needrepaint )
			repaintLine( selectedLine );

		//repaintLine() repaints line only if it's visible
		repaintLine( prevSel );
	}
	CWnd* parent = GetParent();
	if ( parent )
		::SendNotifyMessage( parent->m_hWnd, WM_LOGSELCHANGE, (WPARAM)prevSel, (LPARAM)selectedLine );
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
		updateWindow();
	}
}

void RDOLogCtrl::updateWindow()
{
	CRgn updateRgn;
	int rgn_type = GetUpdateRgn( &updateRgn );
	if ( rgn_type != NULLREGION && rgn_type != ERROR )
		SendNotifyMessage( WM_PAINT, 0, 0 );
}

rbool RDOLogCtrl::makeLineVisible( const int index )
{
	rbool res = false;
	
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

void RDOLogCtrl::addStringToLog( const tstring logStr )
{
	mutex.Lock();

	if ( hwnd ) {
		rbool prevVisible = isVisible( stringsCount - 1 );

		strings.push_back( logStr );
		if ( !stringsCount )
			yPos_iterator = strings.begin();
		stringsCount ++;

		recalcWidth( logStr.length() );

		int lastString = stringsCount - 1;

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

	mutex.Unlock();
}

const RDOLogStyle& RDOLogCtrl::getStyle() const
{
	return (*logStyle);
}

void RDOLogCtrl::setStyle( RDOLogStyle* style, const rbool needRedraw )
{
	logStyle = style;
	setFont( false );
	
	recalcWidth( maxStrWidth );
	updateScrollBars();
	
	if ( needRedraw ) {
		Invalidate();
		updateWindow();
	}
}

void RDOLogCtrl::setFont( const rbool needRedraw )
{
	if ( !logStyle ) return;

	//If some font set for the log control:
	//1. select the default font into device context
	//2. delete log font
	if ( hfontLog ) {
		::SelectObject( hdc, fontInit );
		if ( !::DeleteObject( hfontLog ) )
			return;
	}

	mutex.Lock();

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	// The negative is to allow for leading
	lf.lfHeight    = -MulDiv( logStyle->font->size, ::GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
	lf.lfWeight    = logStyle->theme->style & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = logStyle->theme->style & RDOStyleFont::ITALIC;
	lf.lfUnderline = logStyle->theme->style & RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = BYTE(logStyle->font->characterSet);
#pragma warning(disable: 4996)
	strcpy( lf.lfFaceName, logStyle->font->name.c_str() );
#pragma warning(default: 4996)

	//Create new log font
	hfontLog = ::CreateFontIndirect( &lf );

	if ( hfontLog ) {
		TEXTMETRIC tm;
		if ( hwnd ) {
			//Select new font into device context and recalculate
			//text lineHeight and charWidth
			::SelectObject( hdc, hfontLog );
			::GetTextMetrics( hdc, &tm );
			lineHeight = tm.tmHeight + 2 * logStyle->borders->vertBorder;
			charWidth  = tm.tmAveCharWidth/*tm.tmMaxCharWidth*/;
		}
		
		if ( needRedraw ) {
			Invalidate();   
			updateWindow();
		}
	}

	mutex.Unlock();
}

void RDOLogCtrl::getString( const int index, tstring& str ) const
{
	const_cast<CMutex&>(mutex).Lock();

	tstring res = "";

	if ( index >= 0 && index < stringsCount )
		str.assign( *const_findString( index ) );

	const_cast<CMutex&>(mutex).Unlock();
}

int RDOLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

void RDOLogCtrl::getSelected( tstring& str ) const
{
	getString( selectedLine, str );
}

void RDOLogCtrl::copy()
{
	if ( canCopy() ) {
		if ( !OpenClipboard() || !::EmptyClipboard() )
			return;
		tstring str;
		getSelected( str );
		char* ptr = (char*)::LocalAlloc( LMEM_FIXED, str.length() + 1 );
#pragma warning(disable: 4996)
		strcpy( ptr, str.c_str() );
#pragma warning(default: 4996)
		::SetClipboardData( CF_TEXT, ptr );
		CloseClipboard();
	}
}

void RDOLogCtrl::clear()
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

void RDOLogCtrl::setYPosIterator( const int prev_yPos )
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

stringList::iterator RDOLogCtrl::findString( int index )
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

stringList::reverse_iterator RDOLogCtrl::reverse_findString( int index )
{
	stringList::reverse_iterator rit(findString( index ));
	return rit;
}

stringList::const_iterator RDOLogCtrl::const_findString( int index ) const
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

stringList::const_reverse_iterator RDOLogCtrl::const_reverse_findString( int index ) const
{
	stringList::const_reverse_iterator rit(const_findString( index ));
	return rit;
}

void RDOLogCtrl::find( int& result, const rbool searchDown, const rbool matchCase, const rbool matchWholeWord )
{
	mutex.Lock();

	result = -1;

	tstring strtofind = findStr;
	
	stringList::iterator it;
	stringList::reverse_iterator it_r;

	int startPos = selectedLine + 1;
	int endPos = stringsCount - 1;
	if ( !searchDown ) {
		startPos = selectedLine - 1;
		endPos   = 0;
	}

	posFind = -1;
	if ( searchDown ) {
		it = std::find_if(
				findString( startPos ),
				strings.end(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		if ( it == strings.end() ) {
			posFind = -1;
			startPos = 0;
			endPos   = stringsCount - 1;
			it = std::find_if(
				strings.begin(),
				strings.end(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		}
		if ( it == strings.end() )
			posFind = -1;
		else
			posFind += startPos;
	} else {
		it_r = std::find_if(
				reverse_findString( startPos + 1 ),
				strings.rend(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		if ( it_r == strings.rend() ) {
			posFind = -1;
			startPos = stringsCount - 1;
			endPos   = 0;
			it_r = std::find_if(
				strings.rbegin(),
				strings.rend(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		}
		if ( it_r == strings.rend() )
			posFind = -1;
		else
			posFind = startPos - posFind;
	}
	
	if ( posFind == -1 ) {
		firstFoundLine = -1;
		bHaveFound    = false;
		result = -1;
	} else {
		bHaveFound = true;
		result = posFind;
		if ( firstFoundLine == -1 ) {
			firstFoundLine = posFind;
		} else if ( posFind == firstFoundLine ) {
			firstFoundLine = -1;
			bHaveFound    = false;
			result = -1;
		}
	}
	
	mutex.Unlock();
}

void RDOLogCtrl::setText( tstring text )
{
	clear();
	while ( !text.empty() ) {
		ruint pos = text.find_first_of( "\r\n" );
		if ( pos == tstring::npos )
			pos = text.length();
		addStringToLog( pos ? text.substr( 0, pos ) : "" );
		text.erase( 0, pos );
		text.erase( 0, text.find_first_not_of( "\r\n" ) );
	}
}

void RDOLogCtrl::setDrawLog( const rbool value )
{
	if ( drawLog != value ) {
		drawLog = value;
		updateScrollBars();
		Invalidate();
		updateWindow();
		makeLineVisible( selectedLine );
	}
}

void RDOLogCtrl::OnDestroy() 
{
	if ( hdc ) {
		::RestoreDC( hdc, saved_hdc );
	}
	//If some font is selected into device context
	//select default font and delete log font
	if ( hfontLog ) {
		::DeleteObject( hfontLog );
	}
	CWnd ::OnDestroy();
}
