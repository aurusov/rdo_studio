#include "stdafx.h"
#include "chatmainfrm.h"
#include "chatapp.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatMainFrame
// ----------------------------------------------------------------------------
#define MYMSG_NOTIFYTRAY ( WM_USER + 100 )
#define trayTimer_ID 1
#define awayTimer_ID 2

BEGIN_MESSAGE_MAP( CChatMainFrame, CFrameWnd )

	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()

	ON_MESSAGE( MYMSG_NOTIFYTRAY, OnHandleTrayNotify )
	ON_COMMAND( ID_TRAYMENU_OPEN, OnTrayOpenHide )
	ON_COMMAND( ID_APP_EXIT     , OnTrayCloseApp )
	ON_COMMAND( ID_TRAYMENU_EXIT, OnTrayCloseApp )
	ON_UPDATE_COMMAND_UI( ID_TRAYMENU_OPEN, OnUpdateTrayOpenHide )

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CChatMainFrame::CChatMainFrame():
	CFrameWnd(),
	closeButtonAction( CCBA_Close ),
	minimizeButtonAction( CCMA_Minimize ),
	minimizeByClose( false ),
	trayTimer( 0 ),
	trayShowMessageIcon( false ),
	trayModifyLock( false ),
	trayRestorWndSize( false ),
	trayCloseApp( false ),
	useTray( true ),
	notifyAboutMessage( true ),
	useAutoAway( true ),
	autoAwayMinuts( 10 ),
	useAutoNotAvailible( true ),
	autoNotAvailibleMinuts( 7 ),
	awayTimer( 0 ),
	prev_cursor( CPoint( 0, 0 ) ),
	prev_wnd( 0 ),
	idle_sec( 0 ),
	wasAutoChanged( false ),
	lastStatusMode( CSMT_Online )
{
	trayIconData.cbSize = 0;
}

CChatMainFrame::~CChatMainFrame()
{
}

BOOL CChatMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}

int CChatMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	childView.Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL );

	dock.Create( "aa", this, 0 );
	dock.SetBarStyle( dock.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	CString s;
	s.LoadString( ID_STATUSMODE_TOOLBAR );
	statusModeToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	statusModeToolBar.LoadToolBar( IDR_STATUSMODE_TOOLBAR );
	statusModeToolBar.GetToolBarCtrl().SetWindowText( s );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, 1 );
	statusBar.SetPaneInfo( 0, ID_INFOSTATUSBAR, SBPS_STRETCH, 70 );
//	statusBar.SetPaneInfo( 0, ID_COORDSTATUSBAR          , SBPS_NORMAL , 70 );
//	statusBar.SetPaneInfo( 1, ID_MODIFYSTATUSBAR         , SBPS_NORMAL , 70 );
//	statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITESTATUSBAR, SBPS_NORMAL , 70 );
//	statusBar.SetPaneInfo( 3, ID_INFOSTATUSBAR           , SBPS_STRETCH, 70 );

	dock.EnableDocking( CBRS_ALIGN_ANY );
	statusModeToolBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &statusModeToolBar );
	DockControlBar( &dock, AFX_IDW_DOCKBAR_LEFT );
//	dockControlBarBesideOf( editToolBar, projectToolBar );

	closeButtonAction    = (CChatCloseButtonAction)chatApp.GetProfileInt( "General", "closeButtonAction", CCBA_Tray );
	minimizeButtonAction = (CChatMinimizeButtonAction)chatApp.GetProfileInt( "General", "minimizeButtonAction", CCMA_Minimize );

	useTray            = chatApp.GetProfileInt( "Tray", "useTray", true ) ? true : false;
	notifyAboutMessage = chatApp.GetProfileInt( "Tray", "notifyAboutMessage", true ) ? true : false;
	setUseTray( useTray );

	useAutoAway            = chatApp.GetProfileInt( "AutoStatusMode", "useAutoAway", true ) ? true : false;
	autoAwayMinuts         = chatApp.GetProfileInt( "AutoStatusMode", "autoAwayMinuts", 10 );
	useAutoNotAvailible    = chatApp.GetProfileInt( "AutoStatusMode", "useAutoNotAvailible", true ) ? true : false;
	autoNotAvailibleMinuts = chatApp.GetProfileInt( "AutoStatusMode", "autoNotAvailibleMinuts", 7 );
	setUseAutoAway( useAutoAway );

	return 0;
}

BOOL CChatMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( childView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( dock.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

BOOL CChatMainFrame::DestroyWindow()
{
	closeTray();
	stopAutoStatusModeTimer();

	chatApp.pFrame = NULL;
	return CFrameWnd::DestroyWindow();
}

void CChatMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	CFrameWnd::OnSetFocus( pOldWnd );
	childView.SetFocus();
}

void CChatMainFrame::initTray()
{
	if ( !trayIconData.cbSize ) {
		HICON hIconAtIndex = chatApp.LoadIcon( MAKEINTRESOURCE(IDI_TRAY_APP) );

		trayIconData.cbSize           = sizeof(NOTIFYICONDATA);
		trayIconData.hWnd             = GetSafeHwnd();
		trayIconData.uID              = 100;
		trayIconData.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		trayIconData.uCallbackMessage = MYMSG_NOTIFYTRAY;
		trayIconData.hIcon            = hIconAtIndex;

		CString strToolTip;
		strToolTip.LoadString( IDR_MAINFRAME );
		LPCTSTR lpszToolTip = strToolTip.GetBuffer( strToolTip.GetLength() );
		lstrcpyn( trayIconData.szTip, lpszToolTip, strlen(lpszToolTip) + 1);

		::Shell_NotifyIcon( NIM_ADD, &trayIconData );

		if ( hIconAtIndex ) {
			::DestroyIcon( hIconAtIndex );
		}
	}
}

void CChatMainFrame::closeTray()
{
	stopTrayTimer();
	if ( trayIconData.cbSize ) {
		::Shell_NotifyIcon( NIM_DELETE, &trayIconData );
		trayIconData.cbSize = 0;
	}
}

void CChatMainFrame::stopTrayTimer()
{
	if ( trayTimer ) {
		trayShowMessageIcon = false;
		trayModifyLock      = false;
		modifyTray();
		KillTimer( trayTimer );
		trayTimer      = 0;
		trayModifyLock = false;
	}
}

void CChatMainFrame::modifyTray()
{
	if ( notifyAboutMessage || (!notifyAboutMessage && !trayShowMessageIcon) ) {
		if ( !trayModifyLock ) {
			HICON hIconAtIndex;
			if ( trayShowMessageIcon ) {
				hIconAtIndex = chatApp.LoadIcon( MAKEINTRESOURCE(IDI_TRAY_MESSAGE) );
			} else {
				hIconAtIndex = chatApp.LoadIcon( MAKEINTRESOURCE(IDI_TRAY_APP) );
			}
			trayShowMessageIcon = !trayShowMessageIcon;

			trayIconData.hIcon = hIconAtIndex;

			::Shell_NotifyIcon( NIM_MODIFY, &trayIconData );

			if ( hIconAtIndex ) {
				::DestroyIcon( hIconAtIndex );
			}

			if ( !trayTimer ) {
				trayTimer = SetTimer( trayTimer_ID, 500, NULL );
			}
			trayModifyLock = true;
		}
	}
}

bool CChatMainFrame::getUseAutoAway() const
{
	return useAutoAway;
}

void CChatMainFrame::setUseAutoAway( const bool value )
{
	if ( useAutoAway != value ) {
		useAutoAway = value;
		chatApp.WriteProfileInt( "AutoStatusMode", "useAutoAway", useAutoAway );
	}
	if ( useAutoAway && autoAwayMinuts ) {
		startAutoStatusModeTimer();
	} else {
		stopAutoStatusModeTimer();
	}
}

int  CChatMainFrame::getAutoAwayMinuts() const
{
	return autoAwayMinuts;
}

void CChatMainFrame::setAutoAwayMinuts( const int minuts )
{
	if ( autoAwayMinuts != minuts && minuts ) {
		autoAwayMinuts = minuts;
		chatApp.WriteProfileInt( "AutoStatusMode", "autoAwayMinuts", autoAwayMinuts );
		if ( useAutoAway ) {
			startAutoStatusModeTimer();
		} else {
			stopAutoStatusModeTimer();
		}
	}
}

bool CChatMainFrame::getUseAutoNotAvailible() const
{
	return useAutoNotAvailible;
}

void CChatMainFrame::setUseAutoNotAvailible( const bool value )
{
	if ( useAutoNotAvailible != value ) {
		useAutoNotAvailible = value;
		chatApp.WriteProfileInt( "AutoStatusMode", "useAutoNotAvailible", useAutoNotAvailible );
	}
}

int  CChatMainFrame::getAutoNotAvailibleMinuts() const
{
	return autoNotAvailibleMinuts;
}

void CChatMainFrame::setAutoNotAvailibleMinuts( const int minuts )
{
	if ( autoNotAvailibleMinuts != minuts && minuts ) {
		autoNotAvailibleMinuts = minuts;
		chatApp.WriteProfileInt( "AutoStatusMode", "autoNotAvailibleMinuts", autoNotAvailibleMinuts );
	}
}

void CChatMainFrame::startAutoStatusModeTimer()
{
	if ( awayTimer ) {
		stopAutoStatusModeTimer();
	}
	awayTimer = SetTimer( awayTimer_ID, 1000, NULL );
}

void CChatMainFrame::stopAutoStatusModeTimer()
{
	if ( awayTimer ) {
		KillTimer( awayTimer );
		awayTimer = 0;
		resetAutoStatusMode();
	}
}

bool CChatMainFrame::isAutoStatusMode() const
{
	return wasAutoChanged;
}

void CChatMainFrame::restoreStatusMode()
{
	if ( isAutoStatusMode() ) {
		chatApp.setStatusMode( lastStatusMode, true );
	}
	resetAutoStatusMode();
}

void CChatMainFrame::resetAutoStatusMode()
{
	idle_sec       = 0;
	wasAutoChanged = false;
}

void CChatMainFrame::OnTimer( UINT nIDEvent )
{
	CFrameWnd::OnTimer( nIDEvent );
	if ( nIDEvent == trayTimer ) {
		trayModifyLock = false;
		modifyTray();
	}
	if ( nIDEvent == awayTimer && useAutoAway ) {
		bool change_flag = false;
		CPoint point;
		if ( ::GetCursorPos( &point ) ) {
			if ( prev_cursor != point ) {
				prev_cursor = point;
				change_flag = true;
			}
		}
		HWND wnd = ::GetForegroundWindow();
		if ( prev_wnd != wnd ) {
			prev_wnd    = wnd;
			change_flag = true;
		}
		if ( change_flag ) {
			if ( isAutoStatusMode() ) {
				restoreStatusMode();
			}
		} else {
			idle_sec++;
			if ( idle_sec == autoAwayMinuts * 60 && chatApp.getStatusMode() == CSMT_Online ) {
				lastStatusMode = CSMT_Online;
				wasAutoChanged = true;
				chatApp.setStatusMode( CSMT_Away, true );
				idle_sec = 0;
			} else if ( useAutoNotAvailible && idle_sec == autoNotAvailibleMinuts * 60 && chatApp.getStatusMode() == CSMT_Away ) {
				if ( !wasAutoChanged ) {
					lastStatusMode = CSMT_Away;
					wasAutoChanged = true;
				}
				chatApp.setStatusMode( CSMT_NotAvailible, true );
				idle_sec = 0;
			}
		}
	}
}

void CChatMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CFrameWnd::OnSize( nType, cx, cy );
	trayRestorWndSize = nType == SIZE_MAXIMIZED;
	if ( nType == SIZE_MINIMIZED ) {
		if ( !minimizeByClose && minimizeButtonAction == CCMA_Tray ) {
			ShowWindow( SW_HIDE );
		}
		minimizeByClose = false;
	}
}

void CChatMainFrame::OnClose()
{
	if ( closeButtonAction == CCBA_Close || trayCloseApp ) {

		CFrameWnd::OnClose();

	} else if ( isVisible() ) {

		if ( closeButtonAction == CCBA_Minimize ) {
			minimizeByClose = true;
			ShowWindow( SW_MINIMIZE );
		} else {
			ShowWindow( SW_HIDE );
		}

	}
}

void CChatMainFrame::OnHandleTrayNotify( UINT wParam, LONG lParam )
{
	switch ( lParam ) {
		case WM_LBUTTONDOWN:
			OnTrayOpenHide();
			break;
		case WM_RBUTTONDOWN: {
			CMenu menu;
			menu.LoadMenu( IDR_TRAY_MENU );
			CMenu* subMenu = menu.GetSubMenu( 0 );
			::SetMenuDefaultItem( subMenu->m_hMenu, ID_TRAYMENU_OPEN, MF_BYCOMMAND );
			CPoint p;
			GetCursorPos( &p );
			SetForegroundWindow();
			subMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this );
			menu.DestroyMenu();
			break;
		}
		case WM_LBUTTONDBLCLK:
			break;
		default:
			break;
	}
}

void CChatMainFrame::OnTrayOpenHide()
{
	if ( isVisible() ) {
		ShowWindow( SW_HIDE );
	} else {
		if ( trayRestorWndSize ) {
			ShowWindow( SW_SHOW );
		} else {
			ShowWindow( SW_RESTORE );
		}
		SetForegroundWindow();
		stopTrayTimer();
	}
}

void CChatMainFrame::OnTrayCloseApp()
{
	trayCloseApp = true;
	OnClose();
}

void CChatMainFrame::OnUpdateTrayOpenHide( CCmdUI *pCmdUI )
{
	if ( isVisible() ) {
		pCmdUI->SetText( "Hide" );
	} else {
		pCmdUI->SetText( "Open" );
	}
}

void CChatMainFrame::showPopupMessage( const CString& value )
{
	if ( !popupMessage.GetSafeHwnd() ) {
		CWnd* activeWindow = CWnd::GetActiveWindow();
//		popupMessage.CreateEx( 0, NULL, NULL, 0, CRect(0, 0, 0, 0), NULL, 0 );
		popupMessage.Create( IDD_POPUP_DIALOG, NULL );
		if ( activeWindow ) {
			activeWindow->SetFocus();
		}
	}
	popupMessage.setMessage( value );
}

bool CChatMainFrame::isVisible() const
{
	return GetStyle() & WS_VISIBLE ? true : false;
}

bool CChatMainFrame::isActive()
{
	return CWnd::GetActiveWindow() == this;
}

CChatCloseButtonAction CChatMainFrame::getCloseButtonAction() const
{
	return closeButtonAction;
}

void CChatMainFrame::setCloseButtonAction( const CChatCloseButtonAction value )
{
	if ( closeButtonAction != value ) {
		closeButtonAction = value;
		chatApp.WriteProfileInt( "General", "closeButtonAction", closeButtonAction );
	}
}

CChatMinimizeButtonAction CChatMainFrame::getMinimizeButtonAction() const
{
	return minimizeButtonAction;
}

void CChatMainFrame::setMinimizeButtonAction( const CChatMinimizeButtonAction value )
{
	if ( minimizeButtonAction != value ) {
		minimizeButtonAction = value;
		chatApp.WriteProfileInt( "General", "minimizeButtonAction", minimizeButtonAction );
	}
}

bool CChatMainFrame::getUseTray() const
{
	return useTray;
}

void CChatMainFrame::setUseTray( const bool value )
{
	if ( useTray != value ) {
		useTray = value;
		chatApp.WriteProfileInt( "Tray", "useTray", useTray );
	}
	if ( useTray ) {
		initTray();
	} else {
		closeTray();
	}
}

bool CChatMainFrame::getNotifyAboutMessage() const
{
	return notifyAboutMessage;
}

void CChatMainFrame::setNotifyAboutMessage( const bool value )
{
	if ( notifyAboutMessage != value ) {
		notifyAboutMessage = value;
		chatApp.WriteProfileInt( "Tray", "notifyAboutMessage", notifyAboutMessage );
	}
}

#ifdef _DEBUG
void CChatMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CChatMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif
