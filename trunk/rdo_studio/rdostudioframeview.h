#ifndef RDOSTUDIOFRAMEVIEW_H
#define RDOSTUDIOFRAMEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameView
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc;

class RDOStudioFrameView: public CView
{
friend class RDOStudioModel;

DECLARE_DYNCREATE(RDOStudioFrameView)

private:
	CRect frameBmpRect;
	CRect newClientRect;
	CBitmap frameBmp;
	bool mustBeInit;

public:
	RDOStudioFrameView();
	virtual ~RDOStudioFrameView();

	RDOStudioFrameDoc* GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioFrameView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioFrameView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioFrameDoc* RDOStudioFrameView::GetDocument()
   { return (RDOStudioFrameDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEVIEW_H
