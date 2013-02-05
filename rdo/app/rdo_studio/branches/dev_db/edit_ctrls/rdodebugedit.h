/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdodebugedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDODEBUGEDIT_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDODEBUGEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio/edit_ctrls/editwithreadonlypopupmenu.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDODebugEdit
// --------------------------------------------------------------------------------
class RDODebugEdit
	: public RDOBaseEdit
	, public EditWithReadOnlyPopupMenu
{
public:
	RDODebugEdit(PTR(QWidget) pParent);
	virtual ~RDODebugEdit();

	void appendLine(CREF(QString) str);

private:
	typedef  RDOBaseEdit  super;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext();
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDODEBUGEDIT_H_
