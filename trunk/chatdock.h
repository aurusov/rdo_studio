#ifndef CHATDOCK_H
#define CHATDOCK_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatuserlistctrl.h"
#include "chatnetworkctrl.h"
#include "chatsmilelistctrl.h"

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
	CImageList images;
	CChatUserListCtrl  users;
	CChatNetworkCtrl   network;
	CChatSmileListCtrl smiles;

protected:
	//{{AFX_VIRTUAL(CChatDock)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATDOCK_H
