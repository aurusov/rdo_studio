#ifndef CHATNETWORKDOCK_H
#define CHATNETWORKDOCK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatnetworkctrl.h"

// ----------------------------------------------------------------------------
// ---------- CChatNetworkDock
// ----------------------------------------------------------------------------
class CChatNetworkDock: public CSizingControlBarG
{
public:
	CChatNetworkDock();
	virtual ~CChatNetworkDock();

	CChatNetworkCtrl list;

protected:
	//{{AFX_VIRTUAL(CChatNetworkDock)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetworkDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORKDOCK_H
