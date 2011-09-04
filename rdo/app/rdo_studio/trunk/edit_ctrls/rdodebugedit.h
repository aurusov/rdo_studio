#ifndef RDODEBUGEDIT_H
#define RDODEBUGEDIT_H
#pragma once

#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDODebugEdit
// --------------------------------------------------------------------------------
class RDODebugEdit: public RDOBaseEdit
{
protected:
	//{{AFX_MSG(RDODebugEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDODebugEdit)
	//}}AFX_VIRTUAL

public:
	RDODebugEdit();
	virtual ~RDODebugEdit();

	void appendLine( const std::string& str );
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDODEBUGEDIT_H
