/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
#define _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qmenu.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseView
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorEdit;
}

class RDOStudioEditBaseView: public QWidget
{
Q_OBJECT

public:
	virtual ~RDOStudioEditBaseView();

	QMenu*      m_pPopupMenu;

protected:
	RDOStudioEditBaseView(QWidget* pParent);
};

#endif // _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
