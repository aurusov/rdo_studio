#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#pragma once

#include "chatstatusmode.h"

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
class CChatUserList;

class CChatUser: public CObject
{
friend class CChatUserList;

private:
	CChatUser();
	CChatUser( const std::string& _username, const std::string& _hostname, const std::string& _ip, const CChatStatusModeType _statusMode );
	virtual ~CChatUser();

	std::string         userName;
	std::string         hostName;
	std::string         ip;
	CChatStatusModeType statusMode;

public:
	std::string getUserName() const { return userName; }
	void setUserName( const std::string& value );

	std::string getHostName() const { return hostName; }
	void setHostName( const std::string& value );

	std::string getIP() const       { return ip; }
	void setIP( const std::string& value );

	CChatStatusModeType getStatusMode() const { return statusMode; }
	void setStatusMode( const CChatStatusModeType value );
};

// ----------------------------------------------------------------------------
// ---------- CChatUserList
// ----------------------------------------------------------------------------
class CChatUserList
{
private:
	std::vector< CChatUser* > list;

public:
	CChatUserList();
	virtual ~CChatUserList();

	void addUser( const std::string& username, const std::string& hostname, const std::string& ip, const CChatStatusModeType statusMode = CSMT_Online );
	void deleteUser( const int index );
	void deleteUser( const CChatUser* const user );
	int findUserByHostName( const std::string& hostname ) const;
	int findUserByIP( const std::string& ip ) const;
	CChatUser* getUserByHostName( const std::string& hostname ) const;
	CChatUser* getUserByIP( const std::string& ip ) const;
	void clear( const CChatUser* const dont_delete_user = NULL );
};

#endif // CHATUSERLIST_H
