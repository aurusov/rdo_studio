#ifndef RDOSTUDIOFRAMEVIEW_H
#define RDOSTUDIOFRAMEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "app/rdo_studio_mfc/src/view.h"

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameView
// --------------------------------------------------------------------------------
class RDOStudioFrameDoc;

class RDOStudioFrameView: public RDOStudioView
{
friend class RDOStudioFrameManager;

DECLARE_DYNCREATE(RDOStudioFrameView)

private:
	CRect frameBmpRect;
	CPoint points[5];
	CRect newClientRect;
	int xPos;
	int yPos;
	COLORREF bgColor;
	bool mustBeInit;

	HWND    hwnd;
	HDC     hdc;
	HDC     hmemdc;
	int     saved_hdc;
	int     saved_hmemdc;
	HFONT   hfontInit;
	HFONT   hfontCurrent;
//	HBITMAP hbmpInit;
	HBITMAP hbmp;

	bool mouseOnHScroll;

	void onDraw();
	void updateFont();
	void updateScrollBars();

public:
	RDOStudioFrameView();
	virtual ~RDOStudioFrameView();

	RDOStudioFrameDoc* GetDocument();

	const CRect& getClientRect() const { return newClientRect; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioFrameView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioFrameView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnHelpKeyword();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioFrameDoc* RDOStudioFrameView::GetDocument()
   { return (RDOStudioFrameDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEVIEW_H
