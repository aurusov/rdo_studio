#include "stdafx.h"
#include "chatuserlist.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
CChatUser::CChatUser():
	userName( "" ),
	hostName( "" ),
	ip( "" ),
	statusMode( CSMT_Online ),
	statusInfo( "" ),
	ignored( false )
{
}

CChatUser::CChatUser( const std::string& _username, const std::string& _hostname, const std::string& _ip, const CChatStatusModeType _statusMode, const std::string& _statusInfo ):
	userName( _username ),
	hostName( _hostname ),
	ip( _ip ),
	statusMode( _statusMode ),
	statusInfo( _statusInfo ),
	ignored( false )
{
}

CChatUser::~CChatUser()
{
}

void CChatUser::setUserName( const std::string& value )
{
	if ( userName != value ) {
		userName = value;
		if ( !isIgnored() && chatApp.mainFrame ) {
			chatApp.mainFrame->dock.users.updateUserName( this );
		}
	}
}

void CChatUser::setHostName( const std::string& value )
{
	hostName = value;
	tolower( hostName );
	toupper( hostName, 0, 1 );
}

void CChatUser::setIP( const std::string& value )
{
	ip = value;
}

void CChatUser::setStatusMode( const CChatStatusModeType value )
{
	if ( statusMode != value ) {
		statusMode = value;
		if ( chatApp.mainFrame ) {
			chatApp.mainFrame->dock.users.updateUserStatus( this );
		}
	}
}

void CChatUser::setStatusInfo( const std::string& value )
{
	statusInfo = value;
}

void CChatUser::setIgnored( const bool value )
{
	if ( ignored != value ) {
		ignored = value;
		if ( chatApp.mainFrame ) {
			chatApp.mainFrame->dock.users.updateUserStatus( this );
			chatApp.mainFrame->dock.users.updateUserName( this );
		}
	}
}

std::string CChatUser::getToolTipInfo() const
{
	std::string s = "";
	if ( !statusInfo.empty() ) {
		s = format( IDS_STATUSINFO_VALUE, statusInfo.c_str() );
	}
	if ( !hostName.empty() ) {
		std::string host_str = format( IDS_HOSTNAME_VALUE, hostName.c_str() );
		if ( s.empty() ) {
			s = host_str;
		} else {
			s += ". " + host_str;
		}
	}
	return s;
}

// ----------------------------------------------------------------------------
// ---------- CChatUserList
// ----------------------------------------------------------------------------
CChatUserList::CChatUserList()
{
}

CChatUserList::~CChatUserList()
{
	std::vector< CChatUser* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

void CChatUserList::addUser( const std::string& username, const std::string& hostname, const std::string& ip, const CChatStatusModeType statusMode, const std::string& statusInfo )
{
	if ( findUserByIP( ip ) == -1 ) {
		CChatUser* user = new CChatUser( username, hostname, ip, statusMode, statusInfo );
		list.push_back( user );
		chatApp.mainFrame->dock.users.addUser( user );
	}
}

void CChatUserList::deleteUser( const int index )
{
	if ( index != -1 && index < list.size() ) {
		CChatUser* user = list[index];
		if ( chatApp.mainFrame ) {
			chatApp.mainFrame->dock.users.deleteUser( user );
		}
		list.erase( list.begin() + index );
		delete user;
	}
}

void CChatUserList::deleteUser( const CChatUser* const user )
{
	int index = 0;
	std::vector< CChatUser* >::iterator it = list.begin();
	while ( it != list.end() ) {
		if ( *it == user ) {
			deleteUser( index );
			break;
		}
		it++;
		index++;
	}
}

int CChatUserList::findUserByHostName( const std::string& hostname ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->getHostName() == hostname ) return i;
	}
	return -1;
}

int CChatUserList::findUserByIP( const std::string& ip ) const
{
	int cnt = list.size();
	for ( int i = 0; i < cnt; i++ ) {
		if ( list[i]->getIP() == ip ) return i;
	}
	return -1;
}

CChatUser* CChatUserList::getUserByHostName( const std::string& hostname ) const
{
	int index = findUserByHostName( hostname );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

CChatUser* CChatUserList::getUserByIP( const std::string& ip ) const
{
	int index = findUserByIP( ip );
	if ( index != -1 ) {
		return list[index];
	}
	return NULL;
}

void CChatUserList::clear( const CChatUser* const dont_delete_user )
{
	for ( int i = 0; i < list.size(); i++ ) {
		CChatUser* usr = list[i];
		if ( usr != dont_delete_user ) {
			deleteUser( i );
			i--;
		}
	}
}

const CChatUser* CChatUserList::getOnwer() const
{
	return getUserByIP( chatApp.getIP() );
}

CChatUser* CChatUserList::getSelected() const
{
	HTREEITEM hitem = chatApp.mainFrame->dock.users.GetSelectedItem();
	if ( hitem ) {
		CChatUser* user = reinterpret_cast<CChatUser*>(chatApp.mainFrame->dock.users.GetItemData( hitem ));
		return user;
	}
	return NULL;
}
