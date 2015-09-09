// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/bind.hpp>
#include <boost/range/algorithm/find.hpp>
#include <QMdiSubWindow>
#include <QProcess>
#include <QSettings>
#include <QTextCodec>
#include <vector>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/dialog/about_dialog.h"
#include "app/rdo_studio/src/view_preferences.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/editor/lexer/lexer_find.h"
#include "app/rdo_studio/src/editor/lexer/lexer_model.h"
#include "thirdparty/scintilla/include/Scintilla.h"
#include "thirdparty/scintilla/src/Catalogue.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- MainWindow::InsertMenuData
// --------------------------------------------------------------------------------
MainWindow::InsertMenuData::InsertMenuData(QObject* pParent, const QString& text, const Position& position)
    : QObject   (pParent )
    , m_text    (text    )
    , m_position(position)
{}

const QString& MainWindow::InsertMenuData::text() const
{
    return m_text;
}

const MainWindow::InsertMenuData::Position& MainWindow::InsertMenuData::position() const
{
    return m_position;
}

// --------------------------------------------------------------------------------
// -------------------- MainWindow::SubWindowToAction
// --------------------------------------------------------------------------------
MainWindow::SubWindowToAction::SubWindowToAction(MainWindow* pMainWindow)
    : m_pMainWindow(pMainWindow)
    , m_pSeparator(NULL)
{
    ASSERT(m_pMainWindow);
    ASSERT(m_pMainWindow->menuWindow);
    m_pActionGroup = new QActionGroup(m_pMainWindow->menuWindow);
    m_pActionGroup->setExclusive(true);
}

void MainWindow::SubWindowToAction::onSubWindowActivated(QMdiSubWindow* pSubWindow)
{
    if (pSubWindow)
    {
        Map::const_iterator it = m_map.find(pSubWindow);
        if (it == m_map.end())
            it = addNewSubWindow(pSubWindow);

        ASSERT(it != m_map.end());
        it->second->setChecked(true);
    }
    updateList();
}

void MainWindow::SubWindowToAction::onTitleChanged(QMdiSubWindow* pSubWindow)
{
    ASSERT(pSubWindow);
    Map::const_iterator it = m_map.find(pSubWindow);
    ASSERT(it != m_map.end());
    it->second->setText(pSubWindow->windowTitle());
}

MainWindow::SubWindowToAction::Map::const_iterator
MainWindow::SubWindowToAction::addNewSubWindow(QMdiSubWindow* pSubWindow)
{
    ASSERT(pSubWindow);

    QList<QMdiSubWindow*> windowList = m_pMainWindow->mdiArea->subWindowList();

    if (windowList.count() == 1 && m_pSeparator == NULL)
        addFirstSubWindow();

    pSubWindow->installEventFilter(m_pMainWindow);

    QAction* pAction = m_pMainWindow->menuWindow->addAction(pSubWindow->windowTitle());
    QObject::connect(pAction, &QAction::triggered, boost::function<void()>(boost::bind(&QMdiArea::setActiveSubWindow, m_pMainWindow->mdiArea, pSubWindow)));

    std::pair<Map::const_iterator, bool> result = m_map.insert(Map::value_type(pSubWindow, pAction));
    ASSERT(result.second);

    m_pActionGroup->addAction(pAction);
    pAction->setCheckable(true);

    return result.first;
}

void MainWindow::SubWindowToAction::updateList()
{
    QList<QMdiSubWindow*> windowList = m_pMainWindow->mdiArea->subWindowList();

    for (SubWindowToAction::Map::const_iterator it = m_map.begin(); it != m_map.end();)
    {
        if (!windowList.contains(it->first))
        {
            QObject::disconnect(it->second, &QAction::triggered, NULL, NULL);
            m_pMainWindow->menuWindow->removeAction(it->second);
            m_pActionGroup->removeAction(it->second);
            m_map.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    if (windowList.empty() && m_pSeparator)
        removeLastSubWindow();
}

void MainWindow::SubWindowToAction::addFirstSubWindow()
{
    ASSERT(m_pSeparator == NULL);
    m_pSeparator = m_pMainWindow->menuWindow->addSeparator();
}

void MainWindow::SubWindowToAction::removeLastSubWindow()
{
    ASSERT(m_pSeparator);
    m_pMainWindow->menuWindow->removeAction(m_pSeparator);
    m_pSeparator = NULL;
}

// --------------------------------------------------------------------------------
// -------------------- MainWindow
// --------------------------------------------------------------------------------
MainWindow::MainWindow()
    : m_pModel(NULL)
    , m_updateTimerID(0)
    , m_pInsertMenuSignalMapper(NULL)
{
    setupUi(this);
    installEventFilter(this);
    m_subWindowToAction.reset(new SubWindowToAction(this));
    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);

    createStatusBar();
    createToolBar();
    createInsertMenu();

    addAction(actSearchFindNextCurrent);
    addAction(actSearchFindPreviousCurrent);
    addAction(actSearchLogNext);
    addAction(actSearchLogPrev);

    connect(menuFileReopen, &QMenu::triggered,   this, &MainWindow::onMenuFileReopen);
    connect(actFileExit,    &QAction::triggered, this, &QMainWindow::close);

    connect(mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::onSubWindowActivated);

    connect(actViewSettings, &QAction::triggered, this, &MainWindow::onViewOptions);
    connect(actHelpWhatsNew, &QAction::triggered, this, &MainWindow::onHelpWhatsNew);
    connect(actHelpAbout,    &QAction::triggered, this, &MainWindow::onHelpAbout);

    connect(toolBarModel, &QToolBar::orientationChanged, this, &MainWindow::onToolBarModelOrientationChanged);

    Catalogue::AddLexerModule(&lexerRDOSyntax);
    Catalogue::AddLexerModule(&lexerRDOFind);
    Scintilla_LinkLexers();

    loadMenuFileReopen  ();
    updateMenuFileReopen();
}

// TODO не вызывается диструктор
MainWindow::~MainWindow()
{}

void MainWindow::createStatusBar()
{
    m_pStatusBar = rdo::Factory<StatusBar>::create(this);
}

void MainWindow::createToolBar()
{
    m_pModelSpeedSlider = new QSlider(Qt::Horizontal, this);
    m_pModelSpeedSlider->setRange(0, 100);
    m_pModelSpeedSlider->setValue(100);
    m_pModelSpeedSlider->setMaximumSize(100, 100);

    toolBarModel->insertWidget(actModelFrameNext, m_pModelSpeedSlider);
    toolBarModel->insertSeparator(actModelFrameNext);
}

void MainWindow::createInsertMenu()
{
    ASSERT(!m_pInsertMenuSignalMapper);
    m_pInsertMenuSignalMapper = new QSignalMapper(this);

    class MenuItem
    {
    public:
        typedef  InsertMenuData::Position  Position;

        MenuItem(const char* title, const QString& resourceName = QString(), const Position& position = Position())
            : m_title(title)
            , m_position(position)
        {
            if (!resourceName.isEmpty())
            {
                QFile file(":/insert_menu_template/insert_menu_template/" + resourceName);
                if (file.open(QIODevice::ReadOnly) && file.isOpen())
                    m_textForInsert = file.readAll();
            }
        }

        const QString& title() const
        {
            return m_title;
        }

        const QString textForInsert() const
        {
            return !m_textForInsert.isEmpty()
                ? m_textForInsert
                : m_title;
        }

        const Position& position() const
        {
            return m_position;
        }

    private:
        QString    m_title;
        QString    m_textForInsert;
        Position   m_position;
    };

    typedef  std::vector<MenuItem>                        MenuItemList;
    typedef  std::list<std::pair<QString, MenuItemList>>  MenuList;

    MenuList menuList;
    menuList.push_back(std::make_pair(
            "PAT",
            MenuItemList({
                MenuItem("PAT operation", "pat_operation.txt", 9),
                MenuItem("PAT rule", "pat_rule.txt", 9),
                MenuItem("PAT keyboard", "pat_keyboard.txt", 9),
                "$Pattern", "operation", "event", "rule", "keyboard", "trace", "no_trace", "$Parameters",
                "$Relevant_resources", "Keep", "NoChange", "Create", "Erase", "NonExist", "$Time", "$Body",
                "Convert_begin", "Convert_end", "Convert_event", "Convert_rule", "Choice from",
                "Choice NoCheck", "first", "with_max", "with_min", "set", "$End"
            })));

    menuList.push_back(std::make_pair(
            "EVN",
            MenuItemList({
                MenuItem("EVN", "pat_event.txt", 9),
                "$Pattern", "trace", "no_trace", "$Parameters", "$Relevant_resources", "Keep", "NoChange",
                "Create", "Erase", "NonExist", "$Body", "Convert_event", "$End"
            })));

    menuList.push_back(std::make_pair(
            "PRC",
            MenuItemList({
                MenuItem("PRC", "prc.txt", 9),
                "$Process", "GENERATE", "SEIZE", "RELEASE", "ADVANCE", "QUEUE", "DEPART", "ASSIGN", "TERMINATE"
            })));

    menuList.push_back(std::make_pair(
            "RTP",
            MenuItemList({
                MenuItem("RTP permanent", "rtp_permanent.txt", 15),
                MenuItem("RTP temporary", "rtp_temporary.txt", 15),
                "$Resource_type", "permanent", "temporary", "$Parameters", "integer", "real", "enum", "string",
                "bool", "true", "false", "such_as", "$End"
            })));

    menuList.push_back(std::make_pair(
            "RSS",
            MenuItemList({
                MenuItem("RSS", "rss.txt", 16),
                "$Resources", "trace", "no_trace", "$End"
            })));

    menuList.push_back(std::make_pair(
            "FRM",
            MenuItemList({
                MenuItem("FRM", "frm.txt", 7),
                "$Frame", "$Sprite", "$Back_picture", "bitmap", "text", "line", "rect", "circle", "ellipse", "r_rect",
                "triang", "s_bmp", "active", "$End"
            })));

    menuList.push_back(std::make_pair(
            "FUN",
            MenuItemList({
                MenuItem("FUN algorithmic", "fun_algorithmic.txt", 10),
                MenuItem("SQN", "fun_sequence.txt", 10),
                MenuItem("CNS", "fun_const.txt", 15),
                "$Function", "$Type", "algorithmic", "list", "table", "normal", "uniform", "exponential",
                "triangular", "by_hist", "enumerative", "$Parameters", "$Body", "$Sequence", "$Constant",
                "$End"
            })));

    menuList.push_back(std::make_pair(
            "DPT",
            MenuItemList({
                MenuItem("DPT some", "dpt_some.txt", 16),
                MenuItem("DPT prior", "dpt_prior.txt", 16),
                MenuItem("DPT search", "dpt_search.txt", 16),
                "$Decision_point", "some", "search", "prior", "no_trace", "trace_stat", "trace_tops", "trace_all",
                "$Condition", "$Term_condition", "$Evaluate_by", "$Compare_tops", "YES", "NO", "$Activities",
                "value before", "value after", "$End"
            })));

    menuList.push_back(std::make_pair(
            "SMR",
            MenuItemList({
                MenuItem("SMR", "smr.txt", 17),
                "Frame_number", "Show_mode", "Animation", "Monitor", "NoShow", "Show_rate", "Run_StartTime",
                "Trace_StartTime", "Trace_EndTime", "Terminate_if", "Time_now", "Seconds", "Break_point"
            })));

    menuList.push_back(std::make_pair(
            "PMD",
            MenuItemList({
                MenuItem("PMD", "pmd.txt", 14),
                "$Results", "watch_par", "watch_state", "watch_quant", "watch_value", "get_value", "trace",
                "no_trace", "$End"
            })));

    menuList.push_back(std::make_pair("", MenuItemList()));
    menuList.push_back(std::make_pair(
            "Встроенные функции",
            MenuItemList({
                "Abs", "ArcCos", "ArcSin", "ArcTan", "Cos", "Cotan", "Exist", "Exp", "Floor", "For_All", "Frac",
                "IAbs", "IMax", "IMin", "Int", "IntPower", "Ln", "Log10", "Log2", "LogN", "Max", "Min", "Not_Exist",
                "Not_For_All", "Power", "Round", "Select", "Sin", "Sqrt", "Tan"
            })));

    menuList.push_back(std::make_pair(
            "Процедурный язык",
            MenuItemList({
                MenuItem("if", "algo_if.txt", 4),
                "else",
                MenuItem("if-else", "algo_if_else.txt", 4),
                MenuItem("for", "algo_for.txt", 5),
                "return",
                MenuItem("Локальная переменная", "algo_local_variable.txt")
            })));

    for (const MenuList::value_type& menu: menuList)
    {
        if (menu.first.isEmpty())
        {
            ASSERT(menu.second.empty());
            menuInsert->addSeparator();
            continue;
        }

        QMenu* pMenu = new QMenu(menu.first, this);
        menuInsert->addMenu(pMenu);

        for (const MenuItem& menuItem: menu.second)
        {
            QAction* pAction = pMenu->addAction(menuItem.title());
            pAction->setEnabled(false);
            QObject::connect(pAction, SIGNAL(triggered(bool)), m_pInsertMenuSignalMapper, SLOT(map()));
            InsertMenuData* pInsertMenuData = new InsertMenuData(this, menuItem.textForInsert(), menuItem.position());
            m_pInsertMenuSignalMapper->setMapping(pAction, pInsertMenuData);
        }
    }
}

void MainWindow::init()
{
    QSettings settings;
    settings.beginGroup("style");

    settings.beginGroup("editor");
    settings >> style_editor;
    settings.endGroup();

    settings.beginGroup("build");
    style_build.window.wordWrap          = true;
    style_build.window.showHorzScrollBar = false;
    settings >> style_build;
    settings.endGroup();

    settings.beginGroup("debug");
    style_debug.window.wordWrap          = true;
    style_debug.window.showHorzScrollBar = false;
    settings >> style_debug;
    settings.endGroup();

    settings.beginGroup("trace");
    settings >> style_trace;
    settings.endGroup();

    settings.beginGroup("results");
    settings >> style_results;
    settings.endGroup();

    settings.beginGroup("find");
    settings >> style_find;
    settings.endGroup();

    settings.beginGroup("frame");
    settings >> style_frame;
    settings.endGroup();

    settings.beginGroup("chart");
    settings >> style_chart;
    settings.endGroup();

    settings.endGroup();

    m_pDockBuild   = new DockBuild  (this);
    m_pDockDebug   = new DockDebug  (this);
    m_pDockTrace   = new DockTrace  (this);
    m_pDockResults = new DockResults(this);
    m_pDockFind    = new DockFind   (this);

    QObject::connect(m_pDockBuild, &QDockWidget::visibilityChanged, this, &MainWindow::onDockVisibleChanged);
    QObject::connect(m_pDockFind,  &QDockWidget::visibilityChanged, this, &MainWindow::onDockVisibleChanged);

    addDockWidget(Qt::BottomDockWidgetArea, m_pDockBuild);
    tabifyDockWidget(m_pDockBuild, m_pDockDebug  );
    tabifyDockWidget(m_pDockBuild, m_pDockTrace  );
    tabifyDockWidget(m_pDockBuild, m_pDockResults);
    tabifyDockWidget(m_pDockBuild, m_pDockFind   );
    m_pDockDebug->raise();

    m_pDockChartTree = new DockChartTree(this);
    m_pDockFrame     = new DockFrame    (this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pDockChartTree);
    tabifyDockWidget(m_pDockChartTree, m_pDockFrame);
    m_pDockChartTree->raise();

    QMenu* pMenuDockView = new QMenu("Окна");
    ASSERT(pMenuDockView);
    menuView->insertMenu(actViewSettings, pMenuDockView);
    pMenuDockView->addAction(m_pDockBuild->toggleViewAction());
    pMenuDockView->addAction(m_pDockDebug->toggleViewAction());
    pMenuDockView->addAction(m_pDockTrace->toggleViewAction());
    pMenuDockView->addAction(m_pDockResults->toggleViewAction());
    pMenuDockView->addAction(m_pDockFind->toggleViewAction());
    pMenuDockView->addAction(m_pDockChartTree->toggleViewAction());
    pMenuDockView->addAction(m_pDockFrame->toggleViewAction());

    QMenu* pMenuToolbarView = new QMenu("Панели");
    ASSERT(pMenuToolbarView);
    menuView->insertMenu(actViewSettings, pMenuToolbarView);
    pMenuToolbarView->addAction(toolBarFile->toggleViewAction());
    pMenuToolbarView->addAction(toolBarEdit->toggleViewAction());
    pMenuToolbarView->addAction(toolBarZoom->toggleViewAction());
    pMenuToolbarView->addAction(toolBarModel->toggleViewAction());

    menuView->insertSeparator(actViewSettings);

    new rdo::gui::model::Model();
    m_pModel = g_pModel;
    updateAllStyles();
}

void MainWindow::setVisible(bool visible)
{
    parent_type::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (g_pModel && !g_pModel->closeModel())
        event->ignore();

    if (event->isAccepted())
    {
        update_stop();

        QSettings settings;
        settings.beginGroup("style");

        settings.beginGroup("editor");
        settings << style_editor;
        settings.endGroup();

        settings.beginGroup("build");
        settings << style_build;
        settings.endGroup();

        settings.beginGroup("debug");
        settings << style_debug;
        settings.endGroup();

        settings.beginGroup("trace");
        settings << style_trace;
        settings.endGroup();

        settings.beginGroup("results");
        settings << style_results;
        settings.endGroup();

        settings.beginGroup("find");
        settings << style_find;
        settings.endGroup();

        settings.beginGroup("frame");
        settings << style_frame;
        settings.endGroup();

        settings.beginGroup("chart");
        settings << style_chart;
        settings.endGroup();

        settings.endGroup();
    }
}

void MainWindow::onViewOptions()
{
    ViewPreferences dlg(this);
    dlg.exec();
}

void MainWindow::updateAllStyles()
{
    g_pModel->updateStyleOfAllModel();
    getDockBuild  ().getContext().setEditorStyle (&style_build  );
    getDockDebug  ().getContext().setEditorStyle (&style_debug  );
    getDockTrace  ().getContext().view().setStyle(&style_trace  );
    getDockResults().getContext().setEditorStyle (&style_results);
    getDockFind   ().getContext().setEditorStyle (&style_find   );
    g_pTracer->updateChartsStyles();
}

const LPStatusBar& MainWindow::statusBar() const
{
    return m_pStatusBar;
}

void MainWindow::onHelpWhatsNew()
{
    QByteArray ba;
    ba.append("setSource qthelp://language/doc/rdo_studio_rus/html/rdo_whats_new.htm\n");
    g_pApp->callQtAssistant(ba);
}

void MainWindow::onHelpAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::update_start()
{
    m_updateTimerID = startTimer(1000 / 30);
}

void MainWindow::update_stop()
{
    if (m_updateTimerID)
    {
        killTimer(m_updateTimerID);
        m_updateTimerID = 0;
    }
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    parent_type::timerEvent(event);

    if (event->timerId() == m_updateTimerID)
    {
        update_stop();
        g_pModel->update();
        update_start();
    }
}

void MainWindow::showEvent(QShowEvent*)
{}

void MainWindow::hideEvent(QHideEvent*)
{}

void MainWindow::addSubWindow(QWidget* pWidget)
{
    ASSERT(pWidget);

    QList<QMdiSubWindow*> frameList = mdiArea->subWindowList();

    bool maximized = frameList.empty();
    if (!maximized)
        maximized = frameList.front()->isMaximized();

    QMdiSubWindow* pFrame = mdiArea->addSubWindow(pWidget);
    QObject::connect(pFrame, &QMdiSubWindow::windowStateChanged, this, &MainWindow::onSubWindowStateChanged);

    pWidget->show();

    static const float sizeScale = 0.9f;
    QSize size(mdiArea->size());
    size.setWidth (int(float(size.width ()) * sizeScale));
    size.setHeight(int(float(size.height()) * sizeScale));
    pFrame->move  (frameList.size() * 4, frameList.size() * 4);
    pFrame->resize(size);

    if (maximized)
        pFrame->showMaximized();
}

void MainWindow::activateSubWindow(QWidget* pWidget)
{
    QMdiSubWindow* pSubWindow = dynamic_cast<QMdiSubWindow*>(pWidget);
    if (!pSubWindow)
        return;

    mdiArea->setActiveSubWindow(pSubWindow);
}

void MainWindow::onDockVisibleChanged(bool visible)
{
    if (!visible)
        return;

    QDockWidget* pDock = dynamic_cast<QDockWidget*>(sender());
    ASSERT(pDock);

    rdo::gui::editor::Log* pLog = NULL;

    if (pDock == &getDockBuild())
    {
        pLog = &getDockBuild().getContext();
    }
    else if (pDock == &getDockFind())
    {
        pLog = &getDockFind().getContext();
    }

    if (!pLog)
        return;

    rdo::gui::model::TabCtrl* pEditorTab = g_pModel->getTab();
    if (pEditorTab)
    {
        for (int i = 0; i < pEditorTab->count(); ++i)
            pEditorTab->getItemEdit(i)->setLog(*pLog);
    }
}

void MainWindow::onToolBarModelOrientationChanged(Qt::Orientation orientation)
{
    m_pModelSpeedSlider->setOrientation(orientation);
}

void MainWindow::onMenuFileReopen(QAction* pAction)
{
    QString menuName = pAction->text();
    int pos = menuName.indexOf(' ');
    if (pos == -1)
        return;

    QString fileName = menuName.midRef(pos + 1).toString();
    if (!g_pModel->openModel(fileName) && g_pModel->isPrevModelClosed())
    {
        ReopenList::iterator it = std::find(m_reopenList.begin(), m_reopenList.end(), fileName);
        if (it != m_reopenList.end())
            m_reopenList.erase(it);

        updateMenuFileReopen();
    }
}

void MainWindow::insertMenuFileReopenItem(const QString& item)
{
    if (!item.isEmpty())
    {
        ReopenList::iterator it = boost::range::find(m_reopenList, item);
        if (it != m_reopenList.end())
            m_reopenList.erase(it);

        m_reopenList.push_front(item);

        while (m_reopenList.size() > 10)
        {
            ReopenList::iterator it = m_reopenList.end();
            --it;
            m_reopenList.erase(it);
        }

        updateMenuFileReopen();
    }
}

void MainWindow::updateMenuFileReopen()
{
    menuFileReopen->clear();

    for (ReopenList::size_type reopenIndex = 0; reopenIndex < m_reopenList.size(); ++reopenIndex)
    {
        if (reopenIndex == 4)
            menuFileReopen->addSeparator();

        menuFileReopen->addAction(QString("%1. %2").arg(reopenIndex+1).arg(m_reopenList[reopenIndex]));
    }

    menuFileReopen->setEnabled(!menuFileReopen->isEmpty());

    saveMenuFileReopen();
}

void MainWindow::loadMenuFileReopen()
{
    m_reopenList.clear();

    QSettings settings;
    settings.beginGroup("reopen");

    QStringList groupList = settings.childKeys();
    std::vector<int> indexList;
    for (const QString& index: groupList)
        indexList.push_back(index.toInt());

    std::sort(indexList.begin(), indexList.end());

    for (int index: indexList)
    {
        QString value = settings.value(QString::number(index), QString()).toString();
        if (!value.isEmpty())
            m_reopenList.push_back(value);
    }

    settings.endGroup();
}

void MainWindow::saveMenuFileReopen() const
{
    QSettings settings;
    settings.beginGroup("reopen");

    std::size_t index = 1;
    for (const QString& fileName: m_reopenList)
        settings.setValue(QString::number(index++), fileName);

    settings.endGroup();
}

void MainWindow::updateInsertMenu(bool enabled)
{
    QList<QAction*> menuList = menuInsert->actions();
    for (const QAction* pMenu: menuList)
    {
        if (pMenu->isSeparator())
            continue;

        QList<QAction*> itemList = pMenu->menu()->actions();
        for (QAction* pItem: itemList)
            pItem->setEnabled(enabled);
    }
}

void MainWindow::onUpdateCascadeTitle(bool activated)
{
    ActionActivator::updateAction(
        actWindowCascade,
        activated && mdiArea->viewMode() == QMdiArea::SubWindowView,
        mdiArea, &QMdiArea::cascadeSubWindows
    );
    ActionActivator::updateAction(
        actWindowTitle,
        activated && mdiArea->viewMode() == QMdiArea::SubWindowView,
        mdiArea, &QMdiArea::tileSubWindows
    );
}

void MainWindow::onUpdateTabMode(bool activated)
{
    ActionActivator::updateAction(
        actWindowTabbedViewMode,
        activated,
        boost::function<void (bool)>(boost::bind(&MainWindow::onSetTabbedViewMode, this, _1))
    );
    actWindowTabbedViewMode->setCheckable(activated);
}

void MainWindow::onSetTabbedViewMode(bool checked)
{
    mdiArea->setViewMode(checked ? QMdiArea::TabbedView : QMdiArea::SubWindowView);
    onUpdateCascadeTitle(!checked);
    forAllSubWindows(boost::bind(&QWidget::showMaximized, _1), mdiArea->currentSubWindow());
}

void MainWindow::onSubWindowActivated(QMdiSubWindow* window)
{
    m_subWindowToAction->onSubWindowActivated(window);
    bool hasWindows = !mdiArea->subWindowList().isEmpty();
    onUpdateCascadeTitle(hasWindows);
    onUpdateTabMode(hasWindows);
    updateWindowTitle();
}

void MainWindow::onSubWindowStateChanged(Qt::WindowStates oldState, Qt::WindowStates newState)
{
    if (mdiArea->viewMode() != QMdiArea::SubWindowView)
        return;

    if ((newState & Qt::WindowMaximized) && !(oldState & Qt::WindowMaximized))
    {
        forAllSubWindows(boost::bind(&QWidget::showMaximized, _1), dynamic_cast<QMdiSubWindow*>(sender()));
    }
    else if ((newState & ~Qt::WindowActive) == Qt::WindowNoState && (oldState & ~Qt::WindowActive) != Qt::WindowNoState)
    {
        forAllSubWindows(boost::bind(&QWidget::showNormal, _1), dynamic_cast<QMdiSubWindow*>(sender()));
    }
}

template <class F>
void MainWindow::forAllSubWindows(F functor, QMdiSubWindow* pTopSubWindow)
{
    static bool firstCall = true;
    if (firstCall)
    {
        firstCall = false;
        QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
        for (QMdiSubWindow* pSubWindow: windowList)
        {
            if (pSubWindow != pTopSubWindow)
                functor(pSubWindow);
        }
        if (pTopSubWindow)
        {
            functor(pTopSubWindow);
            pTopSubWindow->raise();
            pTopSubWindow->setFocus();
        }
        firstCall = true;
    }
}

void MainWindow::updateWindowTitle()
{
    QString appName("RAO Studio");
    QString normalTitle = g_pModel->hasModel()
        ? QString("%1 - %2").arg(appName).arg(g_pModel->getName())
        : appName;

    if (windowTitle() != normalTitle)
        setWindowTitle(normalTitle);
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::WindowTitleChange)
    {
        if (dynamic_cast<QMdiSubWindow*>(target))
            m_subWindowToAction->onTitleChanged(static_cast<QMdiSubWindow*>(target));
        else
            updateWindowTitle();
    }

    return parent_type::eventFilter(target, event);
}

rdo::gui::model::Model* MainWindow::getModel()
{
    return m_pModel;
}
