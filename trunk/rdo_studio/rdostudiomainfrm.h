#ifndef RDOSTUDIOMAINFRM_H
#define RDOSTUDIOMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioworkspace.h"
#include "rdostudiooutput.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
class RDOStudioMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RDOStudioMainFrame)

private:
	static void buildNotify( string str );
	static void debugNotify( string str );

protected:
	CToolBar           projectToolBar;
	CImageList         projectToolBarImageList;
	CToolBar           editToolBar;
	CImageList         editToolBarImageList;
	CStatusBar         statusBar;

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

	RDOStudioWorkspace workspace;
	RDOStudioOutput    output;

	BOOL OnCmdMsgForDockOnly( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewProjectToolbar();
	afx_msg void OnViewEditToolbar();
	afx_msg void OnUpdateViewProjectToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEditToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMAINFRM_H
