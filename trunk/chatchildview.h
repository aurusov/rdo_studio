#ifndef CHATCHILDVIEW_H
#define CHATCHILDVIEW_H
#pragma once

#include "chatviewer.h"
#include "chatedit.h"

// ----------------------------------------------------------------------------
// ---------- CChatChildView
// ----------------------------------------------------------------------------
class CChatChildView: public CWnd
{
protected:
	CChatViewer viewer;
	CChatEdit   edit;

public:
	CChatChildView();
	virtual ~CChatChildView();

	void addStringToViewer( const std::string& userName, const std::string& str, CChatStringType type = CSTRT_Message );

protected:
	//{{AFX_VIRTUAL(CChatChildView)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
