#ifndef CHATDOCK_H
#define CHATDOCK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatuserlistctrl.h"
#include "chatnetworkctrl.h"

#include <rdotabctrl.h>

// ----------------------------------------------------------------------------
// ---------- CChatDock
// ----------------------------------------------------------------------------
class CChatDock: public CSizingControlBarG
{
public:
	CChatDock();
	virtual ~CChatDock();

	RDOTabCtrl tab;
	CChatUserListCtrl users;
	CChatNetworkCtrl  network;

protected:
	//{{AFX_VIRTUAL(CChatDock)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATDOCK_H
