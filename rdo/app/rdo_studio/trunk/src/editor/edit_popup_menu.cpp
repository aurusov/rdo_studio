/*!
  \copyright (c) RDO-Team, 2013
  \file      edit_popup_menu.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      25.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/application.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

PopupMenu::PopupMenu(QWidget* pParent)
	: m_pPopupMenu(NULL)
{
	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
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
}

PopupMenu::~PopupMenu()
{}
