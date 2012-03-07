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
class RDOBuildEditLineInfo: public RDOLogEditLineInfo
{
friend class RDOBuildEdit;

protected:
	rdoSimulator::RDOSyntaxError::ErrorCode error_code;
	bool warning;

public:
	RDOBuildEditLineInfo( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, const std::string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1, const int _posInLine = 0, bool _warning = false );
	RDOBuildEditLineInfo( const std::string& _message );
	~RDOBuildEditLineInfo();

	virtual std::string getMessage() const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEdit
// --------------------------------------------------------------------------------
class RDOBuildEdit: public RDOLogEdit
{
protected:
	virtual void updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo );

protected:
	//{{AFX_MSG(RDOBuildEdit)
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOBuildEdit)
	//}}AFX_VIRTUAL

public:
	RDOBuildEdit();
	virtual ~RDOBuildEdit();

	void showFirstError();
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDIT_H_
