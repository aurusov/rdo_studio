#ifndef CHATUDP_H
#define CHATUDP_H
#pragma once

#include <afxsock.h>

// ----------------------------------------------------------------------------
// ---------- CChatUdp
// ----------------------------------------------------------------------------
class CChatUdp: public CAsyncSocket
{
private:
	CString sendBuffer;
	int     bytesSent;
	int     bytesBufferSize;

	CStringList backupBuffer;
	void addToBackupBufferAnsPars( const CString& str );

	void DoAsyncSendBuff();

public:
	CChatUdp();
	virtual ~CChatUdp();

	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void Send( const CString& value );
	virtual void Close();

	CString getStrError() const;

	void parsCommand( const CString& line );
	CString getCommandValue( const CString& line, const CString& command, const bool toEnd = false ) const;
	bool hasCommand( const CString& line, const CString& command ) const;
};

#endif // CHATUDP_H
