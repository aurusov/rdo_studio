#ifndef RDOSTUDIOMODELVIEW_H
#define RDOSTUDIOMODELVIEW_H

#include "app/rdo_studio_mfc/src/edit/view_base.h"

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

public:
	//{{AFX_VIRTUAL(RDOStudioModelView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioModelView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSearchFindInModel();
	//}}AFX_MSG
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

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMODELVIEW_H
