#ifndef RDOSTUDIOEDITBASEVIEW_H
#define RDOSTUDIOEDITBASEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioview.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioEditBaseView
// ----------------------------------------------------------------------------
class RDOStudioEditBaseDoc;
namespace rdoEditor {
	class RDOEditorEdit;
}

class RDOStudioEditBaseView: public RDOStudioView
{
private:
	UINT timerBuf1;
	UINT timerBuf2;
	UINT timerBuf3;
	UINT timerBuf4;
	void stopTimer( UINT& timer );

protected:
	RDOStudioEditBaseView();
	DECLARE_DYNCREATE(RDOStudioEditBaseView)

public:
	virtual ~RDOStudioEditBaseView();

	RDOStudioEditBaseDoc* GetDocument();
	virtual rdoEditor::RDOEditorEdit* getEdit() const;

	CMenu popupMenu;

	std::string buf1;
	std::string buf2;
	std::string buf3;
	std::string buf4;
	bool resetBuf1;
	bool resetBuf2;
	bool resetBuf3;
	bool resetBuf4;
	int  currentBuffer;
	void restartBufTimer( const int bufIndex );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioEditBaseView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioEditBaseView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioEditBaseDoc* RDOStudioEditBaseView::GetDocument()
   { return (RDOStudioEditBaseDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOEDITBASEVIEW_H
