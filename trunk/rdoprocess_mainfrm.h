#ifndef RDO_PROCESS_MAINFRM_H
#define RDO_PROCESS_MAINFRM_H

#include "ctrl/rdoprocess_projectbar.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPMainFrame
// ----------------------------------------------------------------------------
class RPMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RPMainFrame)

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

public:
	RPMainFrame();
	virtual ~RPMainFrame();

	RPProjectBar projectBar;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_VIRTUAL(RPMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateFlowConnector(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFlowRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFlowSelect(CCmdUI* pCmdUI);
	afx_msg void OnFlowSelect();
	afx_msg void OnFlowRotate();
	afx_msg void OnFlowConnector();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_MAINFRM_H
