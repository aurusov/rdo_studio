#ifndef RDOFINDEDIT_H
#define RDOFINDEDIT_H
#pragma once

#include "rdologedit.h"
#include "rdofindeditstyle.h"

namespace rdoEditCtrl {

// ----------------------------------------------------------------------------
// ---------- RDOFindEdit
// ----------------------------------------------------------------------------
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

#endif // RDOFINDEDIT_H
