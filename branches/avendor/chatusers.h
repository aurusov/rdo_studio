#ifndef CHATUSERS_H
#define CHATUSERS_H
#pragma once

#include "chatstatusmode.h"

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
class CChatUsers;

class CChatUser: public CObject
{
friend class CChatUsers;

protected:
	CChatUser();
	CChatUser( const CString& _username, const CString& _hostname, const CString& _ip, const CChatStatusModeType _statusMode );
	virtual ~CChatUser();

	CString             userName;
	CString             hostName;
	CString             ip;
	CChatStatusModeType statusMode;

public:
	CString getUserName() const;
	void setUserName( const CString& value );
	
	CString getHostName() const;
	void setHostName( const CString& value );
	
	CString getIP() const;
	void setIP( const CString& value );

	CChatStatusModeType getStatusMode() const;
	void setStatusMode( const CChatStatusModeType value );
};

// ----------------------------------------------------------------------------
// ---------- CChatUsers
// ----------------------------------------------------------------------------
class CChatUsers: public CPtrArray
{
public:
	CChatUsers();
	virtual ~CChatUsers();

	void addUser( const CString& username, const CString& hostname, const CString& ip, const CChatStatusModeType statusMode = CSMT_Online );
	void deleteUser( const int listIndex );
	void deleteUser( const CChatUser* const user );
	int findUserByHostName( const CString& hostname );
	int findUserByIP( const CString& ip );
	CChatUser* getUserByHostName( const CString& hostname );
	CChatUser* getUserByIP( const CString& ip );
	void clear( const CChatUser* const dont_delete_user = NULL );
};

#endif // CHATUSERS_H
