#include "stdafx.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "chatsplash.h"
#include "htmlhelp.h"
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
	port( 4000 ),
	broadcastIP( "192.168.1.255" ),
	splash( NULL )
{
}

BOOL CChatApp::InitInstance()
{
	CWinApp::InitInstance();

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("localChat") );

	mainFrame = new CChatMainFrame;
	m_pMainWnd = mainFrame;

	splash = new CChatSplash;
	splash->show( mainFrame );

	splash->setInitInfo( IDS_SPLASH_INITINFO_FONT );
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

	splash->setInitInfo( IDS_SPLASH_INITINFO_SOCKET );
	if ( !initSocket() ) return FALSE;

	splash->setInitInfo( IDS_SPLASH_INITINFO_SMILES );
	mainFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), TRUE );
	mainFrame->SetIcon( LoadIcon( MAKEINTRESOURCE(IDR_MAINFRAME) ), FALSE );
//	mainFrame->SetIcon( (HICON)::LoadImage( NULL, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR ), FALSE );

	style.init();

	mainFrame->ShowWindow(SW_SHOW);
	mainFrame->UpdateWindow();

	userName = GetProfileString( "User", "userName", userName.c_str() );

	sounds.init();
	statusModes.init();

	udp.send( "<connect:" + getUserName() + ">" );

	refreshUserList();
	network.refresh();

	delete splash;
	splash = NULL;

	return TRUE;
}

int CChatApp::ExitInstance()
{
	network.stopEnum();

	HtmlHelp( NULL, NULL, HH_CLOSE_ALL, 0 );

	udp.send( "<close>" );
	udp.close();

	sounds.saveSetting();
	statusModes.saveSetting();

	if ( splash ) { delete splash; splash = NULL; }

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

std::string CChatApp::getFullFileName()
{
	std::string fileName = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		fileName = szExeName;
	}
	return fileName;
}

std::string CChatApp::extractFilePath( const std::string& fileName )
{
	std::string s;
	std::string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos != std::string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, std::string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool CChatApp::isFileExists( const std::string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

std::string CChatApp::getFullHelpFileName( std::string str )
{
	str.insert( 0, extractFilePath( getFullFileName() ) );

	if ( !isFileExists( str ) ) {
		::MessageBox( NULL, format( IDS_NO_HELP_FILE, str.c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return "";
	}

	return str;
}
