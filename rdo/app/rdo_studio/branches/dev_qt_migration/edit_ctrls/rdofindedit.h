/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindedit.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDIT_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOFindEdit
// --------------------------------------------------------------------------------
class RDOFindEdit: public LogEdit
{
public:
	RDOFindEdit(PTR(QWidget) pParent);
	virtual ~RDOFindEdit();

	void setEditorStyle(RDOFindEditStyle* pStyle);

	void setKeyword(CREF(tstring) keyword, const rbool matchCase = false) const;

private:
	virtual void onHelpContext();
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDIT_H_
