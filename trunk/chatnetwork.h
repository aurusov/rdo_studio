#ifndef CHATNETWORK_H
#define CHATNETWORK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "netsearch/Network.h"

// ----------------------------------------------------------------------------
// ---------- CChatNet
// ----------------------------------------------------------------------------
class CChatNet
{
friend class CChatNetworkCtrl;
protected:
	CChatNet();
	virtual ~CChatNet();

	std::string name;
	HTREEITEM   item;
	CWinThread* openingThread;
	bool isOpening() const;

	virtual std::string getNameForCtrl() const = 0;

public:
	enum Type { shared, server, domain };
	virtual getType() const = 0;

	virtual std::string getFullPath() const    { return name; }
	virtual std::string getToolTipInfo() const { return "";   }
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

	std::string comment;
	virtual std::string getNameForCtrl() const;

public:
	virtual getType() const { return shared; }
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

	std::string comment;
	std::string ip;
	virtual std::string getNameForCtrl() const;

public:
	virtual getType() const { return server; }
	virtual std::string getToolTipInfo() const;
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

	virtual std::string getNameForCtrl() const;

	int findServerByHostName( const std::string& hostname ) const;
	int findServerByIP( const std::string& ip ) const;
	CChatNetServer* getServerByHostName( const std::string& hostname ) const;
	CChatNetServer* getServerByIP( const std::string& ip ) const;
	void clear();

public:
	virtual getType() const { return domain; }
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
	void stopEnum();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORK_H
