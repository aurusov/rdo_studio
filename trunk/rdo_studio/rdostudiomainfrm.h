#ifndef RDOSTUDIOMAINFRM_H
#define RDOSTUDIOMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioworkspace.h"
#include "rdostudiooutput.h"
#include "rdo_edit/rdoeditoreditstyle.h"
#include "rdo_edit/rdoeditorresultsstyle.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdofindeditstyle.h"
#include "rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "rdostudiostyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
class RDOStudioMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RDOStudioMainFrame)

private:
	CToolBar   fileToolBar;
	CImageList fileToolBarImageList;
	CToolBar   buildToolBar;
	CImageList buildToolBarImageList;
	CToolBar   editToolBar;
	CImageList editToolBarImageList;
	CStatusBar statusBar;

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

	RDOStudioWorkspace workspace;
	RDOStudioOutput    output;

	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOLogEditStyle     style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	rdoStyle::RDOStyleWithTheme      style_frame;
	void updateAllStyles() const;

	BOOL OnCmdMsgForDockOnly( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_VIRTUAL(RDOStudioMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFileToolbar();
	afx_msg void OnViewBuildToolbar();
	afx_msg void OnViewEditToolbar();
	afx_msg void OnUpdateViewFileToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBuildToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEditToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewOptions();
	afx_msg void OnHelpContents();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMAINFRM_H
