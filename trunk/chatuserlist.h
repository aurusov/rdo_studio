#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatstatusmode.h"

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
class CChatUser
{
friend class CChatUserList;

private:
	CChatUser();
	CChatUser( const std::string& _username, const std::string& _hostname, const std::string& _ip, const CChatStatusModeType _statusMode, const std::string& _statusInfo );
	virtual ~CChatUser();

	std::string         userName;
	std::string         hostName;
	std::string         ip;
	CChatStatusModeType statusMode;
	std::string         statusInfo;
	bool                ignored;

public:
	std::string getUserName() const { return userName; }
	void setUserName( const std::string& value );

	std::string getHostName() const { return hostName; }
	void setHostName( const std::string& value );

	std::string getIP() const       { return ip; }
	void setIP( const std::string& value );

	CChatStatusModeType getStatusMode() const { return statusMode; }
	void setStatusMode( const CChatStatusModeType value );

	std::string getStatusInfo() const { return statusInfo; }
	void setStatusInfo( const std::string& value );

	bool isIgnored() const { return ignored; }
	void setIgnored( const bool value );

	std::string getToolTipInfo() const;
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

	void addUser( const std::string& username, const std::string& hostname, const std::string& ip, const CChatStatusModeType statusMode = CSMT_Online, const std::string& statusInfo = "" );
	void deleteUser( const int index );
	void deleteUser( const CChatUser* const user );
	int findUserByHostName( const std::string& hostname ) const;
	int findUserByIP( const std::string& ip ) const;
	CChatUser* getUserByHostName( const std::string& hostname ) const;
	CChatUser* getUserByIP( const std::string& ip ) const;
	void clear( const CChatUser* const dont_delete_user = NULL );

	const CChatUser* getOnwer() const;
	CChatUser* getSelected() const;
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATUSERLIST_H
