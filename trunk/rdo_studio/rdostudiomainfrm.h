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
#include "rdostudiochartviewstyle.h"
#include "rdostudiostatusbar.h"
#include "rdostudioframestyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioMainFrame
// ----------------------------------------------------------------------------
class RDOStudioMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RDOStudioMainFrame)

private:
	CToolBar   fileToolBar;
	CImageList fileToolBarImageList;
	CToolBar   editToolBar;
	CImageList editToolBarImageList;
	CToolBar   zoomToolBar;
	CImageList zoomToolBarImageList;
	CToolBar   modelToolBar;
	CImageList modelToolBarImageList;
	RDOStudioStatusBar statusBar;

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
	RDOStudioFrameStyle              style_frame;
	RDOStudioChartViewStyle          style_chart;
	void updateAllStyles() const;

	void showWorkspace();
	void showOutput();

	void showNewModelTime( const double value );

	void beginProgress( const int lower = 0, const int upper = 100, const int step = 1 );
	void getProgressRange( int& lower, int& upper ) const { statusBar.getRange( lower, upper ); };
	void setProgress( const int pos ) { statusBar.setPos( pos ); };
	int  getProgress() const { return statusBar.getPos(); };
	void offsetProgress( const int offset ) { statusBar.offsetPos( offset ); };
	void stepProgress() { statusBar.stepIt(); };
	void endProgress();

	BOOL OnCmdMsgForDockOnly( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_VIRTUAL(RDOStudioMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFileToolbar();
	afx_msg void OnViewEditToolbar();
	afx_msg void OnViewZoomToolbar();
	afx_msg void OnViewModelToolbar();
	afx_msg void OnUpdateViewFileToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEditToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewModelToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewOptions();
	afx_msg void OnHelpContents();
	afx_msg void OnModelRunNoShow();
	afx_msg void OnModelRunAnimation();
	afx_msg void OnModelRunMonitor();
	afx_msg void OnUpdateModelRunNoShow(CCmdUI* pCmdUI);
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
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomAuto(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnWorkspaceShow();
	afx_msg void OnOutputShow();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMAINFRM_H
