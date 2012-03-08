/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
#define _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseView
// --------------------------------------------------------------------------------
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

	tstring buf1;
	tstring buf2;
	tstring buf3;
	tstring buf4;
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

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOStudioEditBaseDoc* RDOStudioEditBaseView::GetDocument()
   { return (RDOStudioEditBaseDoc*)m_pDocument; }
#endif

#endif // _RDO_STUDIO_MFC_EDIT_VIEW_BASE_H_
