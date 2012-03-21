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
#include <map>
#include <gdiplus.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdoanimation.h"
#include "ui/gdiplus/headers/memdc/memdc.h"
#include "ui/gdiplus/headers/bitmap/bitmap.h"
#include "app/rdo_studio_mfc/src/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameView
// --------------------------------------------------------------------------------
class RDOStudioFrameDoc;

class RDOStudioFrameView: public RDOStudioView
{
DECLARE_DYNCREATE(RDOStudioFrameView)
public:
	RDOStudioFrameView();
	virtual ~RDOStudioFrameView();

	struct Area
	{
		Gdiplus::Rect  m_rect;
	};
	typedef  std::map<tstring, Area>  AreaList;

	void                   update       (CPTRC(rdoAnimation::RDOFrame) pFrame,
	                                      CREF(rdo::gui::BitmapList)   bitmapList,
	                                       REF(rdo::gui::BitmapList)   bitmapGeneratedList,
	                                       REF(AreaList)               areaList);
	void                   updateFont   ();
	PTR(RDOStudioFrameDoc) GetDocument  ();
	CREF(CRect)            getClientRect() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump       (REF(CDumpContext) dc) const;
#endif

private:
	CRect           m_newClientRect;
	Gdiplus::Point  m_pos;
	Gdiplus::Color  m_bgColor;
	HWND            m_hwnd;
	HFONT           m_hfontInit;
	HFONT           m_hfontCurrent;
	rdo::gui::MemDC m_memDC; // hmemdc, hbmp
	rbool           m_mouseOnHScroll;

	rbool valid           ();
	void  init            (CPTRC(rdoAnimation::RDOFrame) pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	void  init            (CREF(Gdiplus::Size) size);
	void  setBGColor      (CREF(Gdiplus::Color) color);
	void  onDraw          (REF(Gdiplus::Graphics) graphics);
	void  updateScrollBars();

	void  drawBackground    (CPTRC(rdoAnimation::RDOFrame) pFrame, CREF(rdo::gui::BitmapList) bitmapList);
	template <class F, class D>
	void  drawColoredElement(CREF(rdoAnimation::RDOColoredElement)  coloredElement, F fillBinder, D drawBinder);
	void  elementText       ( PTR(rdoAnimation::RDOTextElement)     pElement);
	void  elementRect       ( PTR(rdoAnimation::RDORectElement)     pElement);
	void  elementRoundRect  ( PTR(rdoAnimation::RDORRectElement)    pElement);
	void  elementLine       ( PTR(rdoAnimation::RDOLineElement)     pElement);
	void  elementTriang     ( PTR(rdoAnimation::RDOTriangElement)   pElement);
	void  elementCircle     ( PTR(rdoAnimation::RDOCircleElement)   pElement);
	void  elementEllipse    ( PTR(rdoAnimation::RDOEllipseElement)  pElement);
	void  elementBMP        ( PTR(rdoAnimation::RDOBmpElement)      pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementSBMP       ( PTR(rdoAnimation::RDOSBmpElement)     pElement, CREF(rdo::gui::BitmapList) bitmapList, REF(rdo::gui::BitmapList) bitmapGeneratedList);
	void  elementActive     ( PTR(rdoAnimation::RDOActiveElement)   pElement, REF(AreaList) areaList);

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

#endif // _RDO_STUDIO_MFC_FRAME_VIEW_H_
