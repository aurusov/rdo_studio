/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view_base.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qtimer.h>
#include <QtCore/qcoreevent.h>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/src/edit/document_base.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseView
// --------------------------------------------------------------------------------
RDOStudioEditBaseView::Buffer::Buffer(QTimer* pTimer)
	: value (""    )
	, reset (false )
	, pTimer(pTimer)
{}

RDOStudioEditBaseView::RDOStudioEditBaseView(QWidget* pParent)
	: QWidget(pParent)
	, m_currentBuffer(0)
{
	m_thisCWnd.Attach(winId());

	m_bufferList.insert(BufferList::value_type(0, Buffer(new QTimer(this))));
	m_bufferList.insert(BufferList::value_type(1, Buffer(new QTimer(this))));
	m_bufferList.insert(BufferList::value_type(2, Buffer(new QTimer(this))));
	m_bufferList.insert(BufferList::value_type(3, Buffer(new QTimer(this))));

	BOOST_FOREACH(BufferList::value_type& buffer, m_bufferList)
	{
		connect(buffer.second.pTimer, SIGNAL(timeout()), this, SLOT(timerEvent(QTimerEvent*)));
	}

	//! todo qt
	//popupMenu.CreatePopupMenu();

	//CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	//rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//int delta = maximized ? 1 : 0;

	//appendMenu( mainMenu, 4 + delta, &popupMenu );
	//popupMenu.AppendMenu( MF_SEPARATOR );
	//appendMenu( mainMenu->GetSubMenu( 1 + delta ), 3, &popupMenu );
	//appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	//appendMenu( mainMenu->GetSubMenu( 1 + delta ), 5, &popupMenu );
	//popupMenu.AppendMenu( MF_SEPARATOR );
	//appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	//popupMenu.AppendMenu( MF_SEPARATOR );
	//appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	//appendMenu( mainMenu->GetSubMenu( 2 + delta ), 3, &popupMenu );
	//appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
}

RDOStudioEditBaseView::~RDOStudioEditBaseView()
{
	m_thisCWnd.Detach();

	//! todo qt
	// eraseMenu(&m_popupMenu);
}

void RDOStudioEditBaseView::restartBufTimer(ruint bufferID)
{
	BufferList::iterator it = m_bufferList.find(bufferID);
	if (it == m_bufferList.end())
		return;

	stopTimer(it->second.pTimer);
	if (studioApp.getStyle()->style_editor.buffer->canClearBuffer)
	{
		int delay = studioApp.getStyle()->style_editor.buffer->clearBufferDelay * 1000;
		if (delay < 0)
		{
			delay = 0;
		}
		it->second.pTimer->start(delay);
	}
}

void RDOStudioEditBaseView::stopTimer(QTimer* pTimer)
{
	ASSERT(pTimer);
	if (pTimer->isActive())
	{
		pTimer->stop();
	}
}

void RDOStudioEditBaseView::timerEvent(QTimerEvent* event)
{
	BOOST_FOREACH(BufferList::value_type& buffer, m_bufferList)
	{
		if (buffer.second.pTimer->timerId() == event->timerId())
		{
			buffer.second.reset = true;
			stopTimer(buffer.second.pTimer);
			break;
		}
	}
}

RDOEditorEdit* RDOStudioEditBaseView::getEdit() const
{
	return NULL;
}
