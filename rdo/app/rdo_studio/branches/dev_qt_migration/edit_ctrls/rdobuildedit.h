/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDIT_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEdit
// --------------------------------------------------------------------------------
class RDOBuildEdit: public LogEdit
{
public:
	RDOBuildEdit(PTR(QWidget) pParent);
	virtual ~RDOBuildEdit();

	virtual void showFirstError();

protected:
	virtual void updateEdit(rdoEditor::RDOEditorEdit* pEdit, const LogEditLineInfo* pLineInfo);

private:
	typedef rdo::simulation::report::FileMessage RDOSyntaxMessage;

	QMenu* m_pPopupMenu;

	virtual void onHelpContext();
	virtual void mousePressEvent(QMouseEvent* pEvent);
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDIT_H_
