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

private:
	CEdit edit;

public:
	RDOPluginMFCMainFrame();
	virtual ~RDOPluginMFCMainFrame();

	void insertLine( const char* line );

protected:
	//{{AFX_VIRTUAL(RDOPluginMFCMainFrame)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
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
	afx_msg void OnModelShowRateInc();
	afx_msg void OnModelShowRateIncFour();
	afx_msg void OnModelShowRateDecFour();
	afx_msg void OnModelShowRateDec();
	afx_msg void OnUpdateModelShowRateInc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDec(CCmdUI* pCmdUI);
	afx_msg void OnModelFrameNext();
	afx_msg void OnModelFramePrev();
	afx_msg void OnUpdateModelFrameNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelFramePrev(CCmdUI* pCmdUI);
	afx_msg void OnModelNew();
	afx_msg void OnModelOpen();
	afx_msg void OnModelSave();
	afx_msg void OnModelClose();
	afx_msg void OnUpdateModelSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelClose(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFCMAINFRAME_H
