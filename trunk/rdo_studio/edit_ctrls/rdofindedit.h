#ifndef RDOFINDEDIT_H
#define RDOFINDEDIT_H
#pragma once

#include "rdologedit.h"
#include "rdofindeditstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOFindEdit
// ----------------------------------------------------------------------------
class RDOFindEdit: public RDOLogEdit
{
protected:
	string keyword;

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

	void setKeyword( const string& _keyword, const bool matchCase = false ) const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOFINDEDIT_H
