/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDIT_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "app/rdo_studio/edit_ctrls/rdologedit.h"
#include "app/rdo_studio/edit_ctrls/editwithreadonlypopupmenu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Build
	: public Log
	, public PopupMenu
{
public:
	Build(PTR(QWidget) pParent);
	virtual ~Build();

	virtual void showFirstError();

protected:
	virtual void updateEdit(Model* pEdit, const LogEditLineInfo* pLineInfo);

private:
	typedef  Log  super;
	typedef  rdo::simulation::report::FileMessage RDOSyntaxMessage;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext  ();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDIT_H_
