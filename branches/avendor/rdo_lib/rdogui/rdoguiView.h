// rdoguiView.h : interface of the CRdoguiView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOGUIVIEW_H__60C078EA_D2E3_4979_8E40_36A27F239458__INCLUDED_)
#define AFX_RDOGUIVIEW_H__60C078EA_D2E3_4979_8E40_36A27F239458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRdoguiView : public CView
{
protected: // create from serialization only
	CRdoguiView();
	DECLARE_DYNCREATE(CRdoguiView)

// Attributes
public:
	CRdoguiDoc* GetDocument();

	CDC *dc;
	CRect rr;
	CBitmap *bmp;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRdoguiView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRdoguiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRdoguiView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in rdoguiView.cpp
inline CRdoguiDoc* CRdoguiView::GetDocument()
   { return (CRdoguiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOGUIVIEW_H__60C078EA_D2E3_4979_8E40_36A27F239458__INCLUDED_)
