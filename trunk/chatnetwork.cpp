#include "stdafx.h"
#include "chatnetwork.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNet
// ----------------------------------------------------------------------------
CChatNet::CChatNet():
	name( "" ),
	item( 0 ),
	openingThread( NULL )
{
}

CChatNet::~CChatNet()
{
	if ( isOpening() ) {
		::TerminateThread( openingThread->m_hThread, 1 );
		delete openingThread;
		openingThread = NULL;
	}
}

bool CChatNet::isOpening() const
{
	if ( openingThread ) {
		DWORD res;
		::GetExitCodeThread( openingThread->m_hThread, &res );
		return res == STILL_ACTIVE;
	}
	return false;
}

// ----------------------------------------------------------------------------
// ---------- CChatNetShared
// ----------------------------------------------------------------------------
CChatNetShared::CChatNetShared():
	CChatNet(),
	comment( "" )
{
}

CChatNetShared::~CChatNetShared()
{
}

std::string CChatNetShared::getNameForCtrl() const
{
	std::string str = name;
	int pos = str.find_last_of( '\\' );
	if ( pos != std::string::npos ) {
		str.erase( str.begin(), str.begin() + pos + 1 );
	}
	if ( !comment.empty() ) {
		str = format( "%s (%s)", comment.c_str(), str.c_str() );
	}
	return str;
}

// ----------------------------------------------------------------------------
// ---------- CChatNetServer
// ----------------------------------------------------------------------------
CChatNetServer::CChatNetServer():
	CChatNet(),
	comment( "" ),
	ip( "" )
{
}

CChatNetServer::~CChatNetServer()
{
	clear();
}

void CChatNetServer::clear()
{
	std::vector< CChatNetShared* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

std::string CChatNetServer::getNameForCtrl() const
{
	std::string str = name;
	int pos = str.find( "\\\\" );
	if ( pos == 0 ) {
		str.erase( str.begin(), str.begin() + 2 );
	}
	tolower( str );
	toupper( str, 0, 1 );

	if ( !comment.empty() ) {
		std::string str1 = str;
		tolower( str1 );
		std::string str2 = comment;
		tolower( str2 );
		if ( str1 != str2 ) {
			str = format( "%s (%s)", comment.c_str(), str.c_str() );
		}
	}
	return str;
}

std::string CChatNetServer::getToolTipInfo() const
{
	if ( !ip.empty() ) {
		return format( IDS_IP_VALUE, ip.c_str() );
	} else {
		return format( IDS_IP_UNKNOW );
	}
	return "";
}

// ----------------------------------------------------------------------------
// ---------- CChatNetDomain
// ----------------------------------------------------------------------------
CChatNetDomain::CChatNetDomain(): CChatNet()
{
}

CChatNetDomain::~CChatNetDomain()
{
	clear();
}

void CChatNetDomain::clear()
{
	std::vector< CChatNetServer* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

int CChatNetDomain::findServerByHostName( const std::string& hostname ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->name == hostname ) return i;
	}
	return -1;
}

int CChatNetDomain::findServerByIP( const std::string& ip ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->ip == ip ) return i;
	}
	return -1;
}

CChatNetServer* CChatNetDomain::getServerByHostName( const std::string& hostname ) const
{
	int index = findServerByHostName( hostname );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

CChatNetServer* CChatNetDomain::getServerByIP( const std::string& ip ) const
{
	int index = findServerByIP( ip );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

std::string CChatNetDomain::getNameForCtrl() const
{
	return name;
}

// ----------------------------------------------------------------------------
// ---------- CChatNetwork
// ----------------------------------------------------------------------------
CChatNetwork::CChatNetwork(): enumNetworkThread( NULL )
{
}

CChatNetwork::~CChatNetwork()
{
	stopEnum();
	clear();
}

void CChatNetwork::clear()
{
	std::vector< CChatNetDomain* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	if ( chatApp.mainFrame ) {
		chatApp.mainFrame->dock.network.DeleteAllItems();
	}
	list.clear();
}

UINT CChatNetwork::enumNetwork( LPVOID pParam )
{
	chatApp.network.Enumerate( CNetwork::GLOBALNET | CNetwork::TYPE_DISK );
	chatApp.network.enumNetworkThread = NULL;
	return 0;
}

void CChatNetwork::refresh()
{
	clear();
	enumNetworkThread = AfxBeginThread( enumNetwork, NULL );
}

BOOL CChatNetwork::OnHitResource( NETRESOURCE& res )
{
	if ( chatApp.mainFrame ) {
		if ( IsDomain( res ) ) {
			CChatNetDomain* domain = new CChatNetDomain;
			domain->name = GetRemoteName( res );
			list.push_back( domain );
			domain->item = chatApp.mainFrame->dock.network.InsertItem( domain->getNameForCtrl().c_str(), 0, 0 );
			chatApp.mainFrame->dock.network.SetItemData( domain->item, reinterpret_cast<DWORD>(domain) );
			chatApp.mainFrame->dock.network.SortChildren( NULL );
		}
		if ( IsServer( res ) ) {
			if ( !list.empty() ) {
				CChatNetDomain* domain = list[list.size()-1];
				CChatNetServer* server = new CChatNetServer;
				server->name    = GetRemoteName( res );
				server->comment = GetComment( res );

//				WORD wVersionRequested;
//				WSADATA wsaData;
//				wVersionRequested = MAKEWORD( 2, 0 );

//				if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) {
					std::string hostname = server->name;
					std::string::size_type pos = hostname.find( "\\\\" );
					if ( pos == 0 ) {
						hostname.erase( hostname.begin(), hostname.begin() + 2 );
					}
					PHOSTENT hostinfo = gethostbyname( hostname.c_str() );
					if ( hostinfo ) {
						int i = 0;
						while ( true ) {
							if ( !hostinfo->h_addr_list[i] ) {
								break;
							}
							if ( i ) {
								server->ip += "/";
							}
							server->ip += inet_ntoa( *reinterpret_cast<struct in_addr*>(hostinfo->h_addr_list[i]) );
							i++;
						}
					}
//					WSACleanup();
//				}

				domain->list.push_back( server );
				server->item = chatApp.mainFrame->dock.network.InsertItem( server->getNameForCtrl().c_str(), 1, 1, domain->item );
				chatApp.mainFrame->dock.network.SetItemData( server->item, reinterpret_cast<DWORD>(server) );
				chatApp.mainFrame->dock.network.SortChildren( domain->item );
				chatApp.mainFrame->dock.network.Expand( domain->item, TVE_EXPAND );
				Enumerate( &res, CNetwork::GLOBALNET | CNetwork::TYPE_DISK );
			}
		}
		if ( IsShare( res ) ) {
			if ( !list.empty() ) {
				CChatNetDomain* domain = list[list.size()-1];
				if ( !domain->list.empty() ) {
					CChatNetServer* server = domain->list[domain->list.size()-1];
					CChatNetShared* shared = new CChatNetShared;
					shared->name    = GetRemoteName( res );
					shared->comment = GetComment( res );
					server->list.push_back( shared );
					shared->item = chatApp.mainFrame->dock.network.InsertItem( shared->getNameForCtrl().c_str(), 2, 2, server->item );
					chatApp.mainFrame->dock.network.SetItemData( shared->item, reinterpret_cast<DWORD>(shared) );
					chatApp.mainFrame->dock.network.SortChildren( server->item );
					chatApp.mainFrame->dock.network.Expand( server->item, TVE_EXPAND );
				}
			}
		}
	}
	return TRUE;
}

void CChatNetwork::stopEnum()
{
	if ( enumNetworkThread ) {
		::TerminateThread( enumNetworkThread->m_hThread, 1 );
		delete enumNetworkThread;
		enumNetworkThread = NULL;
	}
}
