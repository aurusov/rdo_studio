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
DECLARE_DYNCREATE( CChatChildView )
protected:
	CChatViewer viewer;
	CChatEdit   edit;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus(CWnd *pOldWnd);

	DECLARE_MESSAGE_MAP()

public:
	CChatChildView();
	virtual ~CChatChildView();

	void addStringToViewer( const CString& userName, const CString& str, CChatStringType type = CSTRT_Message );
};

#endif // CHATCHILDVIEW_H
