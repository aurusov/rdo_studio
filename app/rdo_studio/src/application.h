#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QProcess>
#include <QTimer>
#include <QApplication>
#include <QMainWindow>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "simulator/service/src/simulator.h"
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

    QMainWindow*     getMainWnd ();
    MainWindowBase*  getIMainWnd();
    MainWindowBase*  getStyle   ();

    // см. описание RDOKernelGUI
    // Главная треда самого приложения, т.е. кернет для win32-gui, но не кернел системы
    RDOThread* m_pStudioGUI;

    void           broadcastMessage(RDOThread::Message message, void* pParam = NULL);

    bool           getFileAssociationSetup() const;
    void           setFileAssociationSetup(bool value);

    bool           getFileAssociationCheckInFuture() const;
    void           setFileAssociationCheckInFuture(bool value);

    bool           getOpenLastProject() const;
    void           setOpenLastProject(bool value);

    const QString& getLastProjectName() const;
    void           setLastProjectName(const QString& projectName);

    bool           getShowCaptionFullName() const;
    void           setShowCaptionFullName(bool value);

    void           autoCloseByModel ();

    QString        getFullHelpFileName (const QString& helpFileName = "RAO-help.qhc") const;
    QString        chkHelpExist        (const QString& helpFileName) const;
    void           chkAndRunQtAssistant();
    QProcess*      runQtAssistant      () const;
    void           callQtAssistant     (const QByteArray& ba);

    const rdo::gui::editor::LPModelStyle& getModelStyle() const;
    rdo::plugin::Loader&                  getPluginLoader();

private:
    bool                                   m_fileAssociationSetup;
    bool                                   m_fileAssociationCheckInFuture;
    bool                                   m_openLastProject;
    QString                                m_lastProjectName;
    bool                                   m_showCaptionFullName;
    bool                                   m_autoExitByModel;
    bool                                   m_dontCloseIfError;
    rdo::simulation::report::ExitCode   m_exitCode;
    QProcess*                              m_pAssistant;
    MainWindow*                            m_pMainFrame;
    rdo::gui::editor::LPModelStyle         m_pModelStyle;
    QTimer                                 m_initTimer;
    QTimer                                 m_idleTimer;
    rdo::plugin::Loader                    m_pluginLoader;

    void onInit(int argc, char** argv);

    void setupFileAssociation();
#ifdef Q_OS_WIN
    void convertSettings() const;
#endif

private slots:
    void onIdle();
};

// --------------------------------------------------------------------------------
extern Application* g_pApp;
