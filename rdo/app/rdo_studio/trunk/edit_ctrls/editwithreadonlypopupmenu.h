/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      editwithreadonlypopupmenu.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      25.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_
#define _RDO_STUDIO_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <QWidget>
#include <QMenu>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class EditWithReadOnlyPopupMenu
{
protected:
	EditWithReadOnlyPopupMenu(QWidget* pParent);
	virtual ~EditWithReadOnlyPopupMenu();

	QMenu* m_pPopupMenu;
};

#endif // _RDO_STUDIO_EDIT_CTRLS_EDITWITHREADONLYPOPUPMENU_H_
