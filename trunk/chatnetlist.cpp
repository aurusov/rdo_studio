#include "stdafx.h"
#include "chatnetlist.h"

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
// ---------- CChatNetWorkGroup
// ----------------------------------------------------------------------------
CChatNetWorkGroup::CChatNetWorkGroup()
{
}

CChatNetWorkGroup::~CChatNetWorkGroup()
{
	clear();
}

void CChatNetWorkGroup::addPC( const std::string& hostname, const std::string& ip )
{
	if ( findPCByIP( ip ) == -1 ) {
		CChatNetPC* pc = new CChatNetPC( hostname, ip );
		list.push_back( pc );
//		chatApp.mainFrame->userList.list.addUser( user );
	}
}

int CChatNetWorkGroup::findPCByHostName( const std::string& hostname ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->hostname == hostname ) return i;
	}
	return -1;
}

int CChatNetWorkGroup::findPCByIP( const std::string& ip ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->ip == ip ) return i;
	}
	return -1;
}

CChatNetPC* CChatNetWorkGroup::getPCByHostName( const std::string& hostname ) const
{
	int index = findPCByHostName( hostname );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

CChatNetPC* CChatNetWorkGroup::getPCByIP( const std::string& ip ) const
{
	int index = findPCByIP( ip );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

void CChatNetWorkGroup::clear()
{
	std::vector< CChatNetPC* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}
