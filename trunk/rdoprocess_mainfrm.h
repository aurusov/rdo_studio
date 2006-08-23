#ifndef RDO_PROCESS_MAINFRM_H
#define RDO_PROCESS_MAINFRM_H

#include "ctrl/rdoprocess_projectbar.h"
#include "ctrl/rdoprocess_toolbar.h"
#include <rdoprocess_object.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPMainFrameMsg
// ----------------------------------------------------------------------------
class RPMainFrameMsg: public RPObject
{
friend class RPMainFrame;
protected:
	RPMainFrameMsg();
	virtual rp::string getClassName() const { return "RPMainFrameMsg"; }
	virtual void notify( RPObject* from, UINT message, void* param = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RPMainFrame
// ----------------------------------------------------------------------------
class RPMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RPMainFrame)
friend class RPMainFrameMsg;
friend class RPProjectMFC;
private:
	CControlBar* last_docked;
	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

	void insertToolBar( CToolBar* toolbar );

protected:
	CStatusBar  m_wndStatusBar;
	RDOToolBar  m_wndToolBar;
	RPToolBar   m_wndStyleAndColorToolBar;
	CComboBox   m_wndCombo;
	RPMainFrameMsg* m_msg;

public:
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
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateFlowSelect(CCmdUI* pCmdUI);
	afx_msg void OnFlowSelect();
	afx_msg void OnFlowRotate();
	afx_msg void OnFlowConnector();
	afx_msg void OnUpdateBtnFillBrush(CCmdUI* pCmdUI);
	afx_msg void OnBtnFillBrush();
	afx_msg void OnBtnFillPen();
	afx_msg void OnBtnFillText();
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK( UINT lParam, LONG wParam );
	afx_msg LONG OnSelEndDefault( UINT lParam, LONG wParam );
	afx_msg void OnSysColorChange();
	afx_msg void OnMethodCommandRange( UINT id );
	afx_msg void OnMethodUpdateRange( CCmdUI* pCmdUI );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_MAINFRM_H
