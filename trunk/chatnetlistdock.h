#ifndef CHATNETLISTDOCK_H
#define CHATNETLISTDOCK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatnetlistctrl.h"

// ----------------------------------------------------------------------------
// ---------- CChatNetListDock
// ----------------------------------------------------------------------------
class CChatNetListDock: public CSizingControlBarG
{
public:
	CChatNetListDock();
	virtual ~CChatNetListDock();

	CChatNetListCtrl list;

protected:
	//{{AFX_VIRTUAL(CChatNetListDock)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetListDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETLISTDOCK_H
