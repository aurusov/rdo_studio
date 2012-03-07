/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
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
class RDOFindEdit: public RDOLogEdit
{
protected:
	//{{AFX_MSG(RDOFindEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOFindEdit)
	//}}AFX_VIRTUAL

public:
	RDOFindEdit();
	virtual ~RDOFindEdit();

	void setEditorStyle( RDOFindEditStyle* _style );

	void setKeyword( const std::string& keyword, const bool matchCase = false ) const;
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDIT_H_
