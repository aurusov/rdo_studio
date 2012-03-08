/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_APPLICATION_H_
#define _RDO_STUDIO_MFC_APPLICATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "ui/mfc_ctrls/rdolink.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_method_manager.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

//#define PROCGUI_ENABLE

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class RDOStudioMainFrame;
class RDOThreadStudio;
class RDOThreadStudioGUI;
class RDOStudioPlugin;

namespace rdoTracer {
class RDOTracer;
}

class RDOStudioApp: public CWinApp
{
friend class RDOStudioCommandLineInfo;
public:
	// см. описание RDOKernelGUI.
	// Главная треда самого приложения, т.е. кернет для win32-gui, но не кернел системы.
	RDOThread* studioGUI;
private:
#ifdef RDO_MT
	// Используется для рассылки широковещательных уведомлений из приложения.
	// При этом, не происходит остановки работы самого приложения, и имеется возможность
	// обрабатывать новые присылаемые приложению сообщения.
	RDOThreadStudio* studioMT;
#endif

	CMultiDocTemplate* editDocTemplate;

	std::vector< tstring > reopenList;
	void updateReopenSubMenu() const;
	void loadReopen();
	void saveReopen() const;

	rbool       fileAssociationSetup;
	rbool       fileAssociationCheckInFuture;
	rbool       openLastProject;
	tstring     lastProjectName;
	ULONG_PTR   m_gdiplusToken;

	rbool showCaptionFullName;

	void setupFileAssociation();

	rbool autoRun;
	rbool autoExitByModel;
	rbool dontCloseIfError;
	rdoSimulator::RDOExitCode exitCode;
	tstring openModelName;

	std::list< tstring > plugin_start_name;
	std::list< tstring > plugin_exit_name;

public:
	RDOStudioApp();
	RDOStudioMainFrame* mainFrame;
	RPMethodManager methods;
	std::ofstream   log;

	const RPMethodManager& getMethodManager() const { return methods;               }

	void broadcastMessage( RDOThread::RDOTreadMessage message, void* param = NULL );

	void insertReopenItem( CREF(tstring) item );

	rbool getFileAssociationSetup() const              { return fileAssociationSetup;         }
	void setFileAssociationSetup( const rbool value );

	rbool getFileAssociationCheckInFuture() const      { return fileAssociationCheckInFuture; }
	void setFileAssociationCheckInFuture( const rbool value );

	rbool getOpenLastProject() const                   { return openLastProject;              }
	void setOpenLastProject( const rbool value );

	tstring getLastProjectName() const            { return lastProjectName;              }
	void setLastProjectName( CREF(tstring) projectName );

	rbool getShowCaptionFullName() const               { return showCaptionFullName;          }
	void setShowCaptionFullName( const rbool value );

	void autoCloseByModel();
	void autoCloseByPlugin( RDOStudioPlugin* plugin );
	rbool isPluginAutoStart( RDOStudioPlugin* plugin ) const;

	static tstring getFullFileName();
	static tstring getFullHelpFileName( tstring str = "RAO-studio.chm" );
	static rbool shortToLongPath( CREF(tstring) shortPath, tstring& longPath );

private:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

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
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnProjectReopen( UINT nID );
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RDOAboutDlg
// --------------------------------------------------------------------------------
class RDOAboutDlg: public CDialog
{
public:
	RDOAboutDlg();
	virtual ~RDOAboutDlg();

protected:
	enum { IDD = IDD_ABOUT };
	RDOLink	m_web;
	RDOLink	m_email;
	CString	m_caption;
	CString	m_developer;

private:
	virtual void DoDataExchange( CDataExchange* pDX );

protected:
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
extern RDOStudioApp studioApp;

#endif // _RDO_STUDIO_MFC_APPLICATION_H_
