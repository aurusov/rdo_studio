/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/view.h
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHART_VIEW_H_
#define _RDO_STUDIO_MFC_CHART_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/view.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartView
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle;
class RDOTracerSerie;
class RDOStudioDocSerie;

class RDOStudioChartView : public RDOStudioView
{
friend class RDOTracerSerieFindValue;
friend class RDOStudioChartOptionsChart;

protected:
	DECLARE_DYNCREATE(RDOStudioChartView)

	CMutex mutex;

	COleDropTarget target;
	RDOTracerSerie* dragedSerie;

	CRect bmpRect;
	CRect newClientRect;

	int valueCountX;
	std::vector<std::string> captions;
	int valueCountY;
	
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
	int chartShift;
	bool setTo( const int from_max_pos );
	void setFromTo();
	timesList unwrapTimesList;
	
	void drawTitle( CRect& chartRect );
	CRect legendRect;
	void drawLegend( CRect& chartRect );
	void drawYAxis( CRect& chartRect, const RDOStudioDocSerie* axisValues );
	void drawXAxis( CRect& chartRect );
	void drawGrid( CRect& chartRect );

	CMenu popupMenu;
	void copyToClipboard();

	double zoom;
	double old_zoom;
	double auto_zoom;
	double scale_koeff;
	void setZoom( double new_zoom, const bool force_update = false );
	bool zoomAuto;

	bool previewMode;
	RDOStudioChartViewStyle* style;

	RDOStudioDocSerie* yAxis;
	bool needDrawLegend;

	void updateWindow();
	void updateView();

	HBITMAP hbmp;
	HBITMAP hbmpInit;
	HFONT   hfontTitle;
	HFONT   hfontLegend;
	HFONT   hfontAxis;
	HFONT   hfontInit;
	HDC     hdc;
	int     saved_hdc;
	HDC     hmemdc;
	int     saved_hmemdc;
	HWND    hwnd;
	void setFonts( const bool needRedraw = true );

	void onDraw();

public:
	RDOStudioChartView( const bool preview = false);
	virtual ~RDOStudioChartView();
	RDOStudioChartDoc* GetDocument();

	const RDOStudioChartViewStyle& getStyle() const;
	void setStyle( RDOStudioChartViewStyle* _style, const bool needRedraw = true );

	//{{AFX_VIRTUAL(RDOStudioChartView)
	public:
	virtual void OnInitialUpdate();
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
	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );
	afx_msg void OnChartOptions();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnViewZoomauto();
	afx_msg void OnUpdateViewZoomauto(CCmdUI* pCmdUI);
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg LRESULT OnUserUpdateChartView(WPARAM wParam = 0, LPARAM lParam = 0);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioChartDoc* RDOStudioChartView::GetDocument()
   { return (RDOStudioChartDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _RDO_STUDIO_MFC_CHART_VIEW_H_
