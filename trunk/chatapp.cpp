#include "stdafx.h"
#include "chatapp.h"
#include "chatmainfrm.h"
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

	refreshUserList();
	network.refresh();

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

void CChatApp::setUserName( const std::string& value )
{
	if ( userName != value ) {
		userName = value;
		WriteProfileString( "User", "userName", userName.c_str() );
		udp.send( "<username:" + userName + ">" );
	}
}

void CChatApp::refreshUserList()
{
	users.clear( users.getUserByIP( getIP() ) );
	udp.send( "<getallhostname>" );
}

CFont& CChatApp::getFont()
{
	return font;
}
