#ifndef RDOSTUDIOAPP_H
#define RDOSTUDIOAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioApp
// ----------------------------------------------------------------------------
class RDOStudioMainFrame;

class RDOStudioApp: public CWinApp
{
friend class RDOStudioModel;

private:
	bool initInstance;

	CMultiDocTemplate* editDocTemplate;

	std::vector< std::string > reopenList;
	void updateReopenSubMenu() const;
	void loadReopen();
	void saveReopen() const;

public:
	RDOStudioApp();

	RDOStudioMainFrame* mainFrame;

	void insertReopenItem( const std::string& item );

	//{{AFX_VIRTUAL(RDOStudioApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioApp)
	afx_msg void OnFileNew();
	afx_msg void OnWindowNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnRdoRun();
	afx_msg void OnRdoStop();
	afx_msg void OnUpdateRdoRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRdoStop(CCmdUI* pCmdUI);
	afx_msg void OnChartStarttrace();
	afx_msg void OnUpdateChartStarttrace(CCmdUI* pCmdUI);
	afx_msg void OnChartStoptrace();
	afx_msg void OnUpdateChartStoptrace(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnProjectReopen( UINT nID );
	afx_msg void OnAddNewFrame( WPARAM wParam, LPARAM lParam );
	afx_msg void OnUpdateAllFrame( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
extern RDOStudioApp studioApp;

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOAPP_H
