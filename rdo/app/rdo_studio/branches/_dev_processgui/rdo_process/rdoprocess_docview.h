#ifndef RDO_PROCESS_DOCVIEW_H
#define RDO_PROCESS_DOCVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// --------------------------------------------------------------------------------
// -------------------- RPDoc
// --------------------------------------------------------------------------------
class RPView;

class RPDoc: public CDocument
{
DECLARE_DYNCREATE(RPDoc)
protected:
	RPDoc();
	virtual ~RPDoc();

	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnMethodCommandRange( UINT id );
	afx_msg void OnMethodUpdateRange( CCmdUI* pCmdUI );
	DECLARE_MESSAGE_MAP()

public:
	RPView* getView() const;
};

// --------------------------------------------------------------------------------
// -------------------- RPView
// --------------------------------------------------------------------------------
class RPFlowChart;
class RPObjectFlowChart;
class RPObjectClassInfo;

class RPView: public CView
{
DECLARE_DYNCREATE(RPView)

private:
	RPFlowChart* flowchart;

protected:
	RPView();
	virtual ~RPView();

	COleDropTarget target;
	const RPObjectClassInfo* getSrcClassInfo( COleDataObject* pDataObject ) const;

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave();
	virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	RPFlowChart* getFlowchart() { return flowchart; }
	RPDoc* GetDocument() const;
	virtual void OnDraw( CDC* pDC );
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void makeFlowChartWnd( RPObjectFlowChart* flowobj );

protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RPDoc* RPView::GetDocument() const
   { return reinterpret_cast<RPDoc*>(m_pDocument); }
#endif

#endif // RDO_PROCESS_DOCVIEW_H
