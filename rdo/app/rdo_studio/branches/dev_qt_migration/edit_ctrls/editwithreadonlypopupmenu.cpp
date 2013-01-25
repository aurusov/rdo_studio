/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      editwithreadonlypopupmenu.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      25.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "app/rdo_studio_mfc/edit_ctrls/editwithreadonlypopupmenu.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/application.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

EditWithReadOnlyPopupMenu::EditWithReadOnlyPopupMenu()
{}

QMenu* EditWithReadOnlyPopupMenu::createPopupMenu(QWidget* pParent)
{
	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	m_pPopupMenu = new QMenu(pParent);
	m_pPopupMenu->addAction(pMainWindow->actEditCopy);
	m_pPopupMenu->addAction(pMainWindow->actEditSelectAll);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchFind);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindPrevious);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksToggle);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkPrev);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksClearAll);

	return m_pPopupMenu;
}