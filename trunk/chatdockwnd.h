#ifndef CHATDOCKWND_H
#define CHATDOCKWND_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatuserstreectrl.h"

// ----------------------------------------------------------------------------
// ---------- CChatDockWnd
// ----------------------------------------------------------------------------
class CChatDockWnd: public CSizingControlBarG
{
public:
	CChatDockWnd();
	virtual ~CChatDockWnd();

	CChatUsersTreeCtrl tree;

protected:
	//{{AFX_VIRTUAL(CChatDockWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATDOCKWND_H
