#ifndef RDODEBUGEDIT_H
#define RDODEBUGEDIT_H
#pragma once

#include "rdobaseedit.h"

namespace rdoEditCtrl {

// ----------------------------------------------------------------------------
// ---------- RDODebugEdit
// ----------------------------------------------------------------------------
class RDODebugEdit: public RDOBaseEdit
{
protected:
	//{{AFX_MSG(RDODebugEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDODebugEdit)
	//}}AFX_VIRTUAL

public:
	RDODebugEdit();
	virtual ~RDODebugEdit();

	void setEditorStyle( RDOBaseEditStyle* _style );

	void appendLine( const std::string& str );
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDODEBUGEDIT_H
