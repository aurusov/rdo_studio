#ifndef RDOSTUDIOCHARTVIEW_H
#define RDOSTUDIOCHARTVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxole.h>
#include "./rdo_tracer/rdotracerserie.h"
#include "./rdo_tracer/rdotracervalues.h"
#include "rdostudiochartdoc.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartView
// ----------------------------------------------------------------------------
class RDOStudioChartView : public CView
{
//friend class RDOTracerSerie;
friend class RDOTracerSerieFindValue;

protected:
	RDOStudioChartView();
	DECLARE_DYNCREATE(RDOStudioChartView)

	COleDropTarget target;
	RDOTracerSerie* dragedSerie;

	CRect bmpRect;
	CRect newClientRect;
	CBitmap bmp;

	int valueCountX;
	int valueCountY;
	RDOTracerSerie* yaxis;
	int tickWidth;
	COLORREF timeColor;
	
	bool timeWrap;
	bool canUnwrapTime() const { return scale_koeff >= 1 && !zoomAuto; };
	bool doUnwrapTime() const { return canUnwrapTime() && !timeWrap; };

	CRect chartRect;
	void recalcLayout();

	int xMax;
	int xPos;
	bool minXVisible() const { return xPos == 0; };
	bool maxXVisible() const { return xPos == xMax; };
	void setScrollPos( UINT nSBCode, UINT nPos, const bool need_update = true );
	void updateScrollBars( const bool need_update = true );

	long double timeScale;
	RDOTracerTimeNow drawFromX;
	int drawFromEventIndex;
	RDOTracerTimeNow drawToX;
	int drawToEventCount;
	bool setTo( const int from_max_pos );
	void setFromTo();
	timesList unwrapTimesList;
	
	void drawYAxis( CDC &dc, CRect& chartRect, const RDOTracerSerie* axisValues);
	void drawXAxis( CDC &dc, CRect& chartRect );
	void drawGrid(	CDC &dc, CRect& chartRect );

	CMenu popupMenu;
	void copyToClipboard();

	double zoom;
	double old_zoom;
	double auto_zoom;
	double scale_koeff;
	void setZoom( double new_zoom, const bool force_update = false );
	bool zoomAuto;

public:
	
	virtual ~RDOStudioChartView();
	RDOStudioChartDoc* GetDocument();

	//{{AFX_VIRTUAL(RDOStudioChartView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting( CPrintInfo* pInfo );
	virtual void OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo );
	virtual void OnEndPrinting( CDC* pDC, CPrintInfo* pInfo );
	virtual DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(RDOStudioChartView)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChartTimewrap();
	afx_msg void OnUpdateChartTimewrap(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnChartZoomZoomin();
	afx_msg void OnChartZoomZoomout();
	afx_msg void OnChartZoomResetzoom();
	afx_msg void OnUpdateChartZoomZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChartZoomZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI);
	afx_msg void OnChartZoomZoomauto();
	afx_msg void OnUpdateChartZoomZoomauto(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioChartDoc* RDOStudioChartView::GetDocument()
   { return (RDOStudioChartDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTVIEW_H
