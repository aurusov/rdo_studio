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

RDOStudioEditBaseView::RDOStudioEditBaseView(QWidget* pParent)
	: QWidget(pParent)
{

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
	//! todo qt
	// eraseMenu(&m_popupMenu);
}
