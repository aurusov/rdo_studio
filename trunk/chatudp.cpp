#include "stdafx.h"
#include "chatudp.h"
#include "chatapp.h"
#include "chatmainfrm.h"
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
		dwBytes = SendTo( sendBuffer.c_str() + bytesSent, bytesBufferSize - bytesSent, (const SOCKADDR*)&remoteAddr, sizeof(remoteAddr) );

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
	CString s = chatApp.getBroadcastIP().c_str();
	nRead = ReceiveFrom( buff, 70000, s, port ); //make it smaller so we can experiment mulitple notifications
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
					std::string s( buff );
					addToBackupBufferAnsPars( s );
				}
			}
			break;
		default:
			if ( nRead != SOCKET_ERROR && nRead != 0 ) {
				buff[nRead] = 0;
				std::string s( buff );
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

void CChatUdp::send( const char* value )
{
	SYSTEMTIME timeDest;
	::GetSystemTime( &timeDest );
	std::string str_time = format( "<time:%d>", timeDest.wHour * 60 * 60 * 1000 + timeDest.wMinute * 60 * 1000 + timeDest.wSecond * 1000 + timeDest.wMilliseconds );
	std::string s = str_time + "<fromhost:" + chatApp.getHostName() + "><fromip:" + chatApp.getIP() + ">" + value;
	bytesSent       = 0;
	sendBuffer      = s;
	bytesBufferSize = sendBuffer.length() + 1;
	DoAsyncSendBuff();
}

void CChatUdp::Close()
{
	CAsyncSocket::Close();
}

void CChatUdp::addToBackupBufferAnsPars( const std::string& str )
{
	while ( backupBuffer.GetCount() > 200 ) {
		backupBuffer.RemoveHead();
	}
	if ( !backupBuffer.Find( str.c_str() ) ) {
		backupBuffer.AddTail( str.c_str() );
		parsCommand( str );
	}
}

void CChatUdp::parsCommand( const std::string& line )
{
//	chatApp.mainFrame->childView.viewer.addString( "", line );

	std::string fromHost = getCommandValue( line, "fromhost" );
	std::string fromIP   = getCommandValue( line, "fromip" );
	CChatUser* user      = chatApp.users.getUserByIP( fromIP );
	std::string fromUser;
	if ( user ) {
		fromUser = user->getUserName();
	} else {
		fromUser = fromHost;
	}

	// MSG
	if ( hasCommand( line, "msg" ) ) {
		if ( user && user->isIgnored() ) return;
		std::string msg = getCommandValue( line, "msg", true );
		chatApp.mainFrame->childView.viewer.addString( fromUser, msg );

		if ( !chatApp.mainFrame->isVisible() ) {
			chatApp.mainFrame->modifyTray();
		}

		if ( !chatApp.mainFrame->isActive() ) {
			chatApp.mainFrame->showPopupMessage( "(" + fromUser + "): " + msg );
		}

		if ( chatApp.getIP() != fromIP ) {
			chatApp.sounds.play( CST_IncomingMessage );
		}

//		if ( fromIP == chatApp.getIP() ) {
//			if ( msg == "tocryout" ) {
//				send( "<tocryout:q>" );
//			}
//		}

	// TOCRYOUT
	} else if ( hasCommand( line, "tocryout" ) ) {
		if ( user && user->isIgnored() ) return;
		std::string msg = getCommandValue( line, "tocryout", true );
		chatApp.mainFrame->childView.viewer.addString( fromUser, msg, CSTRT_ToCryOut );

		if ( !chatApp.mainFrame->isVisible() ) {
			chatApp.mainFrame->modifyTray();
		}

		if ( !chatApp.mainFrame->isActive() ) {
			chatApp.mainFrame->showPopupMessage( "(" + fromUser + " cry out): " + msg );
		}

		if ( chatApp.getIP() != fromIP ) {
			chatApp.sounds.play( CST_IncomingMessage );
		}

	// PRIVATE
	} else if ( hasCommand( line, "popupmsg" ) && user ) {
		if ( user && user->isIgnored() ) return;
		std::string toIP  = getCommandValue( line, "tohostip" );
		CChatUser* toUser = chatApp.users.getUserByIP( toIP );
		if ( toUser ) {
			if ( user == chatApp.users.getOnwer() ) {
				std::string msg = getCommandValue( line, "popupmsg", true );
				chatApp.mainFrame->childView.viewer.addString( fromUser, msg, CSTRT_PrivateMsgSend, toUser->getUserName() );
				chatApp.sounds.play( CST_PrivateMsg );
			} else if ( toUser == chatApp.users.getOnwer() ) {
				std::string msg = getCommandValue( line, "popupmsg", true );
				chatApp.mainFrame->childView.viewer.addString( fromUser, msg, CSTRT_PrivateMsgReceive, toUser->getUserName() );
				chatApp.sounds.play( CST_PrivateMsg );
			}
		}

	// STATUSMODE
	} else if ( hasCommand( line, "statusmode" ) && user ) {
		std::string s = getCommandValue( line, "statusmode" );
		CChatStatusModeType status = static_cast<CChatStatusModeType>(strtol( s.c_str(), NULL, 10 ));
		if ( user->getStatusMode() != status ) {
			user->setStatusMode( status );
			if ( user->isIgnored() ) return;
			std::string username = user->getUserName();
			std::string s_statusmsg = getCommandValue( line, "statusmsg", true );
			if ( s_statusmsg.empty() ) {
				s = format( IDS_STATUSMODE_VIEWERLOG, username.c_str(), chatApp.statusModes.getStatusMode( status )->name.c_str() );
				user->setStatusInfo( "" );
			} else {
				s = format( IDS_STATUSMODEWITHINFO_VIEWERLOG, username.c_str(), chatApp.statusModes.getStatusMode( status )->name.c_str(), s_statusmsg.c_str() );
				user->setStatusInfo( s_statusmsg );
			}
			chatApp.mainFrame->childView.viewer.addString( username, s, CSTRT_ChangeStatusMode );
			chatApp.sounds.play( CST_ChangeStatusMode );
		}

	// CONNECT
	} else if ( hasCommand( line, "connect" ) ) {
		std::string username = getCommandValue( line, "connect" );
		std::string s = format( IDS_CONNECT_VIEWERLOG, username.c_str() );
		chatApp.mainFrame->childView.viewer.addString( username, s, CSTRT_Connect );
		chatApp.sounds.play( CST_Connect );

	// CLOSE
	} else if ( hasCommand( line, "close" ) ) {
		std::string s = format( IDS_DISCONNECT_VIEWERLOG, fromUser.c_str() );
		chatApp.mainFrame->childView.viewer.addString( fromUser, s, CSTRT_Disconnect );
		chatApp.sounds.play( CST_Disconnect );
		if ( user ) {
			chatApp.users.deleteUser( user );
			user = NULL;
		}

	// GETALLHOSTNAME
	} else if ( hasCommand( line, "getallhostname" ) ) {
		send( "<username:" + chatApp.getUserName() + ">" );
		send( "<hostname:" + chatApp.getHostName() + ">" );
		std::string s = format( "%d", chatApp.getStatusMode() );
		send( "<statusmode_from_getallhostname:" + s + ">" );

	// USERNAME
	} else if ( hasCommand( line, "username" ) ) {
		std::string username = getCommandValue( line, "username" );
		if ( !user ) {
			chatApp.users.addUser( username, "", fromIP );
		} else {
			if ( user->getUserName() != username ) {
				if ( user->isIgnored() ) {
					user->setUserName( username );
				} else {
					chatApp.mainFrame->childView.viewer.addString( username, format( IDS_CHANGENAME_VIEWERLOG, user->getUserName().c_str(), username.c_str() ), CSTRT_ChangeName );
					user->setUserName( username );
				}
			}
		}

	// HOSTNAME
	} else if ( hasCommand( line, "hostname" ) && user ) {
		std::string hostname = getCommandValue( line, "hostname" );
		user->setHostName( hostname );

	// STATUSMODE_FROM_GETALLHOSTNAME
	} else if ( hasCommand( line, "statusmode_from_getallhostname" ) && user ) {
		std::string s = getCommandValue( line, "statusmode_from_getallhostname" );
		CChatStatusModeType status = static_cast<CChatStatusModeType>(strtol( s.c_str(), NULL, 10 ));
		if ( user->getStatusMode() != status ) {
			user->setStatusMode( status );
		}
	}
}

std::string CChatUdp::getCommandValue( const std::string& line, const std::string& command, const bool toEnd ) const
{
	int i = line.find( "<" + command + ":" );
	if ( i != std::string::npos ) {
		int j;
		if ( toEnd ) {
			j = line.length() - 1;
		} else {
			j = line.find( ">", i );
		}
		if ( j != std::string::npos ) {
			i = i + command.length() + 2;
			return line.substr( i, j-i );
		}
	}
	return "";
}

bool CChatUdp::hasCommand( const std::string& line, const std::string& command ) const
{
	int i = line.find( "<" + command );
	if ( i != std::string::npos ) {
		int j = line.find( ">", i );
		if ( j == i + command.length() + 1 ) return true;
		j = line.find( ":", i );
		if ( j == i + command.length() + 1 ) return true;
	}
	return false;
}

std::string CChatUdp::getStrError() const
{
	std::string s = "";
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
			s = format( "%d", GetLastError() );
			break;
	}
	return s;
}
