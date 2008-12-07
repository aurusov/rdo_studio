#ifndef RDOSTUDIOEDITVIEW_H
#define RDOSTUDIOEDITVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioeditbaseview.h"

namespace rdoEditor {
	class RDOEditorEdit;
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioEditView
// ----------------------------------------------------------------------------
class RDOStudioEditDoc;

class RDOStudioEditView: public RDOStudioEditBaseView
{
friend class RDOStudioEditDoc;

private:
	rdoEditor::RDOEditorEdit* edit;

protected:
	RDOStudioEditView();
	DECLARE_DYNCREATE(RDOStudioEditView)

public:
	virtual ~RDOStudioEditView();

	RDOStudioEditDoc* GetDocument();
	virtual rdoEditor::RDOEditorEdit* getEdit() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioEditView)
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
	//{{AFX_MSG(RDOStudioEditView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioEditDoc* RDOStudioEditView::GetDocument()
   { return (RDOStudioEditDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOEDITVIEW_H
