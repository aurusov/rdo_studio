#ifndef CHATSTATUSMODE_H
#define CHATSTATUSMODE_H
#pragma once

#include <afxtempl.h>

// ----------------------------------------------------------------------------
// ---------- CChatStatusMode
// ----------------------------------------------------------------------------
enum CChatStatusModeType {
	CSMT_Online = 0,
	CSMT_Away,
	CSMT_NotAvailible
};

class CChatStatusMode
{
public:
	CChatStatusModeType type;
	CString             name;
	CString             info;

	CChatStatusMode();
	CChatStatusMode( const CChatStatusMode& statusMode );
	virtual ~CChatStatusMode();

	CChatStatusMode& operator= ( const CChatStatusMode& statusMode );
	bool operator== ( const CChatStatusMode& statusMode );
	bool operator!= ( const CChatStatusMode& statusMode );
};

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeList
// ----------------------------------------------------------------------------
class CChatStatusModeList: public CList< CChatStatusMode*, CChatStatusMode* >
{
public:
	CChatStatusModeList();
	virtual ~CChatStatusModeList();

	CChatStatusMode* getStatusMode( const CChatStatusModeType statusModeType ) const;

	CString getDefaultName( const CChatStatusModeType statusModeType ) const;

	void init();
	void saveSetting() const;
};

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeDialog
// ----------------------------------------------------------------------------
class CChatStatusModeDialog: CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );

public:
	CString info;
	int     useAsDefault;

	CChatStatusModeDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	virtual ~CChatStatusModeDialog();

	virtual int DoModal();
};

#endif // CHATSTATUSMODE_H
