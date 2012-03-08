/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDIT_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
// --------------------------------------------------------------------------------

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

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );

private:
	DECLARE_MESSAGE_MAP()

public:
	RDOEditorBaseEdit();
	virtual ~RDOEditorBaseEdit();

	virtual BOOL DestroyWindow();

	void setEditorStyle( RDOEditorBaseEditStyle* _style );

	void replaceCurrent( const std::string str, const int changePosValue = -1 ) const;
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORBASEEDIT_H_
