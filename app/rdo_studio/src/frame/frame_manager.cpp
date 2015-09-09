// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/frame_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/service/src/simulator.h"
#include "repository/rdorepository.h"
#include "app/rdo_studio/src/frame/frame_manager.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/frame/frame_tree_ctrl.h"
#include "app/rdo_studio/src/editor/debug_edit.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::frame;

// --------------------------------------------------------------------------------
// -------------------- Manager::Frame
// --------------------------------------------------------------------------------
Manager::Frame::Frame()
    : m_pTreeWidgetItem(NULL)
    , m_pView          (NULL)
    , m_pContent       (NULL)
{}

Manager::Frame::~Frame()
{
    clear();
}

void Manager::Frame::clear()
{
    m_areaList.clear();
}

// --------------------------------------------------------------------------------
// -------------------- Manager
// --------------------------------------------------------------------------------
Manager::Manager(const OnChangeFrame& onChangeFrame)
    : m_lastShowedFrame(std::size_t(~0))
    , m_currentShowingFrame(std::size_t(~0))
    , m_changed(false)
    , m_onChangeFrame(onChangeFrame)
{
    // TODO А почему объект не удаляется ? Это происходит автоматически ?

    QObject::connect(g_pApp->getMainWndUI()->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onSubWindowActivated(QMdiSubWindow*)));

    QObject::connect(
        &g_pApp->getIMainWnd()->getDockFrame().getContext(), &TreeCtrl::itemDoubleClicked,
        this, &Manager::onTreeWidgetItemDoubleClicked
    );
}

Manager::~Manager()
{
    clear();

    for (Frame* pFrame: m_frameList)
    {
        delete pFrame;
    }
}

void Manager::insertFrame(const QString& frameName)
{
    Frame* item = new Frame();
    item->m_pTreeWidgetItem = g_pApp->getIMainWnd()->getDockFrame().getContext().insertFrame(frameName);
    item->m_name            = frameName;
    m_frameList.push_back(item);
}

std::size_t Manager::findFrameIndex(const QTreeWidgetItem* pTreeWidgetItem) const
{
    std::size_t index = 0;
    for (const Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pTreeWidgetItem == pTreeWidgetItem)
        {
            return index;
        }
        index++;
    }
    return std::size_t(~0);
}

std::size_t Manager::findFrameIndex(const View* pView) const
{
    std::size_t index = 0;
    for (const Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pView == pView)
        {
            return index;
        }
        index++;
    }
    return std::size_t(~0);
}

std::size_t Manager::findFrameIndex(const Content* pContent) const
{
    std::size_t index = 0;
    for (const Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pContent == pContent)
        {
            return index;
        }
        index++;
    }
    return std::size_t(~0);
}

const QString& Manager::getFrameName(std::size_t index) const
{
    ASSERT(index < m_frameList.size());
    return m_frameList[index]->m_name;
}

View* Manager::getFrameView(std::size_t index) const
{
    ASSERT(index < m_frameList.size());
    return m_frameList[index]->m_pView;
}

View* Manager::getFrameViewFirst() const
{
    if (m_frameList.empty())
        return NULL;

    return m_frameList.front()->m_pView;
}

std::size_t Manager::count() const
{
    return m_frameList.size();
}

bool Manager::isChanged()
{
    bool res = m_changed;
    m_changed = false;
    return res;
}

void Manager::areaDown(std::size_t frameIndex, const QPoint& point) const
{
    ASSERT(frameIndex != std::size_t(~0) && frameIndex < m_frameList.size());

    const rdo::gui::animation::AreaList& areaList = m_frameList[frameIndex]->m_areaList;
    for (const rdo::gui::animation::AreaList::value_type& area: areaList)
    {
        if (area.second.m_rect.contains(point))
        {
            std::string areaName = area.first.toStdString();
            g_pModel->sendMessage(kernel->runtime(), RDOThread::Message::RUNTIME_FRAME_AREA_DOWN, &areaName);
        }
    }
}

View* Manager::createView(std::size_t index)
{
    View* pView = NULL;
    if (index != std::size_t(~0))
    {
        pView = new View(NULL);
        g_pApp->getIMainWnd()->addSubWindow(pView);
        pView->parentWidget()->setWindowIcon (QIcon(QString::fromUtf8(":/images/images/mdi_frame.png")));
        pView->parentWidget()->setWindowTitle(QString("кадр: %1").arg(getFrameName(index)));

        m_frameList[index]->m_pView    = pView;
        m_frameList[index]->m_pContent = pView->getContent();
        m_lastShowedFrame              = index;
        setCurrentShowingFrame(index);
    }
    return pView;
}

bool Manager::isShowing() const
{
    for (const Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pView && pFrame->m_pView->isVisible())
            return true;
    }
    return false;
}

void Manager::disconnectView(const View* pView)
{
    std::size_t index = findFrameIndex(pView);
    if (index != std::size_t(~0))
    {
        m_frameList[index]->m_pView = NULL;
    }
    m_changed = true;
}

void Manager::closeAll()
{
    std::size_t backup = m_lastShowedFrame;
    for (Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pView)
        {
            pFrame->m_pView->parentWidget()->close();
            pFrame->m_pView = NULL;
        }
    }
    m_lastShowedFrame = backup;
}

void Manager::clear()
{
    if (g_pApp->getStyle())
    {
        g_pApp->getIMainWnd()->getDockFrame().getContext().clear();
    }
    for (Frame* pFrame: m_frameList)
    {
        if (pFrame->m_pView)
        {
            pFrame->m_pView->parentWidget()->close();
            pFrame->m_pView = NULL;
        }
        delete pFrame;
    }

    m_frameList .clear();
    m_bitmapList.clear();

    m_lastShowedFrame = std::size_t(~0);
    setCurrentShowingFrame(std::size_t(~0));
}

std::size_t Manager::getLastShowedFrame() const
{
    return m_lastShowedFrame;
}

void Manager::setLastShowedFrame(std::size_t index)
{
    if (index != std::size_t(~0) && index < count())
    {
        m_lastShowedFrame = index;
    }
}

void Manager::setCurrentShowingFrame(std::size_t index)
{
    if (index == std::size_t(~0) || (index != std::size_t(~0) && index < count()))
    {
        m_currentShowingFrame = index;
        if (g_pApp->getStyle())
        {
            if (m_currentShowingFrame != std::size_t(~0))
            {
                g_pApp->getIMainWnd()->getDockFrame().getContext().setCurrentItem(m_frameList[m_currentShowingFrame]->m_pTreeWidgetItem);
            }
            else
            {
                g_pApp->getIMainWnd()->getDockFrame().getContext().setCurrentItem(NULL);
            }
        }
        m_onChangeFrame(index);
    }
}

void Manager::resetCurrentShowingFrame(std::size_t index)
{
    if (index == m_currentShowingFrame)
    {
        setCurrentShowingFrame(std::size_t(~0));
    }
}

void Manager::insertBitmap(const QString& bitmapName)
{
    if (m_bitmapList.find(bitmapName) != m_bitmapList.end())
        return;

    g_pApp->getIMainWnd()->getDockDebug().appendString(QString("Загрузка %1...").arg(bitmapName));
    g_pApp->getIMainWnd()->getDockDebug().getContext().update();

    std::stringstream stream(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    rdo::repository::RDOThreadRepository::BinaryFile data(bitmapName.toStdWString(), stream);
    g_pModel->sendMessage(kernel->repository(), RDOThread::Message::REPOSITORY_LOAD_BINARY, &data);

    bool ok = false;
    QPixmap pixmap(QString::fromStdWString(data.m_name.wstring()));
    if (!pixmap.isNull())
    {
        std::pair<rdo::gui::BitmapList::const_iterator, bool> result =
            m_bitmapList.insert(rdo::gui::BitmapList::value_type(bitmapName, pixmap));
        if (result.second)
        {
            ok = true;
        }
    }

    g_pApp->getIMainWnd()->getDockDebug().appendString(ok ? " ok\n" : " failed\n");
    g_pApp->getIMainWnd()->getDockDebug().getContext().update();
}

void Manager::showFrame(const rdo::animation::Frame* const pFrame, std::size_t index)
{
    if (index < count())
    {
        View* pFrameView = getFrameView(index);
        ASSERT(pFrameView);
        rdo::gui::BitmapList bitmapGeneratedList;
        pFrameView->update(pFrame, m_bitmapList, bitmapGeneratedList, m_frameList[index]->m_areaList);
        if (!bitmapGeneratedList.empty())
        {
            std::copy(bitmapGeneratedList.begin(), bitmapGeneratedList.end(), std::inserter(m_bitmapList, m_bitmapList.begin()));
        }
    }
}

void Manager::showNextFrame()
{
    std::size_t cnt = count();
    if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RunTimeMode::MAX_SPEED && cnt > 1 && m_currentShowingFrame < cnt-1)
    {
        std::size_t index = m_currentShowingFrame + 1;
        View* pView = getFrameView(index);
        if (!pView)
        {
            pView = createView(index);
        }
        else
        {
            pView->parentWidget()->raise();
            pView->parentWidget()->activateWindow();
            setLastShowedFrame    (index);
            setCurrentShowingFrame(index);
        }
    }
}

void Manager::showPrevFrame()
{
    std::size_t cnt = count();
    if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RunTimeMode::MAX_SPEED && cnt > 1 && m_currentShowingFrame != std::size_t(~0))
    {
        std::size_t index = m_currentShowingFrame - 1;
        View* pView = getFrameView(index);
        if (!pView)
        {
            pView = createView(index);
        }
        else
        {
            pView->parentWidget()->raise();
            pView->parentWidget()->activateWindow();
            setLastShowedFrame    (index);
            setCurrentShowingFrame(index);
        }
    }
}

void Manager::showFrame(std::size_t index)
{
    std::size_t cnt = count();
    if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RunTimeMode::MAX_SPEED && cnt > 1 && index >= 0 && index < cnt)
    {
        View* pView = getFrameView(index);
        if (!pView)
        {
            pView = createView(index);
        }
        else
        {
            pView->parentWidget()->raise();
            pView->parentWidget()->activateWindow();
            setLastShowedFrame    (index);
            setCurrentShowingFrame(index);
        }
    }
}

bool Manager::canShowNextFrame() const
{
    std::size_t cnt = count();
    return g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RunTimeMode::MAX_SPEED && cnt > 1 && (m_currentShowingFrame == std::size_t(~0) || m_currentShowingFrame < cnt-1);
}

bool Manager::canShowPrevFrame() const
{
    int cnt = count();
    return g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RunTimeMode::MAX_SPEED && cnt > 1 && (m_currentShowingFrame != std::size_t(~0) && m_currentShowingFrame > 0);
}

void Manager::updateStyles() const
{
    for (Frame* pFrame: m_frameList)
    {
        View* pFrameView = pFrame->m_pView;
        if (pFrameView)
        {
            pFrameView->updateFont();
            pFrameView->update    ();
        }
    }
}

void Manager::onSubWindowActivated(QMdiSubWindow* pWindow)
{
    if (!pWindow)
        return;

    View* pFrameAnimationWnd = dynamic_cast<View*>(pWindow->widget());
    if (!pFrameAnimationWnd)
        return;

    std::size_t index = findFrameIndex(pFrameAnimationWnd);
    setLastShowedFrame(index);
    setCurrentShowingFrame(index);
}

void Manager::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int)
{
    if (g_pModel->getRuntimeMode() == rdo::runtime::RunTimeMode::MAX_SPEED)
        return;

    std::size_t index = findFrameIndex(pTreeWidgetItem);
    if (index == std::size_t(~0))
        return;

    View* pView = getFrameView(index);
    if (!pView)
    {
        createView(index);
    }
    else
    {
        g_pApp->getIMainWnd()->activateSubWindow(pView->parentWidget());
    }
}
