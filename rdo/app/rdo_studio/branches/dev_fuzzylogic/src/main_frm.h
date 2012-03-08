/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_frm.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MAIN_FRM_H_
#define _RDO_STUDIO_MFC_MAIN_FRM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/output.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "app/rdo_studio_mfc/src/style.h"
#include "app/rdo_studio_mfc/src/chart/view_style.h"
#include "app/rdo_studio_mfc/src/status_bar.h"
#include "app/rdo_studio_mfc/src/frame/style.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOToolBar
// --------------------------------------------------------------------------------
class RDOToolBar: public CToolBar
{
protected:
	CImageList disabledImage;

public:
	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID );
};

// --------------------------------------------------------------------------------
// -------------------- RDOToolBarModel
// --------------------------------------------------------------------------------
class RDOToolBarModel: public RDOToolBar
{
protected:
	double log101;
	CSliderCtrl slider;

	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	DECLARE_MESSAGE_MAP()

public:
	RDOToolBarModel(): RDOToolBar(), log101( log(101.0) ) {}

	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID );

	double getSpeed() const { return log( double(slider.GetPos() + 1) ) / log101; }
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioMainFrame
// --------------------------------------------------------------------------------
class RDOStudioMainFrame: public CMDIFrameWnd
{
DECLARE_DYNAMIC(RDOStudioMainFrame)
friend class RDOToolBar;
friend class RDOStudioModelDoc;

private:
	RDOToolBar          fileToolBar;
	RDOToolBar          editToolBar;
	RDOToolBar          zoomToolBar;
	RDOToolBarModel     modelToolBar;
	RDOStudioStatusBar  statusBar;


	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );
	
	std::map< HWND, CWnd* > cmd_wnd;

	UINT m_updateTimer;

	// Используется при закрытии модели. Задается вопрос.
	static rbool close_mode;

public:
	RDOStudioMainFrame();
	virtual ~RDOStudioMainFrame();

	RDOStudioWorkspace workspace;
	RDOStudioOutput    output;
	PTR(CControlBar)   m_pLastDocked;

	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	RDOStudioFrameStyle              style_frame;
	RDOStudioChartViewStyle          style_chart;
	void updateAllStyles() const;

	void insertToolBar(PTR(CToolBar) pToolbar);

	void showWorkspace();
	void showOutput();

	double getSpeed() const { return modelToolBar.getSpeed(); }

	void registerCmdWnd( CWnd* wnd, HWND hwnd = 0 ) {
		if ( wnd ) {
			if ( !hwnd ) hwnd = wnd->m_hWnd;
			cmd_wnd[hwnd] = wnd;
		}
	}

	void beginProgress( const int lower = 0, const int upper = 100, const int step = 1 );
	void getProgressRange( int& lower, int& upper ) const  { statusBar.getRange( lower, upper ); };
	void setProgress( const int pos )                      { statusBar.setPos( pos );            };
	int  getProgress() const                               { return statusBar.getPos();          };
	void offsetProgress( const int offset )                { statusBar.offsetPos( offset );      };
	void stepProgress()                                    { statusBar.stepIt();                 };
	void endProgress();

	BOOL OnCmdMsgForDockOnly( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

	void update_start();
	void update_stop();

	static rbool is_close_mode() { return close_mode; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

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
	afx_msg void OnModelRuntimeMaxSpeed();
	afx_msg void OnModelRuntimeJump();
	afx_msg void OnModelRuntimeSync();
	afx_msg void OnModelRuntimePause();
	afx_msg void OnUpdateModelRuntimeMaxSpeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimeJump(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimeSync(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRuntimePause(CCmdUI* pCmdUI);
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnterMenuLoop( BOOL bIsTrackPopupMenu );
	afx_msg void OnExitMenuLoop( BOOL bIsTrackPopupMenu );
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelSpeedStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnWorkspaceShow();
	afx_msg void OnOutputShow();
	afx_msg void OnMethodCommandRange( UINT id );
	afx_msg void OnMethodUpdateRange( CCmdUI* pCmdUI );
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_MAIN_FRM_H_
