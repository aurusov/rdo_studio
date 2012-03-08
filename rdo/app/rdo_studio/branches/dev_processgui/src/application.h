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

#define PROCGUI_ENABLE

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class RDOStudioMainFrame;
class RDOThreadStudio;
class RDOThreadStudioGUI;
class RDOStudioPlugin;

namespace rdoTracer
{
class RDOTracer;
}

class RDOStudioApp: public CWinApp
{
friend class RDOStudioCommandLineInfo;
public:
	RDOStudioApp();
	virtual ~RDOStudioApp();

	PTR(RDOStudioMainFrame) m_pMainFrame;
	//! см. описание RDOKernelGUI
	//! Главная треда самого приложения, т.е. кернет для win32-gui, но не кернел системы
	PTR(RDOThread)          m_pStudioGUI;

	CREF(RPMethodManager) getMethodManager() const;
	REF(std::ofstream)    log             ();

	void           broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam = NULL);
	void           insertReopenItem(CREF(tstring) item);

	rbool          getFileAssociationSetup() const;
	void           setFileAssociationSetup(rbool value);

	rbool          getFileAssociationCheckInFuture() const;
	void           setFileAssociationCheckInFuture(rbool value);

	rbool          getOpenLastProject() const;
	void           setOpenLastProject(rbool value);

	CREF(tstring)  getLastProjectName() const;
	void           setLastProjectName(CREF(tstring) projectName);

	rbool          getShowCaptionFullName() const;
	void           setShowCaptionFullName(rbool value);

	void           autoCloseByModel ();
	void           autoCloseByPlugin(PTR(RDOStudioPlugin) plugin);
	rbool          isPluginAutoStart(PTR(RDOStudioPlugin) plugin) const;

	static tstring getFullExtName();
	static tstring getFullHelpFileName(tstring str = "RAO-studio.chm");
	static rbool   shortToLongPath    (CREF(tstring) shortPath, REF(tstring) longPath);

private:
#ifdef RDO_MT
	// Используется для рассылки широковещательных уведомлений из приложения.
	// При этом, не происходит остановки работы самого приложения, и имеется возможность
	// обрабатывать новые присылаемые приложению сообщения.
	PTR(RDOThreadStudio) m_pStudioMT;
#endif

	typedef  std::vector<tstring>  ReopenList;
	typedef  std::list<tstring>    PluginNameList;

	RPMethodManager            m_methodManager;
	std::ofstream              m_log;
	ReopenList                 m_reopenList;
	PTR(CMultiDocTemplate)     m_editDocTemplate;
	rbool                      m_fileAssociationSetup;
	rbool                      m_fileAssociationCheckInFuture;
	rbool                      m_openLastProject;
	tstring                    m_lastProjectName;
	ULONG_PTR                  m_gdiplusToken;
	rbool                      m_showCaptionFullName;
	rbool                      m_autoRun;
	rbool                      m_autoExitByModel;
	rbool                      m_dontCloseIfError;
	rdoSimulator::RDOExitCode  m_exitCode;
	tstring                    m_openModelName;
	PluginNameList             m_pluginStartNameList;
	PluginNameList             m_pluginExitNameList;

	void setupFileAssociation();
	void updateReopenSubMenu () const;
	void loadReopen          ();
	void saveReopen          () const;

private:
	virtual BOOL InitInstance        ();
	virtual int  ExitInstance        ();
	virtual BOOL PreTranslateMessage (PTR(MSG) pMsg);
	virtual BOOL OnIdle              (LONG lCount);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

	afx_msg void OnFileNew          ();
	afx_msg void OnWindowNew        ();
	afx_msg void OnFileOpen         ();
	afx_msg void OnFileSave         ();
	afx_msg void OnFileSaveAll      ();
	afx_msg void OnFileClose        ();
	afx_msg void OnFileSaveAs       ();
	afx_msg void OnModelBuild       ();
	afx_msg void OnModelRun         ();
	afx_msg void OnModelStop        ();
	afx_msg void OnAppAbout         ();
	afx_msg void OnProjectReopen    (UINT nID);
	afx_msg void OnUpdateFileSave   (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileSaveAll(PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileClose  (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileSaveAs (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelBuild (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelRun   (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelStop  (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileNew    (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileOpen   (PTR(CCmdUI) pCmdUI);
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

private:
	enum { IDD = IDD_ABOUT };

	RDOLink	 m_web;
	RDOLink	 m_email;
	CString	 m_caption;
	CString	 m_developer;

	virtual void DoDataExchange(PTR(CDataExchange) pDX);

	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb  ();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
extern RDOStudioApp studioApp;

#endif // _RDO_STUDIO_MFC_APPLICATION_H_
