#include "stdafx.h"
#include "chatnetwork.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNetPC
// ----------------------------------------------------------------------------
CChatNetPC::CChatNetPC():
	hostname( "" ),
	ip( "" )
{
}

CChatNetPC::CChatNetPC( const std::string& _hostname, const std::string& _ip ):
	hostname( _hostname ),
	ip( _ip )
{
}

CChatNetPC::~CChatNetPC()
{
}

// ----------------------------------------------------------------------------
// ---------- CChatNetWorkgroup
// ----------------------------------------------------------------------------
CChatNetWorkgroup::CChatNetWorkgroup()
{
}

CChatNetWorkgroup::~CChatNetWorkgroup()
{
	clear();
}

void CChatNetWorkgroup::addPC( const std::string& hostname, const std::string& ip )
{
	if ( findPCByIP( ip ) == -1 ) {
		CChatNetPC* pc = new CChatNetPC( hostname, ip );
		list.push_back( pc );
//		chatApp.mainFrame->userList.list.addUser( user );
	}
}

int CChatNetWorkgroup::findPCByHostName( const std::string& hostname ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->hostname == hostname ) return i;
	}
	return -1;
}

int CChatNetWorkgroup::findPCByIP( const std::string& ip ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->ip == ip ) return i;
	}
	return -1;
}

CChatNetPC* CChatNetWorkgroup::getPCByHostName( const std::string& hostname ) const
{
	int index = findPCByHostName( hostname );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

CChatNetPC* CChatNetWorkgroup::getPCByIP( const std::string& ip ) const
{
	int index = findPCByIP( ip );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

void CChatNetWorkgroup::clear()
{
	std::vector< CChatNetPC* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
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
}

UINT CChatNetwork::enumNetwork( LPVOID pParam )
{
	chatApp.network.Enumerate();
	chatApp.network.enumNetworkThread = NULL;
	return 0;
}

void CChatNetwork::refresh()
{
	enumNetworkThread = AfxBeginThread( enumNetwork, NULL );
}

BOOL CChatNetwork::OnHitResource( NETRESOURCE& res )
{
	if ( IsDomain( res ) ) {
		CString str = CString("domain: ") + GetRemoteName( res ) + ", comment: " + GetComment( res ) + ", localname: " + GetLocalName( res ) + ", provider: " + GetProvider( res );
		AfxMessageBox( str );
	}
	if ( IsServer( res ) ) {
		CString str = CString("server: ") + GetRemoteName( res ) + ", comment: " + GetComment( res ) + ", localname: " + GetLocalName( res ) + ", provider: " + GetProvider( res );
		AfxMessageBox( str );
		Enumerate( &res, CNetwork::SEARCHDEFAULT );
	}
	if ( IsShare( res ) ) {
		CString str = CString("share: ") + GetRemoteName( res ) + ", comment: " + GetComment( res ) + ", localname: " + GetLocalName( res ) + ", provider: " + GetProvider( res );
		AfxMessageBox( str );
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
