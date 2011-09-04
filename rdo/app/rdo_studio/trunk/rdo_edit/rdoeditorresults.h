#ifndef RDOEDITORRESULTS_H
#define RDOEDITORRESULTS_H
#pragma once

#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResults
// --------------------------------------------------------------------------------
class RDOEditorResults: public RDOEditorBaseEdit
{
protected:
	//{{AFX_MSG(RDOEditorResults)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorResults)
	//}}AFX_VIRTUAL

public:
	RDOEditorResults();
	virtual ~RDOEditorResults();

//	virtual BOOL DestroyWindow();

	void setEditorStyle( RDOEditorResultsStyle* _style );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORRESULTS_H
