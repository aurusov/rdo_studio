#ifndef CHATUSERLISTDOCK_H
#define CHATUSERLISTDOCK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatuserlistctrl.h"

// ----------------------------------------------------------------------------
// ---------- CChatUserListDock
// ----------------------------------------------------------------------------
class CChatUserListDock: public CSizingControlBarG
{
public:
	CChatUserListDock();
	virtual ~CChatUserListDock();

	CChatUserListCtrl list;

protected:
	//{{AFX_VIRTUAL(CChatUserListDock)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatUserListDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATUSERLISTDOCK_H
