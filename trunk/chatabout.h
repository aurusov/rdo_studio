#ifndef CHATABOUT_H
#define CHATABOUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdolink.h>

// ----------------------------------------------------------------------------
// ---------- CChatAbout
// ----------------------------------------------------------------------------
class CChatAbout: public CDialog
{
private:
	RDOLink email;

public:
	CChatAbout( CWnd* pParent = NULL );

	//{{AFX_DATA(CChatAbout)
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(CChatAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatAbout)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnEMailClick( NMHDR* pNotifyStruct, LRESULT* result );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATABOUT_H
