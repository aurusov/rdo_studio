#ifndef RDOFINDEDIT_H
#define RDOFINDEDIT_H
#pragma once

#include "rdologedit.h"

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

	virtual void setEditorStyle( rdoBaseEdit::RDOBaseEditStyle* style );

	void setKeyword( const string& _keyword );
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOFINDEDIT_H
