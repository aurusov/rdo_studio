#ifndef CHATCHILDVIEW_H
#define CHATCHILDVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatviewer.h"
#include "chatedit.h"

// ----------------------------------------------------------------------------
// ---------- CChatChildView
// ----------------------------------------------------------------------------
class CChatChildView: public CWnd
{
public:
	CChatChildView();
	virtual ~CChatChildView();

	CChatViewer viewer;
	CChatEdit   edit;

protected:
	//{{AFX_VIRTUAL(CChatChildView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatChildView)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATCHILDVIEW_H
