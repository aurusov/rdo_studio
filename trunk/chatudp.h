#ifndef CHATUDP_H
#define CHATUDP_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxsock.h>

// ----------------------------------------------------------------------------
// ---------- CChatUdp
// ----------------------------------------------------------------------------
class CChatUdp: public CAsyncSocket
{
private:
	std::string sendBuffer;
	int         bytesSent;
	int         bytesBufferSize;

	CStringList backupBuffer;
	void addToBackupBufferAnsPars( const std::string& str );

	void DoAsyncSendBuff();

	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void Close();

public:
	CChatUdp();
	virtual ~CChatUdp();

	void send( const char* value );
	void send( const std::string& value ) { send( value.c_str() ); }
	void close()                          { Close();               }

	std::string getStrError() const;

	void parsCommand( const std::string& line );
	std::string getCommandValue( const std::string& line, const std::string& command, const bool toEnd = false ) const;
	bool hasCommand( const std::string& line, const std::string& command ) const;
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATUDP_H
