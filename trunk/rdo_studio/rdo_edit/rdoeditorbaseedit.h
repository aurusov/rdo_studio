#ifndef RDOEDITORBASEEDIT_H
#define RDOEDITORBASEEDIT_H
#pragma once

#include "../edit_ctrls/rdobaseedit.h"
#include "../edit_ctrls/rdologedit.h"
#include "rdoeditoreditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseEdit
// ----------------------------------------------------------------------------
class RDOEditorBaseEdit: public rdoEditCtrl::RDOBaseEdit
{
protected:
	static char* kw0;
	static char* kw1;
	static char* kw2;

	rdoEditCtrl::RDOLogEdit* log;

	//{{AFX_MSG(RDOEditorBaseEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnGotoNext();
	afx_msg void OnUpdateGotoNext(CCmdUI* pCmdUI);
	afx_msg void OnGotoPrev();
	afx_msg void OnUpdateGotoPrev(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorBaseEdit)
	//}}AFX_VIRTUAL

public:
	RDOEditorBaseEdit();
	virtual ~RDOEditorBaseEdit();

	void setEditorStyle( RDOEditorBaseEditStyle* _style );

	void setLog( rdoEditCtrl::RDOLogEdit& _log );
	const rdoEditCtrl::RDOLogEdit* getLog() const;

	void replaceCurrent( const std::string str, const int changePosValue = -1 ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORBASEEDIT_H
