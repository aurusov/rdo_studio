#ifndef RDOSTUDIOCHARTVIEW_H
#define RDOSTUDIOCHARTVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxole.h>
#include "./rdo_tracer/rdotracerrdoclasses.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartView
// ----------------------------------------------------------------------------
class RDOStudioChartDoc;

class RDOStudioChartView : public CView
{
protected:
	RDOStudioChartView();
	DECLARE_DYNCREATE(RDOStudioChartView)

	COleDropTarget target;
	RDOTracerSerie* dragedSerie;

	CRect bmpRect;
	CRect newClientRect;
	CBitmap bmp;

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
