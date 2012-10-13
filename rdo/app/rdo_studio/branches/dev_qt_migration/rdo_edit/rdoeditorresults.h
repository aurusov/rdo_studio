/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresults.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResults
// --------------------------------------------------------------------------------
class RDOEditorResults: public RDOEditorBaseEdit
{
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpKeyword();
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

public:
	RDOEditorResults();
	virtual ~RDOEditorResults();

//	virtual BOOL DestroyWindow();

	void setEditorStyle( RDOEditorResultsStyle* _style );
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTS_H_
