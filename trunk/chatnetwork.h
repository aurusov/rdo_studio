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

	virtual std::string getNameForCtrl() const = 0;

public:
	enum Type { shared, server, domain };
	virtual getType() const = 0;
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

	std::string name;
	std::string comment;
	virtual std::string getNameForCtrl() const;
	virtual getType() const { return shared; }

public:
	std::string getName() const { return name; }
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
	void clear();

	std::string name;
	std::string ip;
	virtual std::string getNameForCtrl() const;
	virtual getType() const { return server; }

public:
	std::string getToolTipInfo() const;
};

// ----------------------------------------------------------------------------
// ---------- CChatNetDomain
// ----------------------------------------------------------------------------
class CChatNetDomain: public CChatNet
{
friend class CChatNetwork;
private:
	CChatNetDomain();
	virtual ~CChatNetDomain();

	std::vector< CChatNetServer* > list;

	std::string name;
	virtual std::string getNameForCtrl() const;
	virtual getType() const { return domain; }

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
