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
	CChatUser( const std::string& _username, const std::string& _hostname, const std::string& _ip, const CChatStatusModeType _statusMode );
	virtual ~CChatUser();

	std::string         userName;
	std::string         hostName;
	std::string         ip;
	CChatStatusModeType statusMode;

public:
	std::string getUserName() const;
	void setUserName( const std::string& value );
	
	std::string getHostName() const;
	void setHostName( const std::string& value );
	
	std::string getIP() const;
	void setIP( const std::string& value );

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

	void addUser( const std::string& username, const std::string& hostname, const std::string& ip, const CChatStatusModeType statusMode = CSMT_Online );
	void deleteUser( const int listIndex );
	void deleteUser( const CChatUser* const user );
	int findUserByHostName( const std::string& hostname );
	int findUserByIP( const std::string& ip );
	CChatUser* getUserByHostName( const std::string& hostname );
	CChatUser* getUserByIP( const std::string& ip );
	void clear( const CChatUser* const dont_delete_user = NULL );
};

#endif // CHATUSERS_H
