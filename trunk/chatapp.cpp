#include "stdafx.h"
#include "chatapp.h"
#include "chatmainfrm.h"
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
	port( 4000 ),
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

	mainFrame = new CChatMainFrame;
	m_pMainWnd = mainFrame;
	mainFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), TRUE );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), FALSE );
//	mainFrame->SetIcon( (HICON)::LoadImage( NULL, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR ), FALSE );

	mainFrame->ShowWindow(SW_SHOW);
	mainFrame->UpdateWindow();

	userName = GetProfileString( "User", "userName" );

	sounds.init();
	statusModes.init();

	if ( !initSocket() ) return FALSE;

	udp.send( "<connect:" + getUserName() + ">" );

	OnRefreshUsersList( 0 );

	return TRUE;
}

int CChatApp::ExitInstance()
{
	udp.send( "<close>" );
	udp.close();

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
			if ( userName.empty() ) {
				userName = hostName;
			}
			if( ( hostinfo = gethostbyname(name)) != NULL ) {
				ip = inet_ntoa( *(struct in_addr*)*hostinfo->h_addr_list );
//				ip = "192.168.1.1";
				int i = ip.rfind( '.' );
				if ( i != -1 ) {
					broadcastIP = std::string( ip.begin(), ip[i+1] ) + "255";
				}
			}
		}
		WSACleanup( );
	}

	if ( !udp.Create( getPort(), SOCK_DGRAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, getIP().c_str() ) ) {
		AfxMessageBox( format( "Failed to create UDP socket: %s! Close and restart app.", udp.getStrError().c_str() ).c_str() );
		return false;
	}

	int rflag = 0, flag =1, len = 0;

	if ( !udp.SetSockOpt( SO_BROADCAST, &flag, sizeof(int) ) ) {
		AfxMessageBox( format( "SetSockOpt failed to set SO_BROADCAST: %s", udp.getStrError().c_str() ).c_str() );
		return false;
	}

	len = sizeof( rflag );

	if ( !udp.GetSockOpt( SO_BROADCAST, &rflag, &len ) && rflag == 0 ) {
		AfxMessageBox( format( "GetSockOpt failed to get SO_BROADCAST: %s", udp.getStrError().c_str() ).c_str() );
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
	std::string s;
	if ( automatically ) {
		s = format( "<statusmode:%d><statusmsg:%s>", statusMode->type, format( IDS_STATUSMODE_CHANGEAUTOMATICALLY ).c_str() );
	} else if ( statusMode->info.empty() ) {
		s = format( "<statusmode:%d>", statusMode->type );
	} else {
		s = format( "<statusmode:%d><statusmsg:%s>", statusMode->type, statusMode->info.c_str() );
	}
	if ( !automatically ) {
		mainFrame->resetAutoStatusMode();
	}
	udp.send( s );
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
			std::string info_backup = statusMode->info;
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
	udp.send( "<getallhostname>" );
}

void CChatApp::OnToCryOut()
{
	CChatToCryOutDialog dlg( IDD_TOCRYOUT_DIALOG );
	if ( dlg.DoModal() == IDOK && !dlg.message.IsEmpty() ) {
		udp.send( "<tocryout:" + dlg.message + ">" );
	}
}

void CChatApp::OnOptions()
{
	CChatOptions dlg;
	dlg.DoModal();
}

void CChatApp::setUserName( const std::string& value )
{
	if ( userName != value ) {
		userName = value;
		WriteProfileString( "User", "userName", userName.c_str() );
		udp.send( "<username:" + userName + ">" );
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
