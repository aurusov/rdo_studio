#include "stdafx.h"
#include "chatnetwork.h"
#include "chatapp.h"
#include "chatmainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNet
// ----------------------------------------------------------------------------
CChatNet::CChatNet():
	item( 0 ),
	name( "" ),
	comment( "" )
{
}

CChatNet::~CChatNet()
{
}

// ----------------------------------------------------------------------------
// ---------- CChatNetShared
// ----------------------------------------------------------------------------
CChatNetShared::CChatNetShared(): CChatNet()
{
}

CChatNetShared::~CChatNetShared()
{
}

// ----------------------------------------------------------------------------
// ---------- CChatNetServer
// ----------------------------------------------------------------------------
CChatNetServer::CChatNetServer():
	CChatNet(),
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

void CChatNetServer::addShared( const std::string& name, const std::string& comment )
{
	CChatNetShared* shared = new CChatNetShared;
	shared->name    = name;
	shared->comment = comment;
	list.push_back( shared );
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

void CChatNetDomain::addServer( const std::string& hostname, const std::string& ip )
{
	if ( findServerByIP( ip ) == -1 ) {
		CChatNetServer* server = new CChatNetServer;
		server->name = hostname;
		server->ip   = ip;
		list.push_back( server );
//		chatApp.mainFrame->userList.list.addUser( user );
	}
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

// ----------------------------------------------------------------------------
// ---------- CChatNetwork
// ----------------------------------------------------------------------------
CChatNetwork::CChatNetwork(): enumNetworkThread( NULL )
{
}

CChatNetwork::~CChatNetwork()
{
	if ( enumNetworkThread ) {
		::TerminateThread( enumNetworkThread->m_hThread, 1 );
		enumNetworkThread = NULL;
	}
	clear();
}

void CChatNetwork::clear()
{
	std::vector< CChatNetDomain* >::iterator it = list.begin();
	while ( it != list.end() ) {
		CChatNetDomain* domain = *it++;
		if ( chatApp.mainFrame ) {
			chatApp.mainFrame->networkList.list.deleteChildren( domain->item );
		}
		delete domain;
	}
	list.clear();
}

UINT CChatNetwork::enumNetwork( LPVOID pParam )
{
	chatApp.network.Enumerate();
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
			CString name    = GetRemoteName( res );
			CString comment = GetComment( res );
			domain->name    = name;
			domain->comment = comment;
			list.push_back( domain );
			domain->item = chatApp.mainFrame->networkList.list.InsertItem( domain->name.c_str(), 0, 0 );
		}
		if ( IsServer( res ) ) {
			if ( !list.empty() ) {
				CChatNetServer* server = new CChatNetServer;
				CString name    = GetRemoteName( res );
				CString comment = GetComment( res );
				server->name    = name;
				server->comment = comment;
				CChatNetDomain* domain = list[list.size()-1];
				domain->list.push_back( server );
				server->item = chatApp.mainFrame->networkList.list.InsertItem( server->name.c_str(), 1, 1, domain->item );

//				Enumerate( &res, CNetwork::SEARCHDEFAULT );
			}
		}
		if ( IsShare( res ) ) {
		}
	}
/*
		int i = 0; 
		for ( ; str[i] == '\\' ; ++i );
		if ( i ) {
			str = str.Mid(i);
		}
		str.MakeLower();
*/
//	} 
	return TRUE;
}
