#ifndef RDOPLUGINMFCMAINFRAME_H
#define RDOPLUGINMFCMAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCMainFrame
// ----------------------------------------------------------------------------
class RDOPluginMFCMainFrame: public CFrameWnd
{
DECLARE_DYNAMIC(RDOPluginMFCMainFrame)

public:
	RDOPluginMFCMainFrame();
	virtual ~RDOPluginMFCMainFrame();

protected:
	//{{AFX_VIRTUAL(RDOPluginMFCMainFrame)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPluginMFCMainFrame)
	afx_msg void OnPluginClose();
	afx_msg void OnModelBuild();
	afx_msg void OnModelRun();
	afx_msg void OnModelStop();
	afx_msg void OnUpdateModelBuild(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelStop(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnModelRunNoshow();
	afx_msg void OnModelRunAnimation();
	afx_msg void OnModelRunMonitor();
	afx_msg void OnUpdateModelRunNoshow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRunAnimation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRunMonitor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFCMAINFRAME_H
