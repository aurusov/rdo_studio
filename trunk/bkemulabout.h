#ifndef BKEMULABOUT_H
#define BKEMULABOUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdolink.h"

// ----------------------------------------------------------------------------
// ---------- BKEmulAbout
// ----------------------------------------------------------------------------
class BKEmulAbout: public CDialog
{
private:
	RDOLink email;

public:
	BKEmulAbout( CWnd* pParent = NULL );

	//{{AFX_DATA(BKEmulAbout)
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(BKEmulAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(BKEmulAbout)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnEMailClick( NMHDR* pNotifyStruct, LRESULT* result );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULABOUT_H
