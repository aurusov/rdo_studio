#ifndef RDOEDITORBASEEDIT_H
#define RDOEDITORBASEEDIT_H
#pragma once

#include "../edit_ctrls/rdobaseedit.h"
#include "rdoeditoreditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorBase
// ----------------------------------------------------------------------------
class RDOEditorBase: public rdoEditCtrl::RDOBaseEdit
{
protected:
	static char* kw0;
	static char* kw1;
	static char* kw2;

	//{{AFX_MSG(RDOEditorBase)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorBase)
	//}}AFX_VIRTUAL

public:
	RDOEditorBase();
	virtual ~RDOEditorBase();

	void setEditorStyle( RDOEditorBaseStyle* _style );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORBASEEDIT_H
