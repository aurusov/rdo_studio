#ifndef CHATNETWORK_H
#define CHATNETWORK_H
#pragma once

#include "netsearch/Network.h"

// ----------------------------------------------------------------------------
// ---------- CChatNet
// ----------------------------------------------------------------------------
class CChatNet
{
protected:
	CChatNet();
	virtual ~CChatNet();

	HTREEITEM item;
	std::string name;
	std::string comment;
};

// ----------------------------------------------------------------------------
// ---------- CChatNetShared
// ----------------------------------------------------------------------------
class CChatNetShared: public CChatNet
{
friend class CChatNetServer;
friend class CChatNetwork;
private:
	CChatNetShared();
	virtual ~CChatNetShared();
};

// ----------------------------------------------------------------------------
// ---------- CChatNetServer
// ----------------------------------------------------------------------------
class CChatNetServer: public CChatNet
{
friend class CChatNetDomain;
friend class CChatNetwork;
private:
	CChatNetServer();
	virtual ~CChatNetServer();

	std::vector< CChatNetShared* > list;

	std::string ip;

public:
	void addShared( const std::string& name, const std::string& comment );
	void clear();
};

// ----------------------------------------------------------------------------
// ---------- CChatNetDomain
// ----------------------------------------------------------------------------
class CChatNetDomain: public CChatNet
{
friend class CChatNetwork;
private:
	std::vector< CChatNetServer* > list;

	CChatNetDomain();
	virtual ~CChatNetDomain();

	void addServer( const std::string& hostname, const std::string& ip );
	int findServerByHostName( const std::string& hostname ) const;
	int findServerByIP( const std::string& ip ) const;
	CChatNetServer* getServerByHostName( const std::string& hostname ) const;
	CChatNetServer* getServerByIP( const std::string& ip ) const;
	void clear();
};

// ----------------------------------------------------------------------------
// ---------- CChatNetwork
// ----------------------------------------------------------------------------
class CChatNetwork: public CNetwork
{
private:
	std::vector< CChatNetDomain* > list;

	CWinThread* enumNetworkThread;
	static UINT enumNetwork( LPVOID pParam );
	BOOL OnHitResource( NETRESOURCE& res );

	void clear();

public:
	CChatNetwork();
	virtual ~CChatNetwork();

	void refresh();
	bool refreshFinished() const { return enumNetworkThread == NULL; }
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORK_H
