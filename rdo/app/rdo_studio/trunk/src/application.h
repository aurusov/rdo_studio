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
#include "utils/warning_disable.h"
#include <fstream>
#include <QProcess>
#include <QTimer>
#include <QApplication>
#include <QMainWindow>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/main_windows_base.h"
#include "app/rdo_studio/src/editor/model_edit_style.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class MainWindow;
class RDOThreadStudio;
class RDOThreadStudioGUI;

namespace rdo { namespace gui { namespace tracer {
class Tracer;
}}}

class RDOStudioApp: public QApplication
{
public:
	RDOStudioApp(int& argc, char** argv);
	virtual ~RDOStudioApp();

	PTR(MainWindow)      getMainWndUI();
	PTR(QMainWindow)     getMainWnd  ();
	PTR(MainWindowBase)  getIMainWnd ();
	PTR(MainWindowBase)  getStyle    ();

	//! см. описание RDOKernelGUI
	//! Главная треда самого приложения, т.е. кернет для win32-gui, но не кернел системы
	PTR(RDOThread)          m_pStudioGUI;

	REF(std::ofstream)      log();

	void           broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam = NULL);

	rbool          getFileAssociationSetup() const;
	void           setFileAssociationSetup(rbool value);

	rbool          getFileAssociationCheckInFuture() const;
	void           setFileAssociationCheckInFuture(rbool value);

	rbool          getOpenLastProject() const;
	void           setOpenLastProject(rbool value);

	CREF(QString)  getLastProjectName() const;
	void           setLastProjectName(CREF(QString) projectName);

	rbool          getShowCaptionFullName() const;
	void           setShowCaptionFullName(rbool value);

	void           autoCloseByModel ();

	QString        getFullHelpFileName (CREF(QString) helpFileName = "RAO-help.qhc") const;
	QString        chkHelpExist        (CREF(QString) helpFileName) const;
	void           chkAndRunQtAssistant();
	PTR(QProcess)  runQtAssistant      () const;
	void           callQtAssistant     (CREF(QByteArray) ba);

	CREF(rdo::gui::editor::LPModelStyle) getModelStyle() const;

private:
#ifdef RDO_MT
	// Используется для рассылки широковещательных уведомлений из приложения.
	// При этом, не происходит остановки работы самого приложения, и имеется возможность
	// обрабатывать новые присылаемые приложению сообщения.
	PTR(RDOThreadStudio) m_pStudioMT;
#endif

	std::ofstream                          m_log;
	rbool                                  m_fileAssociationSetup;
	rbool                                  m_fileAssociationCheckInFuture;
	rbool                                  m_openLastProject;
	QString                                m_lastProjectName;
	rbool                                  m_showCaptionFullName;
	rbool                                  m_autoExitByModel;
	rbool                                  m_dontCloseIfError;
	rdo::simulation::report::RDOExitCode   m_exitCode;
	QProcess*                              m_pAssistant;
	PTR(MainWindow)                        m_pMainFrame;
	rdo::gui::editor::LPModelStyle         m_pModelStyle;
	QTimer                                 m_idleTimer;

	void setupFileAssociation();

#ifdef Q_OS_WIN
	void convertSettings() const;
#endif

private slots:
	void onIdle();
};

// --------------------------------------------------------------------------------
extern RDOStudioApp* g_pApp;

#endif // _RDO_STUDIO_APPLICATION_H_
