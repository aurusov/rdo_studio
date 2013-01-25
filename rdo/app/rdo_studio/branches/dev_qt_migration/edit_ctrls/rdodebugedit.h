/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdodebugedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDODEBUGEDIT_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDODEBUGEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDODebugEdit
// --------------------------------------------------------------------------------
class RDODebugEdit: public RDOBaseEdit
{
public:
	RDODebugEdit(PTR(QWidget) pParent);
	virtual ~RDODebugEdit();

	void appendLine(CREF(tstring) str);

private:
	QMenu* m_pPopupMenu;

	virtual void onHelpContext();
	virtual void mousePressEvent(QMouseEvent* pEvent);
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDODEBUGEDIT_H_
