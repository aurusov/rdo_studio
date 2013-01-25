/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      editwithreadonlypopupmenu.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      25.01.2013
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qmenu.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_

class EditWithReadOnlyPopupMenu
{
	public:
		EditWithReadOnlyPopupMenu();

		QMenu* createPopupMenu(QWidget* pParent);

	private:
		QMenu* m_pPopupMenu;
};

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_