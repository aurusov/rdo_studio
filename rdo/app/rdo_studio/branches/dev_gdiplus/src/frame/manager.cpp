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
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "repository/rdorepository.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager::Frame
// --------------------------------------------------------------------------------
RDOStudioFrameManager::Frame::Frame()
	: m_hitem(0   )
	, m_pDoc (NULL)
	, m_pView(NULL)
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
RDOStudioFrameManager::FrameDocTemplate::FrameDocTemplate(UINT nIDResource, PTR(CRuntimeClass) pDocClass, PTR(CRuntimeClass) pFrameClass, PTR(CRuntimeClass) pViewClass)
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{}

PTR(CFrameWnd) RDOStudioFrameManager::FrameDocTemplate::CreateNewFrame(PTR(CDocument) pDoc, PTR(CFrameWnd) pOther)
{
	PTR(CFrameWnd) pFrame = CMultiDocTemplate::CreateNewFrame(pDoc, pOther);
	static_cast<PTR(RDOStudioFrameDoc)>(pDoc)->frame = pFrame;
	return pFrame;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
RDOStudioFrameManager::RDOStudioFrameManager()
	: m_pFrameDocTemplate  (NULL     )
	, m_lastShowedFrame    (ruint(~0))
	, m_currentShowingFrame(ruint(~0))
	, m_changed            (false    )
{
	//! @todo А почему объект не удаляется ? Это происходит автоматически ?
	m_pFrameDocTemplate = new FrameDocTemplate(IDR_FRAME_TYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView));
	AfxGetApp()->AddDocTemplate(m_pFrameDocTemplate);
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	clear();

	STL_FOR_ALL(m_frameList, it)
	{
		delete *it;
	}
}

void RDOStudioFrameManager::insertFrame(CREF(tstring) frameName)
{
	PTR(Frame) item = new Frame();
	item->m_hitem = studioApp.m_pMainFrame->workspace.frames->InsertItem(frameName.c_str(), 1, 1, studioApp.m_pMainFrame->workspace.frames->GetRootItem());
	item->m_name  = frameName;
	item->m_pDoc  = NULL;
	item->m_pView = NULL;
	m_frameList.push_back(item);
}

ruint RDOStudioFrameManager::findFrameIndex(const HTREEITEM hitem) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_hitem == hitem)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint RDOStudioFrameManager::findFrameIndex(CPTR(RDOStudioFrameDoc) pDoc) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_pDoc == pDoc)
		{
			return index;
		}
		index++;
	}
	return ruint(~0);
}

ruint RDOStudioFrameManager::findFrameIndex(CPTR(RDOStudioFrameView) pView) const
{
	ruint index = 0;
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if ((*it)->m_pView == pView)
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

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::getFrameDoc(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pDoc;
}

PTR(RDOStudioFrameView) RDOStudioFrameManager::getFrameView(ruint index) const
{
	ASSERT(index < m_frameList.size());
	return m_frameList[index]->m_pView;
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

void RDOStudioFrameManager::areaDown(ruint frameIndex, CREF(Gdiplus::Point) point) const
{
	ASSERT(frameIndex != ruint(~0) && frameIndex < m_frameList.size());

	CREF(RDOStudioFrameView::AreaList) areaList = m_frameList[frameIndex]->m_areaList;
	STL_FOR_ALL_CONST(areaList, it)
	{
		if (it->second.m_rect.Contains(point))
		{
			tstring areaName = it->first;
			model->sendMessage(kernel->runtime(), RDOThread::RT_RUNTIME_FRAME_AREA_DOWN, &areaName);
		}
	}
}

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::connectFrameDoc(ruint index)
{
	PTR(RDOStudioFrameDoc) pDoc = NULL;
	if (index != ~0)
	{
		pDoc = static_cast<PTR(RDOStudioFrameDoc)>(m_pFrameDocTemplate->OpenDocumentFile(NULL));
		if (pDoc)
		{
			m_frameList[index]->m_pDoc  = pDoc;
			m_frameList[index]->m_pView = pDoc->getView();
			m_lastShowedFrame           = index;
			pDoc->SetTitle(rdo::format(IDS_FRAME_NAME, getFrameName(index).c_str()).c_str());
			setCurrentShowingFrame(index);
		}
	}
	return pDoc;
}

void RDOStudioFrameManager::disconnectFrameDoc(CPTR(RDOStudioFrameDoc) pDoc)
{
	ruint index = findFrameIndex(pDoc);
	if (index != ~0)
	{
		m_frameList[index]->m_pDoc  = NULL;
		m_frameList[index]->m_pView = NULL;
	}
	m_changed = true;
}

void RDOStudioFrameManager::closeAll()
{
	ruint backup = m_lastShowedFrame;
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->m_pDoc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendNotifyMessage(WM_CLOSE, 0, 0);
			}
		}
	}
	m_lastShowedFrame = backup;
}

void RDOStudioFrameManager::clear()
{
	if (studioApp.m_pMainFrame)
	{
		studioApp.m_pMainFrame->workspace.frames->deleteChildren(studioApp.m_pMainFrame->workspace.frames->GetRootItem());
	}
	STL_FOR_ALL(m_frameList, it)
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = (*it)->m_pDoc;
		if (isValidFrameDoc(pFrameDoc))
		{
			if (pFrameDoc->frame && pFrameDoc->frame->GetSafeHwnd())
			{
				pFrameDoc->frame->SendMessage(WM_CLOSE, 0, 0);
			}
		}
		delete *it;
	}

	STL_FOR_ALL(m_bitmapList, it)
	{
		delete it->second;
	}

	m_frameList .clear();
	m_bitmapList.clear();

	m_lastShowedFrame = ruint(~0);
	setCurrentShowingFrame(ruint(~0));
}

PTR(RDOStudioFrameDoc) RDOStudioFrameManager::getFirstExistDoc() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		if (isValidFrameDoc((*it)->m_pDoc))
		{
			return (*it)->m_pDoc;
		}
	}
	return NULL;
}

void RDOStudioFrameManager::expand() const
{
	studioApp.m_pMainFrame->workspace.frames->expand();
}

rbool RDOStudioFrameManager::isValidFrameDoc(CPTRC(RDOStudioFrameDoc) pFrame) const
{
	POSITION pos = m_pFrameDocTemplate->GetFirstDocPosition();
	while (pos)
	{
		PTR(RDOStudioFrameDoc) pDoc = static_cast<PTR(RDOStudioFrameDoc)>(m_pFrameDocTemplate->GetNextDoc(pos));
		if (pFrame == pDoc)
		{
			return true;
		}
	}
	return false;
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
		if (studioApp.m_pMainFrame)
		{
			PTR(CTreeCtrl) pTree = studioApp.m_pMainFrame->workspace.frames;
			if (m_currentShowingFrame != ruint(~0))
			{
				HTREEITEM hitem = m_frameList[m_currentShowingFrame]->m_hitem;
				pTree->SelectItem(hitem);
			}
			else
			{
				pTree->SelectItem(NULL);
			}
		}
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

	PTR(RDOStudioOutput) pOutput = &studioApp.m_pMainFrame->output;
	ASSERT(pOutput);
	pOutput->appendStringToDebug(rdo::format(IDS_MODEL_RESOURCE_LOADING_NAME, bitmapName.c_str()));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();

	rdo::binarystream stream;
	rdoRepository::RDOThreadRepository::BinaryFile data(bitmapName, stream);
	model->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD_BINARY, &data);

	rbool ok = false;
	PTR(Gdiplus::Bitmap) pBitmap = rdo::gui::Bitmap::load(data.m_name);
	if (pBitmap)
	{
		std::pair<rdo::gui::BitmapList::const_iterator, rbool> result =
			m_bitmapList.insert(rdo::gui::BitmapList::value_type(bitmapName, pBitmap));
		if (result.second)
		{
			ok = true;
		}
	}

	pOutput->appendStringToDebug(rdo::format(ok ? IDS_MODEL_RESOURCE_LOADING_NAME_OK : IDS_MODEL_RESOURCE_LOADING_NAME_FAILED));
	const_cast<PTR(rdoEditCtrl::RDODebugEdit)>(pOutput->getDebug())->UpdateWindow();
}

void RDOStudioFrameManager::showFrame(CPTRC(rdoAnimation::RDOFrame) pFrame, ruint index)
{
	if (index < count())
	{
		PTR(RDOStudioFrameDoc) pFrameDoc = getFrameDoc(index);
		if (pFrameDoc)
		{
			PTR(RDOStudioFrameView) pFrameView = getFrameView(index);
			ASSERT(pFrameView);
			rdo::gui::BitmapList bitmapGeneratedList;
			pFrameView->update(pFrame, m_bitmapList, bitmapGeneratedList, m_frameList[index]->m_areaList);
			if (!bitmapGeneratedList.empty())
			{
				std::copy(bitmapGeneratedList.begin(), bitmapGeneratedList.end(), std::inserter(m_bitmapList, m_bitmapList.begin()));
			}
		}
	}
}

void RDOStudioFrameManager::showNextFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame < cnt-1)
	{
		ruint index = m_currentShowingFrame + 1;
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

void RDOStudioFrameManager::showPrevFrame()
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && m_currentShowingFrame != ruint(~0))
	{
		ruint index = m_currentShowingFrame - 1;
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

void RDOStudioFrameManager::showFrame(ruint index)
{
	ruint cnt = count();
	if (model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && index >= 0 && index < cnt)
	{
		PTR(RDOStudioFrameDoc) pDoc = getFrameDoc(index);
		if (!pDoc)
		{
			pDoc = connectFrameDoc(index);
		}
		else
		{
			pDoc->frame->ActivateFrame();
			setLastShowedFrame    (index);
			setCurrentShowingFrame(index);
		}
	}
}

rbool RDOStudioFrameManager::canShowNextFrame() const
{
	ruint cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame == ruint(~0) || m_currentShowingFrame < cnt-1);
}

rbool RDOStudioFrameManager::canShowPrevFrame() const
{
	int cnt = count();
	return model->isRunning() && model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed && cnt > 1 && (m_currentShowingFrame != ruint(~0) && m_currentShowingFrame > 0);
}

void RDOStudioFrameManager::updateStyles() const
{
	STL_FOR_ALL_CONST(m_frameList, it)
	{
		PTR(RDOStudioFrameView) pFrameView = (*it)->m_pView;
		if (pFrameView)
		{
			pFrameView->updateFont    ();
			pFrameView->InvalidateRect(NULL);
			pFrameView->UpdateWindow  ();
		}
	}
}
