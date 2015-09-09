// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QMessageBox>
#include <QBitmap>
#include <QDir>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "utils/src/common/model_objects.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/plugins/game5/src/board.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_model_generator.h"
// --------------------------------------------------------------------------------

namespace
{
    const QString PLUGIN_GUID           = "{5315750C-964B-4ed2-96FE-21FC6226942B}";
    const QString PLUGIN_ACTION_NAME    = "action" + PLUGIN_GUID;
    const QString PLUGIN_MENU_NAME      = "createdMenu";
    const QString PLUGIN_MENU_TEXT      = "Плагины";
    const QString PLUGIN_TOOLBAR_NAME   = "toolbar" + PLUGIN_GUID;
    const QString RDO_MENUBAR_NAME      = "menubar";
    const size_t  RDOFileType_ENUM_SIZE = 13;

    void backUpModel(rdo::gui::model::Model* pModel)
    {
        boost::filesystem::path modelFolder(pModel->getFullName().toStdWString());
        modelFolder.remove_leaf();
        const QString backupFolderName = "backup" + QDateTime::currentDateTime().toString("_yyyy-MM-dd_HH.mm.ss");
        const boost::filesystem::path backupFolder = modelFolder / backupFolderName.toStdString();
        try
        {
            if (boost::filesystem::create_directory(backupFolder))
            {
                for (size_t i = 0; i < RDOFileType_ENUM_SIZE; i++)
                {
                    std::string fileExtension = rdo::getFileTypeString(rdo::FileType(i));
                    boost::algorithm::to_lower(fileExtension);
                    const std::string fileName = pModel->getName().toStdString() + "." + fileExtension;
                    const boost::filesystem::path filePath = modelFolder / fileName;
                    const boost::filesystem::path backupFilePath = backupFolder / fileName;
                    if (boost::filesystem::exists(filePath))
                    {
                        boost::filesystem::copy(filePath, backupFilePath);
                    }
                }
            }
        }
        catch (const boost::filesystem::filesystem_error& error)
        {
            const std::string system_what = error.boost::system::system_error::what();
            const std::string system_what_utf =
#if defined(OST_WINDOWS)
                rdo::locale::convertFromCLocale(system_what, rdo::locale::get().system());
#elif defined(OST_LINUX)
                system_what;
#endif
            QString what = error.what();
            what.replace(QString::fromStdString(system_what), QString::fromStdString(system_what_utf));
            QMessageBox::critical(g_pApp->getMainWnd(),
                "RAO-Studio",
                QString("Ошибка создания резервной копии модели: ") + what);
        }
    }

    QMenu* findPluginMenu(QWidget* pParent)
    {
        QMenu* pluginMenu = pParent->findChild<QMenu*>(PLUGIN_MENU_NAME);
        if (!pluginMenu)
        {
            QMenuBar* menuBar = pParent->findChild<QMenuBar*>(RDO_MENUBAR_NAME);
            ASSERT(menuBar);
            for (auto action: menuBar->actions())
            {
                if (action->text() == PLUGIN_MENU_TEXT)
                {
                    pluginMenu = action->menu();
                    break;
                }
            }
        }
        return pluginMenu;
    }
} // end anonymous namespace

QString PluginGame5::getPluginName() const
{
    return "game5_debug";
}

QString PluginGame5::getAuthor() const
{
    return "Chernov";
}

QString PluginGame5::getVersion() const
{
    return "0.2.1";
}

QUuid PluginGame5::getGUID()  const
{
    QUuid pluginGUID(PLUGIN_GUID);
    return pluginGUID;
}

void PluginGame5::pluginStartAction(QWidget* pParent, const std::string& commandLine)
{
    if (!g_pApp)
    {
        g_pApp    = static_cast<Application*>(qApp);
        g_pModel  = g_pApp->getMainWndUI()->getModel();
        g_pTracer = g_pApp->getTracer();
        kernel    = g_pApp->getKernel();
    }

    if (!commandLine.empty())
    {
        executeCommand(commandLine);
    }

    QMenu* pluginMenu = findPluginMenu(pParent);
    if (!pluginMenu)
    {
        pluginMenu = new QMenu(PLUGIN_MENU_TEXT, pParent);
        pluginMenu->setObjectName(PLUGIN_MENU_NAME);
    }
    QMenuBar* menuBar = pParent->findChild<QMenuBar*>(RDO_MENUBAR_NAME);
    ASSERT(menuBar);
    menuBar->addMenu(pluginMenu);

    QAction* action = new QAction(getPluginName() + " ver " + getVersion(), pluginMenu);
    action->setObjectName(PLUGIN_ACTION_NAME);
    connect(action, &QAction::triggered, this, &PluginGame5::pluginActivation);
    pluginMenu->addAction(action);

    m_generateSituationDlg = NULL;
    m_graphDlg = NULL;
}

void PluginGame5::pluginStopAction(QWidget* pParent)
{
    QMenu* pluginMenu = findPluginMenu(pParent);
    ASSERT(pluginMenu);

    QAction* pluignAction = pluginMenu->findChild<QAction*>(PLUGIN_ACTION_NAME);
    ASSERT(pluignAction);
    delete pluignAction;

    if (pluginMenu->isEmpty())
        delete pluginMenu;

    if (m_generateSituationDlg)
        delete m_generateSituationDlg;

    if (m_graphDlg)
        delete m_graphDlg;

    QToolBar* pluginGame5ToolBar = pParent->findChild<QToolBar*>(PLUGIN_TOOLBAR_NAME);
    if (pluginGame5ToolBar)
        delete pluginGame5ToolBar;

    if (g_pApp && g_pApp->getMainWndUI())
    {
        rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();
        if (pModel)
        {
            disconnect(pModel, &rdo::gui::model::Model::stopped,
                       this  , &PluginGame5::reemitGraphDlgAction);
            disconnect(pModel, &rdo::gui::model::Model::actionUpdated,
                       this  , &PluginGame5::enablePluginActions);
        }
    }
}

void PluginGame5::executeCommand(const std::string& commandLine)
{
    if (!g_pApp || !g_pApp->getMainWndUI() || !g_pApp->getMainWndUI()->getModel())
        return;

    rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();

    if (!pModel->getTab())
        return;

    QStringList positionList = QString::fromStdString(commandLine).split(' ', QString::SkipEmptyParts);
    std::vector<unsigned int> newState;
    for (const auto& position: positionList)
    {
        newState.push_back(position.toInt());
    }
    Board board;
    board.setTilesPositon(newState);

    for (int i = 0; i < pModel->getTab()->tabBar()->count(); i++)
    {
        pModel->getTab()->getItemEdit(i)->clearAll();
    }
    pModel->getTab()->getItemEdit(rdo::FileType::RTP)->appendText(PluginGame5ModelGenerator::modelRTP(board));
    pModel->getTab()->getItemEdit(rdo::FileType::RSS)->appendText(PluginGame5ModelGenerator::modelRSS(board));
    pModel->getTab()->getItemEdit(rdo::FileType::PAT)->appendText(PluginGame5ModelGenerator::modelPAT());
    pModel->getTab()->getItemEdit(rdo::FileType::DPT)->appendText(PluginGame5ModelGenerator::modelDPT(board));
    pModel->getTab()->getItemEdit(rdo::FileType::FUN)->appendText(PluginGame5ModelGenerator::modelFUN(board));

    pModel->saveModel();
    g_pApp->quit();
}

void PluginGame5::pluginActivation()
{
    QWidget* pParent = qobject_cast<QWidget*>(sender()-> //action
                                              parent()-> //QMenu
                                              parent()); //QMainWindow
    MainWindow* pMainWindow = (MainWindow*)pParent;
    if (pMainWindow->getModel()->getTab())
    {
        bool canStart = true;
        if (pMainWindow->getModel()->isModify())
        {
            const int ret = QMessageBox::question(g_pApp->getMainWnd(),
                                                  "RAO-Studio",
                                                  "Модель будет перезаписана!\nНесохраненные изменения будут потеряны\nХотите сохранить изменения?",
                                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                  QMessageBox::Save
            );
            switch (ret)
            {
                case QMessageBox::Save    : pMainWindow->actFileSaveAll->trigger(); break;
                case QMessageBox::Discard : break;
                default: canStart = false; break;
            }
        }
        else
        {
            if (!pMainWindow->getModel()->isEmpty())
            {
                const int ret = QMessageBox::warning(g_pApp->getMainWnd(),
                                                     "RAO-Studio",
                                                     "Модель будет перезаписана!",
                                                     QMessageBox::Ok | QMessageBox::Cancel,
                                                     QMessageBox::Ok);
                if (ret == QMessageBox::Cancel)
                {
                    canStart = false;
                }
            }
        }
        if (canStart)
        {
            backUpModel(pMainWindow->getModel());
            initDialogs(pParent);
            initToolBar(pMainWindow);
            QAction* senderAction = qobject_cast<QAction*>(sender());
            senderAction->setEnabled(false);
        }
    }
    else
    {
        pMainWindow->actFileNew->trigger();
    }
}

void PluginGame5::initToolBar(MainWindow* pParent) const
{
    QToolBar* pluginToolBar = new QToolBar(pParent);
    pluginToolBar->setObjectName(PLUGIN_TOOLBAR_NAME);

    QAction* graphDlgAction             = new QAction(pParent);
    QAction* generateSituationDlgAction = new QAction(pParent);

    generateSituationDlgAction->setText("Расставить фишки");
    QPixmap pluginGenerateDlgActionPixmap(":/res/images/gen_sit_dialog.png");
    generateSituationDlgAction->setIcon(QIcon(pluginGenerateDlgActionPixmap));

    graphDlgAction->setText("Построить граф");
    QPixmap graphDlgPixmapD(":/res/images/graph_dialog_d.png");
    QPixmap graphDlgPixmap (":/res/images/graph_dialog.png");
    QIcon graphDlgIcon(graphDlgPixmap);
    graphDlgIcon.addPixmap(graphDlgPixmapD, QIcon::Disabled);
    graphDlgAction->setIcon(graphDlgIcon);
    graphDlgAction->setEnabled(false);

    pluginToolBar->addAction(generateSituationDlgAction);
    pluginToolBar->addAction(graphDlgAction);
    pluginToolBar->setIconSize(QSize(16, 15));

    pParent->addToolBar(Qt::TopToolBarArea, pluginToolBar);

    rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();

    connect(generateSituationDlgAction, &QAction::triggered,
            m_generateSituationDlg    , &PluginGame5GenerateSituationDialog::onPluginAction);
    connect(pModel, &rdo::gui::model::Model::stopped,
            this    , &PluginGame5::reemitGraphDlgAction);
    connect(graphDlgAction, &QAction::triggered,
            this          , &PluginGame5::reemitGraphDlgAction);
    connect(this      , &PluginGame5::onGraphDlgAction,
            m_graphDlg, &PluginGame5GraphDialog::onPluginAction);
    connect(this          , &PluginGame5::setGraphDlgActionEnabled,
            graphDlgAction, &QAction::setEnabled);
    connect(this, &PluginGame5::setGenerateSituationDlgActionEnabled,
            generateSituationDlgAction, &QAction::setEnabled);
    connect(pModel, &rdo::gui::model::Model::actionUpdated,
            this    , &PluginGame5::enablePluginActions);
}

void PluginGame5::initDialogs(QWidget* pParent)
{
    m_generateSituationDlg = new PluginGame5GenerateSituationDialog(pParent);
    m_graphDlg  = new PluginGame5GraphDialog(pParent);
}

void PluginGame5::reemitGraphDlgAction()
{
    emit onGraphDlgAction(m_generateSituationDlg->getBoardState());
}

void PluginGame5::enablePluginActions()
{
    if (!g_pApp || !g_pApp->getMainWndUI() || !g_pApp->getMainWndUI()->getModel())
        return;

    rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();

    emit setGraphDlgActionEnabled(pModel->canRun());
    emit setGenerateSituationDlgActionEnabled(pModel->canRun());
}
