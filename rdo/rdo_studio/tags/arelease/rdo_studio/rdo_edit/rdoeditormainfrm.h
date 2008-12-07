#ifndef RDOEDITORMAINFRM_H
#define RDOEDITORMAINFRM_H
#pragma once

/*
#include "rdoeditorproject.h"
#include "rdoeditorchildview.h"

// ----------------------------------------------------------------------------
// ---------- RDOEditorStatusBar
// ----------------------------------------------------------------------------
class RDOEditorStatusBar: public CStatusBar
{
protected:
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorStatusBar();
	virtual ~RDOEditorStatusBar();
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorMainFrame
// ----------------------------------------------------------------------------
enum RDORunError {
	RDORE_None = 0,
	RDORE_Syntax,
	RDORE_Runtime,
	RDORE_Load,
	RDORE_License
};

enum RDODebug {
	RDOD_None = 0,
	RDOD_Hook,
	RDOD_Timer,
	RDOD_WaitForSingleObject
};

class RDOEditorMainFrame: public CFrameWnd
{
private:
	void updateReopenSubMenu() const;
	void loadReopen();
	void saveReopen() const;
	CString loadRdoPath() const;
	void saveRdoPath( const CString& path ) const;

protected:
	CToolBar           projectToolBar;
	CImageList         projectToolBarImageList;
	CToolBar           editToolBar;
	CImageList         editToolBarImageList;
	RDOEditorStatusBar statusBar;

	CStringList        reopenList;

	PROCESS_INFORMATION pi;
	HWND                rdoMainHWnd;
	RDORunError         runError;
	CString             strError;
	RDODebug            debug;
	RDODebug            debugNext;
	int                 debugX;
	int                 debugY;
	RDOEditorTabItem    debugTabItem;
	HHOOK               hook;

	UINT timerRDO;

	void stopTimer( UINT& timer );

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

	void dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar );

	int getIntegerAfterWord( const CString& str, const CString& word ) const;

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnClose();
	afx_msg void OnSetFocus( CWnd *pOldWnd );

	afx_msg void OnProjectNew()           { rdoEditorProject.newProject();    };
	afx_msg void OnProjectLoad();
	afx_msg void OnProjectReopen( UINT nID );
	afx_msg void OnProjectSave()          { rdoEditorProject.saveProject();   };
	afx_msg void OnProjectSaveAs()        { rdoEditorProject.saveAsProject(); };
	afx_msg void OnProjectStart();
	afx_msg void OnProjectStop();
	afx_msg void OnProjectPath();
	afx_msg void OnProjectLoadPMV()       { rdoEditorProject.loadPMV();       };
	afx_msg void OnProjectLoadTRC()       { rdoEditorProject.loadTRC();       };

	afx_msg void OnTimer( UINT nIDEvent );

	afx_msg void OnUpdateProjectSave( CCmdUI *pCmdUI )        { pCmdUI->Enable( rdoEditorProject.isModify() );                                };
	afx_msg void OnUpdateProjectStart( CCmdUI *pCmdUI )       { pCmdUI->Enable( rdoEditorProject.canRun() && !rdoEditorProject.isRunning() ); };
	afx_msg void OnUpdateProjectStop( CCmdUI *pCmdUI )        { pCmdUI->Enable( rdoEditorProject.isRunning() );                               };
	afx_msg void OnUpdateProjectLoadPMVTRC( CCmdUI *pCmdUI )  { pCmdUI->Enable( rdoEditorProject.canUpdateResults() );                        };

	afx_msg void OnOptions();

	afx_msg void OnHelpContents();

	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInfoStatusBar( CCmdUI *pCmdUI );

	DECLARE_MESSAGE_MAP()

protected: 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

public:
	RDOEditorChildView childView;

	RDOEditorMainFrame();
	virtual ~RDOEditorMainFrame();

	void insertReopenItem( const CString& item );

	void showRunError();
	void gotoRunError() const;

	RDODebug getDebug() const;
	void setDebug( const RDODebug value );
	bool canHook() const;
};
*/
#endif // RDOEDITORMAINFRM_H
