#ifndef RDO_PROCESS_CHILDFRM_H
#define RDO_PROCESS_CHILDFRM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_flowchart.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCChildFrame
// ----------------------------------------------------------------------------
class RDOPROCChildFrame: public CMDIChildWnd
{
DECLARE_DYNCREATE(RDOPROCChildFrame)

public:
	RDOPROCFlowChart m_wndView;

	RDOPROCChildFrame();
	virtual ~RDOPROCChildFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_VIRTUAL(RDOPROCChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOPROCChildFrame)
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_CHILDFRM_H
