#ifndef CHATSTATUSMODE_H
#define CHATSTATUSMODE_H

#if _MSC_VER > 1000
#pragma once
#endif

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
	std::string         name;
	std::string         info;

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
class CChatStatusModeList
{
private:
	std::vector< CChatStatusMode* > list;
	void clear();

public:
	CChatStatusModeList();
	virtual ~CChatStatusModeList();

	CChatStatusModeList& operator= ( const CChatStatusModeList& statusModeList );
	bool operator== ( const CChatStatusModeList& statusModeList );
	bool operator!= ( const CChatStatusModeList& statusModeList );
	CChatStatusMode* operator[] ( const int index ) { return list[index]; }
	int count() const                               { return list.size(); }

	CChatStatusMode* getStatusMode( const CChatStatusModeType statusModeType ) const;

	std::string getDefaultName( const CChatStatusModeType statusModeType ) const;

	void init();
	void saveSetting() const;
};

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeDialog
// ----------------------------------------------------------------------------
class CChatStatusModeDialog: CDialog
{
public:
	CString info;
	int     useAsDefault;

	CChatStatusModeDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	virtual ~CChatStatusModeDialog();

protected:
	//{{AFX_VIRTUAL(CChatStatusModeDialog)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatStatusModeDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSTATUSMODE_H
