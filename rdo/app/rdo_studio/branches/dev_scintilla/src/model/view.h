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
#include "app/rdo_studio_mfc/src/edit/view_base.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
class RDOStudioModelDoc;

namespace rdoEditor {
	class RDOEditorEdit;
	class RDOEditorTabCtrl;
}

class RDOStudioModelView: public RDOStudioEditBaseView
{
friend class RDOStudioModelDoc;
friend class RDOStudioModel;

private:
	rdoEditor::RDOEditorTabCtrl* tab;

protected:
	RDOStudioModelView();
	DECLARE_DYNCREATE(RDOStudioModelView)

public:
	virtual ~RDOStudioModelView();

	RDOStudioModelDoc* GetDocument();
	virtual rdoEditor::RDOEditorEdit* getEdit() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSearchFindInModel();
	afx_msg LRESULT OnFindInModelMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioModelDoc* RDOStudioModelView::GetDocument()
   { return (RDOStudioModelDoc*)m_pDocument; }
#endif

#endif // RDOSTUDIOMODELVIEW_H
