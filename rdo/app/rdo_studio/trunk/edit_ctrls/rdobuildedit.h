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
// -------------------- RDOBuildEditLineInfo
// --------------------------------------------------------------------------------
class RDOBuildEditLineInfo : public RDOLogEditLineInfo
{
public:
	typedef  rdo::service::simulation::RDOSyntaxMessage  RDOSyntaxMessage;

	explicit RDOBuildEditLineInfo(CREF(RDOSyntaxMessage) error);
	explicit RDOBuildEditLineInfo(CREF(tstring)          text );
	virtual ~RDOBuildEditLineInfo();

	virtual tstring getMessage() const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEdit
// --------------------------------------------------------------------------------
class RDOBuildEdit : public RDOLogEdit
{
public:
	RDOBuildEdit();
	virtual ~RDOBuildEdit();

	virtual void showFirstError();

protected:
	virtual void updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo );

private:
	afx_msg void OnHelpKeyword();
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDIT_H_
