#ifndef RDO_PROCESS_DOCKWND_H
#define RDO_PROCESS_DOCKWND_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCDockWnd
// ----------------------------------------------------------------------------
class RDOPROCDockWnd: public CSizingControlBarG
{
public:
	RDOPROCDockWnd();
	virtual ~RDOPROCDockWnd();

protected:
	//{{AFX_VIRTUAL(RDOPROCDockWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPROCDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_DOCKWND_H
