#include "stdafx.h"
#include "chatpopupmessage.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatPopupMessage
// ----------------------------------------------------------------------------
#define popupTimer_ID 1

const int x_border = 2;
const int y_border = 1;
const int x_delta  = 1;
const int y_delta  = 1;

BEGIN_MESSAGE_MAP( CChatPopupMessage, CDialog )
	//{{AFX_MSG_MAP(CChatPopupMessage)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
//	ON_WM_PAINT()
END_MESSAGE_MAP()

CChatPopupMessage::CChatPopupMessage():
	CDialog(),
	message( "" ),
	popupTimer( 0 )
{
	usePopup        = chatApp.GetProfileInt( "Popup", "usePopup", true ) ? true : false;
	position        = (CChatPopupPosition)chatApp.GetProfileInt( "Popup", "position", CPP_RightBottom );
	timerDelay      = chatApp.GetProfileInt( "Popup", "timerDelay", 5 );
	textColor       = chatApp.GetProfileInt( "Popup", "textColor", RGB( 0xFF, 0xFF, 0x00 ) );
	backgroundColor = chatApp.GetProfileInt( "Popup", "backgroundColor", RGB( 0x30, 0x30, 0x30 ) ); 
}

CChatPopupMessage::~CChatPopupMessage()
{
}

BOOL CChatPopupMessage::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = WS_VISIBLE | WS_POPUP;
	cs.dwExStyle |= ( WS_EX_TOPMOST | WS_EX_TOOLWINDOW );
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), 0, 0 );
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	return TRUE;
}

BOOL CChatPopupMessage::DestroyWindow()
{
	if ( popupTimer ) {
		KillTimer( popupTimer );
		popupTimer = 0;
	}
	return CWnd::DestroyWindow();
}

void CChatPopupMessage::OnTimer( UINT nIDEvent )
{
	CWnd::OnTimer( nIDEvent );
	if ( nIDEvent == popupTimer ) {
		ShowWindow( SW_HIDE );
		popupTimer = 0;
	}
}
/*
void CChatPopupMessage::OnPaint()
{
	CPaintDC dc( this );

	CRect r;
	GetClientRect( &r );
	dc.FillSolidRect( r, backgroundColor );

	CFont* prevFont = dc.SelectObject( &chatApp.getFont() );

	dc.SetTextColor( textColor );
	dc.SetBkMode( TRANSPARENT );

	r.DeflateRect( x_border, y_border );
	dc.DrawText( message, r, 0 );

	dc.SelectObject( prevFont );
}
*/

BOOL CChatPopupMessage::OnEraseBkgnd( CDC* pDC )
{
	CRect r;
	GetClientRect( &r );
	pDC->FillSolidRect( r, backgroundColor );

	CFont* prevFont = pDC->SelectObject( &chatApp.getFont() );

	pDC->SetTextColor( textColor );
	pDC->SetBkMode( TRANSPARENT );

	r.DeflateRect( x_border, y_border );
	pDC->DrawText( message.c_str(), r, 0 );

	pDC->SelectObject( prevFont );

	return TRUE;
}

void CChatPopupMessage::showMessage()
{
	if ( usePopup ) {
		CRect messageRect( 0, 0, 1, 1 );
		CDC* dc = GetDC();
		CFont* prevFont = dc->SelectObject( &chatApp.getFont() );
		dc->DrawText( message.c_str(), messageRect, DT_CALCRECT );
		dc->SelectObject( prevFont );
		ReleaseDC( dc );

		RECT screenRect = { 0, 0, 0, 0 };
		::SystemParametersInfo( SPI_GETWORKAREA, 0, &screenRect, 0 );
		switch ( position ) {
			case CPP_RightBottom: {
				SetWindowPos( &wndTopMost, screenRect.right - messageRect.right - x_border * 2 - x_delta, screenRect.bottom - messageRect.bottom - y_border * 2 - y_delta, messageRect.right + x_border * 2, messageRect.bottom + y_border * 2, SWP_NOACTIVATE | SWP_SHOWWINDOW );
				break;
			}
			case CPP_RightTop: {
				SetWindowPos( &wndTopMost, screenRect.right - messageRect.right - x_border * 2 - x_delta, screenRect.top + messageRect.top + y_delta, messageRect.right + x_border * 2, messageRect.bottom + y_border * 2, SWP_NOACTIVATE | SWP_SHOWWINDOW );
				break;
			}
			case CPP_LeftBottom: {
				SetWindowPos( &wndTopMost, screenRect.left + x_delta, screenRect.bottom - messageRect.bottom - y_border * 2 - y_delta, messageRect.right + x_border * 2, messageRect.bottom + y_border * 2, SWP_NOACTIVATE | SWP_SHOWWINDOW );
				break;
			}
			case CPP_LeftTop: {
				SetWindowPos( &wndTopMost, screenRect.left + x_delta, screenRect.top + y_delta, messageRect.right + x_border * 2, messageRect.bottom + y_border * 2, SWP_NOACTIVATE | SWP_SHOWWINDOW );
				break;
			}
		}

		RedrawWindow();

		if ( popupTimer ) {
			KillTimer( popupTimer );
		}
		popupTimer = SetTimer( popupTimer_ID, timerDelay * 1000, NULL );
	}
}

void CChatPopupMessage::setMessage( const std::string& value )
{
	if ( usePopup ) {
		if ( popupTimer ) {
			message = message + "\r\n" + value;
		} else {
			message = value;
		}
		showMessage();
	}
}

void CChatPopupMessage::setUsePopup( const bool value )
{
	if ( usePopup != value ) {
		usePopup = value;
		if ( !usePopup && popupTimer ) {
			ShowWindow( SW_HIDE );
			KillTimer( popupTimer );
			popupTimer = 0;
		}
		chatApp.WriteProfileInt( "Popup", "usePopup", usePopup );
	}
}

void CChatPopupMessage::setPosition( const CChatPopupPosition value )
{
	if ( position != value ) {
		position = value;
		if ( popupTimer ) {
			showMessage();
		}
		chatApp.WriteProfileInt( "Popup", "position", position );
	}
}

void CChatPopupMessage::setTimerDelay( const int value )
{
	if ( value >= 1 ) {
		if ( timerDelay != value ) {
			timerDelay = value;
			if ( popupTimer ) {
				KillTimer( popupTimer );
				popupTimer = SetTimer( popupTimer_ID, timerDelay * 1000, NULL );
			}
			chatApp.WriteProfileInt( "Popup", "timerDelay", timerDelay );
		}
	}
}

void CChatPopupMessage::setTextColor( const COLORREF value )
{
	if ( textColor != value ) {
		textColor = value;
		if ( popupTimer ) {
			showMessage();
		}
		chatApp.WriteProfileInt( "Popup", "textColor", textColor );
	}
}

void CChatPopupMessage::setBackgroundColor( const COLORREF value )
{
	if ( backgroundColor != value ) {
		backgroundColor = value;
		if ( popupTimer ) {
			showMessage();
		}
		chatApp.WriteProfileInt( "Popup", "backgroundColor", backgroundColor );
	}
}
