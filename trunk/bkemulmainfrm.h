#ifndef BKEMULMAINFRM_H
#define BKEMULMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "bkemulchildview.h"

// --------------------------------------------------------------
// ---------- BKMainFrame
// --------------------------------------------------------------
class BKMainFrame: public CFrameWnd
{
DECLARE_DYNAMIC(BKMainFrame)

private:
//	CToolBar    toolBar;
//	CStatusBar  statusBar;
	
public:
	BKMainFrame();
	virtual ~BKMainFrame();

	BKChildView childView;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(BKMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULMAINFRM_H
