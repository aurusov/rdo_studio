#ifndef RDOSTUDIOAPP_H
#define RDOSTUDIOAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <rdolink.h>
#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioApp
// ----------------------------------------------------------------------------
class RDOStudioMainFrame;

class RDOStudioApp: public CWinApp
{
friend class RDOStudioModel;
friend class RDOStudioCommandLineInfo;

private:
	bool initInstance;

	CMultiDocTemplate* editDocTemplate;

	std::vector< std::string > reopenList;
	void updateReopenSubMenu() const;
	void loadReopen();
	void saveReopen() const;

	bool fileAssociationSetup;
	bool fileAssociationCheckInFuture;
	bool openLastProject;
	std::string lastProjectName;

	void setupFileAssociation();

	bool autoRun;
	bool autoExit;
	std::string openModelName;

public:
	RDOStudioApp();

	RDOStudioMainFrame* mainFrame;

	void insertReopenItem( const std::string& item );

	bool getFileAssociationSetup() const              { return fileAssociationSetup;         }
	void setFileAssociationSetup( const bool value );

	bool getFileAssociationCheckInFuture() const      { return fileAssociationCheckInFuture; }
	void setFileAssociationCheckInFuture( const bool value );

	bool getOpenLastProject() const                   { return openLastProject;              }
	void setOpenLastProject( const bool value );

	std::string getLastProjectName() const            { return lastProjectName;              }
	void setLastProjectName( const std::string& projectName );

	void autoClose();

	static std::string getFullFileName();
	static std::string extractFilePath( const std::string& fileName );
	static bool isFileExists( const std::string& fileName );
	static std::string getFullHelpFileName( std::string str = "RAO-studio.chm" );
	static bool shortToLongPath( const std::string& shortPath, std::string& longPath );

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
	afx_msg void OnModelBuild();
	afx_msg void OnModelRun();
	afx_msg void OnModelStop();
	afx_msg void OnUpdateModelBuild(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelStop(CCmdUI* pCmdUI);
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	afx_msg void OnProjectReopen( UINT nID );
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- RDOAboutDlg
// ----------------------------------------------------------------------------
class RDOAboutDlg: public CDialog
{
public:
	RDOAboutDlg();
	virtual ~RDOAboutDlg();

protected:
	//{{AFX_DATA(RDOAboutDlg)
	enum { IDD = IDD_ABOUT };
	RDOLink	m_web;
	RDOLink	m_email;
	CString	m_caption;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOAboutDlg)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOAboutDlg)
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
extern RDOStudioApp studioApp;

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOAPP_H
