#include "stdafx.h"
#include "chatapp.h"
#include "chatoptions.h"
#include "resource.h"

#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatApp
// ----------------------------------------------------------------------------
CChatApp chatApp;

BEGIN_MESSAGE_MAP( CChatApp, CWinApp )
	//{{AFX_MSG_MAP(CChatApp)
	ON_COMMAND_EX( ID_FILE_REFRESHUSERSLIST, OnRefreshUsersList )
	ON_COMMAND_EX( ID_STATUSMODE_ONLINE      , OnStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_AWAY        , OnStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_NOTAVAILIBLE, OnStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_ONLINE      , OnUpdateStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_AWAY        , OnUpdateStatusMode )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_NOTAVAILIBLE, OnUpdateStatusMode )
	ON_COMMAND_EX( ID_STATUSMODE_ONLINE_INFO      , OnStatusModeInfo )
	ON_COMMAND_EX( ID_STATUSMODE_AWAY_INFO        , OnStatusModeInfo )
	ON_COMMAND_EX( ID_STATUSMODE_NOTAVAILIBLE_INFO, OnStatusModeInfo )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_ONLINE_INFO      , OnUpdateStatusModeInfo )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_AWAY_INFO        , OnUpdateStatusModeInfo )
	ON_UPDATE_COMMAND_UI( ID_STATUSMODE_NOTAVAILIBLE_INFO, OnUpdateStatusModeInfo )
	ON_COMMAND( ID_CHAT_TOCRYOUT, OnToCryOut )
	ON_COMMAND( ID_CHAT_OPTIONS, OnOptions )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatApp::CChatApp():
	CWinApp(),
	userName( "" ),
	hostName( "" ),
	ip( "" ),
	port( 4002 ),
	broadcastIP( "192.168.1.255" )
{
}

BOOL CChatApp::InitInstance()
{
	CWinApp::InitInstance();

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("localChat") );

	HFONT hf = (HFONT)::GetStockObject( SYSTEM_FIXED_FONT );
//	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );
			font.CreateFontIndirect( &lf );
		}
	}

/*
	CFont* font = new CFont();
	LOGFONT lf;
	memset( &lf, 0, sizeof( LOGFONT ) );
	lf.lfHeight = 8;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_ROMAN;
	font->CreateFontIndirect( &lf );
*/

	pFrame = new CChatMainFrame;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );
	pFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), TRUE );
	pFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), FALSE );
//	pFrame->SetIcon( (HICON)::LoadImage( NULL, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR ), FALSE );

	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	userName = GetProfileString( "User", "userName" );

	sounds.init();
	statusModes.init();

	if ( !initSocket() ) return FALSE;

	udp.Send( "<connect:" + getUserName() + ">" );

	OnRefreshUsersList( 0 );

	return TRUE;
}

int CChatApp::ExitInstance()
{
	udp.Send( "<close>" );
	udp.Close();

	sounds.saveSetting();
	statusModes.saveSetting();

	return CWinApp::ExitInstance();
}

bool CChatApp::initSocket()
{
	if ( !AfxSocketInit() ) {
		AfxMessageBox( "Sockets init failed" );
		return false;
	}

	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );

	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) {
		if( gethostname( name, sizeof(name) ) == 0 ) {
			hostName = name;
			if ( userName.IsEmpty() ) {
				userName = hostName;
			}
			if( ( hostinfo = gethostbyname(name)) != NULL ) {
				ip = inet_ntoa( *(struct in_addr*)*hostinfo->h_addr_list );
//				ip = "192.168.1.1";
				int i = ip.ReverseFind( '.' );
				if ( i != -1 ) {
					broadcastIP = ip.Left( i + 1 ) + "255";
				}
			}
		}
		WSACleanup( );
	}

	if ( !udp.Create( getPort(), SOCK_DGRAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, getIP() ) ) {
		CString s;
		s.Format( "Failed to create UDP socket: %s! Close and restart app.", udp.getStrError() );
		AfxMessageBox( s );
		return false;
	}

	int rflag = 0, flag =1, len = 0;

	if ( !udp.SetSockOpt( SO_BROADCAST, &flag, sizeof(int) ) ) {
		CString s;
		s.Format( "SetSockOpt failed to set SO_BROADCAST: %s", udp.getStrError() );
		AfxMessageBox( s );
		return false;
	}

	len = sizeof( rflag );

	if ( !udp.GetSockOpt( SO_BROADCAST, &rflag, &len ) && rflag == 0 ) {
		CString s;
		s.Format( "GetSockOpt failed to get SO_BROADCAST: %s", udp.getStrError() );
		AfxMessageBox( s );
		return false;
	}

	return true;
}

CChatStatusModeType CChatApp::getStatusMode()
{
	CChatUser* user = users.getUserByIP( getIP() );
	if ( user ) {
		return user->getStatusMode();
	}
	return CSMT_Online;
}

void CChatApp::setStatusMode( const CChatStatusModeType value, const bool automatically )
{
	CChatStatusMode* statusMode = statusModes.getStatusMode( value );
	CString s;
	if ( automatically ) {
		CString s2;
		s2.LoadString( IDS_STATUSMODE_CHANGEAUTOMATICALLY );
		s.Format( "<statusmode:%d><statusmsg:%s>", statusMode->type, s2 );
	} else if ( statusMode->info.IsEmpty() ) {
		s.Format( "<statusmode:%d>", statusMode->type );
	} else {
		s.Format( "<statusmode:%d><statusmsg:%s>", statusMode->type, statusMode->info );
	}
	if ( !automatically ) {
		pFrame->resetAutoStatusMode();
	}
	udp.Send( s );
}

void CChatApp::OnStatusMode( UINT nID )
{
	CChatStatusModeType statusType;
	switch ( nID ) {
		case ID_STATUSMODE_ONLINE      : statusType = CSMT_Online; break;
		case ID_STATUSMODE_AWAY        : statusType = CSMT_Away; break;
		case ID_STATUSMODE_NOTAVAILIBLE: statusType = CSMT_NotAvailible; break;
		default                        : statusType = CSMT_Online;
	}
	setStatusMode( statusType );
}

void CChatApp::OnUpdateStatusMode( CCmdUI* pCmdUI )
{
	CChatStatusModeType statusType = getStatusMode();
	switch ( pCmdUI->m_nID ) {
		case ID_STATUSMODE_ONLINE      : pCmdUI->SetCheck( statusType == CSMT_Online ); break;
		case ID_STATUSMODE_AWAY        : pCmdUI->SetCheck( statusType == CSMT_Away ); break;
		case ID_STATUSMODE_NOTAVAILIBLE: pCmdUI->SetCheck( statusType == CSMT_NotAvailible ); break;
	}
}

void CChatApp::OnStatusModeInfo( UINT nID )
{
	CChatStatusModeType statusType;
	switch ( nID ) {
		case ID_STATUSMODE_ONLINE_INFO      : statusType = CSMT_Online; break;
		case ID_STATUSMODE_AWAY_INFO        : statusType = CSMT_Away; break;
		case ID_STATUSMODE_NOTAVAILIBLE_INFO: statusType = CSMT_NotAvailible; break;
		default                             : statusType = CSMT_Online;
	}
	if ( statusType != getStatusMode() ) {
		CChatStatusModeDialog dlg( IDD_STATUSMODEINFO_DIALOG );
		if ( dlg.DoModal() == IDOK ) {
			CChatStatusMode* statusMode = statusModes.getStatusMode( statusType );
			CString info_backup = statusMode->info;
			statusMode->info = dlg.info;
			setStatusMode( statusType );
			if ( !dlg.useAsDefault ) {
				statusMode->info = info_backup;
			}
		}
	}
}

void CChatApp::OnUpdateStatusModeInfo( CCmdUI* pCmdUI )
{
	CChatStatusModeType statusType = getStatusMode();
	switch ( pCmdUI->m_nID ) {
		case ID_STATUSMODE_ONLINE_INFO      : pCmdUI->Enable( statusType != CSMT_Online ); break;
		case ID_STATUSMODE_AWAY_INFO        : pCmdUI->Enable( statusType != CSMT_Away ); break;
		case ID_STATUSMODE_NOTAVAILIBLE_INFO: pCmdUI->Enable( statusType != CSMT_NotAvailible ); break;
	}
}

void CChatApp::OnRefreshUsersList( UINT nID )
{
	users.clear( users.getUserByIP( getIP() ) );
	udp.Send( "<getallhostname>" );
}

void CChatApp::OnToCryOut()
{
	CChatToCryOutDialog dlg( IDD_TOCRYOUT_DIALOG );
	if ( dlg.DoModal() == IDOK && !dlg.message.IsEmpty() ) {
		udp.Send( "<tocryout:" + dlg.message + ">" );
	}
}

void CChatApp::OnOptions()
{
	CChatOptions dlg;
	dlg.DoModal();
}

void CChatApp::setUserName( const CString& value )
{
	if ( userName != value ) {
		userName = value;
		WriteProfileString( "User", "userName", userName );
		udp.Send( "<username:" + userName + ">" );
	}
}

CFont& CChatApp::getFont()
{
	return font;
}

// ----------------------------------------------------------------------------
// ---------- CChatToCryOutDialog
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatToCryOutDialog, CDialog )
	//{{AFX_MSG_MAP(CChatToCryOutDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatToCryOutDialog::CChatToCryOutDialog( UINT nIDTemplate, CWnd* pParentWnd ):
	CDialog( nIDTemplate, pParentWnd ),
	message( "" )
{
}

CChatToCryOutDialog::~CChatToCryOutDialog()
{
}

void CChatToCryOutDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_TOCRYOUT_EDIT, message );
}
