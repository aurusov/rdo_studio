#ifndef RDO_PROCESS_MAINFRM_H
#define RDO_PROCESS_MAINFRM_H

#include "ctrl/rdoprocess_projectbar.h"
#include "ctrl/rdoprocess_toolbar.h"

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
	CToolBar    m_wndToolBlockBarMJ; // MJ 2.04.06 панель для блоков
	CComboBox   m_wndCombo;
	RPToolBar   m_wndStyleAndColorToolBar;

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

public:
	void blank_rdo_MJ();
	RPMainFrame();
	virtual ~RPMainFrame();

	RPProjectBar projectBar;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_VIRTUAL(RPMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
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
	afx_msg void OnGenerate();
	afx_msg void OnGenType();
	afx_msg void OnUpdateBtnFillBrush(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
	afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_MAINFRM_H
