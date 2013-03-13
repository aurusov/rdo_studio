/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_manager.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      28.03.2003
  \brief     Отрисовка кадров анимации
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "repository/rdorepository.h"
#include "app/rdo_studio/src/frame/frame_manager.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
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
Manager::Manager(CREF(OnChangeFrame) onChangeFrame)
	: m_lastShowedFrame    (ruint(~0))
	, m_currentShowingFrame(ruint(~0))
	, m_changed            (false    )
	, m_onChangeFrame      (onChangeFrame)
{
	//! @todo А почему объект не удаляется ? Это происходит автоматически ?

	g_pApp->getIMainWnd()->connectOnActivateSubWindow(this);
}

Manager::~Manager()
{
	clear();

	BOOST_FOREACH(Frame* pFrame, m_frameList)
	{
		delete pFrame;
	}
}

rbool Manager::init()
{
	connect(
		&g_pApp->getIMainWnd()->getDockFrame().getContext(), &TreeCtrl::itemDoubleClicked,
		this, &Manager::onTreeWidgetItemDoubleClicked
	);

	return true;
}

void Manager::insertFrame(CREF(QString) frameName)
{
	PTR(Frame) item = new Frame();
	item->m_pTreeWidgetItem = g_pApp->getIMainWnd()->getDockFrame().getContext().insertFrame(frameName);
	item->m_name            = frameName;
	m_frameList.push_back(item);
}

ruint Manager::findFrameIndex(CPTR(QTreeWidgetItem) pTreeWidgetItem) const
{
	ruint index = 0;
	BOOST_FOREACH(const Frame* pFrame, m_frameList)
	{
		if (pFrame->m_pTreeWidgetItem == pTreeWidgetItem)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint Manager::findFrameIndex(CPTR(View) pView) const
{
	ruint index = 0;
	BOOST_FOREACH(const Frame* pFrame, m_frameList)
	{
		if (pFrame->m_pView == pView)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint Manager::findFrameIndex(CPTR(Content) pContent) const
{
	ruint index = 0;
	BOOST_FOREACH(const Frame* pFrame, m_frameList)
	{
		if (pFrame->m_pContent == pContent)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

CREF(QString) Manager::getFrameName(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_name;
}

PTR(View) Manager::getFrameView(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pView;
}

PTR(View) Manager::getFrameViewFirst() const
{
	if (m_frameList.empty())
		return NULL;

	return m_frameList.front()->m_pView;
}

ruint Manager::count() const
{
	return m_frameList.size();
}

rbool Manager::isChanged()
{
	rbool res = m_changed;
	m_changed = false;
	return res;
}

void Manager::areaDown(ruint frameIndex, CREF(QPoint) point) const
{
	ASSERT(frameIndex != ruint(~0) && frameIndex < m_frameList.size());

	CREF(rdo::gui::animation::AreaList) areaList = m_frameList[frameIndex]->m_areaList;
	BOOST_FOREACH(const rdo::gui::animation::AreaList::value_type& area, areaList)
	{
		if (area.second.m_rect.contains(point))
		{
			tstring areaName = area.first.toStdString();
			g_pModel->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &areaName);
		}
	}
}

PTR(View) Manager::createView(ruint index)
{
	PTR(View) pView = NULL;
	if (index != ~0)
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

rbool Manager::isShowing() const
{
	BOOST_FOREACH(const Frame* pFrame, m_frameList)
	{
		if (pFrame->m_pView && pFrame->m_pView->isVisible())
			return true;
	}
	return false;
}

void Manager::disconnectView(CPTR(View) pView)
{
	ruint index = findFrameIndex(pView);
	if (index != ~0)
	{
		m_frameList[index]->m_pView = NULL;
	}
	m_changed = true;
}

void Manager::closeAll()
{
	ruint backup = m_lastShowedFrame;
	BOOST_FOREACH(Frame* pFrame, m_frameList)
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
	BOOST_FOREACH(Frame* pFrame, m_frameList)
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

	m_lastShowedFrame = ruint(~0);
	setCurrentShowingFrame(ruint(~0));
}

ruint Manager::getLastShowedFrame() const
{
	return m_lastShowedFrame;
}

void Manager::setLastShowedFrame(ruint index)
{
	if (index != ruint(~0) && index < count())
	{
		m_lastShowedFrame = index;
	}
}

void Manager::setCurrentShowingFrame(ruint index)
{
	if (index == ruint(~0) || (index != ruint(~0) && index < count()))
	{
		m_currentShowingFrame = index;
		if (g_pApp->getStyle())
		{
			if (m_currentShowingFrame != ruint(~0))
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

void Manager::resetCurrentShowingFrame(ruint index)
{
	if (index == m_currentShowingFrame)
	{
		setCurrentShowingFrame(ruint(~0));
	}
}

void Manager::insertBitmap(CREF(QString) bitmapName)
{
	if (m_bitmapList.find(bitmapName) != m_bitmapList.end())
		return;

	g_pApp->getIMainWnd()->getDockDebug().appendString(QString("Загрузка %1...").arg(bitmapName));
	g_pApp->getIMainWnd()->getDockDebug().getContext().update();

	rdo::binarystream stream;
	rdo::repository::RDOThreadRepository::BinaryFile data(bitmapName.toStdString(), stream);
	g_pModel->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data);

	rbool ok = false;
	QPixmap pixmap(QString::fromStdString(data.m_name));
	if (!pixmap.isNull())
	{
		std::pair<rdo::gui::BitmapList::const_iterator, rbool> result =
			m_bitmapList.insert(rdo::gui::BitmapList::value_type(bitmapName, pixmap));
		if (result.second)
		{
			ok = true;
		}
	}

	g_pApp->getIMainWnd()->getDockDebug().appendString(ok ? " ok\n" : " failed\n");
	g_pApp->getIMainWnd()->getDockDebug().getContext().update();
}

void Manager::showFrame(CPTRC(rdo::animation::Frame) pFrame, ruint index)
{
	if (index < count())
	{
		PTR(View) pFrameView = getFrameView(index);
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
	ruint cnt = count();
	if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame < cnt-1)
	{
		ruint index = m_currentShowingFrame + 1;
		PTR(View) pView = getFrameView(index);
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
	ruint cnt = count();
	if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame != ruint(~0))
	{
		ruint index = m_currentShowingFrame - 1;
		PTR(View) pView = getFrameView(index);
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

void Manager::showFrame(ruint index)
{
	ruint cnt = count();
	if (g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && index >= 0 && index < cnt)
	{
		PTR(View) pView = getFrameView(index);
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

rbool Manager::canShowNextFrame() const
{
	ruint cnt = count();
	return g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame == ruint(~0) || m_currentShowingFrame < cnt-1);
}

rbool Manager::canShowPrevFrame() const
{
	int cnt = count();
	return g_pModel->isRunning() && g_pModel->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame != ruint(~0) && m_currentShowingFrame > 0);
}

void Manager::updateStyles() const
{
	BOOST_FOREACH(Frame* pFrame, m_frameList)
	{
		PTR(View) pFrameView = pFrame->m_pView;
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

	ruint index = findFrameIndex(pFrameAnimationWnd);
	setLastShowedFrame    (index);
	setCurrentShowingFrame(index);
}

void Manager::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int)
{
	if (g_pModel->getRuntimeMode() == rdo::runtime::RTM_MaxSpeed)
		return;

	ruint index = findFrameIndex(pTreeWidgetItem);
	if (index == ruint(~0))
		return;

	PTR(View) pView = getFrameView(index);
	if (!pView)
	{
		createView(index);
	}
	else
	{
		g_pApp->getIMainWnd()->activateSubWindow(pView->parentWidget());
	}
}
