#include "stdafx.h"
#include "chatudp.h"
#include "chatapp.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatUdp
// ----------------------------------------------------------------------------
CChatUdp::CChatUdp():
	CAsyncSocket(),
	sendBuffer( "" ),
	bytesSent( 0 ),
	bytesBufferSize( 0 )
{
}

CChatUdp::~CChatUdp()
{
	backupBuffer.RemoveAll();
}

void CChatUdp::DoAsyncSendBuff()
{
	while ( bytesSent < bytesBufferSize ) { // bytesBufferSize should be sendBuffer.GetLength() +1

		int dwBytes;

		SOCKADDR_IN remoteAddr;
		remoteAddr.sin_family      = AF_INET;
		remoteAddr.sin_port        = htons( chatApp.getPort() );
		remoteAddr.sin_addr.s_addr = htonl( INADDR_BROADCAST );
		dwBytes = SendTo( (LPCTSTR)sendBuffer + bytesSent, bytesBufferSize - bytesSent, (const SOCKADDR*)&remoteAddr, sizeof(remoteAddr) );

		if ( dwBytes == SOCKET_ERROR ) {
			if ( GetLastError() == WSAEWOULDBLOCK ) {
				break;
			} else {
				TCHAR szError[256];
				wsprintf( szError, "Server Socket failed to send: %s", getStrError() );
				Close();
				AfxMessageBox( szError );
				break;
			}
		} else {
			bytesSent += dwBytes;
		}
	}

	if ( bytesSent == bytesBufferSize ) {
		bytesSent = bytesBufferSize = 0;
		sendBuffer = "";
	}
}

void CChatUdp::OnReceive( int nErrorCode ) 
{
	TCHAR buff[70001];
	int nRead;

	unsigned int port = chatApp.getPort();
	nRead = ReceiveFrom( buff, 70000, chatApp.getBroadcastIP(), port ); //make it smaller so we can experiment mulitple notifications
	switch ( nRead ) {
		case 0           : Close(); break;
		case SOCKET_ERROR:
			if ( GetLastError() != WSAEWOULDBLOCK ) {
				if ( GetLastError() != WSAEMSGSIZE ) {
					TCHAR szError[256];
					wsprintf( szError, "OnReceive error: %s", getStrError() );
					AfxMessageBox( szError );
				} else {
					AfxMessageBox( "The datagram was too large and was truncated" );
					CString s( buff );
					addToBackupBufferAnsPars( s );
				}
			}
			break;
		default:
			if ( nRead != SOCKET_ERROR && nRead != 0 ) {
				buff[nRead] = 0;
				CString s( buff );
				addToBackupBufferAnsPars( s );
			}	
	}

	CAsyncSocket::OnReceive( nErrorCode );
}

void CChatUdp::OnSend( int nErrorCode ) 
{
	DoAsyncSendBuff();
	CAsyncSocket::OnSend( nErrorCode );
}

void CChatUdp::Send( const CString& value )
{
	SYSTEMTIME timeDest;
	::GetSystemTime( &timeDest );
	CString str_time;
	str_time.Format( "<time:%d>", timeDest.wHour * 60 * 60 * 1000 + timeDest.wMinute * 60 * 1000 + timeDest.wSecond * 1000 + timeDest.wMilliseconds );
	CString s = str_time + "<fromhost:" + chatApp.getHostName() + "><fromip:" + chatApp.getIP() + ">" + value;
	bytesSent       = 0;
	sendBuffer      = s;
	bytesBufferSize = sendBuffer.GetLength() + 1;
	DoAsyncSendBuff();
}

void CChatUdp::Close()
{
	CAsyncSocket::Close();
}

void CChatUdp::addToBackupBufferAnsPars( const CString& str )
{
	while ( backupBuffer.GetCount() > 200 ) {
		backupBuffer.RemoveHead();
	}
	if ( !backupBuffer.Find( str ) ) {
		backupBuffer.AddTail( str );
		parsCommand( str );
	}
}

void CChatUdp::parsCommand( const CString& line )
{
	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

//	mainFrame->childView->addStringToViewer( "", line );

	CString fromHost = getCommandValue( line, "fromhost" );
	CString fromIP   = getCommandValue( line, "fromip" );
	CChatUser* user  = chatApp.users.getUserByIP( fromIP );

	// CONNECT
	if ( hasCommand( line, "connect" ) ) {
		CString username = getCommandValue( line, "connect" );
		CString s;
		s.Format( IDS_CONNECT_VIEWERLOG, username );
		mainFrame->childView->addStringToViewer( username, s, CSTRT_Connect );
		chatApp.sounds.play( CST_Connect );

	// CLOSE
	} else if ( hasCommand( line, "close" ) ) {
		CString username;
		if ( user ) {
			username = user->getUserName();
		} else {
			username = fromHost;
		}
		CString s;
		s.Format( IDS_DISCONNECT_VIEWERLOG, username );
		mainFrame->childView->addStringToViewer( username, s, CSTRT_Disconnect );
		chatApp.sounds.play( CST_Disconnect );
		if ( user ) {
			chatApp.users.deleteUser( user );
			user = NULL;
		}

	// GETALLHOSTNAME
	} else if ( hasCommand( line, "getallhostname" ) ) {
		Send( "<username:" + chatApp.getUserName() + ">" );
		Send( "<hostname:" + chatApp.getHostName() + ">" );
		CString s;
		s.Format( "%d", chatApp.getStatusMode() );
		Send( "<statusmode_from_getallhostname:" + s + ">" );

	// USERNAME
	} else if ( hasCommand( line, "username" ) ) {
		CString username = getCommandValue( line, "username" );
		if ( !user ) {
			chatApp.users.addUser( username, "", fromIP );
		} else {
			if ( user->getUserName() != username ) {
				mainFrame->childView->addStringToViewer( username, user->getUserName() + " сменил имя на " + username, CSTRT_ChangeName );
				user->setUserName( username );
			}
		}

	// HOSTNAME
	} else if ( hasCommand( line, "hostname" && user ) ) {
		CString hostname = getCommandValue( line, "hostname" );
		user->setHostName( hostname );

	// STATUSMODE_FROM_GETALLHOSTNAME
	} else if ( hasCommand( line, "statusmode_from_getallhostname" ) && user ) {
		CString s = getCommandValue( line, "statusmode_from_getallhostname" );
		CChatStatusModeType status = (CChatStatusModeType)strtol( s, NULL, 10 );
		if ( user->getStatusMode() != status ) {
			user->setStatusMode( status );
		}

	// STATUSMODE
	} else if ( hasCommand( line, "statusmode" ) && user ) {
		CString s = getCommandValue( line, "statusmode" );
		CChatStatusModeType status = (CChatStatusModeType)strtol( s, NULL, 10 );
		if ( user->getStatusMode() != status ) {
			user->setStatusMode( status );
			CString username = user->getUserName();
			CString s_statusmsg = getCommandValue( line, "statusmsg" );
			if ( s_statusmsg.IsEmpty() ) {
				s.Format( IDS_STATUSMODE_VIEWERLOG, username, chatApp.statusModes.getStatusMode( status )->name );
			} else {
				s.Format( IDS_STATUSMODEWITHINFO_VIEWERLOG, username, chatApp.statusModes.getStatusMode( status )->name, s_statusmsg );
			}
			mainFrame->childView->addStringToViewer( username, s, CSTRT_ChangeStatusMode );
			chatApp.sounds.play( CST_ChangeStatusMode );
		}

	// MSG
	} else if ( hasCommand( line, "msg" ) ) {
		CString msg = getCommandValue( line, "msg", true );
		CString fromUser;
		if ( user ) {
			fromUser = user->getUserName();
		} else {
			fromUser = fromHost;
		}
		mainFrame->childView->addStringToViewer( fromUser, msg );

		if ( !mainFrame->isVisible() ) {
			mainFrame->modifyTray();
		}

		if ( !mainFrame->isActive() ) {
			mainFrame->showPopupMessage( "(" + fromUser + "): " + msg );
		}

		if ( chatApp.getIP() != fromIP ) {
			chatApp.sounds.play( CST_IncomingMessage );
		}

//		if ( fromIP == chatApp.getIP() ) {
//			if ( msg == "tocryout" ) {
//				Send( "<tocryout:q>" );
//			}
//		}

	// TOCRYOUT
	} else if ( hasCommand( line, "tocryout" ) ) {
		CString msg = getCommandValue( line, "tocryout", true );
		CString fromUser;
		if ( user ) {
			fromUser = user->getUserName();
		} else {
			fromUser = fromHost;
		}
		mainFrame->childView->addStringToViewer( fromUser, msg, CSTRT_ToCryOut );

		if ( !mainFrame->isVisible() ) {
			mainFrame->modifyTray();
		}

		if ( !mainFrame->isActive() ) {
			mainFrame->showPopupMessage( "(" + fromUser + " cry out): " + msg );
		}

		if ( chatApp.getIP() != fromIP ) {
			chatApp.sounds.play( CST_IncomingMessage );
		}
	}
}

CString CChatUdp::getCommandValue( const CString& line, const CString& command, const bool toEnd ) const
{
	int i = line.Find( "<" + command + ":" );
	if ( i != -1 ) {
		int j;
		if ( toEnd ) {
			j = line.GetLength() - 1;
		} else {
			j = line.Find( ">", i );
		}
		if ( j != -1 ) {
			i = i + command.GetLength() + 2;
			return line.Mid( i, j - i );
		}
	}
	return "";
}

bool CChatUdp::hasCommand( const CString& line, const CString& command ) const
{
	int i = line.Find( "<" + command );
	if ( i != -1 ) {
		int j = line.Find( ">", i );
		if ( j == i + command.GetLength() + 1 ) return true;
		j = line.Find( ":", i );
		if ( j == i + command.GetLength() + 1 ) return true;
	}
	return false;
}

CString CChatUdp::getStrError() const
{
	CString s = "";
	switch( GetLastError() ) {
		case 0:
			s = "no error";
			break;
		case WSAEADDRINUSE:
			s = "The specified address is already in use";
			break;
		case WSAEADDRNOTAVAIL:
			s = "The specified address is not available from the local machine";
			break;
		case WSAEAFNOSUPPORT:
			s = "Addresses in the specified family cannot be used with this socket";
			break;
		case WSAECONNREFUSED:
			s = "The attempt to connect was forcefully rejected";
			break;
		case WSAEDESTADDRREQ:
			s = "A destination address is required";
			break;
		case WSAEFAULT:
			s = "The lpSockAddrLen argument is incorrect";
			break;
		case WSAEINVAL:
			s = "The socket is already bound to an address";
			break;
		case WSAEISCONN:
			s = "The socket is already connected";
			break;
		case WSAEMFILE:
			s = "No more file descriptors are available";
			break;
		case WSAENETUNREACH:
			s = "The network cannot be reached from this host at this time";
			break;
		case WSAENOBUFS:
			s = "No buffer space is available. The socket cannot be connected";
			break;
		case WSAENOTCONN:
			s = "The socket is not connected";
			break;
		case WSAENOTSOCK:
			s = "The descriptor is a file, not a socket";
			break;
		case WSAETIMEDOUT:
			s = "The attempt to connect timed out without establishing a connection";
			break;
		default:
			s.Format( "%d", GetLastError() );
			break;
	}
	return s;
}
