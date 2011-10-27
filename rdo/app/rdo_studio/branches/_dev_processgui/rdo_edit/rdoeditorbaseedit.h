#ifndef RDOEDITORBASEEDIT_H
#define RDOEDITORBASEEDIT_H
#pragma once

#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEdit
// --------------------------------------------------------------------------------
class RDOEditorBaseEdit: public rdoEditCtrl::RDOBaseEdit
{
protected:
	std::string kw0;
	std::string kw1;
	std::string kw2;
	std::string kw3;
	std::string getAllKW() const;
	static std::string convertToLexer( const std::string& kw );

	//{{AFX_MSG(RDOEditorBaseEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorBaseEdit)
	//}}AFX_VIRTUAL

public:
	RDOEditorBaseEdit();
	virtual ~RDOEditorBaseEdit();

	virtual BOOL DestroyWindow();

	void setEditorStyle( RDOEditorBaseEditStyle* _style );

	void replaceCurrent( const std::string str, const int changePosValue = -1 ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORBASEEDIT_H
