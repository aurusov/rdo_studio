#include "stdafx.h"
#include "chatmainfrm.h"
#include "chatapp.h"
#include "chatoptions.h"
#include "chatabout.h"
#include "htmlhelp.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatMessageDialog
// ----------------------------------------------------------------------------
class CChatMessageDialog: public CDialog
{
private:
	std::string caption;

public:
	CString message;

	CChatMessageDialog( UINT nIDTemplate, const std::string& _caption, CWnd* pParentWnd = NULL );
	virtual ~CChatMessageDialog();

protected:
	//{{AFX_VIRTUAL(CChatMessageDialog)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatMessageDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP( CChatMessageDialog, CDialog )
	//{{AFX_MSG_MAP(CChatMessageDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatMessageDialog::CChatMessageDialog( UINT nIDTemplate, const std::string& _caption, CWnd* pParentWnd ):
	CDialog( nIDTemplate, pParentWnd ),
	caption( _caption ),
	message( "" )
{
	//{{AFX_DATA_INIT(CChatMessageDialog)
	//}}AFX_DATA_INIT
}

CChatMessageDialog::~CChatMessageDialog()
{
}

void CChatMessageDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	//{{AFX_DATA_MAP(CChatMessageDialog)
	DDX_Text( pDX, IDC_MESSAGE_EDIT, message );
	//}}AFX_DATA_MAP
}

BOOL CChatMessageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText( caption.c_str() );

	return TRUE;
}

// ----------------------------------------------------------------------------
// ---------- CChatMainFrame
// ----------------------------------------------------------------------------
#define trayTimer_ID 1
#define awayTimer_ID 2

const int MYMSG_NOTIFYTRAY = ::RegisterWindowMessage( "MYMSG_NOTIFYTRAY" );

BEGIN_MESSAGE_MAP( CChatMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CChatMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND( ID_TRAYMENU_OPEN, OnTrayOpenHide )
	ON_COMMAND( ID_APP_EXIT     , OnTrayCloseApp )
	ON_UPDATE_COMMAND_UI( ID_TRAYMENU_OPEN, OnUpdateTrayOpenHide )
	ON_COMMAND(ID_FILE_REFRESHUSERLIST, OnFileRefreshUserList)
	ON_COMMAND(ID_FILE_REFRESHNETWORK, OnFileRefreshNetwork)
	ON_UPDATE_COMMAND_UI(ID_FILE_REFRESHNETWORK, OnUpdateFileRefreshNetwork)
	ON_COMMAND(ID_VIEW_DOCKWINDOW, OnViewDockWindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCKWINDOW, OnUpdateViewDockWindow)
	ON_COMMAND(ID_VIEW_MAINTOOLBAR, OnViewMainToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINTOOLBAR, OnUpdateViewMainToolbar)
	ON_COMMAND(ID_VIEW_EDITTOOLBAR, OnViewEditToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITTOOLBAR, OnUpdateViewEditToolbar)
	ON_COMMAND(ID_VIEW_STATUSMODE_TOOLBAR, OnViewStatusModeToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUSMODE_TOOLBAR, OnUpdateViewStatusModeToolbar)
	ON_COMMAND(ID_VIEW_STATUSBAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUSBAR, OnUpdateViewStatusBar)
	ON_COMMAND(ID_SOUND, OnSound)
	ON_UPDATE_COMMAND_UI(ID_SOUND, OnUpdateSound)
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_ONLINE      , OnUpdateStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_ONLINE_INFO      , OnUpdateStatusModeInfo )
	ON_COMMAND( ID_CHAT_TOCRYOUT, OnToCryOut )
	ON_COMMAND( ID_CHAT_OPTIONS, OnOptions )
	ON_COMMAND(ID_USER_SENDMESSAGE, OnUserSendMessage)
	ON_UPDATE_COMMAND_UI(ID_USER_SENDMESSAGE, OnUpdateUserSendMessage)
	ON_COMMAND(ID_USER_INGNORE, OnUserIngnore)
	ON_UPDATE_COMMAND_UI(ID_USER_INGNORE, OnUpdateUserIngnore)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_SHOW_USERLIST, OnShowUserList)
	ON_COMMAND(ID_SHOW_NETWORK, OnShowNetwork)
	ON_COMMAND(ID_SHOW_SMILES, OnShowSmiles)
	ON_COMMAND( ID_TRAYMENU_EXIT, OnTrayCloseApp )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_AWAY        , OnUpdateStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_NOTAVAILIBLE, OnUpdateStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_AWAY_INFO        , OnUpdateStatusModeInfo )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_NOTAVAILIBLE_INFO, OnUpdateStatusModeInfo )
	ON_COMMAND(ID_SHOW_EDIT, OnShowEdit)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX( ID_STATUSMODE_ONLINE      , OnStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_AWAY        , OnStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_NOTAVAILIBLE, OnStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_ONLINE_INFO      , OnStatusModeInfo )
	ON_COMMAND_EX( ID_STATUSMODE_AWAY_INFO        , OnStatusModeInfo )
	ON_COMMAND_EX( ID_STATUSMODE_NOTAVAILIBLE_INFO, OnStatusModeInfo )
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

	dock.Create( format( IDS_DOCKWINDOW ).c_str(), this, 0 );
	dock.SetBarStyle( dock.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	mainToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	mainToolBar.LoadToolBar( IDR_MAIN_TOOLBAR );
	mainToolBar.GetToolBarCtrl().SetWindowText( format( IDS_MAIN_TOOLBAR ).c_str() );

	mainToolBarImageList.Create( IDB_MAIN_TOOLBAR_D, 16, 0, RGB( 0xFF, 0x00, 0xFF ) );
	mainToolBar.GetToolBarCtrl().SetDisabledImageList( &mainToolBarImageList );

	editToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	editToolBar.LoadToolBar( IDR_EDIT_TOOLBAR );
	editToolBar.GetToolBarCtrl().SetWindowText( format( IDS_EDIT_TOOLBAR ).c_str() );

	editToolBarImageList.Create( IDB_EDIT_TOOLBAR_D, 16, 0, RGB( 0xFF, 0x00, 0xFF ) );
	editToolBar.GetToolBarCtrl().SetDisabledImageList( &editToolBarImageList );

	statusModeToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	statusModeToolBar.LoadToolBar( IDR_STATUSMODE_TOOLBAR );
	statusModeToolBar.GetToolBarCtrl().SetWindowText( format( IDS_STATUSMODE_TOOLBAR ).c_str() );

	statusModeToolBarImageList.Create( IDB_STATISMODE_TOOLBAR_D, 16, 0, RGB( 0xFF, 0x00, 0xFF ) );
	statusModeToolBar.GetToolBarCtrl().SetDisabledImageList( &statusModeToolBarImageList );

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, 1 );
	statusBar.SetPaneInfo( 0, IDS_INFOSTATUSBAR, SBPS_STRETCH, 70 );

	mainToolBar.EnableDocking( CBRS_ALIGN_ANY );
	editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	statusModeToolBar.EnableDocking( CBRS_ALIGN_ANY );
	dock.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &mainToolBar );
	dockControlBarBesideOf( editToolBar, mainToolBar );
	dockControlBarBesideOf( statusModeToolBar, editToolBar );
	DockControlBar( &dock, AFX_IDW_DOCKBAR_LEFT );

	closeButtonAction    = static_cast<CChatCloseButtonAction>(chatApp.GetProfileInt( "General", "closeButtonAction", CCBA_Tray ));
	minimizeButtonAction = static_cast<CChatMinimizeButtonAction>(chatApp.GetProfileInt( "General", "minimizeButtonAction", CCMA_Minimize ));

	useTray            = chatApp.GetProfileInt( "Tray", "useTray", true ) ? true : false;
	notifyAboutMessage = chatApp.GetProfileInt( "Tray", "notifyAboutMessage", true ) ? true : false;
	setUseTray( useTray );

	useAutoAway            = chatApp.GetProfileInt( "AutoStatusMode", "useAutoAway", true ) ? true : false;
	autoAwayMinuts         = chatApp.GetProfileInt( "AutoStatusMode", "autoAwayMinuts", 10 );
	useAutoNotAvailible    = chatApp.GetProfileInt( "AutoStatusMode", "useAutoNotAvailible", true ) ? true : false;
	autoNotAvailibleMinuts = chatApp.GetProfileInt( "AutoStatusMode", "autoNotAvailibleMinuts", 7 );
	setUseAutoAway( useAutoAway );

	childView.SetFocus();

	return 0;
}

void CChatMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
{
	RecalcLayout( TRUE );

	CRect rect;
	baseBar.GetWindowRect( rect );

	DWORD dw = baseBar.GetBarStyle();

	UINT n = 0;
	n = ( dw & CBRS_ALIGN_TOP          ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = ( dw & CBRS_ALIGN_BOTTOM && !n ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = ( dw & CBRS_ALIGN_LEFT   && !n ) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = ( dw & CBRS_ALIGN_RIGHT  && !n ) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	int dx = 0;
	int dy = 0;
	if ( n == AFX_IDW_DOCKBAR_TOP || n == AFX_IDW_DOCKBAR_BOTTOM ) dx = 1;
	if ( n == AFX_IDW_DOCKBAR_LEFT || n == AFX_IDW_DOCKBAR_RIGHT ) dy = 1;

	rect.OffsetRect( dx, dy );

	DockControlBar( &bar, n, rect );
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

	chatApp.mainFrame = NULL;
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

void CChatMainFrame::setUseAutoNotAvailible( const bool value )
{
	if ( useAutoNotAvailible != value ) {
		useAutoNotAvailible = value;
		chatApp.WriteProfileInt( "AutoStatusMode", "useAutoNotAvailible", useAutoNotAvailible );
	}
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
			} else {
				resetAutoStatusMode();
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

void CChatMainFrame::showPopupMessage( const std::string& value )
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

void CChatMainFrame::setCloseButtonAction( const CChatCloseButtonAction value )
{
	if ( closeButtonAction != value ) {
		closeButtonAction = value;
		chatApp.WriteProfileInt( "General", "closeButtonAction", closeButtonAction );
	}
}

void CChatMainFrame::setMinimizeButtonAction( const CChatMinimizeButtonAction value )
{
	if ( minimizeButtonAction != value ) {
		minimizeButtonAction = value;
		chatApp.WriteProfileInt( "General", "minimizeButtonAction", minimizeButtonAction );
	}
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

LRESULT CChatMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message == MYMSG_NOTIFYTRAY ) {
		OnHandleTrayNotify( wParam, lParam );
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CChatMainFrame::OnFileRefreshUserList()
{
	chatApp.refreshUserList();
}

void CChatMainFrame::OnFileRefreshNetwork() 
{
	chatApp.network.refresh();
}

void CChatMainFrame::OnUpdateFileRefreshNetwork(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( chatApp.network.refreshFinished() );
}

void CChatMainFrame::OnViewDockWindow()
{
	ShowControlBar( &dock, !dock.IsVisible(), false );
}

void CChatMainFrame::OnUpdateViewDockWindow(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( dock.IsVisible() );
}

void CChatMainFrame::OnViewMainToolbar()
{
	ShowControlBar( &mainToolBar, !(mainToolBar.GetStyle() & WS_VISIBLE), false );
}

void CChatMainFrame::OnUpdateViewMainToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( mainToolBar.GetStyle() & WS_VISIBLE );
}

void CChatMainFrame::OnViewEditToolbar()
{
	ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void CChatMainFrame::OnUpdateViewEditToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( editToolBar.GetStyle() & WS_VISIBLE );
}

void CChatMainFrame::OnViewStatusModeToolbar()
{
	ShowControlBar( &statusModeToolBar, !(statusModeToolBar.GetStyle() & WS_VISIBLE), false );
}

void CChatMainFrame::OnUpdateViewStatusModeToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( statusModeToolBar.GetStyle() & WS_VISIBLE );
}

void CChatMainFrame::OnViewStatusBar()
{
	ShowControlBar( &statusBar, !(statusBar.GetStyle() & WS_VISIBLE), false );
}

void CChatMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( statusBar.GetStyle() & WS_VISIBLE );
}

void CChatMainFrame::OnSound()
{
	chatApp.sounds.setUseSound( !chatApp.sounds.getUseSound() );
}

void CChatMainFrame::OnUpdateSound(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( chatApp.sounds.getUseSound() );
}

void CChatMainFrame::OnStatusMode( UINT nID )
{
	CChatStatusModeType statusType;
	switch ( nID ) {
		case ID_STATUSMODE_ONLINE      : statusType = CSMT_Online; break;
		case ID_STATUSMODE_AWAY        : statusType = CSMT_Away; break;
		case ID_STATUSMODE_NOTAVAILIBLE: statusType = CSMT_NotAvailible; break;
		default                        : statusType = CSMT_Online;
	}
	chatApp.setStatusMode( statusType );
}

void CChatMainFrame::OnUpdateStatusMode( CCmdUI* pCmdUI )
{
	CChatStatusModeType statusType = chatApp.getStatusMode();
	switch ( pCmdUI->m_nID ) {
		case ID_STATUSMODE_ONLINE      : pCmdUI->SetCheck( statusType == CSMT_Online ); break;
		case ID_STATUSMODE_AWAY        : pCmdUI->SetCheck( statusType == CSMT_Away ); break;
		case ID_STATUSMODE_NOTAVAILIBLE: pCmdUI->SetCheck( statusType == CSMT_NotAvailible ); break;
	}
}

void CChatMainFrame::OnStatusModeInfo( UINT nID )
{
	CChatStatusModeType statusType;
	switch ( nID ) {
		case ID_STATUSMODE_ONLINE_INFO      : statusType = CSMT_Online; break;
		case ID_STATUSMODE_AWAY_INFO        : statusType = CSMT_Away; break;
		case ID_STATUSMODE_NOTAVAILIBLE_INFO: statusType = CSMT_NotAvailible; break;
		default                             : statusType = CSMT_Online;
	}
	if ( statusType != chatApp.getStatusMode() ) {
		CChatStatusModeDialog dlg( IDD_STATUSMODEINFO_DIALOG );
		if ( dlg.DoModal() == IDOK ) {
			CChatStatusMode* statusMode = chatApp.statusModes.getStatusMode( statusType );
			std::string info_backup = statusMode->info;
			statusMode->info = dlg.info;
			chatApp.setStatusMode( statusType );
			if ( !dlg.useAsDefault ) {
				statusMode->info = info_backup;
			}
		}
	}
}

void CChatMainFrame::OnUpdateStatusModeInfo( CCmdUI* pCmdUI )
{
	CChatStatusModeType statusType = chatApp.getStatusMode();
	switch ( pCmdUI->m_nID ) {
		case ID_STATUSMODE_ONLINE_INFO      : pCmdUI->Enable( statusType != CSMT_Online ); break;
		case ID_STATUSMODE_AWAY_INFO        : pCmdUI->Enable( statusType != CSMT_Away ); break;
		case ID_STATUSMODE_NOTAVAILIBLE_INFO: pCmdUI->Enable( statusType != CSMT_NotAvailible ); break;
	}
}

void CChatMainFrame::OnToCryOut()
{
	CChatMessageDialog dlg( IDD_MESSAGE_DIALOG, format( IDS_TOCRYOUT_DIALOG ) );
	if ( dlg.DoModal() == IDOK && !dlg.message.IsEmpty() ) {
		chatApp.udp.send( "<tocryout:" + dlg.message + ">" );
	}
}

void CChatMainFrame::OnOptions()
{
	CChatOptions dlg;
	dlg.DoModal();
}

void CChatMainFrame::OnUserSendMessage()
{
	CChatUser* user = chatApp.users.getSelected();
	if ( user && user != chatApp.users.getOnwer() ) {
		CChatMessageDialog dlg( IDD_MESSAGE_DIALOG, format( IDS_SENDMESSAGE_DIALOG, user->getUserName().c_str() ) );
		if ( dlg.DoModal() == IDOK && !dlg.message.IsEmpty() ) {
			chatApp.udp.send( "<tohostip:" + user->getIP() + "><popupmsg:" + std::string(static_cast<LPCTSTR>(dlg.message)) + ">" );
		}
	}
}

void CChatMainFrame::OnUpdateUserSendMessage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( chatApp.users.getSelected() != NULL && chatApp.users.getSelected() != chatApp.users.getOnwer() );
}

void CChatMainFrame::OnUserIngnore()
{
	CChatUser* user = chatApp.users.getSelected();
	if ( user ) {
		user->setIgnored( !user->isIgnored() );
	}
}

void CChatMainFrame::OnUpdateUserIngnore(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( chatApp.users.getSelected() != NULL && chatApp.users.getSelected() != chatApp.users.getOnwer() );
}

void CChatMainFrame::OnHelpContents() 
{
	std::string filename = chatApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}

void CChatMainFrame::OnHelpAbout() 
{
	CChatAbout dlg;
	dlg.DoModal();
}

void CChatMainFrame::OnEditCut()
{
	if ( &childView.edit == GetFocus() ) {
		if ( childView.edit.isSelected() ) {
			childView.edit.Cut();
		}
	}
}

void CChatMainFrame::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( &childView.edit == GetFocus() && childView.edit.isSelected() );
}

void CChatMainFrame::OnEditCopy()
{
	if ( &childView.edit == GetFocus() ) {
		if ( childView.edit.isSelected() ) {
			childView.edit.Copy();
		}
	} else if ( &childView.viewer == GetFocus() ) {
		if ( childView.viewer.isSelected() ) {
			childView.viewer.copy();
		}
	}
}

void CChatMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( (&childView.edit == GetFocus() && childView.edit.isSelected()) || (&childView.viewer == GetFocus() && childView.viewer.isSelected()) );
}

void CChatMainFrame::OnEditPaste()
{
	if ( &childView.edit == GetFocus() ) {
		childView.edit.paste();
	}
}

void CChatMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( &childView.edit == GetFocus() && ::IsClipboardFormatAvailable( CF_TEXT ) );
}

void CChatMainFrame::OnShowUserList()
{
	dock.tab.setCurrentItem( dock.tab.findItem( &dock.users ) );
	dock.users.SetFocus();
}

void CChatMainFrame::OnShowNetwork()
{
	dock.tab.setCurrentItem( dock.tab.findItem( &dock.network ) );
	dock.network.SetFocus();
}

void CChatMainFrame::OnShowSmiles()
{
	if ( chatApp.isWinNT() ) {
		dock.tab.setCurrentItem( dock.tab.findItem( &dock.smiles ) );
		dock.smiles.SetFocus();
	}
}

void CChatMainFrame::OnShowEdit()
{
	childView.edit.SetFocus();
}
