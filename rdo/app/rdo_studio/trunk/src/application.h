/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_APPLICATION_H_
#define _RDO_STUDIO_APPLICATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <fstream>
#include <QProcess>
#include <QTimer>
#include <QApplication>
#include <QMainWindow>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "simulator/service/src/rdosimwin.h"
#include "app/rdo_studio/src/main_window_base.h"
#include "app/rdo_studio/src/editor/model_edit_style.h"
#include "app/rdo_studio/src/plugins/plugin_loader.h"
#include "app/rdo_studio/src/shared_object_service.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class MainWindow;
class ThreadStudio;
class ThreadStudioGUI;

namespace rdo { namespace gui { namespace tracer {
class Tracer;
}}}

class Application
	: public QApplication
	, public rdo::gui::ISharedObjectService
{
public:
	Application(int& argc, char** argv);
	virtual ~Application();

	virtual RDOKernel*                getKernel   () const;
	virtual rdo::gui::tracer::Tracer* getTracer   () const;
	virtual MainWindow*               getMainWndUI() const;

	PTR(QMainWindow)     getMainWnd  ();
	PTR(MainWindowBase)  getIMainWnd ();
	PTR(MainWindowBase)  getStyle    ();

	//! см. описание RDOKernelGUI
	//! Главная треда самого приложения, т.е. кернет для win32-gui, но не кернел системы
	PTR(RDOThread)          m_pStudioGUI;

	REF(std::ofstream)      log();

	void           broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam = NULL);

	bool           getFileAssociationSetup() const;
	void           setFileAssociationSetup(bool value);

	bool           getFileAssociationCheckInFuture() const;
	void           setFileAssociationCheckInFuture(bool value);

	bool           getOpenLastProject() const;
	void           setOpenLastProject(bool value);

	CREF(QString)  getLastProjectName() const;
	void           setLastProjectName(CREF(QString) projectName);

	bool           getShowCaptionFullName() const;
	void           setShowCaptionFullName(bool value);

	void           autoCloseByModel ();

	QString        getFullHelpFileName (CREF(QString) helpFileName = "RAO-help.qhc") const;
	QString        chkHelpExist        (CREF(QString) helpFileName) const;
	void           chkAndRunQtAssistant();
	PTR(QProcess)  runQtAssistant      () const;
	void           callQtAssistant     (CREF(QByteArray) ba);

	CREF(rdo::gui::editor::LPModelStyle) getModelStyle() const;
	rdo::plugin::Loader&                 getPluginLoader();

private:
#ifdef RDO_MT
	// Используется для рассылки широковещательных уведомлений из приложения.
	// При этом, не происходит остановки работы самого приложения, и имеется возможность
	// обрабатывать новые присылаемые приложению сообщения.
	PTR(ThreadStudio) m_pStudioMT;
#endif

	std::ofstream                          m_log;
	bool                                   m_fileAssociationSetup;
	bool                                   m_fileAssociationCheckInFuture;
	bool                                   m_openLastProject;
	QString                                m_lastProjectName;
	bool                                   m_showCaptionFullName;
	bool                                   m_autoExitByModel;
	bool                                   m_dontCloseIfError;
	rdo::simulation::report::RDOExitCode   m_exitCode;
	QProcess*                              m_pAssistant;
	PTR(MainWindow)                        m_pMainFrame;
	rdo::gui::editor::LPModelStyle         m_pModelStyle;
	QTimer                                 m_idleTimer;

	rdo::plugin::Loader                    m_pluginLoader;

	void setupFileAssociation();
#ifdef Q_OS_WIN
	void convertSettings() const;
#endif

private slots:
	void onIdle();
};

// --------------------------------------------------------------------------------
extern Application* g_pApp;

#endif // _RDO_STUDIO_APPLICATION_H_
