/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_VIEW_H_
#define _RDO_STUDIO_MFC_FRAME_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/memdc.h"
#include "app/rdo_studio_mfc/src/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameView
// --------------------------------------------------------------------------------
class RDOStudioFrameDoc;

class RDOStudioFrameView: public RDOStudioView
{
friend class RDOStudioFrameManager;

DECLARE_DYNCREATE(RDOStudioFrameView)

private:
	typedef  std::auto_ptr<Gdiplus::Graphics>  TDC;

	Gdiplus::Rect   frameBmpRect;
	Gdiplus::Point  points[5];
	CRect           newClientRect;
	Gdiplus::Point  m_pos;
	Gdiplus::Color  m_bgColor;
	rbool mustBeInit;

	HWND   m_hwnd;
	TDC    m_pGraphics; // hdc
	HFONT  hfontInit;
	HFONT  hfontCurrent;
	MemDC  m_memDC; // hmemdc, hbmp
	rbool  mouseOnHScroll;

	void onDraw();
	void updateFont();
	void updateScrollBars();

public:
	RDOStudioFrameView();
	virtual ~RDOStudioFrameView();

	PTR(RDOStudioFrameDoc) GetDocument();

	CREF(CRect) getClientRect() const { return newClientRect; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump       (REF(CDumpContext) dc) const;
#endif

private:
	virtual BOOL PreCreateWindow  (REF(CREATESTRUCT) cs);
	virtual BOOL OnPreparePrinting(PTR(CPrintInfo) pInfo);
	virtual void OnBeginPrinting  (PTR(CDC) pDC, PTR(CPrintInfo) pInfo);
	virtual void OnEndPrinting    (PTR(CDC) pDC, PTR(CPrintInfo) pInfo);
	virtual void OnDraw           (PTR(CDC) pDC);
	virtual void OnActivateView   (BOOL bActivate, PTR(CView) pActivateView, PTR(CView) pDeactiveView);

	afx_msg int  OnCreate       (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy      ();
	afx_msg void OnPaint        ();
	afx_msg void OnHelpKeyword  ();
	afx_msg void OnSize         (UINT nType,    int    cx,      int cy);
	afx_msg void OnHScroll      (UINT nSBCode,  UINT   nPos,    PTR(CScrollBar) pScrollBar);
	afx_msg void OnVScroll      (UINT nSBCode,  UINT   nPos,    PTR(CScrollBar) pScrollBar);
	afx_msg void OnKeyDown      (UINT nChar,    UINT   nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp        (UINT nChar,    UINT   nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel   (UINT nFlags,   short  zDelta,  CPoint pt);
	afx_msg void OnLButtonDown  (UINT nFlags,   CPoint point);
	afx_msg void OnMouseMove    (UINT nFlags,   CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove  (UINT nHitTest, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline PTR(RDOStudioFrameDoc) RDOStudioFrameView::GetDocument()
{
	return static_cast<PTR(RDOStudioFrameDoc)>(m_pDocument);
}
#endif

#endif // _RDO_STUDIO_MFC_FRAME_VIEW_H_
