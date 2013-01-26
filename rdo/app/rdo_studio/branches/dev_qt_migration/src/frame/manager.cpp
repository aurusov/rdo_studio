/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      28.03.2003
  \brief     Отрисовка кадров анимации
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "repository/rdorepository.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager::Frame
// --------------------------------------------------------------------------------
RDOStudioFrameManager::Frame::Frame()
	: m_pTreeWidgetItem(NULL)
	, m_pView          (NULL)
	, m_pContent       (NULL)
{}

RDOStudioFrameManager::Frame::~Frame()
{
	clear();
}

void RDOStudioFrameManager::Frame::clear()
{
	m_areaList.clear();
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
RDOStudioFrameManager::RDOStudioFrameManager(CREF(OnChangeFrame) onChangeFrame)
	: m_lastShowedFrame    (ruint(~0))
	, m_currentShowingFrame(ruint(~0))
	, m_changed            (false    )
	, m_onChangeFrame      (onChangeFrame)
{
	//! @todo А почему объект не удаляется ? Это происходит автоматически ?

	studioApp.getIMainWnd()->connectOnActivateSubWindow(this);
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	clear();

	STL_FOR_ALL(m_frameList, it)
	{
		delete *it;
	}
}

rbool RDOStudioFrameManager::init()
{
	connect(
		&studioApp.getIMainWnd()->getDockFrame().getContext(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this,                                                  SLOT(onTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int))
	);

	return true;
}

void RDOStudioFrameManager::insertFrame(CREF(tstring) frameName)
{
	PTR(Frame) item = new Frame();
	item->m_pTreeWidgetItem = studioApp.getIMainWnd()->getDockFrame().getContext().insertFrame(QString::fromStdString(frameName));
	item->m_name            = frameName;
	m_frameList.push_back(item);
}

ruint RDOStudioFrameManager::findFrameIndex(CPTR(QTreeWidgetItem) pTreeWidgetItem) const
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

ruint RDOStudioFrameManager::findFrameIndex(CPTR(FrameAnimationWnd) pView) const
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

ruint RDOStudioFrameManager::findFrameIndex(CPTR(FrameAnimationContent) pContent) const
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

CREF(tstring) RDOStudioFrameManager::getFrameName(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_name;
}

PTR(FrameAnimationWnd) RDOStudioFrameManager::getFrameView(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pView;
}

PTR(FrameAnimationWnd) RDOStudioFrameManager::getFrameViewFirst() const
{
	if (m_frameList.empty())
		return NULL;

	return m_frameList.front()->m_pView;
}

ruint RDOStudioFrameManager::count() const
{
	return m_frameList.size();
}

rbool RDOStudioFrameManager::isChanged()
{
	rbool res = m_changed;
	m_changed = false;
	return res;
}

void RDOStudioFrameManager::areaDown(ruint frameIndex, CREF(QPoint) point) const
{
	ASSERT(frameIndex != ruint(~0) && frameIndex < m_frameList.size());

	CREF(rdo::gui::animation::AreaList) areaList = m_frameList[frameIndex]->m_areaList;
	BOOST_FOREACH(const rdo::gui::animation::AreaList::value_type& area, areaList)
	{
		if (area.second.m_rect.contains(point))
		{
			tstring areaName = area.first;
			model->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &areaName);
		}
	}
}

PTR(FrameAnimationWnd) RDOStudioFrameManager::createView(ruint index)
{
	PTR(FrameAnimationWnd) pView = NULL;
	if (index != ~0)
	{
		pView = new FrameAnimationWnd(NULL);
		studioApp.getIMainWnd()->addSubWindow(pView);
		pView->parentWidget()->setWindowIcon (QIcon(QString::fromUtf8(":/images/images/mdi_frame.png")));
		pView->parentWidget()->setWindowTitle(QString::fromStdString(rdo::format("кадр: %s", getFrameName(index).c_str()).c_str()));

		m_frameList[index]->m_pView    = pView;
		m_frameList[index]->m_pContent = pView->getContent();
		m_lastShowedFrame              = index;
		setCurrentShowingFrame(index);
	}
	return pView;
}

rbool RDOStudioFrameManager::isShowing() const
{
	BOOST_FOREACH(const Frame* pFrame, m_frameList)
	{
		if (pFrame->m_pView && pFrame->m_pView->isVisible())
			return true;
	}
	return false;
}

void RDOStudioFrameManager::disconnectView(CPTR(FrameAnimationWnd) pView)
{
	ruint index = findFrameIndex(pView);
	if (index != ~0)
	{
		m_frameList[index]->m_pView = NULL;
	}
	m_changed = true;
}

void RDOStudioFrameManager::closeAll()
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

void RDOStudioFrameManager::clear()
{
	if (studioApp.getStyle())
	{
		studioApp.getIMainWnd()->getDockFrame().getContext().clear();
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

	//STL_FOR_ALL(m_bitmapList, it)
	//{
	//	delete it->second;
	//}

	m_frameList .clear();
	m_bitmapList.clear();

	m_lastShowedFrame = ruint(~0);
	setCurrentShowingFrame(ruint(~0));
}

ruint RDOStudioFrameManager::getLastShowedFrame() const
{
	return m_lastShowedFrame;
}

void RDOStudioFrameManager::setLastShowedFrame(ruint index)
{
	if (index != ruint(~0) && index < count())
	{
		m_lastShowedFrame = index;
	}
}

void RDOStudioFrameManager::setCurrentShowingFrame(ruint index)
{
	if (index == ruint(~0) || (index != ruint(~0) && index < count()))
	{
		m_currentShowingFrame = index;
		if (studioApp.getStyle())
		{
			if (m_currentShowingFrame != ruint(~0))
			{
				studioApp.getIMainWnd()->getDockFrame().getContext().setCurrentItem(m_frameList[m_currentShowingFrame]->m_pTreeWidgetItem);
			}
			else
			{
				studioApp.getIMainWnd()->getDockFrame().getContext().setCurrentItem(NULL);
			}
		}
		m_onChangeFrame(index);
	}
}

void RDOStudioFrameManager::resetCurrentShowingFrame(ruint index)
{
	if (index == m_currentShowingFrame)
	{
		setCurrentShowingFrame(ruint(~0));
	}
}

void RDOStudioFrameManager::insertBitmap(CREF(tstring) bitmapName)
{
	if (m_bitmapList.find(bitmapName) != m_bitmapList.end())
		return;

	studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format("Загрузка %s...", bitmapName.c_str()));
	studioApp.getIMainWnd()->getDockDebug().getContext().update();

	rdo::binarystream stream;
	rdo::repository::RDOThreadRepository::BinaryFile data(bitmapName, stream);
	model->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data);

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

	studioApp.getIMainWnd()->getDockDebug().appendString(ok ? "ok\n" : "failed\n");
	studioApp.getIMainWnd()->getDockDebug().getContext().update();
}

void RDOStudioFrameManager::showFrame(CPTRC(rdo::animation::Frame) pFrame, ruint index)
{
	if (index < count())
	{
		PTR(FrameAnimationWnd) pFrameView = getFrameView(index);
		ASSERT(pFrameView);
		rdo::gui::BitmapList bitmapGeneratedList;
		pFrameView->update(pFrame, m_bitmapList, bitmapGeneratedList, m_frameList[index]->m_areaList);
		if (!bitmapGeneratedList.empty())
		{
			std::copy(bitmapGeneratedList.begin(), bitmapGeneratedList.end(), std::inserter(m_bitmapList, m_bitmapList.begin()));
		}
	}
}

void RDOStudioFrameManager::showNextFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame < cnt-1)
	{
		ruint index = m_currentShowingFrame + 1;
		PTR(FrameAnimationWnd) pView = getFrameView(index);
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

void RDOStudioFrameManager::showPrevFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame != ruint(~0))
	{
		ruint index = m_currentShowingFrame - 1;
		PTR(FrameAnimationWnd) pView = getFrameView(index);
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

void RDOStudioFrameManager::showFrame(ruint index)
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && index >= 0 && index < cnt)
	{
		PTR(FrameAnimationWnd) pView = getFrameView(index);
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

rbool RDOStudioFrameManager::canShowNextFrame() const
{
	ruint cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame == ruint(~0) || m_currentShowingFrame < cnt-1);
}

rbool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame != ruint(~0) && m_currentShowingFrame > 0);
}

void RDOStudioFrameManager::updateStyles() const
{
	BOOST_FOREACH(Frame* pFrame, m_frameList)
	{
		PTR(FrameAnimationWnd) pFrameView = pFrame->m_pView;
		if (pFrameView)
		{
			pFrameView->updateFont();
			pFrameView->update    ();
		}
	}
}

void RDOStudioFrameManager::onSubWindowActivated(QMdiSubWindow* pWindow)
{
	if (!pWindow)
		return;

	FrameAnimationWnd* pFrameAnimationWnd = dynamic_cast<FrameAnimationWnd*>(pWindow->widget());
	if (!pFrameAnimationWnd)
		return;

	ruint index = findFrameIndex(pFrameAnimationWnd);
	setLastShowedFrame    (index);
	setCurrentShowingFrame(index);
}

void RDOStudioFrameManager::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int)
{
	if (model->getRuntimeMode() == rdo::runtime::RTM_MaxSpeed)
		return;

	ruint index = findFrameIndex(pTreeWidgetItem);
	if (index == ruint(~0))
		return;

	PTR(FrameAnimationWnd) pView = getFrameView(index);
	if (!pView)
	{
		createView(index);
	}
	else
	{
		studioApp.getIMainWnd()->activateSubWindow(pView->parentWidget());
	}
}
