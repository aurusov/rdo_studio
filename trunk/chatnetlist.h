#ifndef CHATNETLIST_H
#define CHATNETLIST_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- CChatNetPC
// ----------------------------------------------------------------------------
class CChatNetPC
{
friend class CChatNetWorkGroup;

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
// ---------- CChatNetWorkGroup
// ----------------------------------------------------------------------------
class CChatNetWorkGroup
{
private:
	std::vector< CChatNetPC* > list;

public:
	CChatNetWorkGroup();
	virtual ~CChatNetWorkGroup();

	void addPC( const std::string& hostname, const std::string& ip );
	int findPCByHostName( const std::string& hostname ) const;
	int findPCByIP( const std::string& ip ) const;
	CChatNetPC* getPCByHostName( const std::string& hostname ) const;
	CChatNetPC* getPCByIP( const std::string& ip ) const;
	void clear();
};

#endif // CHATNETLIST_H
