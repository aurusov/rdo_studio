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
	//{{AFX_MSG(RDOFindEdit)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOFindEdit)
	//}}AFX_VIRTUAL

public:
	RDOFindEdit();
	virtual ~RDOFindEdit();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOFINDEDIT_H
