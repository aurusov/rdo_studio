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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOFindEdit)
	//}}AFX_VIRTUAL

public:
	RDOFindEdit();
	virtual ~RDOFindEdit();

	void setEditorStyle( RDOFindEditStyle* _style );

	void setKeyword( const string& keyword, const bool matchCase = false ) const;
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDOFINDEDIT_H
