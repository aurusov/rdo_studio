#include "stdafx.h"
#include "chatusers.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
CChatUser::CChatUser():
	CObject(),
	userName( "" ),
	hostName( "" ),
	ip( "" ),
	statusMode( CSMT_Online )
{
}

CChatUser::CChatUser( const CString& _username, const CString& _hostname, const CString& _ip, const CChatStatusModeType _statusMode ):
	CObject(),
	userName( _username ),
	hostName( _hostname ),
	ip( _ip ),
	statusMode( _statusMode )
{
}

CChatUser::~CChatUser()
{
}

CString CChatUser::getUserName() const
{
	return userName;
}

void CChatUser::setUserName( const CString& value )
{
	if ( userName != value ) {
		userName = value;
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.updateUserName( this );
		}
	}
}

CString CChatUser::getHostName() const
{
	return hostName;
}

void CChatUser::setHostName( const CString& value )
{
	hostName = value;
}

CString CChatUser::getIP() const
{
	return ip;
}

void CChatUser::setIP( const CString& value )
{
	ip = value;
}

CChatStatusModeType CChatUser::getStatusMode() const
{
	return statusMode;
}

void CChatUser::setStatusMode( const CChatStatusModeType value )
{
	if ( statusMode != value ) {
		statusMode = value;
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.updateUserStatus( this );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- CChatUsers
// ----------------------------------------------------------------------------
CChatUsers::CChatUsers(): CPtrArray()
{
}

CChatUsers::~CChatUsers()
{
	clear();
}

void CChatUsers::addUser( const CString& username, const CString& hostname, const CString& ip, const CChatStatusModeType statusMode )
{
	if ( findUserByIP( ip ) == -1 ) {
		CChatUser* user = new CChatUser( username, hostname, ip, statusMode );
		Add( user );
		((CChatMainFrame*)AfxGetMainWnd())->dock.tree.addUser( user );
	}
}

void CChatUsers::deleteUser( const int listIndex )
{
	if ( listIndex != -1 && listIndex < GetSize() ) {
		CChatUser* user = (CChatUser*)GetAt( listIndex );
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.deleteUser( user );
		}
		delete user;
		RemoveAt( listIndex );
	}
}

void CChatUsers::deleteUser( const CChatUser* const user )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* usr = (CChatUser*)GetAt(i);
		if ( usr == user ) {
			deleteUser( i );
			break;
		}
	}
}

int CChatUsers::findUserByHostName( const CString& hostname )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* user = (CChatUser*)GetAt(i);
		if ( user->getHostName() == hostname ) return i;
	}
	return -1;
}

int CChatUsers::findUserByIP( const CString& ip )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* user = (CChatUser*)GetAt(i);
		if ( user->getIP() == ip ) return i;
	}
	return -1;
}

CChatUser* CChatUsers::getUserByHostName( const CString& hostname )
{
	int listIndex = findUserByHostName( hostname );
	if ( listIndex != -1 ) {
		return (CChatUser*)GetAt( listIndex );
	}
	return NULL;
}

CChatUser* CChatUsers::getUserByIP( const CString& ip )
{
	int listIndex = findUserByIP( ip );
	if ( listIndex != -1 ) {
		return (CChatUser*)GetAt( listIndex );
	}
	return NULL;
}

void CChatUsers::clear( const CChatUser* const dont_delete_user )
{
	int cnt = GetSize();
	for ( int i = 0; i < GetSize(); i++ ) {
		CChatUser* usr = (CChatUser*)GetAt(i);
		if ( usr != dont_delete_user ) {
			deleteUser( i );
			i--;
		}
	}
}
