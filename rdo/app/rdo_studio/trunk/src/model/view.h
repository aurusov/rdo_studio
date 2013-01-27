/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MODEL_VIEW_H_
#define _RDO_STUDIO_MFC_MODEL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "app/rdo_studio_mfc/src/edit/view_base.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorEdit;
	class RDOEditorTabCtrl;
}

class RDOStudioModelView: public RDOStudioEditBaseView, public IInit
{
private:
	rdoEditor::RDOEditorTabCtrl* tab;

public:
	RDOStudioModelView(QWidget* pParent);
	virtual ~RDOStudioModelView();

	REF(rdoEditor::RDOEditorTabCtrl)  getTab ();
	virtual rdoEditor::RDOEditorEdit* getEdit() const;

private:
	typedef  RDOStudioEditBaseView  parent_type;

	void closeEvent (PTR(QCloseEvent)  event);
	void resizeEvent(PTR(QResizeEvent) event);

	DECLARE_IInit;

	afx_msg void OnSearchFindInModel();
	afx_msg LRESULT OnFindInModelMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
};

#endif // RDOSTUDIOMODELVIEW_H
