#ifndef CHATNETWORK_H
#define CHATNETWORK_H
#pragma once

#include "netsearch/Network.h"

// ----------------------------------------------------------------------------
// ---------- CChatNetPC
// ----------------------------------------------------------------------------
class CChatNetPC
{
friend class CChatNetWorkgroup;

private:
	CChatNetPC();
	CChatNetPC( const std::string& _hostname, const std::string& _ip );
	virtual ~CChatNetPC();

	std::string hostname;
	std::string ip;

public:
	std::string getHostName() const { return hostname; }
	std::string getIP() const       { return ip;   }
};

// ----------------------------------------------------------------------------
// ---------- CChatNetWorkgroup
// ----------------------------------------------------------------------------
class CChatNetWorkgroup
{
private:
	std::vector< CChatNetPC* > list;

public:
	CChatNetWorkgroup();
	virtual ~CChatNetWorkgroup();

	void addPC( const std::string& hostname, const std::string& ip );
	int findPCByHostName( const std::string& hostname ) const;
	int findPCByIP( const std::string& ip ) const;
	CChatNetPC* getPCByHostName( const std::string& hostname ) const;
	CChatNetPC* getPCByIP( const std::string& ip ) const;
	void clear();
};

// ----------------------------------------------------------------------------
// ---------- CChatNetwork
// ----------------------------------------------------------------------------
class CChatNetwork: public CNetwork
{
private:
	CWinThread* enumNetworkThread;
	static UINT enumNetwork( LPVOID pParam );
	BOOL OnHitResource( NETRESOURCE& res );

public:
	CChatNetwork();
	virtual ~CChatNetwork();

	void refresh();
	bool refreshFinished() const { return enumNetworkThread == NULL; }
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORK_H
